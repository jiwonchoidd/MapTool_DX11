Texture2D<float4>		InputMap : register(t0);
Texture2D<float4>		CopyMap : register(t1);
RWTexture2D<float4>		OutputMap : register(u0);

struct CBuf_Brush
{
	float3 vPickPos;
	float3 vRect[4];
	float  g_fRadius;
	int    iIndex;
};
StructuredBuffer<CBuf_Brush> Buffer0 : register(t2);
// Group size
#define size_x 32
#define size_y 32
[numthreads(size_x, size_y, 1)]
void CS(uint3 GroupID : SV_GroupID, uint3 DispatchThreadID : SV_DispatchThreadID, uint3 GroupThreadID : SV_GroupThreadID, uint GroupIndex : SV_GroupIndex)
{
	int3 texturelocation = int3(0, 0, 0);
	// 0 ~ 1024, 1024
	texturelocation.x = GroupID.x * size_x + GroupThreadID.x; // u
	texturelocation.y = GroupID.y * size_y + GroupThreadID.y; // v
	//texturelocation.x = DispatchThreadID.x; //위랑 같음
	//texturelocation.y = DispatchThreadID.y;

	float4 Color = InputMap.Load(texturelocation);
	// 0 ~1 
	float2 uv = float2(texturelocation.x / 1280.0f, //현재 지형 크기 1280 고정
		texturelocation.y / 1280.0f);
	// vRect[0]   ~   vRect[1]  
	float1 width = (Buffer0[0].vRect[1].x - Buffer0[0].vRect[0].x) / 2.0f;
	//
	// vRect[3]   ~   vRect[2]  
	float1 height = (Buffer0[0].vRect[0].y - Buffer0[0].vRect[3].y) / 2.0f;
	float3 vPos = float3((uv.x * 2 - 1.0f) * width,
		-(uv.y * 2 - 1.0f) * height,
		0.0f);

	float fRadius = distance(vPos.xyz, Buffer0[0].vPickPos.xyz);
	//텍스쳐 복사한것
	float4 fAlpha = CopyMap.Load(texturelocation);
	float fDot = 1.0f - (fRadius / Buffer0[0].g_fRadius);

	//4개의 텍스쳐의 알파값을 저장한 텍스쳐
	switch (Buffer0[0].iIndex)
	{
		case 0: fAlpha.x = max(fAlpha.x, fDot); break;
		case 1: fAlpha.y = max(fAlpha.y, fDot); break;
		case 2: fAlpha.z = max(fAlpha.z, fDot); break;
		case 3: fAlpha.w = max(fAlpha.w, fDot); break;
	}
	OutputMap[texturelocation.xy] = float4(fAlpha.xyzw);
}