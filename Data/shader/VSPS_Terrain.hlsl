cbuffer CBuf : register(b0)
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
	matrix g_matNormal : packoffset(c12);
};
cbuffer CBuf_Shadow: register(b2)
{
	matrix g_matShadow	: packoffset(c0);
};
cbuffer CBuf_Extra: register(b3)
{
	float4 g_lightColor : packoffset(c0);  //라이트 색상
	float4 g_lightPos : packoffset(c1);		//라이트 위치
	float4 g_lightDir : packoffset(c2);		//라이트 방향
	float4 g_camPos : packoffset(c3);		//카메라 방향
	float4 g_value : packoffset(c4);		//기타 시간 값등
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
	float4 c : COLOR0;
	float3 mViewDir  : TEXCOORD1;
	float3 mLightDir : TEXCOORD2;
	float4 mShadow	 : TEXCOORD3; //뎁스 맵 쉐도우 추가
	float3 mT        : TEXCOORD4;
	float3 mB        : TEXCOORD5;
	float3 mN        : TEXCOORD6;
	float3 mR        : TEXCOORD7;
};

VS_OUTPUT VS(VS_INPUT Input)
{
	//난반사광의 계산 동일한 계산을 PS VS 둘다 할 수 있음. 
    //하지만 픽셀단위 계산보다 정점단위 계산이 더 연산량이 적음
	VS_OUTPUT Output = (VS_OUTPUT)0;
	float4 vLocal = float4(Input.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);

	//보는 방향
	float3 viewDir = vWorld.xyz - g_camPos.xyz;
	Output.mViewDir = normalize(viewDir);
	float3 lightDir = vWorld.xyz - g_lightPos.xyz;
	Output.mLightDir = normalize(lightDir);
	//쉐도우 행렬곱
	Output.mShadow = mul(vWorld, g_matShadow);

	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	float3 worldTangent = mul(Input.mTangent, (float3x3)g_matWorld);
	float3 worldBinormal = mul(Input.mBinormal, (float3x3)g_matWorld);
	float3 worldNormal = mul(Input.n, (float3x3)g_matWorld);
	Output.p = vProj;
	Output.t = Input.t;
	float depth = vProj.z * 1.0f / (1300.0f - 1.0f) + -1.0f / (1300.0f - 1.0f);
	Output.c = float4(depth, depth, depth, 1);
	Output.mT = normalize(worldTangent);
	Output.mB = normalize(worldBinormal);
	Output.mN = normalize(worldNormal);
	return  Output;
}
float3 Specular(float3 vNormal, float3 vViewDir, float3 vLightDir)
{
	// Specular Lighting
	float3 specular = 0;
	float3 reflection = reflect(vLightDir, vNormal); //반사벡터
	specular = saturate(dot(reflection, -vViewDir));
	specular = pow(specular, 20.0f);
	return specular;
}

float3 Diffuse(float3 vNormal, float3 vLightDir)
{
	float3 diffuse = 0;
	diffuse = saturate(dot(vNormal, -vLightDir)); // 빛드리우는 디퓨즈
	return diffuse;
}

Texture2D		g_txDiffuse : register(t0); //난반사
Texture2D		g_txSpecular : register(t1);//정반사
Texture2D		g_txNormal : register(t2); //노말매핑
Texture2D		g_txShadow  : register(t3); //뎁스맵
TextureCube	    g_txCubeMap : register(t6); //환경매핑

Texture2D		g_txSubTex1 : register(t7); //텍스쳐 스프라이팅 텍스쳐
Texture2D		g_txSubTex2 : register(t8); //텍스쳐 스프라이팅 텍스쳐
Texture2D		g_txSubTex3 : register(t9); //텍스쳐 스프라이팅 텍스쳐
Texture2D		g_txSubTex4 : register(t10); //텍스쳐 스프라이팅 텍스쳐
Texture2D		g_txMapMask : register(t11); //텍스쳐 스프라이팅 텍스쳐

SamplerState	g_Sample : register(s0);
SamplerState	 g_SamplerClamp : register(s1);;
float4 PS(VS_OUTPUT Input) : SV_TARGET
{
	//텍스쳐에서 노말, 법선 좌표 구해옴
   float3 tangentNormal = g_txNormal.Sample(g_Sample, Input.t * 4).xyz;
   tangentNormal = normalize(tangentNormal * 2 - 1);

   float3x3 TBN = float3x3(normalize(Input.mT), normalize(Input.mB),normalize(Input.mN));
   TBN = transpose(TBN);
   float3 worldNormal = mul(TBN, tangentNormal);
   //----------------------------------------------------------------------
   float4 albedo = g_txDiffuse.Sample(g_Sample, Input.t); //알베도 기본 색상 텍스쳐
   float4 mapMask = g_txMapMask.Sample(g_Sample, Input.t /4); // 맵 마스크 작업

   float4 subTexture1 = g_txSubTex1.Sample(g_Sample, Input.t); // 맵 서브 텍스쳐
   float4 subTexture2 = g_txSubTex2.Sample(g_Sample, Input.t); // 맵 서브 텍스쳐
   float4 subTexture3 = g_txSubTex3.Sample(g_Sample, Input.t); // 맵 서브 텍스쳐
   float4 subTexture4 = g_txSubTex4.Sample(g_Sample, Input.t); // 맵 서브 텍스쳐
   
   albedo = lerp(albedo, subTexture1, mapMask.r); // 첫번째 레이어 작업
   albedo = lerp(albedo, subTexture2, mapMask.g); // 두번째 레이어 작업
   albedo = lerp(albedo, subTexture3, mapMask.b); // 세번째 레이어 작업
   albedo = lerp(albedo, subTexture4, mapMask.a); // 네번째 레이어 작업

   //쉐도우 
   float3 vShadowProj;
   vShadowProj.xy = Input.mShadow.xy / Input.mShadow.w;
   float shadow = g_txShadow.Sample(g_SamplerClamp, vShadowProj.xy);
   float depth = Input.mShadow.z * 1.0f / (1300.0f - 1.0f) + -1.0f / (1300.0f - 1.0f);
   if (shadow + 0.006f <= depth)
   {
	   //그림자
	   albedo = albedo * float4(0.5f, 0.5f, 0.5f, 1.0f);
   }

   //방향 벡터 정규화
   float3 lightDir = normalize(Input.mLightDir);
   float3 viewDir = normalize(Input.mViewDir);

   //디퓨즈 반사, 난반사
   float3 diffuse = Diffuse(worldNormal, lightDir);
   diffuse = g_lightColor.rgb * albedo.rgb * diffuse;

   //스페큘러 반사, 정반사
   float3 specular = 0;
   if (diffuse.x > 0.0f)
   {
	  specular = Specular(worldNormal, viewDir, lightDir);
	  float4 specularInten = g_txSpecular.Sample(g_Sample, Input.t);
	  specular *= specularInten.rgb * g_lightColor;
   }
   float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;
   float4 final = float4(ambient + diffuse + specular , albedo.a);

   return final;
}
float4 PSDepth(VS_OUTPUT Input) : SV_TARGET
{
	return Input.c;
}