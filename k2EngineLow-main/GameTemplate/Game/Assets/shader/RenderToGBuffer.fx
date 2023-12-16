// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    SSkinVSIn skinVert; //スキン用のデータ。
    float3 tangent : TANGENT;
    float3 biNomal : BINORMAL;
};

//ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION; //座標。
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 worldPos : TEXCOORD1;
    float3 normalInView : TEXCOORD2;
    float3 tangent : TANGENT;
    float3 biNomal : BINORMAL;
};

// ピクセルシェーダーからの出力
struct SPSOut
{
    float4 albedo : SV_Target0;                 // アルベド
    float4 normal : SV_Target1;                 // 法線
    float3 worldPos : SV_Target2;               // ワールド座標
    float3 normalInView : SV_Target3;           // カメラ空間の法線
    float4 metallicSmooth : SV_Target4;         // メタリックスムース
};


//シェーダーリソース
Texture2D<float4> g_albedo : register(t0);              // アルベドマップ
Texture2D<float4> g_normalMap : register(t1);           // 法線マップ
Texture2D<float4> g_metallicSmoothMap : register(t2);   // メタリックスムース(スペキュラ)マップ。rにメタリック、aにスムース
StructuredBuffer<float4x4> g_boneMatrix : register(t3); // ボーン行列

//サンプラーステート
sampler g_sampler : register(s0);

float3 CalcNormal(SPSIn psIn);

//スキン行列を計算する。
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

//モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    float4x4 m;
    if (hasSkin)
    {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else
    {
        m = mWorld;
    }

    psIn.pos = mul(m, vsIn.pos);                    // モデルの頂点をワールド座標系に変換
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);                // ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos);                // カメラ座標系からスクリーン座標系に変換
    psIn.normal = normalize(mul(m, vsIn.normal));
    psIn.normalInView = mul(mView, psIn.normal);    // カメラ空間の法線を求める
    psIn.tangent = normalize(mul(m, vsIn.tangent)); // 接ベクトルをワールド空間に変換する
    psIn.biNomal = normalize(mul(m, vsIn.biNomal)); // 従ベクトルをワールド空間に変換する

    psIn.uv = vsIn.uv;
    
    return psIn;
}

// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
// スキンありメッシュの頂点シェーダーのエントリー関数。
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}


//モデル用のピクセルシェーダーのエントリーポイント
SPSOut PSMainCore(SPSIn psIn, uniform bool isShadowReceiver)
{
    //GBufferに出力
    SPSOut psOut;
    
    //アルベドカラーの抽出
    psOut.albedo = g_albedo.Sample(g_sampler, psIn.uv);
    
    //法線マップによる法線情報の抽出
    psOut.normal.xyz = CalcNormal(psIn);
    
    //影を落とされるだけかを抽出
    if (isShadowReceiver == true)
    {
        psOut.normal.w = 1.0f;
    }
    else
    {
        psOut.normal.w = 0.0f;
    }
    
    //ワールド座標の抽出
    psOut.worldPos = psIn.worldPos;
    
    //カメラ空間の法線の抽出
    psOut.normalInView = psIn.normalInView;
    
    //メタリックスムースマップ(スペキュラマップ)の抽出
    psOut.metallicSmooth = g_metallicSmoothMap.Sample(g_sampler, psIn.uv);
    
    
    return psOut;
}

//影を落とさないし受けない
SPSOut PSMain(SPSIn psIn) : SV_Target0
{
    return PSMainCore(psIn, false);
}

//影を落とすし落とされるほう            シャドウレシーバー(影を落とさず受けるだけの方)のエントリーポイント
SPSOut PSShadowMain(SPSIn psIn) : SV_Target0
{
    return PSMainCore(psIn, true);
}


////ココから関数/////////////////////////////////////////////////////////////////////

float3 CalcNormal(SPSIn psIn)
{
    // 法線マップからタンジェントスペースの法線をサンプリングする
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    localNormal = (localNormal - 0.5f) * 2.0f;
    
    // タンジェントスペースの法線をワールドスペースに変換する
    float3 normal = psIn.tangent * localNormal.x + psIn.biNomal * localNormal.y + psIn.normal * localNormal.z;
    
    // 出力は0～1に丸められてしまいマイナスの値が失われてしまうので-1～1を0～1に変換する
    normal = (normal / 2.0f) + 0.5f;

    return normal;
}