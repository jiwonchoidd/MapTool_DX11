#pragma once
#include "KStd.h"
#include <strsafe.h>
static class FileDirParser
{
public:
	//��γ��� ��� �����̸� �о��
	static DWORD LoadAllPath(const TCHAR* argv, std::vector<std::wstring>& list);
	static void DisplayErrorBox(const WCHAR* lpszFunction);
};

