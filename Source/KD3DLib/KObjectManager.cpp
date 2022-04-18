#include "KObjectManager.h"

void KObjectManager::AddCollisionExecute(KCollider* owner, CollisionFunction func)
{
	KCollider* temp = (KCollider*)owner;
	temp->m_ID = m_iExcueteCollisionID++;
	//아이디 객체 
	m_ObjectList.insert(std::make_pair(temp->m_ID, temp));
	//아이디 함수
	m_fnCollisionExecute.insert(std::make_pair(temp->m_ID, func));
}

void KObjectManager::DeleteExecute(KCollider* owner, CollisionFunction func)
{
	KCollider* temp = (KCollider*)owner;
	std::map<int, KCollider*>::iterator iter;

	iter = m_ObjectList.find(temp->m_ID);
	if (iter != m_ObjectList.end())
	{
		m_ObjectList.erase(iter);
	}
	FuncionIterator colliter = m_fnCollisionExecute.find(owner->m_ID);
	if (colliter != m_fnCollisionExecute.end())
	{
		m_fnCollisionExecute.erase(colliter);
	}
}

//ui select
void KObjectManager::AddSelectExecute(KCollider* owner, CollisionFunction func)
{
	KCollider* temp = (KCollider*)owner;
	temp->m_SelectID = m_iExcueteSelectID++;
	//아이디 객체 
	m_SelectList.insert(std::make_pair(temp->m_SelectID, temp));
	//아이디 함수
	m_fnSelectExecute.insert(std::make_pair(temp->m_SelectID, func));
}

void KObjectManager::DeleteSelectExecute(KCollider* owner, CollisionFunction func)
{
	KCollider* temp = (KCollider*)owner;
	std::map<int, KCollider*>::iterator iter;
	iter = m_SelectList.find(owner->m_SelectID);
	if (iter != m_SelectList.end())
	{
		m_SelectList.erase(iter);
	}

	FuncionIterator colliter = m_fnSelectExecute.find(owner->m_SelectID);
	if (colliter != m_fnSelectExecute.end())
	{
		m_fnSelectExecute.erase(colliter);
	}
}

bool KObjectManager::Init()
{
	return true;
}

bool KObjectManager::Frame()
{
	//coll 
	for (auto src : m_ObjectList)
	{
		KCollider* pObjSrc = (KCollider*)src.second;
		//ignore 형태의 콜라이더를 무시한다.
		if (pObjSrc->m_CollisonType == KCollisionType::Ignore) continue;
		DWORD dwState = KCollisionType::Overlap;
		for (auto dest : m_ObjectList)
		{
			KCollider* pObjDest = (KCollider*)dest.second;
			if (pObjSrc == pObjDest) continue;
			if (KCollision::ToRect(pObjSrc->m_rtColl,(pObjDest->m_rtColl)))
			{
				FuncionIterator colliter = m_fnCollisionExecute.find(pObjSrc->m_ID);
				if (colliter != m_fnCollisionExecute.end())
				{
					CollisionFunction call = colliter->second;
					call(pObjDest, dwState);
				}
			}
		}
	}
	//mouse select 
	for (auto src : m_SelectList)
	{
		KCollider* pObjSrc = (KCollider*)src.second;
		DWORD dwState = KSelectState::S_DEFAULT;

		if (pObjSrc->m_SelectType != KSelectType::Select_Ignore &&
			KCollision::RectToPoint(pObjSrc->m_rtColl,
			g_InputData.iMousePos[0], g_InputData.iMousePos[1]))
		{
			BYTE mouseState = g_InputData.bMouseState[0];
			pObjSrc->m_SelectState = KSelectState::S_HOVER;
			if (mouseState == 2)//push
			{
				pObjSrc->m_SelectState = KSelectState::S_ACTIVE;
			}
			if (mouseState == 3)//hold
			{
				pObjSrc->m_SelectState = KSelectState::S_FOCUS;
			}
			if (mouseState == 1)//keyup
			{
				pObjSrc->m_SelectState = KSelectState::S_SELECTED;
			}

			FuncionIterator iter = m_fnSelectExecute.find(pObjSrc->m_SelectID);

			if (iter != m_fnSelectExecute.end())
			{
				CollisionFunction call = iter->second;
				call(pObjSrc, dwState);
			}
		}
		else
		{
			if (pObjSrc->m_SelectState != KSelectState::S_DEFAULT)
			{
				pObjSrc->m_SelectState = KSelectState::S_DEFAULT;
				FuncionIterator colliter = m_fnSelectExecute.find(pObjSrc->m_SelectID);
				if (colliter != m_fnSelectExecute.end())
				{
					CollisionFunction call = colliter->second;
					call(pObjSrc, dwState);
				}
			}
		}
	}
	return true;
}

bool KObjectManager::Release()
{
	if (!m_ObjectList.empty())
	m_ObjectList.clear();
	if(!m_SelectList.empty())
	m_SelectList.clear();
	m_iExcueteSelectID = 0;
	m_iExcueteCollisionID = 0;
	return true;
}
