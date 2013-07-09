//ImageHandleList.h

#ifndef DREAMSKIN_IMAGEHANDLELIST_H
#define DREAMSKIN_IMAGEHANDLELIST_H

class CImageHandleList
{
public:
	CImageHandleList();
	~CImageHandleList();

public:
	//Clear
	void   Clear();
	//Load a image from file
	HANDLE LoadImage(const WCHAR *wstrImageFilePath);
	//Get the image handle by file path
	HANDLE GetImageHandleByPath(const WCHAR *wstrImageFilePath);

private:
	void* m_pImageHandleList;
};

#endif // DREAMSKIN_IMAGEHANDLELIST_H