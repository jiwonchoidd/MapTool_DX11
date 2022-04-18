cbuffer CBuf
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
	matrix g_matNormal : packoffset(c12);
	float4 g_lightPos : packoffset(c16);	//라이트 방향
	float4 g_lightColor : packoffset(c17);  //라이트 색상
	float4 g_camPos : packoffset(c18);		//카메라 방향
	float4 g_value : packoffset(c19);		//기타 시간 값등
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
	float3 mLightDir : TEXCOORD1; //방향
	float3 mViewDir : TEXCOORD2; //방향
	float3 mT        : TEXCOORD3;
	float3 mB        : TEXCOORD4;
	float3 mN        : TEXCOORD5;
};

VS_OUTPUT VS(VS_INPUT Input) 
{
	//난반사광의 계산 동일한 계산을 PS VS 둘다 할 수 있음. 
   //하지만 픽셀단위 계산보다 정점단위 계산이 더 연산량이 적음
	VS_OUTPUT Output = (VS_OUTPUT)0;
	float4 vLocal = float4(Input.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);

	//라이트 방향 월드 행렬 곱함, 월드 공간에서의 위치여서 여기서 광원의 위치를 뺀다.
	float3 lightDir = vWorld.xyz - g_lightPos.xyz;
	Output.mLightDir= normalize(lightDir);
	//보는 방향
	float3 viewDir = vWorld.xyz - g_camPos.xyz;
	Output.mViewDir = normalize(viewDir);
	//

	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	float3 worldTangent = mul(Input.mTangent, (float3x3)g_matWorld);
	float3 worldBinormal = mul(Input.mBinormal, (float3x3)g_matWorld);
	float3 worldNormal = mul(Input.n, (float3x3)g_matWorld);
	Output.p = vProj;
	Output.t = Input.t;
	Output.c = Input.c;
	Output.mT = normalize(worldTangent);
	Output.mB = normalize(worldBinormal);
	Output.mN = normalize(worldNormal);

	return  Output;
}
