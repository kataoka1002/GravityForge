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
};

//ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION; //座標。
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

// ピクセルシェーダーからの出力
struct SPSOut
{
    float4 albedo : SV_Target0; // アルベド
};


//シェーダーリソース
Texture2D<float4> g_albedo : register(t0); //アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //ボーン行列。

//サンプラーステート
sampler g_sampler : register(s0);


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

    psIn.pos = mul(m, vsIn.pos);        // モデルの頂点をワールド座標系に変換
    psIn.pos = mul(mView, psIn.pos);    // ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos);    // カメラ座標系からスクリーン座標系に変換

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
SPSOut PSMain(SPSIn psIn)
{
    //GBufferに出力
    SPSOut psOut;
    
    //アルベドカラーの抽出
    psOut.albedo = g_albedo.Sample(g_sampler, psIn.uv);
    
    return psOut;
}
