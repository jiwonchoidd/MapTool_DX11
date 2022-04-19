#pragma once
#include "KStd.h"
#include <strsafe.h>
static class FileDirParser
{
public:
	//경로내에 모든 파일이름 읽어옴
	static DWORD LoadAllPath(const TCHAR* argv, std::vector<std::wstring>& list);
	static void DisplayErrorBox(const WCHAR* lpszFunction);
};

