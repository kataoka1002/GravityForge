/*!
 * @brief	スプライト用のシェーダー
 */
#include "IBL.h"


static const int MAX_DIRECTION_LIGHT = 4;
static const int MAX_POINT_LIGHT = 4;
static const int MAX_SPOT_LIGHT = 4;
static const float PI = 3.1415926f;
static const int NUM_SHADOW_MAP = 3;
static const int INFINITY = 40.0f;
static const int NUM_REFLECTION_TEXTURE = 5;


// 構造体
struct DirectionLight
{
    float3 dirDirection;    //方向
    float3 dirColor;        //カラー
};

struct PointLight
{
    float3 ptPosition;   //ポジション
    float3 ptColor;      //カラー
    float ptRange;       //影響範囲
};

struct SpotLight
{
    float3 spPosition;  //ポジション
    float3 spColor;     //カラー
    float spRange;      //影響範囲
    float3 spDirection; //射出方向
    float spAngle;      //射出角度
};

struct HemLight
{
    float3 skyColor;        //空の色
    float3 groundColor;     //地面の色
    float3 groundNormal;    //地面の法線
};


cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
};

cbuffer LightCb : register(b1)
{
    DirectionLight directionLight[MAX_DIRECTION_LIGHT];
    float3 eyePos;
    PointLight pointLight[MAX_POINT_LIGHT];
    SpotLight spotLight[MAX_SPOT_LIGHT];
    HemLight hemLight;
    float4x4 mLVP[NUM_SHADOW_MAP];				//ライトビュープロプロジェクション行列
    float iblIntencity;                         // IBLの強度
    int isIBL;                                  // IBLを行う
    int isEnableRaytracing;                     // レイトレが有効
}

struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

// 変数定義
Texture2D<float4> albedoTexture : register(t0);             // アルベド
Texture2D<float4> normalTexture : register(t1);             // 法線
Texture2D<float4> worldPosTexture : register(t2);           // ワールド座標
Texture2D<float4> normalInViewTexture : register(t3);       // カメラ空間の法線
Texture2D<float4> metallicSmoothTexture : register(t4);     // メタリックスムース(スペキュラ)
Texture2D<float4> shadowMap[NUM_SHADOW_MAP] : register(t5); // シャドウマップ(GBufferではない)
TextureCube<float4> g_skyCubeMap : register(t8);           // スカイキューブ

Texture2D<float4> g_raytracingTexture[NUM_REFLECTION_TEXTURE] : register(t20); //レイトレの画像

//sampler Sampler : register(s0);                           // サンプラー

// 関数定義
float3 CalcLambertDiffuse(float3 direction, float3 color, float3 normal);
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal, float speclarPow);
float3 CalcDirectionLight(float3 normal, float3 worldPos, float specularPow);
float3 CalcPBRDirectionLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float isShadowReceiver, float3 worldPos);
float3 CalcPointLight(float3 normal, float3 worldPos, float specularPow);
float3 CalcPBRPointLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos);
float3 CalcSpotLight(float3 normal, float3 worldPos, float specularPow);
float3 CalcPBRSpotLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos);
float3 CalcLimPower(float3 normal, float3 normalInView);
float3 CalcHemLight(float3 normal);
float CalcBeckmann(float m, float t);
float CalcSpcFresnel(float f0, float u);
float CookTorranceSpecular(float3 L, float3 V, float3 N, float smooth);
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V, float smooth);
float3 CalcPBR(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 direction, float3 color);
float CalcShadowPow(float isShadowReceiver, float3 worldPos);
float Chebyshev(float2 moments, float depth);
float4 SampleReflectionColor(float2 uv, float level);

// 頂点シェーダー
PSInput VSMain(VSInput vsIn)
{
    PSInput psIn;
    psIn.pos = mul(mvp, vsIn.pos);
    psIn.uv = vsIn.uv;
    return psIn;
}

