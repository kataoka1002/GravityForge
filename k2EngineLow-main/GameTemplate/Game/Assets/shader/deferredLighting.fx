/*!
 * @brief	スプライト用のシェーダー
 */
static const int MAX_DIRECTION_LIGHT = 4;
static const int MAX_POINT_LIGHT = 128;

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
Texture2D<float4> albedoTexture : register(t0);     // アルベド
Texture2D<float4> normalTexture : register(t1);     // 法線
Texture2D<float4> worldPosTexture : register(t2);   // ワールド座標
sampler Sampler : register(s0);                     // サンプラー

// 関数定義
float3 CalcLambertDiffuse(float3 direction, float3 color, float3 normal);
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal);
float3 CalcDirectionLight(float3 normal,float3 worldPos);
float3 CalcPointLight(float3 normal, float3 worldPos);

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
    normal = (normal * 2.0f) - 1.0f;
    
    // ワールド座標
    float3 worldPos = worldPosTexture.Sample(Sampler, In.uv).xyz;

    // --------------------------------------------------------------------
    
    
    // ディレクションライトの強さ計算
    float3 dirLig = CalcDirectionLight(normal, worldPos);
    
    // ポイントライトの強さ設定
    float3 ptLig = CalcPointLight(normal, worldPos);
    
    
    // 全てのライトの影響力を求める
    float3 lightPow = dirLig + ptLig;
    
    // ライトの光を計算し最終的なカラーを設定    
    float4 finalColor = albedo;
    finalColor.xyz *= lightPow;
	
    return finalColor;
}

//////ココから関数/////////////////////////////////////////////////////////////////////////////////////////////////////

//拡散反射の計算
float3 CalcLambertDiffuse(float3 direction,float3 color,float3 normal)
{
    float power = max(0.0f, dot(normal, direction) * -1.0f);
    
    float3 lig = color * power;
    
    return lig;
}

//鏡面反射の計算
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal)
{
    float3 toEye = normalize(eyePos - worldPos);
    
    float3 refVec = reflect(direction, normal);
    
    float power = max(0.0f, dot(toEye, refVec));
    power = pow(power, 5.0f);
    
    float3 lig = color * power;
    
    return lig;
}

//ディレクションライトの計算
float3 CalcDirectionLight(float3 normal, float3 worldPos)
{
    float3 lig;
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {
        lig += CalcLambertDiffuse(directionLight[i].dirDirection, directionLight[i].dirColor, normal);
        lig += CalcPhongSpecular(directionLight[i].dirDirection, directionLight[i].dirColor, worldPos, normal);
    }
    
    return lig;
}

//ポイントライトによる反射光を計算する
float3 CalcPointLight(float3 normal,float3 worldPos)
{
    float3 lig;
    
    for (int i = 0; i < 2; i++)
    {
     	// このサーフェイスに入射しているポイントライトの光の向きを計算する
        float3 ligDir = worldPos - pointLight[i].ptPosition;

        // 正規化して大きさ1のベクトルにする
        ligDir = normalize(ligDir);

        // 減衰なしのLambert拡散反射光を計算する
        float3 diffPoint = CalcLambertDiffuse(ligDir, pointLight[i].ptColor, normal);

        // 減衰なしのPhong鏡面反射光を計算する
        float3 specPoint = CalcPhongSpecular(ligDir, pointLight[i].ptColor, worldPos, normal);

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
        lig += diffPoint;
        lig += specPoint;
    }

    return lig;
}