#pragma once
#include "KUIModelManager.h"
#include "KSoundManager.h"
#include "KObjectManager.h"

// 상속 관계 KObject->KCollider->K2DAsset->KUIModel->KUI
class KUI : public KUIModel
{
private:
    KUIModel* Clone()
    {
        KUIModel* copy = new KUI(*this);
        copy->CreateVertexBuffer();
        copy->CreateIndexBuffer();
        copy->CreateConstantBuffer();
        copy->CreateVertexLayout();
        return copy;
    }
    void  UpdateData() override
    {
        m_rtColl = KRect(m_pos, m_rtSize.width, m_rtSize.height);
        SetCollisionType(KCollisionType::Ignore,KSelectType::Select_Overlap);
        m_matWorld._41 = m_pos.x;
        m_matWorld._42 = m_pos.y;
        g_ObjManager.AddCollisionExecute(this,
            std::bind(&KCollider::ObjectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
        g_ObjManager.AddSelectExecute(this,
            std::bind(&KCollider::SelectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
    }

public:
    virtual bool Init(ID3D11DeviceContext* context, std::wstring vs,
        std::wstring ps, std::wstring tex = nullptr, std::wstring mask = nullptr);
    virtual bool Frame()override;
    virtual bool Render(ID3D11DeviceContext* context)override;

    // 전체크기 이미지 깨짐 방지 위해 UI는 9등분으로 나눔,
    bool SetVertexData()override;
    bool SetIndexData()override;
public:
    KUI();
    ~KUI();
};

class KImage : public KUI
{

private:
    KUIModel* Clone()
    {
        KImage* copy = new KImage(*this);
        copy->CreateVertexBuffer();
        copy->CreateIndexBuffer();
        copy->CreateConstantBuffer();
        copy->CreateVertexLayout();

        return copy;
    }
    void  UpdateData()
    {
        m_rtColl = KRect(m_pos, m_rtSize.width, m_rtSize.height);
        m_matWorld._41 = m_pos.x;
        m_matWorld._42 = m_pos.y;
        g_ObjManager.AddCollisionExecute(this,
            std::bind(&KCollider::ObjectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
        g_ObjManager.AddSelectExecute(this,
            std::bind(&KCollider::SelectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
    }
public:
    bool    m_bImgHover = false;
public:  
    virtual bool    Frame()override;
    virtual void    FadeIn();
    virtual void    MoveIMG();
    virtual void    FadeOut();
    virtual void	SelectOverlap(KCollider* pObj, DWORD dwState) override;
};

class KButton : public KUI
{
private:
    KUIModel* Clone()
    {
        KButton* copy = new KButton(*this);
        copy->CreateVertexBuffer();
        copy->CreateIndexBuffer();
        copy->CreateConstantBuffer();
        copy->CreateVertexLayout();

        return copy;
    }
    void  UpdateData()
    {
        m_rtColl = KRect(m_pos, m_rtSize.width, m_rtSize.height);
        m_matWorld._41 = m_pos.x;
        m_matWorld._42 = m_pos.y;
        g_ObjManager.AddCollisionExecute(this,
            std::bind(&KCollider::ObjectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
        g_ObjManager.AddSelectExecute(this,
            std::bind(&KCollider::SelectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
    }
public:
    virtual void	SelectOverlap(KCollider* pObj, DWORD dwState) override;
    virtual bool    Frame()override;
};

class KListCtrlObject : public KUIModelComposite
{
public:
    KUIModel* Clone()
    {
        KUIModelComposite* pModel = new KListCtrlObject;
        std::list< KUIModel*>::iterator iter;
        for (iter = m_Components.begin(); iter != m_Components.end();
            iter++)
        {
            pModel->Add((*iter)->Clone());
        }
        return pModel;
    };
    bool  Create(int xCount, int yCount, std::wstring ui_name);
    void  UpdateData()
    {
        m_rtColl = KRect(m_pos, m_rtSize.width, m_rtSize.height);
        m_matWorld._41 = m_pos.x;
        m_matWorld._42 = m_pos.y;
        g_ObjManager.AddCollisionExecute(this,
            std::bind(&KCollider::ObjectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
        g_ObjManager.AddSelectExecute(this,
            std::bind(&KCollider::SelectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
    }
    virtual void	SelectOverlap(KCollider* pObj, DWORD dwState) override;
public:
    KListCtrlObject() {}
    virtual ~KListCtrlObject() {}
};