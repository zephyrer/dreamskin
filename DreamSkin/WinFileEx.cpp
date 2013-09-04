// WinFileEx.cpp

#include <windows.h>

#include "WinFileEx.h"

int GetDirectoryName(const WCHAR *wstrFilePath, DWORD nBufferLength, WCHAR *wstrDirectoryName)
{
	WCHAR wstrFullPath[MAX_PATH];
	WCHAR drive[MAX_PATH];
	WCHAR dir[MAX_PATH];
	
	int nResult = GetFullPathName(wstrFilePath, MAX_PATH, wstrFullPath, NULL);
	if (nResult > 0 && nResult <= MAX_PATH)
	{
		if (!_wsplitpath_s(wstrFullPath, drive, MAX_PATH, dir, MAX_PATH, NULL, 0, NULL, 0))
		{
			if ((wcslen(drive) + wcslen(dir)) < nBufferLength)
			{
				wsprintf(wstrDirectoryName, L"%s%s", drive, dir);
				nResult = wcslen(wstrDirectoryName);
			}
			else
			{
				nResult = 0;
			}
		}
		else
		{
			nResult = 0;
		}
	}
	else
	{
		nResult = 0;
	}

	return nResult;
}