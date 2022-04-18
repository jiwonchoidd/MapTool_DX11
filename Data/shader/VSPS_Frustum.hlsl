cbuffer CBuf
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
	matrix g_matNormal : packoffset(c12);
};
struct VS_INPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c	: COLOR;
	float2 t	: TEXCOORD;

	float3 mTangent	: TANGENT;
	float3 mBinormal : BINORMAL;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0; //πÊ«‚
	float2 t : TEXCOORD0;
	float3 r  : TEXCOORD1;
};
VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;
	float4 vLocal = float4(Input.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	Output.p = vProj;
	Output.c = Input.c;
	Output.t = Input.t;
	Output.r = normalize(vLocal.xyz);
	return Output;
}
float4 PS(VS_OUTPUT Input) : SV_TARGET
{
	float4 vColor = Input.c;
	return vColor;
}