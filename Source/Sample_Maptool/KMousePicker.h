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
		C_Object,
	};
public:
	int					 m_iState = C_None;
public:
	ID3D11DeviceContext* m_pContext;
	KMapSpace*			 m_pSpace;
	KCamera*			 m_pCamera;
public:
	bool					m_bImgui; // UI 조작에는 높이가 들어가면 안됨
	KVector3				m_vIntersect;
	KBox					m_Sel_Box;
	KPlane					m_Plane[6];
	KDebugBoxRenderer		m_Sel_BoxRender;
	float					m_Sel_Brush_Size = 30.0f;
	float					m_Sel_Brush_Strenght= 50.0f;
public:
	bool IntersectTriangle(const KVector3& orig, const KVector3& dir, KVector3& v0, KVector3& v1, KVector3& v2, FLOAT* t, FLOAT* u, FLOAT* v);
public:
	bool Init(ID3D11DeviceContext* pContext, KMapSpace* pSpace, KCamera* pCam);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	bool Map_HeightControl(float HeightScale, float BrushSize);
	bool Map_HeightControl_MakeSameHeight(KNode* pNode);
public:
	KMousePicker();
	~KMousePicker();
};