// ピクセルシェーダー
float4 PSMain(PSInput In) : SV_Target0
{
	// GBufferの内容を使ってライティング-----------------------------------
    
    // アルベド
    float4 albedo = albedoTexture.Sample(Sampler, In.uv);
    
    // 法線
    float3 normal = normalTexture.Sample(Sampler, In.uv).xyz;
    normal = (normal - 0.5f) * 2.0f;
    
    // ワールド座標
    float3 worldPos = worldPosTexture.Sample(Sampler, In.uv).xyz;
    
    // カメラ空間の法線
    float3 normalInView = normalInViewTexture.Sample(Sampler, In.uv).xyz;
    
    // スペキュラカラーはアルベドカラーと同じ
    float3 specColor = albedo.xyz;

    // 金属度(スペキュラ)
    float metallic = metallicSmoothTexture.Sample(Sampler, In.uv).r;

    // 滑らかさ
    float smooth = metallicSmoothTexture.Sample(Sampler, In.uv).a;

    // 視線に向かって伸びるベクトルを計算する
    float3 toEye = normalize(eyePos - worldPos);
        
    // --------------------------------------------------------------------
    
    
    // ディレクションライトの計算
    //float3 lig = CalcDirectionLight(normal, worldPos, metallic);
    float3 lig = CalcPBRDirectionLight(normal, toEye, albedo, specColor, metallic, smooth, normalTexture.Sample(Sampler, In.uv).w, worldPos);
    
    // ポイントライトの強さ設定
    //lig += CalcPointLight(normal, worldPos, metallic);
    lig += CalcPBRPointLight(normal, toEye, albedo, specColor, metallic, smooth, worldPos);
    
    // スポットライトの強さ設定
    //lig += CalcSpotLight(normal, worldPos, metallic);
    lig += CalcPBRSpotLight(normal, toEye, albedo, specColor, metallic, smooth, worldPos);
    
    // リムライトの強さ設定(おかしい)
    //lig += CalcLimPower(normal, normalInView);
    
    // 半球ライトの強さ設定
    lig += CalcHemLight(normal);
    
    //if (isEnableRaytracing)
    //{
    //    // レイトレを行う場合はレイトレで作った反射テクスチャとIBLテクスチャを合成する。
    //    // GLテクスチャ
    //    float reflectionRate = 1.0f - ((smooth - 0.5f) * 2.0f);
    //    float level = lerp(0.0f, (float) (NUM_REFLECTION_TEXTURE - 1), pow(reflectionRate, 3.0f));
    //    if (level < NUM_REFLECTION_TEXTURE - 1)
    //    {
    //        lig += albedo * SampleReflectionColor(In.uv, level);
    //    }
    //    else if (isIBL == 1)
    //    {
    //        // IBLがあるなら。
    //        lig += albedo * SampleIBLColorFromSkyCube(
    //            g_skyCubeMap,
    //            toEye,
    //            normal,
    //            smooth,
    //            iblIntencity
    //        );
    //    }
    //    else
    //    {
    //        // 環境光による底上げ
    //        float ambientLight = 1.1f;
    //        lig += albedo * ambientLight;
    //    }
    //}
    //else if (isIBL == 1)
    //{
    //    // 視線からの反射ベクトルを求める。
    //    lig += albedo * SampleIBLColorFromSkyCube(
    //        g_skyCubeMap,
    //        toEye,
    //        normal,
    //        smooth,
    //        iblIntencity
    //    );
    //}
    //else
    {
        // 環境光による底上げ
        float ambientLight = 1.1f;
        lig += albedo * ambientLight;
    }
    
    // 影の落ち具合を計算する。
    float shadow = CalcShadowPow(normalTexture.Sample(Sampler, In.uv).w, worldPos) * normalTexture.Sample(Sampler, In.uv).w;
    lig *= max(0.5f, 1.0f - shadow);
    
    float4 finalColor = 1.0f;
    finalColor.xyz = lig;    
        
    //色調を青色強めで補正する
    finalColor.r *= 0.9f;
    finalColor.g *= 0.9f;
    finalColor.b *= 1.2f;
    
    return finalColor;
}

