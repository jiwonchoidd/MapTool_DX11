#pragma once
#include "K2DAsset.h"
//간단한 obj 파일 로드 클래스
//obj: 버텍스나 노말같은 것들이나 UV 데이터같은 기본적인 정보만 저장되는 포맷

class KObjObject : public K2DAsset
{
private:
    std::vector<KVector3> m_vlist;
    std::vector<KVector2> m_vtlist;
    std::vector<KVector3> m_vnlist;
    std::vector<int> m_vertexindex;//f값 저장
    std::vector<int> m_uvindex;//f값 저장
    std::vector<int> m_normalindex;//f값 저장
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

