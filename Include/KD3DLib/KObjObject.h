#pragma once
#include "K2DAsset.h"
//������ obj ���� �ε� Ŭ����
//obj: ���ؽ��� �븻���� �͵��̳� UV �����Ͱ��� �⺻���� ������ ����Ǵ� ����

class KObjObject : public K2DAsset
{
private:
    std::vector<KVector3> m_vlist;
    std::vector<KVector2> m_vtlist;
    std::vector<KVector3> m_vnlist;
    std::vector<int> m_vertexindex;//f�� ����
    std::vector<int> m_uvindex;//f�� ����
    std::vector<int> m_normalindex;//f�� ����
public:
    std::vector<int> m_UVindexlist;
public:
    virtual bool Init(ID3D11DeviceContext* context, std::wstring vs,
        std::wstring ps, std::wstring tex = L"", std::wstring obj = L"");
    virtual bool Frame()override;
    virtual bool Render(ID3D11DeviceContext* context)override;

    bool ObjParse(std::wstring	objfile);
    bool SetVertexData()override;
    bool SetIndexData()override;
public:
    KObjObject();
    ~KObjObject();
};