////////ココから関数/////////////////////////////////////////////////////////////////////////////////////////////////////

//拡散反射の計算
float3 CalcLambertDiffuse(float3 direction, float3 color, float3 normal)
{
    float power = max(0.0f, dot(normal, direction) * -1.0f);
    
    float3 lig = color * power;
    
    return lig;
}

//鏡面反射の計算
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal, float speclarPow)
{
    float3 toEye = normalize(eyePos - worldPos);
    
    float3 refVec = reflect(direction, normal);
    
    float power = max(0.0f, dot(toEye, refVec));
    power = pow(power, 5.0f);
    
    float3 lig = color * power * speclarPow;
    
    return lig;
}

//ディレクションライトの計算
float3 CalcDirectionLight(float3 normal, float3 worldPos, float specularPow)
{
    float3 lig;
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {
        lig += CalcLambertDiffuse(directionLight[i].dirDirection, directionLight[i].dirColor, normal);
        lig += CalcPhongSpecular(directionLight[i].dirDirection, directionLight[i].dirColor, worldPos, normal, specularPow);
    }
    
    return lig;
}

//PBRのディレクションライトの計算
float3 CalcPBRDirectionLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float isShadowReceiver, float3 worldPos)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {        
        // PBRによるライトの強さを求める
        lig += CalcPBR(normal, toEye, albedo, specColor, metallic, smooth, directionLight[i].dirDirection, directionLight[i].dirColor);
    }
    
    return lig;
}

//ポイントライトの計算
float3 CalcPointLight(float3 normal, float3 worldPos, float specularPow)
{
    float3 lig;
    
    for (int i = 0; i < MAX_POINT_LIGHT; i++)
    {
     	// このサーフェイスに入射しているポイントライトの光の向きを計算する
        float3 ligDir = worldPos - pointLight[i].ptPosition;
        
        // 正規化して大きさ1のベクトルにする
        ligDir = normalize(ligDir);

        // 減衰なしのLambert拡散反射光を計算する
        float3 diffPoint = CalcLambertDiffuse(ligDir, pointLight[i].ptColor, normal);

        // 減衰なしのPhong鏡面反射光を計算する
        float3 specPoint = CalcPhongSpecular(ligDir, pointLight[i].ptColor, worldPos, normal, specularPow);

        // 距離による影響率を計算する
        // ポイントライトとの距離を計算する
        float3 distance = length(worldPos - pointLight[i].ptPosition);

        // 影響率は距離に比例して小さくなっていく
        float affect = 1.0f - 1.0f / pointLight[i].ptRange * distance;
        // 影響力がマイナスにならないように補正をかける
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

        // 影響の仕方を指数関数的にする。今回のサンプルでは3乗している
        affect = pow(affect, 3.0f);

        // 拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
        diffPoint *= affect;
        specPoint *= affect;
        
        // 最終的に返す強さに足していく
        lig += diffPoint + specPoint;
    }

    return lig;
}

//PBRのポイントライトを計算
float3 CalcPBRPointLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_POINT_LIGHT; i++)
    {
     	// このサーフェイスに入射しているポイントライトの光の向きを計算する
        float3 ligDir = worldPos - pointLight[i].ptPosition;

        // 正規化して大きさ1のベクトルにする
        ligDir = normalize(ligDir);

        // PBRによる光の強さを計算する        
        float3 PBRLig = CalcPBR(normal, toEye, albedo, specColor, metallic, smooth, ligDir, pointLight[i].ptColor);

        // 距離による影響率を計算する
        // ポイントライトとの距離を計算する
        float3 distance = length(worldPos - pointLight[i].ptPosition);

        // 影響率は距離に比例して小さくなっていく
        float affect = 1.0f - 1.0f / pointLight[i].ptRange * distance;
        // 影響力がマイナスにならないように補正をかける
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

        // 影響の仕方を指数関数的にする。今回のサンプルでは3乗している
        affect = pow(affect, 3.0f);

        // 減衰率を乗算して影響を弱める
        lig += PBRLig * affect;
    }

    return lig;
}

