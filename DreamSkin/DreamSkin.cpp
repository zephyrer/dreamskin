//DreamSkin.cpp
#include <windows.h>
#include "DreamSkin.h"
#include "DreamSkinMain.h"


DREAMSKIN_API BOOL DreamSkinInit()
{
	return theSkinMain.InitInstance();
}

DREAMSKIN_API void DreamSkinExit()
{
	theSkinMain.ExitInstance();
}

DREAMSKIN_API BOOL DreamSkinLoadW(const WCHAR* wstrSkinFilePath)
{
	return theSkinMain.LoadSkin(wstrSkinFilePath);
}

DREAMSKIN_API BOOL DreamSkin_LoadA(const char* strSkinFilePath)
{
	BOOL bReturn;
	DWORD dwError;
	WCHAR wstrSkinFilePath[MAX_PATH + 1];

	if (MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strSkinFilePath, -1, wstrSkinFilePath, MAX_PATH + 1))
	{
		bReturn = DreamSkinLoadW(wstrSkinFilePath);
	}
	else
	{
		dwError = GetLastError();
		//TODO: Error Handle
		bReturn = FALSE;
	}

	return bReturn;
}

DREAMSKIN_API BOOL DreamSkinStatusW(DREAMSKIN_STATUSW* pDreamSkinStatus)
{
	if (pDreamSkinStatus)
	{
		pDreamSkinStatus->nHookedWindowCount = theSkinMain.GetHookedWindowCount();
		pDreamSkinStatus->nHookedThreadCount = theSkinMain.GetHookedThreadCount();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

DREAMSKIN_API BOOL DreamSkinStatusA(DREAMSKIN_STATUSA* pDreamSkinStatus)
{
	DREAMSKIN_STATUSW status;
	if (pDreamSkinStatus)
	{
		if(DreamSkinStatusW(&status))
		{
			pDreamSkinStatus->nHookedWindowCount = status.nHookedWindowCount;
			pDreamSkinStatus->nHookedThreadCount = status.nHookedThreadCount;
			return TRUE;
		}
	}
	
	return FALSE;
}

