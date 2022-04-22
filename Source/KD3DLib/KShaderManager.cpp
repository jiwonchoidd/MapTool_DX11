#include "KShaderManager.h"
HRESULT	KShader::Load(std::wstring vs, std::wstring ps)
{
	HRESULT hr = S_OK;
	ID3DBlob* error = nullptr;
	m_pVSCodeResult = LoadShaderBlob(vs, "VS", "vs_5_0");
	if (m_pVSCodeResult.Get() == nullptr)
	{
		return hr;
	}
	hr = g_pd3dDevice->CreateVertexShader(
		m_pVSCodeResult.Get()->GetBufferPointer(),
		m_pVSCodeResult.Get()->GetBufferSize(),
		NULL, m_pVertexShader.GetAddressOf());
	if (FAILED(hr)) return hr;

	m_pPSCodeResult = LoadShaderBlob(ps, "PS", "ps_5_0");
	if (m_pPSCodeResult.Get() == nullptr)
	{
		return hr;
	}
	hr = g_pd3dDevice->CreatePixelShader(
		m_pPSCodeResult.Get()->GetBufferPointer(),
		m_pPSCodeResult.Get()->GetBufferSize(),
		NULL, m_pPixelShader.GetAddressOf());
	if (FAILED(hr)) return hr;

	return hr;
}

ID3DBlob* KShader::LoadShaderBlob(std::wstring vsps, std::string function, std::string version)
{
	HRESULT hr = S_OK;
	ID3DBlob* ret = nullptr;
	ID3DBlob* error = nullptr;
	hr = D3DCompileFromFile(
		vsps.c_str(),
		nullptr,
		nullptr,
		function.c_str(),
		version.c_str(),
		0,
		0,
		&ret,
		&error);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)error->GetBufferPointer(),
			"error", MB_OK);
		return ret;
	}
	return ret;
}

bool	KShader::Init()
{
	return true;
}
bool	KShader::Frame()
{
	return true;
}
bool	KShader::Render()
{
	return true;
}
bool	KShader::Release()
{
	if (m_pVertexShader) m_pVertexShader.Reset();
	if (m_pPixelShader) m_pPixelShader.Reset();
	if (m_pComputeShader) m_pComputeShader.Reset();
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pComputeShader = nullptr;
	return true;
}
bool KShader::CreateVertexShader(std::wstring filename, std::string entry)
{
	m_pVSCodeResult = LoadShaderBlob(filename, entry, "vs_5_0");
	if (m_pVSCodeResult.Get() == nullptr)
	{
		return false;
	}
	HRESULT hr = g_pd3dDevice->CreateVertexShader(
		m_pVSCodeResult.Get()->GetBufferPointer(),
		m_pVSCodeResult.Get()->GetBufferSize(),
		NULL, m_pVertexShader.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool KShader::CreatePixelShader(std::wstring filename, std::string entry)
{
	m_pPSCodeResult = LoadShaderBlob(filename, entry, "ps_5_0");
	if (m_pPSCodeResult.Get() == nullptr)
	{
		return false;
	}
	HRESULT hr = g_pd3dDevice->CreatePixelShader(
		m_pPSCodeResult.Get()->GetBufferPointer(),
		m_pPSCodeResult.Get()->GetBufferSize(),
		NULL, m_pPixelShader.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool KShader::CreateComputeShader(std::wstring filename, std::string entry)
{
	m_pCSCodeResult = LoadShaderBlob(filename, entry, "cs_5_0");
	if (m_pCSCodeResult.Get() == nullptr)
	{
		return false;
	}
	HRESULT hr = g_pd3dDevice->CreateComputeShader(
		m_pCSCodeResult.Get()->GetBufferPointer(),
		m_pCSCodeResult.Get()->GetBufferSize(),
		NULL, m_pComputeShader.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
KShader::KShader()
{
	m_iIndex = 0;
	m_szBuffer[256] = { 0, };
}
KShader::~KShader()
{
	if (m_pVertexShader) m_pVertexShader.Reset();
	if (m_pPixelShader) m_pPixelShader.Reset();
	if (m_pComputeShader) m_pComputeShader.Reset();
	m_pVertexShader = nullptr;
	m_pComputeShader = nullptr;
	m_pPixelShader = nullptr;
}

KShader* KShaderManager::CreateVertexShader(std::wstring filename, std::string entry)
{
	std::wstring name = Splitpath(filename, to_mw(entry));
	//중복 방지
	KShader* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new KShader;
	if (!pData->CreateVertexShader(filename, entry))
	{
		delete pData;
		return nullptr;
	}
	pData->m_Name = name;
	m_list.insert(make_pair(pData->m_Name, pData));
	m_iIndex++;
	return pData;
}
KShader* KShaderManager::CreatePixelShader(std::wstring filename, std::string entry)
{
	std::wstring name = Splitpath(filename, to_mw(entry));
	KShader* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new KShader;
	if (!pData->CreatePixelShader(filename, entry))
	{
		delete pData;
		return nullptr;
	}
	pData->m_Name = name;
	m_list.insert(make_pair(pData->m_Name, pData));
	m_iIndex++;
	return pData;
}
KShader* KShaderManager::CreateComputeShader(std::wstring filename, std::string entry)
{
	std::wstring name = Splitpath(filename, to_mw(entry));
	KShader* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new KShader;
	if (!pData->CreateComputeShader(filename, entry))
	{
		delete pData;
		return nullptr;
	}
	pData->m_Name = name;
	m_list.insert(make_pair(pData->m_Name, pData));
	m_iIndex++;
	return pData;
}

KShaderManager::KShaderManager()
{
	m_iIndex = 0;
}
KShaderManager::~KShaderManager()
{
	Release();
}