//スポットライトの計算
float3 CalcSpotLight(float3 normal, float3 worldPos, float specularPow)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_SPOT_LIGHT; i++)
    {
        //サーフェイスに入射するスポットライトの光の向きを計算する
        float3 LigDir = worldPos - spotLight[i].spPosition;
	    //正規化
        LigDir = normalize(LigDir);
    
	    //減衰なしのLambert拡散反射光を計算する
        float3 diffSpot = CalcLambertDiffuse(LigDir, spotLight[i].spColor, normal);
    
	    //減衰なしのPhong鏡面反射の計算
        float3 specSpot = CalcPhongSpecular(LigDir, spotLight[i].spColor, worldPos, normal, specularPow);
    
	    //スポットライトとの距離を計算する
        float distance = length(worldPos - spotLight[i].spPosition);
    
	    //影響率は距離に比例して小さくなっていく
        float affect = 1.0f - 1.0f / spotLight[i].spRange * distance;
	    //影響力がマイナスにならないように
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }
    
	    //影響を指数関数的にする
        affect = pow(affect, 3.0f);
    
	    //拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
        diffSpot *= affect;
        specSpot *= affect;
    
        // ここからは角度による影響率を求める
        
	    //入射光と射出方向の角度を求める
        float angle = dot(LigDir, spotLight[i].spDirection);
    
	    //dot()で求めた値をacos()に渡して角度を求める
        angle = abs(acos(angle));
    
	    //角度に比例して小さくなっていく影響率を計算する
        affect = 1.0f - 1.0f / spotLight[i].spAngle * angle;
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }
    
	    //影響を指数関数的にする
        affect = pow(affect, 0.5f);
    
	    //角度による影響率を反射光に乗算して、影響を弱める
        diffSpot *= affect;
        specSpot *= affect;

        //最終的に返す強さに足していく
        lig += diffSpot;
        lig += specSpot;

    }
	
    return lig;
}

//PBRのスポットライトを計算
float3 CalcPBRSpotLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_SPOT_LIGHT; i++)
    {
        //サーフェイスに入射するスポットライトの光の向きを計算する
        float3 ligDir = worldPos - spotLight[i].spPosition;
        
	    //正規化
        ligDir = normalize(ligDir);
    
        // PBRによる光の強さを計算する        
        float3 PBRLig = CalcPBR(normal, toEye, albedo, specColor, metallic, smooth, ligDir, spotLight[i].spColor);

	    //スポットライトとの距離を計算する
        float distance = length(worldPos - spotLight[i].spPosition);
    
	    //影響率は距離に比例して小さくなっていく
        float affect = 1.0f - 1.0f / spotLight[i].spRange * distance;
	    //影響力がマイナスにならないように
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }
    
	    //影響を指数関数的にする
        affect = pow(affect, 3.0f);
    
	    //減衰率を乗算して影響を弱める
        PBRLig *= affect;
    
        // ここからは角度による影響率を求める
        
	    //入射光と射出方向の角度を求める
        float angle = dot(ligDir, spotLight[i].spDirection);
    
	    //dot()で求めた値をacos()に渡して角度を求める
        angle = abs(acos(angle));
    
	    //角度に比例して小さくなっていく影響率を計算する
        affect = 1.0f - 1.0f / spotLight[i].spAngle * angle;
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }
    
	    //影響を指数関数的にする
        affect = pow(affect, 0.5f);
    
	    //角度による影響率を反射光に乗算して、影響を弱める
        PBRLig *= affect;

        //最終的に返す強さに足していく
        lig += PBRLig;
    }
	
    return lig;
}

//リムライトの計算
float3 CalcLimPower(float3 normal, float3 normalInView)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {
        //サーフェイスの法線と光の入射方向に依存するリムの強さを求める
        float power1 = 1.0f - max(0.0f, dot(directionLight[i].dirDirection, normal));
        
	    //サーフェイスの法線と視線の方向に依存するリムの強さを求める
        float power2 = 1.0f - max(0.0f, normalInView.z * -1.0f);
        
	    //最終的なリムの強さを求める
        float limPower = power1 * power2;
        
        limPower = max(0.0f, limPower);
        
	    //pow()を使用し強さの変化を指数関数的にする
        limPower = pow(limPower, 1.3f);
        
        //リムライトのカラーを計算し足していく
        lig += directionLight[i].dirColor * limPower;
    }

    return lig;
}

