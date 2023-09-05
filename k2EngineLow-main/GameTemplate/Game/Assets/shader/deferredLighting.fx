/*!
 * @brief	スプライト用のシェーダー
 */

cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
};
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

Texture2D<float4> albedoTexture : register(t0);

sampler Sampler : register(s0);

PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}
float4 PSMain(PSInput In) : SV_Target0
{
	//GBufferの内容を使ってライティング
    float4 albedo = albedoTexture.Sample(Sampler, In.uv);
	
    float4 finalColor = albedo;
	
    return finalColor;
}