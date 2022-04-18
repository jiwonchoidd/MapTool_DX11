#include "KUIModelManager.h"

bool KUIModelComposite::Frame()
{
	//�ټ��� ������Ʈ(ui)�� ���� ������Ʈ ��ü �ݺ������� Frame()
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
	//������Ʈ ����
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
	//������Ʈ ����Ʈ�� �߰�
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
