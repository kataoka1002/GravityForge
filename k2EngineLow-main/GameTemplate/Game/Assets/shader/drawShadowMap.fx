/*!
 * @brief シャドウマップ描画用のシェーダー
 */

static const int INFINITY = 40.0f;

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
    float4 pos : POSITION; // モデルの頂点座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD0; // UV座標
    SSkinVSIn skinVert; //スキン用のデータ。
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION; // スクリーン空間でのピクセルの座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD0; // uv座標
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0); // アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3); // ボーン行列
//Texture2D<float4> g_shadowMap : register(t10); // シャドウマップ
sampler g_sampler : register(s0); // サンプラーステート
StructuredBuffer<float4x4> g_worldMatrixArray : register(t10); //ワールド行列の配列。インスタンシング描画の際に有効。


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

/// <summary>
/// 頂点シェーダー
/// <summary>
SPSIn VSMainCore(SVSIn vsIn, bool hasSkin)
{
    // シャドウマップ描画用の頂点シェーダーを実装
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
    
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = mul(m, vsIn.normal);
    return psIn;
}

// インスタンシングモデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCoreInstancing(SVSIn vsIn, uint instanceId : SV_InstanceID)
{
    SPSIn psIn;
    
    float4x4 m = g_worldMatrixArray[instanceId];
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = mul(m, vsIn.normal);
    
    return psIn;
}

SPSIn VSMainCoreInstancingAnim(SVSIn vsIn, uint instanceId : SV_InstanceID)
{
    SPSIn psIn;
    float4x4 mWorldLocal = CalcSkinMatrix(vsIn.skinVert);
    mWorldLocal = mul(g_worldMatrixArray[instanceId], mWorldLocal);
    psIn.pos = mul(mWorldLocal, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = mul(mWorldLocal, vsIn.normal);
    
    return psIn;
}

/// 事前計算済みの頂点バッファを使う頂点シェーダーのエントリー関数。
SPSIn VSMainSkinUsePreComputedVertexBuffer(SVSIn vsIn)
{
    SPSIn psIn;
    
    psIn.pos = vsIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = mul((float4x4) 0, vsIn.normal);
    
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


/// <summary>
/// シャドウマップ描画用のピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    // シャドウマップにZ値を描き込む
    //return float4(psIn.pos.z, psIn.pos.z, psIn.pos.z, 1.0f);
    
    float depth = psIn.pos.z;
    float pos = exp(INFINITY * depth);
    return float4(pos, pos * pos, 0.0f, 1.0f);
}