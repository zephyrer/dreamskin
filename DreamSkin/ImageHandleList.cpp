//ImageHandleList.cpp

#include <windows.h>
#include <string>
#include <map>
using namespace std;

#include "ImageHandleList.h"

typedef map<wstring, HANDLE> IMAGEHANDLELIST;
typedef pair<wstring, HANDLE>  IMAGEPATHHANDLEPAIR;

CImageHandleList::CImageHandleList()
{
	m_pImageHandleList = new IMAGEHANDLELIST();
}

CImageHandleList::~CImageHandleList()
{
	Clear();
	delete m_pImageHandleList;
}

void CImageHandleList::Clear()
{
	HANDLE hImage;
	IMAGEHANDLELIST::iterator iter; 
	
	for(iter = ((IMAGEHANDLELIST*)m_pImageHandleList)->begin(); iter != ((IMAGEHANDLELIST*)m_pImageHandleList)->end(); iter++)
	{
		hImage = (HANDLE)(iter->second);
		::DeleteObject(hImage);
	}

	((IMAGEHANDLELIST*)m_pImageHandleList)->clear();
}

HANDLE CImageHandleList::GetImageHandleByPath(const WCHAR *wstrImageFilePath)
{
	HANDLE hResult = NULL;
	IMAGEHANDLELIST::iterator iter; 
	if(wstrImageFilePath != NULL)
	{
		iter = ((IMAGEHANDLELIST*)m_pImageHandleList)->find(wstring(wstrImageFilePath));
		if(iter != ((IMAGEHANDLELIST*)m_pImageHandleList)->end())
		{
			hResult = (HANDLE)(iter->second);
		}
	}

	return hResult;
}

HANDLE CImageHandleList::LoadImage(const WCHAR *wstrImageFilePath)
{
	HANDLE hResult = GetImageHandleByPath(wstrImageFilePath);
	if (!hResult)
	{
		hResult = ::LoadImageW(NULL, wstrImageFilePath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		if (hResult)
		{
			((IMAGEHANDLELIST*)m_pImageHandleList)->insert(IMAGEPATHHANDLEPAIR(wstring(wstrImageFilePath), hResult));
		}
	}

	return hResult;
}