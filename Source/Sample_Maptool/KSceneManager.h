#pragma once
#include "KScene_Maptool.h" // intro
#include "KSoundManager.h"
//씬 헤더 포함
class KSceneManager : public Singleton <KSceneManager>
{
public:
	enum SceneID
	{
		S_MapTool,
		S_Scene1,
		S_Scene2,
	};
public:
	friend class Singleton<KSceneManager>;
public:
	KScene*						m_pCurrentScene;
	KCamera*					m_pCamera;
	BYTE						m_CurrentScene_Index;
	ID3D11DeviceContext*		m_pContext;	// 디바이스 컨텍스트 객체
public:
	KSound*						m_BGM;
public:
	KScene* GetScene();
	bool	SetScene(BYTE index);
public:
	bool	Init(ID3D11DeviceContext* context);
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	KSceneManager();
};

#define g_SceneManager KSceneManager::Get()