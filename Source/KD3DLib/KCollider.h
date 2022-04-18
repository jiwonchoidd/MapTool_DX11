#pragma once
#include "KObject.h"
//클래스 열거형은 중복이되어도 상관이없음
//명시적으로 가능
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
enum KSelectState // 마우스 선택의 상태값
{
	S_DEFAULT	= 0, // 기본 상태, 커서가 위에 없을때
	S_HOVER		= 1, // 커서가 위에 있을때
	S_FOCUS		= 2, // S_ACTIVE 상태에서 다른 곳에 놓았을때
	S_ACTIVE	= 4, // 마우스 왼쪽 버튼 누르고 있을때
	S_SELECTED	= 8, // S_ACTIVE 이후에 같은 곳에서 왼쪽 버튼 놓았을때
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
	Rt_Size				m_rtSize; //사이즈 구조체
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

