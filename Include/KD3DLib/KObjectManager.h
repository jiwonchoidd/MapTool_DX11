#pragma once
#include "K2DAsset.h"
//충돌 났을때 어떤 함수 호출할지 결정 시스템
using CollisionFunction = std::function<void(KCollider*, DWORD)>;

//마우스 유아이 버튼 누를때 처리하는 시스템
using SelectFunction = std::function<void(KCollider*, DWORD)>;

class KObjectManager : public Singleton <KObjectManager>
{
private:
	int							m_iExcueteCollisionID;
	int							m_iExcueteSelectID;
	std::map<int, KCollider*>  m_ObjectList;
	std::map<int, KCollider*>  m_SelectList;
public:
	friend class Singleton<KObjectManager>;
public:
	//세가지 조건에 해당할 수 있게끔
	typedef std::map<int, CollisionFunction>::iterator FuncionIterator;
	std::map<int, CollisionFunction> m_fnCollisionExecute;
	typedef std::map<int, SelectFunction>::iterator FuncionIterator;
	std::map<int, SelectFunction>	m_fnSelectExecute;

public:
	//충돌이 발생하면 어떤 함수를 호출할까 결정
	void AddCollisionExecute(KCollider* owner, CollisionFunction func);
	void DeleteExecute(KCollider* owner, CollisionFunction func);

	void AddSelectExecute(KCollider* owner, CollisionFunction func);
	void DeleteSelectExecute(KCollider* owner, CollisionFunction func);
public:
	bool Init();
	bool Frame();
	bool Release();
private:
	KObjectManager() 
	{
		m_iExcueteCollisionID = 0;
		m_iExcueteSelectID = 0;
	};
public:
	virtual ~KObjectManager() {};
};
#define g_ObjManager KObjectManager::Get()

