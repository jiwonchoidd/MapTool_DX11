#include "KUIModelManager.h"

bool KUIModelComposite::Frame()
{
	//다수의 컴포넌트(ui)를 지닌 컴포지트 객체 반복문으로 Frame()
	std::list<KUIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		(*iter)->Frame();
	}
	return true;
}

bool KUIModelComposite::Render(ID3D11DeviceContext* context)
{
	//컴포지트 렌더
	std::list<KUIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		if((*iter)->m_bVisibility)
		(*iter)->Render(context);
	}
	return true;
}

bool KUIModelComposite::Release()
{
	std::list<KUIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		(*iter)->Release();
	}
	return true;
}

void KUIModelComposite::Add(KUIModel* pObj)
{
	//컴포지트 리스트에 추가
	m_Components.push_back(pObj);

	if (pObj->m_datalist.size()>1)
	{
		m_ButtonComponents.push_back(pObj);
	}
}

KUIModel* KUIModelComposite::Clone()
{
	KUIModelComposite* pModel = new KUIModelComposite;
	std::list< KUIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		pModel->Add((*iter)->Clone());

	}
	return pModel;
}

void KUIModelComposite::UpdateData()
{
	std::list< KUIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		(*iter)->UpdateData();
		(*iter)->SetCollisionType(KCollisionType::Ignore,
			KSelectType::Select_Overlap);
	}
}
