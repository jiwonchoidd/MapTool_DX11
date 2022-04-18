#include "KScene.h"
#include "KState.h"
bool KScene::Init(ID3D11DeviceContext* context)
{
	m_pContext = context;
	return true;
}

bool KScene::Frame()
{
	for (auto obj : m_UIObj)
	{
		KObject* pObj = obj.get();
		if (pObj != nullptr)
		{
			pObj->Frame();
		}
	}
	for (auto obj : m_MapObj)
	{
		KObject* pObj = obj.get();
		if (pObj != nullptr)
		{
			pObj->Frame();
		}
	}
	return true;
}

bool KScene::Render()
{

	for (auto obj : m_MapObj)
	{
		KObject* pObj = obj.get();
		if (pObj != nullptr)
		{
			if (pObj->m_bVisibility)
			pObj->Render(m_pContext);
		}
	}
	ApplyDSS(m_pContext, KState::g_pDSS_Disabled);
	for (auto obj : m_UIObj)
	{
		KObject* pObj = obj.get();
		if (pObj != nullptr)
		{
			if(pObj->m_bVisibility)
			pObj->Render(m_pContext);
		}
	}
	ApplyDSS(m_pContext, KState::g_pDSS);
	return true;
}

bool KScene::Release()
{
	for (auto obj : m_UIObj)
	{
		if (obj!=nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
	}
	for (auto obj : m_MapObj)
	{
		if (obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
	}
	m_UIObj.clear();
	m_MapObj.clear();
	return true;
}

KScene::KScene()
{
	m_pContext = nullptr;
	m_SceneID = -1;
}

KScene::~KScene()
{
}
