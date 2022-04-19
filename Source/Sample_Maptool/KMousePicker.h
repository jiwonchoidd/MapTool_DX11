#pragma once
#include "KMapSpace.h"
#include "KDebugBoxRenderer.h"

class KMousePicker
{
public:
	enum ControlState
	{
		C_None,
		C_Height,
		C_Texture,
	};
	enum BrushState
	{
		B_UpBrush,
		B_DownBrush,
		B_SmoothBrush,
		B_FlattenBrush,
	};
public:
	int					 m_iControlState = C_None;
	int					 m_iBrushState = B_UpBrush;
public:					
	KTexture*			 m_pSeletedTex= nullptr;//IMGUI 인터페이스에서 선택당한 포인터 변수
	KMapObject*			 m_pSeletedObj= nullptr;
public:
	bool					m_bImgui; // UI 조작에는 레이, 조작이 되어서는 안됨
public:
	ID3D11DeviceContext* m_pContext;
	KMapSpace*			 m_pSpace;
	KCamera*			 m_pCamera;
public:
	KVector3				m_vIntersect;
	KBox					m_Sel_Box;
	KPlane					m_Plane[6];
	KDebugBoxRenderer		m_Sel_BoxRender;
	float					m_Sel_Brush_Size = 30.0f;
	float					m_Sel_Brush_Strenght= 50.0f;
public:
	bool IntersectTriangle(const KVector3& orig, const KVector3& dir, KVector3& v0, KVector3& v1, KVector3& v2, FLOAT* t, FLOAT* u, FLOAT* v);
	bool GetIntersectionBox(TRay ray, KBox box);
public:
	bool Init(ID3D11DeviceContext* pContext, KMapSpace* pSpace, KCamera* pCam);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	bool Map_HeightBrushType(KVector3 pos, float& height, float scale);
	bool Map_HeightControl(float HeightScale, float BrushSize);
	bool Map_HeightControl_MakeSameHeight(KNode* pNode);
	bool Map_TextureControl(float HeightScale, float BrushSize);
public:
	KMousePicker();
	~KMousePicker();
};

