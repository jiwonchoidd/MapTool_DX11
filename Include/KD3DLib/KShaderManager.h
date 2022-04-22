#pragma once
#include "KManager.h"
#include <d3dcompiler.h>
class KShader
{
public:
	int				m_iIndex;
	std::wstring	m_Name;
	std::wstring	m_szMsg;
	TCHAR			m_szBuffer[256];
	wrl::ComPtr<ID3D11VertexShader> m_pVertexShader;
	wrl::ComPtr<ID3D11PixelShader>	m_pPixelShader;
	wrl::ComPtr<ID3DBlob>  m_pVSCodeResult = nullptr;
	wrl::ComPtr<ID3DBlob>  m_pPSCodeResult = nullptr;
	wrl::ComPtr<ID3DBlob>  m_pErrorMsgs = nullptr;
public:
	wrl::ComPtr<ID3D11ComputeShader> m_pComputeShader;
	wrl::ComPtr<ID3DBlob> m_pCSCodeResult = nullptr;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	virtual bool    CreateVertexShader(std::wstring filename, std::string entry = "VS");
	virtual bool    CreatePixelShader(std::wstring filename, std::string entry = "PS");
	virtual bool    CreateComputeShader(std::wstring filename, std::string entry = "CS");
	HRESULT	Load(std::wstring vs=L"", std::wstring ps= L"");
	virtual ID3DBlob* LoadShaderBlob(std::wstring vs, std::string function, std::string version);
public:
	KShader();
	virtual ~KShader();
};
class KShaderManager : public KManager<KShader, KShaderManager>
{
	friend class Singleton<KShaderManager>;
public:
	KShader* CreateVertexShader(std::wstring filename, std::string entry = "VS");
	KShader* CreatePixelShader(std::wstring filename, std::string entry = "PS");
	KShader* CreateComputeShader(std::wstring filename, std::string entry = "CS");
private:
	KShaderManager();
public:
	~KShaderManager();
};
#define g_ShaderManager KShaderManager::Get()