//半球ライトの計算
float3 CalcHemLight(float3 normal)
{
	//サーフェイスの法線と地面の法線との内積を計算する
    float t = dot(normal, hemLight.groundNormal);
    
	//内積の結果を0～1の範囲に変換
    t = (t + 1.0f) / 2.0f;
    
	//地面と天球色を補完率tで線形補完し、返す
    return lerp(hemLight.groundColor, hemLight.skyColor, t);
}

//ベックマン分布を計算する
float CalcBeckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

//フレネルを計算
float CalcSpcFresnel(float f0, float u)
{
    return f0 + (1 - f0) * pow(1 - u, 5);
}

/// <summary>
/// Cook-Torranceモデルの鏡面反射を計算
/// </summary>
/// <param name="L">光源に向かうベクトル</param>
/// <param name="V">視点に向かうベクトル</param>
/// <param name="N">法線ベクトル</param>
/// <param name="metallic">金属度</param>
float CookTorranceSpecular(float3 L, float3 V, float3 N, float smooth)
{
    // マイクロファセットが小さくなりすぎると、鏡面反射が強くなりすぎることがあるので、下限を0.5にした
    float microfacet = max(0.5f, 1.0f - smooth);

    // 金属度を垂直入射の時のフレネル反射率として扱う
    // 金属度が高いほどフレネル反射は大きくなる
    float f0 = 0.5;

    // ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);

    // 各種ベクトルがどれくらい似ているかを内積を利用して求める
    float NdotH = max(saturate(dot(N, H)), 0.001f);
    float VdotH = max(saturate(dot(V, H)), 0.001f);
    float NdotL = max(saturate(dot(N, L)), 0.001f);
    float NdotV = max(saturate(dot(N, V)), 0.001f);

    // D項をベックマン分布を用いて計算する
    float D = CalcBeckmann(microfacet, NdotH);

    // F項をSchlick近似を用いて計算する
    float F = CalcSpcFresnel(f0, VdotH);

    // G項を求める
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m項を求める
    float m = max(0.01f,PI * NdotV * NdotH);  

    // ここまで求めた、値を利用して、クックトランスモデルの鏡面反射を求める
    return max(F * D * G / m, 0.0);
}

/// <summary>
/// フレネル反射を考慮した拡散反射を計算
/// </summary>
/// <param name="N">法線</param>
/// <param name="L">光源に向かうベクトル。光の方向と逆向きのベクトル。</param>
/// <param name="V">視線に向かうベクトル。</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V, float smooth)
{
    // 光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);
    
    //粗さは0.5で固定。
    float roughness = 1.0f - smooth;
    
    //これはエネルギーの保存
    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

    // 光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める
    float dotLH = saturate(dot(L, H));
    // 光源に向かうベクトルとハーフベクトル、光が平行に入射したときの拡散反射量を求めている。
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;
    
    // 法線と光源に向かうベクトルｗを利用して拡散反射率を求めています
    float dotNL = saturate(dot(N, L));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    
    // 法線と視点に向かうベクトルを利用して拡散反射率を求めています
    float dotNV = saturate(dot(N, V));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    //法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
    // 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
    return (FL * FV * energyFactor);
}

float3 CalcPBR(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 direction, float3 color)
{
    // フレネル反射を考慮した拡散反射を計算
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -direction, toEye, smooth);

    // 正規化Lambert拡散反射を求める
    float NdotL = saturate(dot(normal, -direction));
    float3 lambertDiffuse = color * NdotL / PI;

    // 最終的な拡散反射光を計算する
    float3 diffuse = albedo * diffuseFromFresnel * lambertDiffuse;

    // クックトランスモデルの鏡面反射率を計算する
    float3 spec = CookTorranceSpecular(-direction, toEye, normal, smooth)* color;

    // 金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
    // スペキュラカラーの強さを鏡面反射率として扱う
    spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metallic);

    // 滑らかさを使って、拡散反射光と鏡面反射光を合成する
    return max(float3(0.0f, 0.0f, 0.0f), diffuse * (1.0f - smooth) + spec * smooth);
}

