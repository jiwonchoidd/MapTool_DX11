cbuffer CBuf: register(b0)
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
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float2 t : TEXCOORD0;
	float3 mViewDir : TEXCOORD1;
	float3 mDiffuse  : TEXCOORD2;
	float3 mReflection: TEXCOORD3;
};

Texture2D		g_txDiffuse : register(t0);
Texture2D		g_txSpecular : register(t1);
Texture2D		g_txNormal : register(t2);
SamplerState	g_Sample : register(s0);

float4 PS(VS_OUTPUT Input) : SV_TARGET
{
	//디퓨즈맵 생플링
   float4 albedo = g_txDiffuse.Sample(g_Sample, Input.t);
   // 픽셀이 반사하는 색깔 
   float3 diffuse = g_lightColor * albedo.rgb * saturate(Input.mDiffuse);

   float3 reflection = normalize(Input.mReflection);
   float3 viewDir = normalize(Input.mViewDir);
   float3 specular = 0;

   if (diffuse.x > 0.0f)
   {
	  specular = saturate(dot(reflection, -viewDir));
	  //20번 거듭제곱
	  specular = pow(specular,20.0f);

	  float4 specularInten = g_txSpecular.Sample(g_Sample, Input.t);
	  specular *= specularInten.rgb * g_lightColor;
   }

   float3 ambient = float3(0.1f, 0.1f, 0.1f);

   //return albedo;
   return float4(ambient + diffuse + specular,1);
}