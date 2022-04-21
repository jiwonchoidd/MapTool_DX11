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
	float4 g_lightColor : packoffset(c0);  //����Ʈ ����
	float4 g_lightPos : packoffset(c1);		//����Ʈ ��ġ
	float4 g_lightDir : packoffset(c2);		//����Ʈ ����
	float4 g_camPos : packoffset(c3);		//ī�޶� ����
	float4 g_value : packoffset(c4);		//��Ÿ �ð� ����
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
	float4 mShadow	 : TEXCOORD3; //���� �� ������ �߰�
	float3 mT        : TEXCOORD4;
	float3 mB        : TEXCOORD5;
	float3 mN        : TEXCOORD6;
	float3 mR        : TEXCOORD7;
};

VS_OUTPUT VS(VS_INPUT Input)
{
	//���ݻ籤�� ��� ������ ����� PS VS �Ѵ� �� �� ����. 
   //������ �ȼ����� ��꺸�� �������� ����� �� ���귮�� ����
	VS_OUTPUT Output = (VS_OUTPUT)0;
	float4 vLocal = float4(Input.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);

	//���� ����
	float3 viewDir = vWorld.xyz - g_camPos.xyz;
	Output.mViewDir = normalize(viewDir);
	float3 lightDir = vWorld.xyz - g_lightPos.xyz;
	Output.mLightDir = normalize(lightDir);
	//������ ��İ�
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
	float3 reflection = reflect(vLightDir, vNormal); //�ݻ纤��
	specular = saturate(dot(reflection, -vViewDir));
	specular = pow(specular, 20.0f);
	return specular;
}

float3 Diffuse(float3 vNormal, float3 vLightDir)
{
	float3 diffuse = 0;
	diffuse = saturate(dot(vNormal, -vLightDir)); // ���帮��� ��ǻ��
	return diffuse;
}

Texture2D		g_txDiffuse : register(t0); //���ݻ�
Texture2D		g_txSpecular : register(t1);//���ݻ�
Texture2D		g_txNormal : register(t2); //�븻����
Texture2D		g_txShadow  : register(t3); //������
TextureCube	    g_txCubeMap : register(t6); //ȯ�����

SamplerState	g_Sample : register(s0);
SamplerState	 g_SamplerClamp : register(s1);;
float4 PS(VS_OUTPUT Input) : SV_TARGET
{
	//�ؽ��Ŀ��� �븻, ���� ��ǥ ���ؿ�
   float3 tangentNormal = g_txNormal.Sample(g_Sample, Input.t).xyz;
   tangentNormal = normalize(tangentNormal * 2 - 1);

   float3x3 TBN = float3x3(normalize(Input.mT), normalize(Input.mB),normalize(Input.mN));
   TBN = transpose(TBN);
   float3 worldNormal = mul(TBN, tangentNormal);
   //----------------------------------------------------------------------
   float4 albedo = g_txDiffuse.Sample(g_Sample, Input.t); //�˺��� �⺻ ���� �ؽ���
   //������ 
   float3 vShadowProj;
   vShadowProj.xy = Input.mShadow.xy / Input.mShadow.w;
   float shadow = g_txShadow.Sample(g_SamplerClamp, vShadowProj.xy);
   float depth = Input.mShadow.z * 1.0f / (1300.0f - 1.0f) + -1.0f / (1300.0f - 1.0f);
   if (shadow + 0.006f <= depth)
   {
	   //�׸���
	   albedo = albedo * float4(0.5f, 0.5f, 0.5f, 1.0f);
   }

   //���� ���� ����ȭ
   float3 lightDir = normalize(Input.mLightDir);
   float3 viewDir = normalize(Input.mViewDir);

   //��ǻ�� �ݻ�, ���ݻ�
   float3 diffuse = Diffuse(worldNormal, lightDir);
   diffuse = g_lightColor.rgb * albedo.rgb * diffuse;

   //����ŧ�� �ݻ�, ���ݻ�
   float3 specular = 0;
   if (diffuse.x > 0.0f)
   {
	  specular = Specular(worldNormal, viewDir, lightDir);
	  float4 specularInten = g_txSpecular.Sample(g_Sample, Input.t);
	  specular *= specularInten.rgb * g_lightColor;
   }
   float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;
   float4 final = float4(ambient + diffuse + specular, albedo.w);

   //���� �׽��� �۾� (���� ����� ���� �������϶� ���)
   //������ �����ϱ� ����ﶧ �ָ��� ���İ��� ����, ���ĵ� ȿ���� ����
   if (final.a < 0.5f)
   {
	   discard;
   }
   return final;
}
float4 PSDepth(VS_OUTPUT Input) : SV_TARGET
{
	return Input.c;
}