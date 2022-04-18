cbuffer CBuf
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
	matrix g_matNormal : packoffset(c12);
	float4 g_lightPos : packoffset(c16);
	float4 g_lightColor : packoffset(c17);
	float4 g_camPos : packoffset(c18);
	float4 g_value : packoffset(c19);
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
	float2 t : TEXCOORD0;
	float3 mViewDir : TEXCOORD1;
	float3 mDiffuse  : TEXCOORD2;
	float3 mReflection: TEXCOORD3;
};

VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;
	float4 vLocal = float4(Input.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);

	//월드 공간
	float3 lightDir = vWorld.xyz- g_lightPos.xyz;
	lightDir = normalize(lightDir);

	float3 viewDir = vWorld.xyz - g_camPos.xyz;

	float3 worldNormal = mul(Input.n, (float3x3)g_matWorld);
	worldNormal = normalize(worldNormal);
	
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	Output.p = vProj;
	Output.t = Input.t;
	Output.mViewDir = viewDir;
	Output.mDiffuse = dot(-lightDir, worldNormal);
	Output.mReflection = reflect(lightDir, worldNormal);
	return  Output;
}
