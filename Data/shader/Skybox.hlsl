TextureCube		g_txSky : register(t0);
SamplerState	g_Sample : register(s0);

cbuffer CBuf: register(b0)
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
	matrix g_matNormal : packoffset(c12);
	float4 g_lightPos : packoffset(c16);	//����Ʈ ����
	float4 g_lightColor : packoffset(c17);  //����Ʈ ����
	float4 g_camPos : packoffset(c18);		//ī�޶� ����
	float4 g_value : packoffset(c19);		//��Ÿ �ð� ����
};

struct VS_INPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float3 t : TEXCOORD;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float3 t : TEXCOORD;	// float3��
};

VS_OUTPUT VS(VS_INPUT vIn) 
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;
	vOut.p			= mul( float4(vIn.p,1.0f), g_matWorld );
	vOut.p			= mul(vOut.p, g_matView );
	vOut.p			= mul(vOut.p, g_matProj );
	vOut.p			= vOut.p.xyww;

	vOut.c			= vIn.c;
	vOut.t			= normalize(vIn.p.xyz); 
	vOut.n			= normalize(mul(vIn.n, (float3x3)g_matWorld));	
    return vOut;
}


float4 PS(VS_OUTPUT input) : SV_TARGET
{	
	//�ؽ�ó���� t��ǥ�� �ش��ϴ� �÷���(�ȼ�) ��ȯ
	return g_txSky.Sample(g_Sample, input.t);
}