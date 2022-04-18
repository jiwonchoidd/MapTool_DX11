#pragma once
#include "KShaderManager.h"
#include "KTextureManager.h"
struct PNCT_VERTEX
{
	KVector3 pos;
	KVector3 normal; // ����	
	KVector4 color;
	KVector2 tex;
	PNCT_VERTEX()
	{
		pos = { 1.0f,1.0f,1.0f };
		normal = { 0.0f,0.0f, 0.0f };
		color = { 1.0f,1.0f,1.0f,1.0f };
		tex = {0.0f,0.0f};
	}
};
struct BT_VERTEX
{
	KVector3 tangent; // ����
	KVector3 binormal; // ������
	BT_VERTEX()
	{
		tangent = { 0.0f,0.0f, 0.0f };
		binormal = { 0.0f,0.0f, 0.0f };
	}
};
struct CB_DATA
{
	KMatrix  matWorld;
	KMatrix  matView;
	KMatrix  matProj;
	KMatrix  matNormal;
};
struct CB_DATA_EX
{
	KVector4 vLightColor;
	KVector4 vLightPos;
	KVector4 vLightDir;
	KVector4 vCamPos;
	KVector4 vValue; // �ð� �� xyzw
};
namespace wrl = Microsoft::WRL;
class KObject
{
//������Ʈ ���� ���
public:
	KMatrix			m_matWorld;
	KMatrix			m_matView;
	KMatrix			m_matProj;
public:
	bool					m_bVisibility=true;
//��Ӱ��� ����
public:
	KObject*				m_pParent = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
public:
	UINT					m_iNumIndex = 0;
	UINT					m_iVertexSize = 0;
	CB_DATA					m_cbData;
	CB_DATA_EX				m_cbDataEX;
public:
	std::vector <PNCT_VERTEX>		m_VertexList;
	std::vector <BT_VERTEX>			m_BTList;
	std::vector <DWORD>				m_IndexList;
public:
	KShader*		m_pVS = nullptr;
	KShader*		m_pPS = nullptr;
	KShader*		m_pPS_Swaped = nullptr;
public://�ؽ��� ��ǻ���, ����ŧ����, �븻��
	KTexture*		m_pTexture_Diffuse = nullptr;
	KTexture*		m_pTexture_Specular = nullptr;
	KTexture*		m_pTexture_Normal = nullptr;
	D3D11_TEXTURE2D_DESC		m_TextureDesc;
public:
	wrl::ComPtr <ID3D11Buffer>	 	 m_pVertexBuffer;
	wrl::ComPtr <ID3D11Buffer>	 	 m_pVertexBTBuffer;
	wrl::ComPtr <ID3D11Buffer>		 m_pIndexBuffer;
	wrl::ComPtr <ID3D11Buffer>		 m_pConstantBuffer;
	wrl::ComPtr <ID3D11Buffer>		 m_pConstantBuffer_EX;
	wrl::ComPtr <ID3D11InputLayout>  m_pVertexLayout;
public:
	virtual void		SetMatrix(KMatrix* pMatWorld,
		KMatrix* pMatView, KMatrix* pMatProj);
	virtual void		SetMatrix_Billboard(KMatrix* pMatWorld,
		KMatrix* pMatView, KMatrix* pMatProj);
public:
	virtual bool		LoadShader(std::wstring vsFile, std::wstring psFile);
	virtual bool		LoadTexture(std::wstring tex1= L"", std::wstring tex2 = L"", std::wstring tex3 = L"");
public:
	virtual bool		SwapVisibility();
	virtual bool		SwapPSShader(KShader* pShader= nullptr);
public:
	virtual bool		CheckVertexData();
	virtual bool		CheckIndexData();
	virtual bool		CreateVertexData();
	virtual bool		CreateIndexData();
	virtual HRESULT		CreateConstantBuffer();
	virtual HRESULT		CreateVertexBuffer();
	virtual HRESULT		CreateIndexBuffer();
	virtual HRESULT		CreateVertexLayout();
public:
	virtual bool		CreateObject(std::wstring vsFile, std::wstring psFile,
						std::wstring tex1= L"", std::wstring tex2= L"", std::wstring tex3 = L"");
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		PreRender(ID3D11DeviceContext* pContext);
	virtual bool		Render(ID3D11DeviceContext* pContext);
	virtual bool		PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex = 0);
	virtual bool		Release();
	KObject();
};

