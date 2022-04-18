#pragma once
#include "KStd.h"
//�� �Ŵ����� �⺻���� �̸�, ������ ������ �����Ѵ�.
//�� �Ŵ����� ��Ӿ��� �Ŵ������� �ε����� ������
template <class T, class S>
class KManager : public  Singleton<S>
{
public:
	friend class  Singleton<KManager>;
public:
	int		m_iIndex;
	std::map<std::wstring, std::shared_ptr<T>>  m_list;
public:
	std::wstring Splitpath(std::wstring path, std::wstring entry);
	T* CheckLoad(std::wstring name);
	virtual T* Load(std::wstring filename);
	T* GetPtr(std::wstring key);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	KManager();
public:
	~KManager();
};
template<class T, class S>
std::wstring KManager<T, S>::Splitpath(std::wstring path,
	std::wstring entry)
{
	TCHAR szFileName[MAX_PATH] = { 0, };
	TCHAR Dirve[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR FileName[MAX_PATH] = { 0, };
	TCHAR FileExt[MAX_PATH] = { 0, };

	std::wstring fullpathname = path;
	_tsplitpath_s(fullpathname.c_str(), Dirve, Dir, FileName, FileExt);
	std::wstring name = FileName;
	name += FileExt;
	if (entry.empty() == false)
	{
		name += entry;
	}
	return name;
}
//�ߺ� �߻� ����
template<class T, class S>
T* KManager<T, S>::CheckLoad(std::wstring name)
{
	for (auto data : m_list)
	{
		if (data.second->m_Name == name)
		{
			return data.second.get();
		}
	}
	return nullptr;
}
template<class T, class S>
T* KManager<T, S>::GetPtr(std::wstring key)
{
	auto iter = m_list.find(key);
	if (iter != m_list.end())
	{
		return (*iter).second.get();
	}
	return nullptr;
}
template<class T, class S>
T* KManager<T, S>::Load(std::wstring filename)
{
	std::wstring name = Splitpath(filename, L"");
	T* pData = CheckLoad(name);
	//������ �����Ͱ� �ִٸ� return
	if (pData != nullptr)
	{
		return pData;
	}
	std::shared_ptr<T> pNewData = std::make_shared<T>();
	if (pNewData->Load(filename) == false)
	{
		MessageBox(g_hWnd, L"�Ŵ��� ��ü�� �ε����� ���߽��ϴ�.", L"Warning", 0);
		return nullptr;
	}
	pNewData->m_Name = name;
	m_list.insert(make_pair(pNewData->m_Name, pNewData));
	m_iIndex++;
	return pNewData.get();
}
template<class T, class S>
bool	KManager<T, S>::Init()
{
	return true;
}
template<class T, class S>
bool	KManager<T, S>::Frame()
{
	return true;
}
template<class T, class S>
bool	KManager<T, S>::Render()
{
	return true;
}
template<class T, class S>
bool	KManager<T, S>::Release()
{
	for (auto data : m_list)
	{
		data.second->Release();
	}
	m_list.clear();
	return true;
}
template<class T, class S>
KManager<T, S>::KManager()
{
	m_iIndex = 0;
}
template<class T, class S>
KManager<T, S>::~KManager()
{
	Release();
}