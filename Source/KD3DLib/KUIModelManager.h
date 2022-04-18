#pragma once
#include "KManager.h"
#include "K2DAsset.h"
#include "KSoundManager.h"
struct KUIData
{
	KTexture* pTex;
	KSound* pSound;

	KUIData()
	{
		pTex = nullptr;
		pSound = nullptr;
	}
	KUIData(KTexture* a, KSound* b)
	{
		pTex = a;
		pSound = b;
	}
};
class KUIModel : public K2DAsset
{
public:
public:
	public:
	    std::vector<KUIData> m_datalist;
	public:
	    RECT				m_rtOffset;
	    RECT				m_rtOffsetTex;
public:
	//모델을 복제해서 재활용
	//상속해서 사용하는 함수
	virtual KUIModel* Clone() { return nullptr; };
	virtual void  UpdateData() {}
public:
	bool	Load(std::wstring filename) 
	{
		return true;
	};
};

/// <summary>
/// Composite Pattern
/// 재활용하는 용도
/// UI의 프로토타입을 리스트로 두어복수개의 컴포넌트를 리스트로 엮음
/// </summary>
class KUIModelComposite: public KUIModel
{
public:
	std::list <KUIModel*> m_Components;
	std::vector <KUIModel*> m_ButtonComponents;
	int					  m_iSelected =0;
public:
	virtual bool		Frame();
	virtual bool		Render(ID3D11DeviceContext* context);
	virtual bool		Release();
	virtual void		Add(KUIModel* pObj);
	virtual KUIModel*	Clone();
	virtual bool		Load(std::wstring filename) 
	{
		return true;
	};
	virtual void		UpdateData();
};
class KUIModelManager : public KManager<KUIModel, KUIModelManager>
{
	friend class Singleton<KUIModelManager>;
private:
	KUIModelManager() {};
public:
	~KUIModelManager() {};
};
#define g_UIModelManager KUIModelManager::Get()

