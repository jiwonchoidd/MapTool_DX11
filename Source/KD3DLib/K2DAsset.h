#pragma once
#include "KCollider.h"
//KObject -> KCollider -> K2DAsset ���
class K2DAsset :  public KCollider
{
public:
	float				m_Speed;
	float				m_fAlpha = 1.0f;
	bool				m_bFadeIn = false;
	bool				m_bFadeOut = false;
	bool				m_bMoveImg = false;
	float				m_Timer;
public:
	RECT				m_rtSource;
	RECT				m_rtDraw;
	virtual	void		RegisterOverlap();
	virtual void		ObjectOverlap(KCollider* pObj, DWORD dwState);
	virtual void		SelectOverlap(KCollider* pObj, DWORD dwState);
public:
	virtual void		SetRectSource(RECT rt);
	virtual void		SetRectDraw(RECT rt);
	virtual void		AddPosition(KVector2 vPos);
	virtual void		SetPosition(KVector2 vPos);
	virtual void		UpdateRectDraw(RECT rt);
	virtual bool		CreateObject_Mask(std::wstring vsFile,
		std::wstring psFile, std::wstring tex=nullptr, std::wstring mask = nullptr);
	// ȭ����ǥ ��ġ�� �������� NDC ��ȯ
	virtual void	Convert(
		KVector2 center, float fWidth, float fHeight,
		std::vector<PNCT_VERTEX>& retList);
	// ȭ����ǥ�踦 NDC ��ȯ
	virtual void	Convert(std::vector<PNCT_VERTEX>& list,
		std::vector<PNCT_VERTEX>& retList);
	// ȭ����ǥ ��ġ�� �������� NDC ��ȯ
	virtual void	ConvertIndex(
		KVector2 center, float fWidth, float fHeight,
		std::vector<PNCT_VERTEX>& retList);
	// ȭ����ǥ�踦 NDC ��ȯ
	virtual void	ConvertIndex(std::vector<PNCT_VERTEX>& list,
		std::vector<PNCT_VERTEX>& retList);
	virtual void		SetUVcoord(std::vector<PNCT_VERTEX>& retList);
public:
	virtual bool		SetVertexData();
	virtual bool		SetIndexData();
	virtual bool		CheckVertexData() override;
public:
	K2DAsset();
	~K2DAsset();
};

