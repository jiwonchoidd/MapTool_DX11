Texture2D g_txDiffuse : register(t0);
SamplerState g_Sampler : register(s0);
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
float4 PS(VS_OUTPUT Input ) : SV_TARGET
{
	return g_txDiffuse.Sample(g_Sampler, Input.t);//v.c;
}