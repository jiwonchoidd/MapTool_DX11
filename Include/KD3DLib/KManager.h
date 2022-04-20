#pragma once
#include "KStd.h"
#include <strsafe.h>
//이 매니져는 기본으로 이름, 데이터 맵으로 저장한다.
//이 매니져를 상속안한 매니져들은 인덱스로 저장함
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
//중복 발생 제거
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
	//기존에 데이터가 있다면 return
	if (pData != nullptr)
	{
		return pData;
	}
	std::shared_ptr<T> pNewData = std::make_shared<T>();
	if (pNewData->Load(filename) == false)
	{
		MessageBox(g_hWnd, L"매니져 객체가 로드하지 못했습니다.", L"Warning", 0);
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

//파일 전체 파일 이름 불러오는 기능, Static 함수로 구현 
namespace KDirParser
{
	static void DisplayErrorBox(const WCHAR* lpszFunction)
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			lpszFunction, dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
	}
	static DWORD LoadAllPath(const TCHAR* argv, std::vector<std::wstring>& list)
	{
		WIN32_FIND_DATA ffd;
		LARGE_INTEGER filesize;
		TCHAR szDir[MAX_PATH];
		size_t length_of_arg;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		DWORD dwError = 0;

		StringCchLength(argv, MAX_PATH, &length_of_arg);
		if (length_of_arg > (MAX_PATH - 3))
		{
			return (-1);
		}

		StringCchCopy(szDir, MAX_PATH, argv);
		StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
		// Find the first file in the directory.
		hFind = FindFirstFile(szDir, &ffd);

		if (INVALID_HANDLE_VALUE == hFind)
		{
			DisplayErrorBox(TEXT("FindFirstFile"));
			return dwError;
		}
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
			}
			else
			{
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;
				std::wstring path = argv;
				path += L"/";
				path += ffd.cFileName;
				list.push_back(path);
				//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		dwError = GetLastError();
		if (dwError != ERROR_NO_MORE_FILES)
		{
			DisplayErrorBox(TEXT("FindFirstFile"));
		}

		FindClose(hFind);
		return dwError;
	}
}
