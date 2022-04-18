#pragma once
#include "KObject.h"
//Ŭ���� �������� �ߺ��̵Ǿ ����̾���
//��������� ����
//enum vs enum class
enum KCollisionType
{
	Block = 0,
	Overlap,
	Ignore,
};
enum KSelectType
{
	Select_Block = 0,
	Select_Overlap,
	Select_Ignore,
};
enum KSelectState // ���콺 ������ ���°�
{
	S_DEFAULT	= 0, // �⺻ ����, Ŀ���� ���� ������
	S_HOVER		= 1, // Ŀ���� ���� ������
	S_FOCUS		= 2, // S_ACTIVE ���¿��� �ٸ� ���� ��������
	S_ACTIVE	= 4, // ���콺 ���� ��ư ������ ������
	S_SELECTED	= 8, // S_ACTIVE ���Ŀ� ���� ������ ���� ��ư ��������
};
struct Rt_Size
{
	float width;
	float height;
};
class KCollider : public KObject
{
public:
	int					m_ID;
	std::wstring		m_Name;
	DWORD				m_CollisonType;

public:
	bool				m_bSelect;
	int					m_SelectID;
	DWORD				m_SelectType;
	DWORD				m_SelectState;
	DWORD				m_PreSelectState;
public:
	KRect				m_rtColl;
	KVector2			m_pos;
	int					m_blockstate;
	KVector2			m_dir;
	Rt_Size				m_rtSize; //������ ����ü
public:
	virtual void	ObjectOverlap(KCollider* pObj, DWORD dwState);
	virtual void	SelectOverlap(KCollider* pObj, DWORD dwState);
	virtual void	SetCollisionType(DWORD colltype, DWORD selecttype)
	{
		m_CollisonType = colltype;
		m_SelectType = selecttype;
	}

public:

	KCollider()
	{
		m_rtSize = { 0,0 };
		m_ID = -1;
		m_SelectID = -1;
		m_pos = {0,0};
		m_bSelect = false;
		m_CollisonType = Ignore;
		m_SelectType = Select_Ignore;
		m_SelectState= S_DEFAULT;
		m_Name = L"Undefined";
	}
};