// チェビシェフの不等式を利用して、影になる可能性を計算する。
float Chebyshev(float2 moments, float depth)
{
    if (depth <= moments.x)
    {
        return 0.0f;
    }
    // 遮蔽されているなら、チェビシェフの不等式を利用して光が当たる確率を求める
    float depth_sq = moments.x * moments.x;
    // このグループの分散具合を求める
    // 分散が大きいほど、varianceの数値は大きくなる
    float variance = moments.y - depth_sq;
    // このピクセルのライトから見た深度値とシャドウマップの平均の深度値の差を求める
    float md = depth - moments.x;
    // 光が届く確率を求める
    float lit_factor = variance / (variance + md * md);
    float lig_factor_min = 0.3f;
    // 光が届く確率の下限以下は影になるようにする。
    lit_factor = saturate((lit_factor - lig_factor_min) / (1.0f - lig_factor_min));
    // 光が届く確率から影になる確率を求める。
    return 1.0f - lit_factor;
}

// 影を落とすかどうかの計算
float CalcShadowPow(float isShadowReceiver, float3 worldPos)
{
    float shadow = 0.0f;
    
    //影を受けないなら
    if(isShadowReceiver == false)
    {
        return shadow;
    }
    //クリアカラーなら
    else if (worldPos.x == 10000.0f, worldPos.y == 10000.0f, worldPos.z == 10000.0f)
    {
        return shadow;
    }
    
    for (int cascadeIndex = 0; cascadeIndex < NUM_SHADOW_MAP; cascadeIndex++)
    {
        float4 posInLVP = mul(mLVP[cascadeIndex], float4(worldPos, 1.0f));
        float2 shadowMapUV = posInLVP.xy / posInLVP.w;
        float zInLVP = posInLVP.z / posInLVP.w;
        shadowMapUV *= float2(0.5f, -0.5f);
        shadowMapUV += 0.5f;
        
        // シャドウマップUVが範囲内か判定
        if (shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f
            && shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f
            && zInLVP < 0.98f && zInLVP > 0.02f)
        {
            // シャドウマップから値をサンプリング
            float4 shadowValue = shadowMap[cascadeIndex].Sample(Sampler, shadowMapUV);
            zInLVP -= 0.001f;
            float pos = exp(INFINITY * zInLVP);
            
            // ソフトシャドウ。
            shadow = Chebyshev(shadowValue.xy, pos);
                       
            break;
        }
    }
    
    return shadow;
}

/*!
 *@brief	GIライトをサンプリング
 *@param[in]	uv				uv座標
 *@param[in]	level           反射レベル
 */
float4 SampleReflectionColor(float2 uv, float level)
{
    int iLevel = (int) level;
    float4 col_0;
    float4 col_1;
    if (iLevel == 0)
    {
        col_0 = g_raytracingTexture[0].Sample(Sampler, uv);
        col_1 = g_raytracingTexture[1].Sample(Sampler, uv);
    }
    else if (iLevel == 1)
    {
        col_0 = g_raytracingTexture[1].Sample(Sampler, uv);
        col_1 = g_raytracingTexture[2].Sample(Sampler, uv);
    }
    else if (iLevel == 2)
    {
        col_0 = g_raytracingTexture[2].Sample(Sampler, uv);
        col_1 = g_raytracingTexture[3].Sample(Sampler, uv);
    }
    else if (iLevel == 3)
    {
        col_0 = g_raytracingTexture[3].Sample(Sampler, uv);
        col_1 = g_raytracingTexture[4].Sample(Sampler, uv);
    }

    return lerp(col_0, col_1, frac(level));
}


