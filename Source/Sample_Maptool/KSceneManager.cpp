#include "KSceneManager.h"
bool KSceneManager::Init(ID3D11DeviceContext* context)
{
	m_pContext = context;

	//첫번째 씬은 인트로 설정한다. 
	m_pCurrentScene = new KScene_Maptool;
	m_CurrentScene_Index = S_MapTool;
	m_pCurrentScene->Init(m_pContext);
	m_pCurrentScene->Load(L"noload");
	return true;
}

KScene* KSceneManager::GetScene()
{
	return m_pCurrentScene;
}

bool KSceneManager::SetScene(BYTE index)
{
	//씬 해제 초기화
	if (m_pCurrentScene != nullptr)
	{
		//오브젝트 리스트 삭제
		g_ObjManager.Release();
		if (m_pCurrentScene != nullptr)
		{
			m_pCurrentScene->Release();
			delete m_pCurrentScene;
			m_pCurrentScene = nullptr;
		}
	}

	//상태 패턴
	switch (index)
	{
		case S_MapTool:
		{
			m_pCurrentScene = new KScene_Maptool;
			m_CurrentScene_Index = S_MapTool;
			break;
		}
		default:
			break;
		}
	m_pCurrentScene->Init(m_pContext);
	m_pCurrentScene->Load(L"");

	return true;
}

bool KSceneManager::Frame()
{
	m_pCurrentScene->Frame();
	return true;
}

bool KSceneManager::Render()
{
	m_pCurrentScene->Render();
	return true;
}

bool KSceneManager::Release()
{
	if (m_pCurrentScene != nullptr)
	{
		m_pCurrentScene->Release();
		delete m_pCurrentScene;
		m_pCurrentScene = nullptr;
		return true;
	}
	return true;
}

KSceneManager::KSceneManager()
{
	m_pCurrentScene = nullptr;
	m_CurrentScene_Index = -1;
	m_pContext = nullptr;
	m_BGM = nullptr;
}
