#pragma once
#include "KObjectManager.h"
#include "KCamera.h"
class KScene
{
public:
	enum SceneID 
	{
		S_MapTool,
		S_Scene1,
		S_Scene2,
	};
public:
	BYTE				 m_SceneID;
	ID3D11DeviceContext* m_pContext;	// 다비이스 컨텍스트 객체
	std::vector<std::shared_ptr<KObject>> m_UIObj;
	std::vector<std::shared_ptr<KObject>> m_MapObj;
	
	using m_iter = std::vector<KObject*>::iterator;
public:
	//순수 가상 함수들
	virtual bool	Load(std::wstring file) = 0;
public:
	virtual bool	Init(ID3D11DeviceContext* context);
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	KScene();
	virtual ~KScene();
};

