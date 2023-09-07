/*!
 * @brief	スプライト用のシェーダー
 */
static const int MAX_DIRECTION_LIGHT = 4;

// 構造体
struct DirectionLight
{
    float3 dirDirection;    //ディレクションライトの方向
    float3 dirColor;        //ディレクションライトのカラー
};


cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
};

cbuffer LightCb : register(b1)
{
    DirectionLight directionLight[MAX_DIRECTION_LIGHT];
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
Texture2D<float4> albedoTexture : register(t0); // アルベド
Texture2D<float4> normalTexture : register(t1); // 法線
sampler Sampler : register(s0);                 // サンプラー

// 関数定義
float3 CalcDirectionLight(float3 normal);

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
	// GBufferの内容を使ってライティング
    // アルベド
    float4 albedo = albedoTexture.Sample(Sampler, In.uv);
    // 法線
    float3 normal = normalTexture.Sample(Sampler, In.uv).xyz;
    normal = (normal * 2.0f) - 1.0f;

    // ディレクションライトの強さ計算
    float3 dirLig = CalcDirectionLight(normal);

    //ライトの光を計算し最終的なカラーを設定    
    float4 finalColor = albedo;
    finalColor.xyz *= dirLig;
	
    return finalColor;
}

float3 CalcDirectionLight(float3 normal)
{
    float3 lig;
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT;i++)
    {
        float t = max(0.0f, dot(normal, directionLight[i].dirDirection) * -1.0f);
        lig += directionLight[i].dirColor * t;        
    }
    
    return lig;
}