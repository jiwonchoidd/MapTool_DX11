cbuffer CBuf : register(b0)
{ 
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
	matrix g_matNormal : packoffset(c12);
	float4 g_lightPos : packoffset(c16); //라이트 방향
	float4 g_lightColor : packoffset(c17); //라이트 색상
	float4 g_camPos : packoffset(c18); //카메라 방향
	float4 g_value : packoffset(c19); //기타 시간 값등
};

cbuffer cbDataShadow: register(b2)
{
	matrix g_matShadow	: packoffset(c0);
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float2 t : TEXCOORD0;
	float4 c : COLOR0;
	float3 mLightDir : TEXCOORD1; //방향
	float3 mViewDir : TEXCOORD2; //방향
	float3 mT        : TEXCOORD3;
	float3 mB        : TEXCOORD4;
	float3 mN        : TEXCOORD5;
};
Texture2D		g_txDiffuse : register(t0);
Texture2D		g_txSpecular : register(t1);
Texture2D		g_txNormal : register(t2);
SamplerState	g_Sample : register(s0);
float4 PS(VS_OUTPUT Input) : SV_TARGET
{
	//텍스쳐에서 노말, 법선 좌표 구해옴
   float3 tangentNormal = g_txNormal.Sample(g_Sample, Input.t).xyz;
   tangentNormal = normalize(tangentNormal * 2 - 1);

   float3x3 TBN = float3x3(normalize(Input.mT), normalize(Input.mB),normalize(Input.mN));
   TBN = transpose(TBN);
   float3 worldNormal = mul(TBN, tangentNormal);
   //디퓨즈 텍스쳐
   float4 albedo = g_txDiffuse.Sample(g_Sample, Input.t);
   float3 lightDir = normalize(Input.mLightDir);
   float3 diffuse = saturate(dot(worldNormal, -lightDir));
   diffuse = g_lightColor.rgb*albedo.rgb * diffuse;

   float3 specular = 0;

   if (diffuse.x > 0.0f)
   {
	  float3 reflection = reflect(lightDir, worldNormal);
	  float3 viewDir = normalize(Input.mViewDir);

	  specular = saturate(dot(reflection, -viewDir));
	  specular = pow(specular,30.0f);

	  //스페큘러 텍스쳐
	  float4 specularInten = g_txSpecular.Sample(g_Sample, Input.t);
	  specular *= specularInten.rgb* g_lightColor;
   }

   float3 ambient = float3(0.05f, 0.05f, 0.05f) * albedo;

   return float4(ambient + diffuse + specular, 1) * Input.c;
}
float4 PSDepth(VS_OUTPUT Input) : SV_TARGET
{
	return Input.c;
}
