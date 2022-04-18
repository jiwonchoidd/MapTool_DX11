#pragma once
#include "K2DAsset.h"
//�浹 ������ � �Լ� ȣ������ ���� �ý���
using CollisionFunction = std::function<void(KCollider*, DWORD)>;

//���콺 ������ ��ư ������ ó���ϴ� �ý���
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
	//������ ���ǿ� �ش��� �� �ְԲ�
	typedef std::map<int, CollisionFunction>::iterator FuncionIterator;
	std::map<int, CollisionFunction> m_fnCollisionExecute;
	typedef std::map<int, SelectFunction>::iterator FuncionIterator;
	std::map<int, SelectFunction>	m_fnSelectExecute;

public:
	//�浹�� �߻��ϸ� � �Լ��� ȣ���ұ� ����
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

