//DreamSkinLoader.cpp

#include <windows.h>

#include <string>
using namespace std;

//xcerces support
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/dom/DOM.hpp"
#include "xercesc/sax/HandlerBase.hpp"
#include "xercesc/util/XMLString.hpp"
using namespace xercesc;

#include "ImageHandleList.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinDialog.h"
#include "DreamSkinButton.h"
#include "DreamSkinStatic.h"

#include "HexBin.h"

inline const XMLCh * WStringtoXString(const WCHAR *wstrTotrans) 
{ 
	return (XMLCh*) wstrTotrans; 
};


inline const WCHAR * XStringtoWString(const XMLCh *xstrTotrans) 
{
	return (WCHAR*) xstrTotrans; 
};

DOMNode * GetNamedChild(DOMNode* pParentNode, const WCHAR *wstrNodeName)
{
	DOMNode *pReturn = NULL;

	DOMNode *pCurNode = pParentNode->getFirstChild();

	while(pCurNode != NULL)
	{
		const XMLCh *  xstrNodeName = pCurNode->getNodeName();
		if(wcscmp(XStringtoWString(xstrNodeName), wstrNodeName) == 0)
		{
			pReturn = pCurNode;
			break;
		}
		else
		{
			pCurNode = pCurNode->getNextSibling();
		}
	}

	return pReturn;
}

WCHAR CDreamSkinLoader::wstrSkinFileNodeNameDialog[] = L"dialog";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameButton[] = L"button";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameCheckBox[] = L"checkbox";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameStatic[] = L"static";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameBackground[] = L"background";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameBorder[] = L"border";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameColor[] = L"color";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameFont[] = L"font";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameText[] = L"text";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameIcon[] = L"icon";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameImage[] = L"image";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameLeft[] = L"left";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameTop[] = L"top";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameRight[] = L"right";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameBottom[] = L"bottom";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameTitle[] = L"title";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameNormal[] = L"normal";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameHover[] = L"hover";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNamePress[] = L"press";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameDisable[] = L"disable";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameDefault[] = L"default";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameSelected[] = L"selected";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameUnselected[] = L"unselected";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameIndeterminate[] = L"indeterminate";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameClose[] = L"close";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameMaximize[] = L"maximize";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameRestore[] = L"restore";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameMinimize[] = L"minimize";

WCHAR CDreamSkinLoader::wstrSkinFileAttrNameWidth[] = L"width";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameHeight[] = L"height";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameType[] = L"type";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameRegion[] = L"region";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameTransparent[] = L"transparent";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameSource[] = L"source";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameStart[] = L"start";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameEnd[] = L"end";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameBold[] = L"bold";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameX[] = L"x";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameY[] = L"y";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameMarginLeft[] = L"left-margin";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameMarginTop[] = L"top-margin";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameMarginBottom[] = L"bottom-margin";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameMarginRight[] = L"right-margin";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameIncludeBorder[] = L"incborder";

COLORREF CDreamSkinLoader::TextToColor(const WCHAR *wstrColor)
{
	DWORD dwColor;
	HexToBinW(wstrColor, (unsigned char*)&dwColor, (int)wcslen(wstrColor), 4);

	WORD HiWord = HIWORD(dwColor);
	WORD LoWord = LOWORD(dwColor);

	BYTE Red = HIBYTE(LoWord);
	BYTE Green = LOBYTE(HiWord);
	BYTE Blue = HIBYTE(HiWord);

	return RGB(Red, Green, Blue);
}

CDreamSkinLoader::CDreamSkinLoader(CImageHandleList *pImageHandleList)
{
	XMLPlatformUtils::Initialize();

	m_pImageHandleList = pImageHandleList;
}

CDreamSkinLoader::~CDreamSkinLoader()
{
	XMLPlatformUtils::Terminate();
}

BOOL CDreamSkinLoader::Load(const WCHAR *wstrSkinFilePath)
{
	BOOL bResult = TRUE;

	ErrorHandler* errHandler = NULL;
	XercesDOMParser* parser = NULL;
	try
	{
		parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Always);    // optional.
        parser->setDoNamespaces(true);    // optional

        errHandler = (ErrorHandler*) new HandlerBase();
        parser->setErrorHandler(errHandler);

		//TODO: Add DreamSkin XML schema validator to ensure it's really dreamskin file
		parser->parse(wstrSkinFilePath);

		//Load Skin Dialog
		CDreamSkinDialog::GetDefaultSkin(&m_SkinDialog);
		LoadSkinDialog(parser);

		//Load Skin Button
		CDreamSkinButton::GetDefaultButtonSkin(&m_SkinButton);
		LoadSkinButton(parser);

		//Load Skin CheckBox
		CDreamSkinButton::GetDefaultCheckBoxSkin(&m_SkinCheckBox);
		LoadSkinCheckBox(parser);

		//Load Skin Status
		CDreamSkinStatic::GetDefaultSkin(&m_SkinStatic);
		LoadSkinStatic(parser);

		parser->setErrorHandler(NULL);
	}
	catch (const XMLException& ) 
	{
		//TODO: Do your failure processing here
		bResult = FALSE;
	}
	catch (const DOMException& toCatch) {
		//TODO:
		char* message = XMLString::transcode(toCatch.msg);
		//     << message << "\n";
		XMLString::release(&message);
		bResult = FALSE;
	}
	catch (const SAXParseException&) {
		//TODO: Invalid XML format
		bResult = FALSE;
	}

	if (errHandler)
		delete errHandler;

	if (parser)
		delete parser;

	return bResult;
}

void CDreamSkinLoader::GetSkinButton(SKINBUTTON *pSkinButton) const
{
	if (pSkinButton)
	{
		memcpy(pSkinButton, &m_SkinButton, sizeof(SKINBUTTON));
	}
}

void CDreamSkinLoader::GetSkinCheckBox(SKINCHECKBOX *pSkinCheckBox) const
{
	if (pSkinCheckBox)
	{
		memcpy(pSkinCheckBox, &m_SkinCheckBox, sizeof(SKINCHECKBOX));
	}
}

void CDreamSkinLoader::GetSkinDialog(SKINDIALOG *pSkinDialog) const
{
	if (pSkinDialog)
	{
		memcpy(pSkinDialog, &m_SkinDialog, sizeof(SKINDIALOG));
	}
}

void CDreamSkinLoader::GetSkinStatic(SKINSTATIC *pSkinStatic) const
{
	if (pSkinStatic)
	{
		memcpy(pSkinStatic, &m_SkinStatic, sizeof(SKINSTATIC));
	}
}

BOOL CDreamSkinLoader::LoadSkinButton(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pButtonNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameButton);
	const WCHAR *wstrNodeName;
	SKINBORDER *pSkinBorderList[DRAWSTATUS_DEFAULT + 1];
	SKINTEXT *pSkinTextList[DRAWSTATUS_DEFAULT + 1];

	if (pButtonNode)
	{
		//loop to load all settings
		pParentNode = pButtonNode->getFirstChild();
		while (pParentNode != NULL)
		{
			wstrNodeName = XStringtoWString(pParentNode->getNodeName());
			if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBackground) == 0)
			{//background
				pAttr = pParentNode->getAttributes();
				if (pAttr)
				{
					//background draw type
					pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
					if (pTempNode)
					{
						m_SkinButton.skinBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						m_SkinButton.skinBkHover.nDrawType = m_SkinButton.skinBkNormal.nDrawType;
						m_SkinButton.skinBkDisable.nDrawType = m_SkinButton.skinBkNormal.nDrawType;
						m_SkinButton.skinBkPress.nDrawType = m_SkinButton.skinBkNormal.nDrawType;
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadBackground(pChildNode, &(m_SkinButton.skinBkNormal));
					}//normal
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
					{//disable
						LoadBackground(pChildNode, &(m_SkinButton.skinBkDisable));
					}//disable
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
					{//hover
						LoadBackground(pChildNode, &(m_SkinButton.skinBkHover));
					}//hover
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
					{//press
						LoadBackground(pChildNode, &(m_SkinButton.skinBkPress));
					}//press
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDefault) == 0)
					{//default
						LoadBackground(pChildNode, &(m_SkinButton.skinBkDefault));
					}//default

					pChildNode = pChildNode->getNextSibling();
				}

				if (m_SkinButton.skinBkDisable.nDefault)
					m_SkinButton.skinBkDisable = m_SkinButton.skinBkNormal;

				if (m_SkinButton.skinBkHover.nDefault)
					m_SkinButton.skinBkHover = m_SkinButton.skinBkNormal;

				if (m_SkinButton.skinBkPress.nDefault)
					m_SkinButton.skinBkPress = m_SkinButton.skinBkHover;

				if (m_SkinButton.skinBkDefault.nDefault)
					m_SkinButton.skinBkDefault = m_SkinButton.skinBkNormal;
			}//background
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
			{//border
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
					{//left
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinButton.skinLBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinButton.skinLBorderDisable;
						pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinButton.skinLBorderHover;
						pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinButton.skinLBorderPress;
						pSkinBorderList[DRAWSTATUS_DEFAULT] = &m_SkinButton.skinLBorderDefault;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DEFAULT + 1);
					}//left
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
					{//right
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinButton.skinRBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinButton.skinRBorderDisable;
						pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinButton.skinRBorderHover;
						pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinButton.skinRBorderPress;
						pSkinBorderList[DRAWSTATUS_DEFAULT] = &m_SkinButton.skinRBorderDefault;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DEFAULT + 1);
					}//right
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
					{//top
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinButton.skinTBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinButton.skinTBorderDisable;
						pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinButton.skinTBorderHover;
						pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinButton.skinTBorderPress;
						pSkinBorderList[DRAWSTATUS_DEFAULT] = &m_SkinButton.skinTBorderDefault;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DEFAULT + 1);
					}//top
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
					{//bottom
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinButton.skinBBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinButton.skinBBorderDisable;
						pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinButton.skinBBorderHover;
						pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinButton.skinBBorderPress;
						pSkinBorderList[DRAWSTATUS_DEFAULT] = &m_SkinButton.skinBBorderDefault;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DEFAULT + 1);
					}//bottom

					pChildNode = pChildNode->getNextSibling();
				}
			}//border
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
			{//text
				pSkinTextList[DRAWSTATUS_NORMAL] = &m_SkinButton.skinTxtNormal;
				pSkinTextList[DRAWSTATUS_DISABLE] = &m_SkinButton.skinTxtDisable;
				pSkinTextList[DRAWSTATUS_HOVER] = &m_SkinButton.skinTxtHover;
				pSkinTextList[DRAWSTATUS_PRESS] = &m_SkinButton.skinTxtPress;
				pSkinTextList[DRAWSTATUS_DEFAULT] = &m_SkinButton.skinTxtDefault;
				LoadText(pParentNode, pSkinTextList, DRAWSTATUS_DEFAULT + 1);
			}//text

			pParentNode = pParentNode->getNextSibling();
		}
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinCheckBox(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pStatusNode, *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pCheckBoxNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameCheckBox);
	const WCHAR *wstrNodeName;
	SKINTEXT *pSkinTextList[DRAWSTATUS_PRESS + 1];
	SKINICON *pSkinIconList[DRAWSTATUS_PRESS + 1];

	if (pCheckBoxNode)
	{
		//loop to load all settings
		pStatusNode = pCheckBoxNode->getFirstChild();
		while (pStatusNode != NULL)
		{
			wstrNodeName = XStringtoWString(pStatusNode->getNodeName());
			if (wcscmp(wstrNodeName, wstrSkinFileNodeNameUnselected) == 0)
			{
				//loop to load all settings
				pParentNode = pStatusNode->getFirstChild();
				while (pParentNode != NULL)
				{
					wstrNodeName = XStringtoWString(pParentNode->getNodeName());
					if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBackground) == 0)
					{//background
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//background draw type
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
							if (pTempNode)
							{
								m_SkinCheckBox.skinBkNormalUnchecked.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinCheckBox.skinBkDisableUnchecked.nDrawType = m_SkinCheckBox.skinBkNormalUnchecked.nDrawType;
								m_SkinCheckBox.skinBkHoverUnchecked.nDrawType = m_SkinCheckBox.skinBkNormalUnchecked.nDrawType;
								m_SkinCheckBox.skinBkPressUnchecked.nDrawType = m_SkinCheckBox.skinBkNormalUnchecked.nDrawType;
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkNormalUnchecked));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkDisableUnchecked));
							}//disable
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
							{//hover
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkHoverUnchecked));
							}//hover
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
							{//press
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkPressUnchecked));
							}//press

							pChildNode = pChildNode->getNextSibling();
						}
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
					{//text
						pSkinTextList[DRAWSTATUS_NORMAL] = &m_SkinCheckBox.skinTxtNormalUnchecked;
						pSkinTextList[DRAWSTATUS_DISABLE] = &m_SkinCheckBox.skinTxtDisableUnchecked;
						pSkinTextList[DRAWSTATUS_HOVER] = &m_SkinCheckBox.skinTxtHoverUnchecked;
						pSkinTextList[DRAWSTATUS_PRESS] = &m_SkinCheckBox.skinTxtPressUnchecked;
						LoadText(pParentNode, pSkinTextList, DRAWSTATUS_PRESS + 1);
					}//text
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameIcon) == 0)
					{//icon
						pSkinIconList[DRAWSTATUS_NORMAL] = &m_SkinCheckBox.iconNormalUnchecked;
						pSkinIconList[DRAWSTATUS_DISABLE] = &m_SkinCheckBox.iconDisableUnchecked;
						pSkinIconList[DRAWSTATUS_HOVER] = &m_SkinCheckBox.iconHoverUnchecked;
						pSkinIconList[DRAWSTATUS_PRESS] = &m_SkinCheckBox.iconPressUnchecked;
						LoadIcon(pParentNode, pSkinIconList, DRAWSTATUS_PRESS + 1);
					}//icon

					pParentNode = pParentNode->getNextSibling();
				}
			}//unselected
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameSelected) == 0)
			{
				//loop to load all settings
				pParentNode = pStatusNode->getFirstChild();
				while (pParentNode != NULL)
				{
					wstrNodeName = XStringtoWString(pParentNode->getNodeName());
					if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBackground) == 0)
					{//background
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//background draw type
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
							if (pTempNode)
							{
								m_SkinCheckBox.skinBkNormalChecked.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinCheckBox.skinBkDisableChecked.nDrawType = m_SkinCheckBox.skinBkNormalChecked.nDrawType;
								m_SkinCheckBox.skinBkHoverChecked.nDrawType = m_SkinCheckBox.skinBkNormalChecked.nDrawType;
								m_SkinCheckBox.skinBkPressChecked.nDrawType = m_SkinCheckBox.skinBkNormalChecked.nDrawType;
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkNormalChecked));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkDisableChecked));
							}//disable
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
							{//hover
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkHoverChecked));
							}//hover
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
							{//press
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkPressChecked));
							}//press

							pChildNode = pChildNode->getNextSibling();
						}
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
					{//text
						pSkinTextList[DRAWSTATUS_NORMAL] = &m_SkinCheckBox.skinTxtNormalChecked;
						pSkinTextList[DRAWSTATUS_DISABLE] = &m_SkinCheckBox.skinTxtDisableChecked;
						pSkinTextList[DRAWSTATUS_HOVER] = &m_SkinCheckBox.skinTxtHoverChecked;
						pSkinTextList[DRAWSTATUS_PRESS] = &m_SkinCheckBox.skinTxtPressChecked;
						LoadText(pParentNode, pSkinTextList, DRAWSTATUS_PRESS + 1);
					}//text
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameIcon) == 0)
					{//icon
						pSkinIconList[DRAWSTATUS_NORMAL] = &m_SkinCheckBox.iconNormalChecked;
						pSkinIconList[DRAWSTATUS_DISABLE] = &m_SkinCheckBox.iconDisableChecked;
						pSkinIconList[DRAWSTATUS_HOVER] = &m_SkinCheckBox.iconHoverChecked;
						pSkinIconList[DRAWSTATUS_PRESS] = &m_SkinCheckBox.iconPressChecked;
						LoadIcon(pParentNode, pSkinIconList, DRAWSTATUS_PRESS + 1);
					}//icon

					pParentNode = pParentNode->getNextSibling();
				}
			}//selected
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameIndeterminate) == 0)
			{
				//loop to load all settings
				pParentNode = pStatusNode->getFirstChild();
				while (pParentNode != NULL)
				{
					wstrNodeName = XStringtoWString(pParentNode->getNodeName());
					if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBackground) == 0)
					{//background
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//background draw type
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
							if (pTempNode)
							{
								m_SkinCheckBox.skinBkNormalPartChecked.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinCheckBox.skinBkDisablePartChecked.nDrawType = m_SkinCheckBox.skinBkNormalPartChecked.nDrawType;
								m_SkinCheckBox.skinBkHoverPartChecked.nDrawType = m_SkinCheckBox.skinBkNormalPartChecked.nDrawType;
								m_SkinCheckBox.skinBkPressPartChecked.nDrawType = m_SkinCheckBox.skinBkNormalPartChecked.nDrawType;
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkNormalPartChecked));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkDisablePartChecked));
							}//disable
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
							{//hover
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkHoverPartChecked));
							}//hover
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
							{//press
								LoadBackground(pChildNode, &(m_SkinCheckBox.skinBkPressPartChecked));
							}//press

							pChildNode = pChildNode->getNextSibling();
						}
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
					{//text
						pSkinTextList[DRAWSTATUS_NORMAL] = &m_SkinCheckBox.skinTxtNormalPartChecked;
						pSkinTextList[DRAWSTATUS_DISABLE] = &m_SkinCheckBox.skinTxtDisablePartChecked;
						pSkinTextList[DRAWSTATUS_HOVER] = &m_SkinCheckBox.skinTxtHoverPartChecked;
						pSkinTextList[DRAWSTATUS_PRESS] = &m_SkinCheckBox.skinTxtPressPartChecked;
						LoadText(pParentNode, pSkinTextList, DRAWSTATUS_PRESS + 1);
					}//text
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameIcon) == 0)
					{//icon
						pSkinIconList[DRAWSTATUS_NORMAL] = &m_SkinCheckBox.iconNormalPartChecked;
						pSkinIconList[DRAWSTATUS_DISABLE] = &m_SkinCheckBox.iconDisablePartChecked;
						pSkinIconList[DRAWSTATUS_HOVER] = &m_SkinCheckBox.iconHoverPartChecked;
						pSkinIconList[DRAWSTATUS_PRESS] = &m_SkinCheckBox.iconPressPartChecked;
						LoadIcon(pParentNode, pSkinIconList, DRAWSTATUS_PRESS + 1);
					}//icon

					pParentNode = pParentNode->getNextSibling();
				}
			}//partially selected

			pStatusNode = pStatusNode->getNextSibling();
		}

		if (m_SkinCheckBox.skinBkDisableUnchecked.nDefault)
			m_SkinCheckBox.skinBkDisableUnchecked = m_SkinCheckBox.skinBkNormalUnchecked;

		if (m_SkinCheckBox.skinBkHoverUnchecked.nDefault)
			m_SkinCheckBox.skinBkHoverUnchecked = m_SkinCheckBox.skinBkNormalUnchecked;

		if (m_SkinCheckBox.skinBkPressUnchecked.nDefault)
			m_SkinCheckBox.skinBkPressUnchecked = m_SkinCheckBox.skinBkHoverUnchecked;

		if (m_SkinCheckBox.skinBkNormalChecked.nDefault)
			m_SkinCheckBox.skinBkNormalChecked = m_SkinCheckBox.skinBkNormalUnchecked;

		if (m_SkinCheckBox.skinBkNormalPartChecked.nDefault)
			m_SkinCheckBox.skinBkNormalPartChecked = m_SkinCheckBox.skinBkNormalUnchecked;

		if (m_SkinCheckBox.skinBkNormalChecked.nDefault)
			m_SkinCheckBox.skinBkNormalChecked = m_SkinCheckBox.skinBkNormalUnchecked;

		if (m_SkinCheckBox.skinBkDisableChecked.nDefault)
			m_SkinCheckBox.skinBkDisableChecked = m_SkinCheckBox.skinBkNormalChecked;

		if (m_SkinCheckBox.skinBkHoverChecked.nDefault)
			m_SkinCheckBox.skinBkHoverChecked = m_SkinCheckBox.skinBkNormalChecked;

		if (m_SkinCheckBox.skinBkPressChecked.nDefault)
			m_SkinCheckBox.skinBkPressChecked = m_SkinCheckBox.skinBkHoverChecked;

		if (m_SkinCheckBox.skinBkNormalPartChecked.nDefault)
			m_SkinCheckBox.skinBkNormalPartChecked = m_SkinCheckBox.skinBkNormalChecked;

		if (m_SkinCheckBox.skinBkDisablePartChecked.nDefault)
			m_SkinCheckBox.skinBkDisablePartChecked = m_SkinCheckBox.skinBkNormalPartChecked;

		if (m_SkinCheckBox.skinBkHoverPartChecked.nDefault)
			m_SkinCheckBox.skinBkHoverPartChecked = m_SkinCheckBox.skinBkNormalPartChecked;

		if (m_SkinCheckBox.skinBkPressPartChecked.nDefault)
			m_SkinCheckBox.skinBkPressPartChecked = m_SkinCheckBox.skinBkHoverPartChecked;
	} //end load checkbox

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinDialog(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pDialogNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameDialog);
	const WCHAR *wstrNodeName;
	SKINBORDER *pSkinBorderList[DRAWSTATUS_DISABLE + 1];

	//dialog settings
	if (pDialogNode)
	{
		pAttr = pDialogNode->getAttributes();
		if (pAttr)
		{
			//region type
			pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameRegion));
			if (pTempNode)
			{
				m_SkinDialog.nRgnType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			}

			//transparent color
			pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameTransparent));
			if (pTempNode)
			{
				m_SkinDialog.clrTansparent = TextToColor(XStringtoWString(pTempNode->getNodeValue()));
			}
		}

		//loop to load all settings
		pParentNode = pDialogNode->getFirstChild();
		while (pParentNode != NULL)
		{
			wstrNodeName = XStringtoWString(pParentNode->getNodeName());
			if(wcscmp(wstrNodeName, wstrSkinFileNodeNameBackground) == 0)
			{//background
				pAttr = pParentNode->getAttributes();
				if (pAttr)
				{
					//background draw type
					pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
					if (pTempNode)
					{
						m_SkinDialog.skinBkActive.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						m_SkinDialog.skinBkInactive.nDrawType = m_SkinDialog.skinBkActive.nDrawType;
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadBackground(pChildNode, &(m_SkinDialog.skinBkActive));
					}//normal
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
					{//disable
						LoadBackground(pChildNode, &(m_SkinDialog.skinBkInactive));
					}//disable

					pChildNode = pChildNode->getNextSibling();
				}

				if (m_SkinDialog.skinBkInactive.nDefault)
					m_SkinDialog.skinBkInactive = m_SkinDialog.skinBkActive;
			}//background
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
			{//border
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
					{//left
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinDialog.skinLBorderActive;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinDialog.skinLBorderInactive;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//left
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
					{//right
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinDialog.skinRBorderActive;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinDialog.skinRBorderInactive;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//right
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
					{//top
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinDialog.skinTBorderActive;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinDialog.skinTBorderInactive;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//top
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
					{//bottom
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinDialog.skinBBorderActive;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinDialog.skinBBorderInactive;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//bottom
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTitle) == 0)
					{//title
						LoadTitleBar(pChildNode, &(m_SkinDialog.skinTitleBar));
					}//title

					pChildNode = pChildNode->getNextSibling();
				}
			}//border
			
			pParentNode = pParentNode->getNextSibling();
		}//loop to load all settings
	}//dialog settings

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinStatic(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pStaticNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameStatic);
	const WCHAR *wstrNodeName;
	SKINTEXT *pSkinTextList[DRAWSTATUS_NORMAL + 1];

	if (pStaticNode)
	{
		//loop to load all settings
		pParentNode = pStaticNode->getFirstChild();
		while (pParentNode != NULL)
		{
			wstrNodeName = XStringtoWString(pParentNode->getNodeName());
			if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBackground) == 0)
			{//background
				pAttr = pParentNode->getAttributes();
				if (pAttr)
				{
					//background draw type
					pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
					if (pTempNode)
					{
						m_SkinStatic.skinBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadBackground(pChildNode, &(m_SkinStatic.skinBkNormal));
					}//normal

					pChildNode = pChildNode->getNextSibling();
				}
			}//background
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
			{//text
				pSkinTextList[DRAWSTATUS_NORMAL] = &m_SkinStatic.skinTxtNormal;
				LoadText(pParentNode, pSkinTextList, DRAWSTATUS_NORMAL + 1);
			}//text

			pParentNode = pParentNode->getNextSibling();
		}
	}//static settings

	return bResult;
}

BOOL CDreamSkinLoader::LoadColor(void *color, DRAWCOLOR *pDrawColor)
{
	BOOL bResult = FALSE;
	DOMNamedNodeMap *pAttr;
	DOMNode *pTempNode;

	pAttr = ((DOMNode*)color)->getAttributes();
	if (pAttr && pDrawColor)
	{
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameStart));
		if (pTempNode)
			pDrawColor->clrStart = TextToColor(XStringtoWString(pTempNode->getNodeValue()));

		bResult = TRUE;
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadColorItem(void *color, WCHAR* wstrColorItemName, COLORREF *pColor)
{
	BOOL bResult = FALSE;
	DOMNamedNodeMap *pAttr;
	DOMNode *pTempNode;

	pAttr = ((DOMNode*)color)->getAttributes();
	if (pAttr && pColor)
	{
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrColorItemName));
		if (pTempNode)
			*pColor= TextToColor(XStringtoWString(pTempNode->getNodeValue()));

		bResult = TRUE;
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadFont(void *font, SKINTEXTFONT *pSkinFont)
{
	BOOL bResult = FALSE;
	DOMNamedNodeMap *pAttr;
	DOMNode *pTempNode;

	pAttr = ((DOMNode*)font)->getAttributes();
	if (pAttr && pSkinFont)
	{
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameBold));
		if (pTempNode)
			pSkinFont->nBold = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

		bResult = TRUE;
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadIcon(void *icon, SKINICON **pSkinIconList, int nCount)
{
	BOOL bResult = TRUE;
	DOMNode *pIconNode = (DOMNode*)icon;
	const WCHAR *wstrNodeName;
	DOMNode *pTempNode;
	DOMNamedNodeMap *pAttr;
	int nTemp;

	if (nCount > 0 && pSkinIconList)
	{
		pAttr = pIconNode->getAttributes();

		//draw type
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
		if (pTempNode)
		{
			nTemp = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			if (pSkinIconList[DRAWSTATUS_NORMAL])
				pSkinIconList[DRAWSTATUS_NORMAL]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_DISABLE && pSkinIconList[DRAWSTATUS_DISABLE])
				pSkinIconList[DRAWSTATUS_DISABLE]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_HOVER && pSkinIconList[DRAWSTATUS_HOVER])
				pSkinIconList[DRAWSTATUS_HOVER]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_PRESS && pSkinIconList[DRAWSTATUS_PRESS])
				pSkinIconList[DRAWSTATUS_PRESS]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_DEFAULT && pSkinIconList[DRAWSTATUS_DEFAULT])
				pSkinIconList[DRAWSTATUS_DEFAULT]->nDrawType = nTemp;
		}

		//icon width
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
		if (pTempNode)
		{
			nTemp = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			if (pSkinIconList[DRAWSTATUS_NORMAL])
				pSkinIconList[DRAWSTATUS_NORMAL]->nWidth = nTemp;

			if (nCount > DRAWSTATUS_DISABLE && pSkinIconList[DRAWSTATUS_DISABLE])
				pSkinIconList[DRAWSTATUS_DISABLE]->nWidth = nTemp;

			if (nCount > DRAWSTATUS_HOVER && pSkinIconList[DRAWSTATUS_HOVER])
				pSkinIconList[DRAWSTATUS_HOVER]->nWidth = nTemp;

			if (nCount > DRAWSTATUS_PRESS && pSkinIconList[DRAWSTATUS_PRESS])
				pSkinIconList[DRAWSTATUS_PRESS]->nWidth = nTemp;

			if (nCount > DRAWSTATUS_DEFAULT && pSkinIconList[DRAWSTATUS_DEFAULT])
				pSkinIconList[DRAWSTATUS_DEFAULT]->nWidth = nTemp;
		}

		//icon height
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameHeight));
		if (pTempNode)
		{
			nTemp = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			if (pSkinIconList[DRAWSTATUS_NORMAL])
				pSkinIconList[DRAWSTATUS_NORMAL]->nHeight = nTemp;

			if (nCount > DRAWSTATUS_DISABLE && pSkinIconList[DRAWSTATUS_DISABLE])
				pSkinIconList[DRAWSTATUS_DISABLE]->nHeight = nTemp;

			if (nCount > DRAWSTATUS_HOVER && pSkinIconList[DRAWSTATUS_HOVER])
				pSkinIconList[DRAWSTATUS_HOVER]->nHeight = nTemp;

			if (nCount > DRAWSTATUS_PRESS && pSkinIconList[DRAWSTATUS_PRESS])
				pSkinIconList[DRAWSTATUS_PRESS]->nHeight = nTemp;

			if (nCount > DRAWSTATUS_DEFAULT && pSkinIconList[DRAWSTATUS_DEFAULT])
				pSkinIconList[DRAWSTATUS_DEFAULT]->nHeight = nTemp;
		}

		pTempNode = pIconNode->getFirstChild();
		while (pTempNode != NULL)
		{
			wstrNodeName = XStringtoWString(pTempNode->getNodeName());

			if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
			{//normal
				if (pSkinIconList[DRAWSTATUS_NORMAL])
					LoadIconItem(pTempNode, pSkinIconList[DRAWSTATUS_NORMAL]);
			}//normal
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
			{//disable
				if (nCount > DRAWSTATUS_DISABLE && pSkinIconList[DRAWSTATUS_DISABLE])
					LoadIconItem(pTempNode, pSkinIconList[DRAWSTATUS_DISABLE]);
			}//disable
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
			{//hover
				if (nCount > DRAWSTATUS_HOVER && pSkinIconList[DRAWSTATUS_HOVER])
					LoadIconItem(pTempNode, pSkinIconList[DRAWSTATUS_HOVER]);
			}//hover
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
			{//press
				if (nCount > DRAWSTATUS_PRESS && pSkinIconList[DRAWSTATUS_PRESS])
					LoadIconItem(pTempNode, pSkinIconList[DRAWSTATUS_PRESS]);
			}//press
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameDefault) == 0)
			{//default
				if (nCount > DRAWSTATUS_DEFAULT && pSkinIconList[DRAWSTATUS_DEFAULT])
					LoadIconItem(pTempNode, pSkinIconList[DRAWSTATUS_DEFAULT]);
			}//default
			pTempNode = pTempNode->getNextSibling();
		}

		if (nCount > DRAWSTATUS_DISABLE && pSkinIconList[DRAWSTATUS_DISABLE] && pSkinIconList[DRAWSTATUS_DISABLE]->nDefault && pSkinIconList[DRAWSTATUS_NORMAL])
			*(pSkinIconList[DRAWSTATUS_DISABLE]) = *(pSkinIconList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_HOVER && pSkinIconList[DRAWSTATUS_HOVER] && pSkinIconList[DRAWSTATUS_HOVER]->nDefault && pSkinIconList[DRAWSTATUS_NORMAL])
			*(pSkinIconList[DRAWSTATUS_HOVER]) = *(pSkinIconList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_DEFAULT && pSkinIconList[DRAWSTATUS_DEFAULT] && pSkinIconList[DRAWSTATUS_DEFAULT]->nDefault && pSkinIconList[DRAWSTATUS_NORMAL])
			*(pSkinIconList[DRAWSTATUS_DEFAULT]) = *(pSkinIconList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_PRESS && pSkinIconList[DRAWSTATUS_PRESS] && pSkinIconList[DRAWSTATUS_PRESS]->nDefault)
			if (pSkinIconList[DRAWSTATUS_HOVER])
				*(pSkinIconList[DRAWSTATUS_PRESS]) = *(pSkinIconList[DRAWSTATUS_HOVER]);
			else if (pSkinIconList[DRAWSTATUS_NORMAL])
				*(pSkinIconList[DRAWSTATUS_PRESS]) = *(pSkinIconList[DRAWSTATUS_HOVER]);
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadIconItem(void *iconitem, SKINICON *pSkinIcon)
{
	BOOL bResult = TRUE;
	DOMNode *pNode;
	const WCHAR *wstrNodeName;
	
	pSkinIcon->nDefault = 0;

	pNode = ((DOMNode*)iconitem)->getFirstChild();
	while(pNode != NULL)
	{
		wstrNodeName = XStringtoWString(pNode->getNodeName());

		if(wcscmp(wstrNodeName, wstrSkinFileNodeNameColor) == 0)
		{//color
			bResult = LoadColorItem(pNode, wstrSkinFileAttrNameStart, &(pSkinIcon->clrDraw.clrStart));
		}//color
		else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameImage) == 0)
		{//image
			bResult = LoadImage(pNode, &(pSkinIcon->imgDraw));
		}//image

		pNode = pNode->getNextSibling();
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadImage(void *image, DRAWIMAGE *pDrawImage)
{
	BOOL bResult = FALSE;
	DOMNamedNodeMap *pAttr;
	DOMNode *pTempNode;

	pAttr = ((DOMNode*)image)->getAttributes();
	if (pAttr && pDrawImage)
	{
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameX));
		if (pTempNode)
			pDrawImage->x = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameY));
		if (pTempNode)
			pDrawImage->y = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
		if (pTempNode)
			pDrawImage->nWidth = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameHeight));
		if (pTempNode)
			pDrawImage->nHeight = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

		if (m_pImageHandleList)
		{
			pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameSource));
			if (pTempNode)
				pDrawImage->hImage = m_pImageHandleList->LoadImage(XStringtoWString(pTempNode->getNodeValue()));
		}

		bResult = TRUE;
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadBackground(void *bkitem, SKINBACKGROUND *pSkinBackground)
{
	BOOL bResult = TRUE;
	DOMNode *pNode;
	const WCHAR *wstrNodeName;
	
	pSkinBackground->nDefault = 0;

	pNode = ((DOMNode*)bkitem)->getFirstChild();
	while(pNode != NULL)
	{
		wstrNodeName = XStringtoWString(pNode->getNodeName());

		if(wcscmp(wstrNodeName, wstrSkinFileNodeNameColor) == 0)
		{//color
			bResult = LoadColor(pNode, &(pSkinBackground->clrDraw));
		}//color
		else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameImage) == 0)
		{//image
			bResult = LoadImage(pNode, &(pSkinBackground->imgDraw));
		}//image

		pNode = pNode->getNextSibling();
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadBorder(void *border, SKINBORDER **pSkinBorderList, int nCount)
{
	BOOL bResult = TRUE;
	DOMNode *pBorderNode = (DOMNode*)border;
	const WCHAR *wstrNodeName;
	DOMNode *pTempNode;
	DOMNamedNodeMap *pAttr;
	int nTemp;

	if (nCount > 0 && pSkinBorderList)
	{
		pAttr = pBorderNode->getAttributes();

		//border width
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
		if (pTempNode)
		{
			nTemp = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			if (pSkinBorderList[DRAWSTATUS_NORMAL])
				pSkinBorderList[DRAWSTATUS_NORMAL]->nWidth = nTemp;

			if (nCount > DRAWSTATUS_DISABLE && pSkinBorderList[DRAWSTATUS_DISABLE])
				pSkinBorderList[DRAWSTATUS_DISABLE]->nWidth = nTemp;

			if (nCount > DRAWSTATUS_HOVER && pSkinBorderList[DRAWSTATUS_HOVER])
				pSkinBorderList[DRAWSTATUS_HOVER]->nWidth = nTemp;

			if (nCount > DRAWSTATUS_PRESS && pSkinBorderList[DRAWSTATUS_PRESS])
				pSkinBorderList[DRAWSTATUS_PRESS]->nWidth = nTemp;

			if (nCount > DRAWSTATUS_DEFAULT && pSkinBorderList[DRAWSTATUS_DEFAULT])
				pSkinBorderList[DRAWSTATUS_DEFAULT]->nWidth = nTemp;
		}

		//draw type
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
		if (pTempNode)
		{
			nTemp = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			if (pSkinBorderList[DRAWSTATUS_NORMAL])
				pSkinBorderList[DRAWSTATUS_NORMAL]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_DISABLE && pSkinBorderList[DRAWSTATUS_DISABLE])
				pSkinBorderList[DRAWSTATUS_DISABLE]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_HOVER && pSkinBorderList[DRAWSTATUS_HOVER])
				pSkinBorderList[DRAWSTATUS_HOVER]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_PRESS && pSkinBorderList[DRAWSTATUS_PRESS])
				pSkinBorderList[DRAWSTATUS_PRESS]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_DEFAULT && pSkinBorderList[DRAWSTATUS_DEFAULT])
				pSkinBorderList[DRAWSTATUS_DEFAULT]->nDrawType = nTemp;
		}

		//start size
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameStart));
		if (pTempNode)
		{
			nTemp = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			if (pSkinBorderList[DRAWSTATUS_NORMAL])
				pSkinBorderList[DRAWSTATUS_NORMAL]->nStart = nTemp;

			if (nCount > DRAWSTATUS_DISABLE && pSkinBorderList[DRAWSTATUS_DISABLE])
				pSkinBorderList[DRAWSTATUS_DISABLE]->nStart = nTemp;

			if (nCount > DRAWSTATUS_HOVER && pSkinBorderList[DRAWSTATUS_HOVER])
				pSkinBorderList[DRAWSTATUS_HOVER]->nStart = nTemp;

			if (nCount > DRAWSTATUS_PRESS && pSkinBorderList[DRAWSTATUS_PRESS])
				pSkinBorderList[DRAWSTATUS_PRESS]->nStart = nTemp;

			if (nCount > DRAWSTATUS_DEFAULT && pSkinBorderList[DRAWSTATUS_DEFAULT])
				pSkinBorderList[DRAWSTATUS_DEFAULT]->nStart = nTemp;
		}

		//end size
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameEnd));
		if (pTempNode)
		{
			nTemp = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			if (pSkinBorderList[DRAWSTATUS_NORMAL])
				pSkinBorderList[DRAWSTATUS_NORMAL]->nEnd = nTemp;

			if (nCount > DRAWSTATUS_DISABLE && pSkinBorderList[DRAWSTATUS_DISABLE])
				pSkinBorderList[DRAWSTATUS_DISABLE]->nEnd = nTemp;

			if (nCount > DRAWSTATUS_HOVER && pSkinBorderList[DRAWSTATUS_HOVER])
				pSkinBorderList[DRAWSTATUS_HOVER]->nEnd = nTemp;

			if (nCount > DRAWSTATUS_PRESS && pSkinBorderList[DRAWSTATUS_PRESS])
				pSkinBorderList[DRAWSTATUS_PRESS]->nEnd = nTemp;

			if (nCount > DRAWSTATUS_DEFAULT && pSkinBorderList[DRAWSTATUS_DEFAULT])
				pSkinBorderList[DRAWSTATUS_DEFAULT]->nEnd = nTemp;
		}

		pTempNode = pBorderNode->getFirstChild();
		while (pTempNode != NULL)
		{
			wstrNodeName = XStringtoWString(pTempNode->getNodeName());

			if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
			{//normal
				if (pSkinBorderList[DRAWSTATUS_NORMAL])
					LoadBorderItem(pTempNode, pSkinBorderList[DRAWSTATUS_NORMAL]);
			}//normal
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
			{//disable
				if (nCount > DRAWSTATUS_DISABLE && pSkinBorderList[DRAWSTATUS_DISABLE])
					LoadBorderItem(pTempNode, pSkinBorderList[DRAWSTATUS_DISABLE]);
			}//disable
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
			{//hover
				if (nCount > DRAWSTATUS_HOVER && pSkinBorderList[DRAWSTATUS_HOVER])
					LoadBorderItem(pTempNode, pSkinBorderList[DRAWSTATUS_HOVER]);
			}//hover
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
			{//press
				if (nCount > DRAWSTATUS_PRESS && pSkinBorderList[DRAWSTATUS_PRESS])
					LoadBorderItem(pTempNode, pSkinBorderList[DRAWSTATUS_PRESS]);
			}//press
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameDefault) == 0)
			{//default
				if (nCount > DRAWSTATUS_DEFAULT && pSkinBorderList[DRAWSTATUS_DEFAULT])
					LoadBorderItem(pTempNode, pSkinBorderList[DRAWSTATUS_DEFAULT]);
			}//default
			pTempNode = pTempNode->getNextSibling();
		}

		if (nCount > DRAWSTATUS_DISABLE && pSkinBorderList[DRAWSTATUS_DISABLE] && pSkinBorderList[DRAWSTATUS_DISABLE]->nDefault && pSkinBorderList[DRAWSTATUS_NORMAL])
			*(pSkinBorderList[DRAWSTATUS_DISABLE]) = *(pSkinBorderList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_HOVER && pSkinBorderList[DRAWSTATUS_HOVER] && pSkinBorderList[DRAWSTATUS_HOVER]->nDefault && pSkinBorderList[DRAWSTATUS_NORMAL])
			*(pSkinBorderList[DRAWSTATUS_HOVER]) = *(pSkinBorderList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_DEFAULT && pSkinBorderList[DRAWSTATUS_DEFAULT] && pSkinBorderList[DRAWSTATUS_DEFAULT]->nDefault && pSkinBorderList[DRAWSTATUS_NORMAL])
			*(pSkinBorderList[DRAWSTATUS_DEFAULT]) = *(pSkinBorderList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_PRESS && pSkinBorderList[DRAWSTATUS_PRESS] && pSkinBorderList[DRAWSTATUS_PRESS]->nDefault)
			if (pSkinBorderList[DRAWSTATUS_HOVER])
				*(pSkinBorderList[DRAWSTATUS_PRESS]) = *(pSkinBorderList[DRAWSTATUS_HOVER]);
			else if (pSkinBorderList[DRAWSTATUS_NORMAL])
				*(pSkinBorderList[DRAWSTATUS_PRESS]) = *(pSkinBorderList[DRAWSTATUS_HOVER]);
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadBorderItem(void *bditem, SKINBORDER *pSkinBorder)
{
	BOOL bResult = TRUE;
	DOMNode *pNode;
	const WCHAR *wstrNodeName;
	
	pSkinBorder->nDefault = 0;

	pNode = ((DOMNode*)bditem)->getFirstChild();
	while(pNode != NULL)
	{
		wstrNodeName = XStringtoWString(pNode->getNodeName());

		if(wcscmp(wstrNodeName, wstrSkinFileNodeNameColor) == 0)
		{//color
			bResult = LoadColor(pNode, &(pSkinBorder->clrDraw));
		}//color
		else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameImage) == 0)
		{//image
			bResult = LoadImage(pNode, &(pSkinBorder->imgDraw));
		}//image

		pNode = pNode->getNextSibling();
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadSysButton(void *sysbtn, SKINSYSBUTTON *pSkinSysButton)
{
	BOOL bResult = TRUE;
	DOMNode *pNode;
	const WCHAR *wstrNodeName;
	
	pSkinSysButton->nDefault = 0;

	pNode = ((DOMNode*)sysbtn)->getFirstChild();
	while(pNode != NULL)
	{
		wstrNodeName = XStringtoWString(pNode->getNodeName());

		if(wcscmp(wstrNodeName, wstrSkinFileNodeNameColor) == 0)
		{//color
			bResult = LoadColor(pNode, &(pSkinSysButton->clrBk));
		}//color
		else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameImage) == 0)
		{//image
			bResult = LoadImage(pNode, &(pSkinSysButton->imgDraw));
		}//image

		pNode = pNode->getNextSibling();
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadText(void *text, SKINTEXT **pSkinTextList, int nCount)
{
	BOOL bResult = TRUE;
	DOMNode *pTextNode = (DOMNode*)text;
	const WCHAR *wstrNodeName;
	DOMNode *pTempNode;

	if (nCount > 0 && pSkinTextList)
	{
		pTempNode = pTextNode->getFirstChild();
		while (pTempNode != NULL)
		{
			wstrNodeName = XStringtoWString(pTempNode->getNodeName());

			if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
			{//normal
				if (pSkinTextList[DRAWSTATUS_NORMAL])
					LoadTextItem(pTempNode, pSkinTextList[DRAWSTATUS_NORMAL]);
			}//normal
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
			{//disable
				if (nCount > DRAWSTATUS_DISABLE && pSkinTextList[DRAWSTATUS_DISABLE])
					LoadTextItem(pTempNode, pSkinTextList[DRAWSTATUS_DISABLE]);
			}//disable
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
			{//hover
				if (nCount > DRAWSTATUS_HOVER && pSkinTextList[DRAWSTATUS_HOVER])
					LoadTextItem(pTempNode, pSkinTextList[DRAWSTATUS_HOVER]);
			}//hover
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
			{//press
				if (nCount > DRAWSTATUS_PRESS && pSkinTextList[DRAWSTATUS_PRESS])
					LoadTextItem(pTempNode, pSkinTextList[DRAWSTATUS_PRESS]);
			}//press
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameDefault) == 0)
			{//default
				if (nCount > DRAWSTATUS_DEFAULT && pSkinTextList[DRAWSTATUS_DEFAULT])
					LoadTextItem(pTempNode, pSkinTextList[DRAWSTATUS_DEFAULT]);
			}//default
			pTempNode = pTempNode->getNextSibling();
		}

		if (nCount > DRAWSTATUS_DISABLE && pSkinTextList[DRAWSTATUS_DISABLE] && pSkinTextList[DRAWSTATUS_DISABLE]->nDefault && pSkinTextList[DRAWSTATUS_NORMAL])
			*(pSkinTextList[DRAWSTATUS_DISABLE]) = *(pSkinTextList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_HOVER && pSkinTextList[DRAWSTATUS_HOVER] && pSkinTextList[DRAWSTATUS_HOVER]->nDefault && pSkinTextList[DRAWSTATUS_NORMAL])
			*(pSkinTextList[DRAWSTATUS_HOVER]) = *(pSkinTextList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_DEFAULT && pSkinTextList[DRAWSTATUS_DEFAULT] && pSkinTextList[DRAWSTATUS_DEFAULT]->nDefault && pSkinTextList[DRAWSTATUS_NORMAL])
			*(pSkinTextList[DRAWSTATUS_DEFAULT]) = *(pSkinTextList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_PRESS && pSkinTextList[DRAWSTATUS_PRESS] && pSkinTextList[DRAWSTATUS_PRESS]->nDefault)
			if (pSkinTextList[DRAWSTATUS_HOVER])
				*(pSkinTextList[DRAWSTATUS_PRESS]) = *(pSkinTextList[DRAWSTATUS_HOVER]);
			else if (pSkinTextList[DRAWSTATUS_NORMAL])
				*(pSkinTextList[DRAWSTATUS_PRESS]) = *(pSkinTextList[DRAWSTATUS_HOVER]);
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadTextItem(void *txtitem, SKINTEXT *pSkinText)
{
	BOOL bResult = TRUE;
	DOMNode *pNode;
	const WCHAR *wstrNodeName;
	
	pSkinText->nDefault = 0;

	pNode = ((DOMNode*)txtitem)->getFirstChild();
	while(pNode != NULL)
	{
		wstrNodeName = XStringtoWString(pNode->getNodeName());

		if(wcscmp(wstrNodeName, wstrSkinFileNodeNameColor) == 0)
		{//color
			bResult = LoadColorItem(pNode, wstrSkinFileAttrNameStart, &(pSkinText->clrDraw));
		}//color
		if(wcscmp(wstrNodeName, wstrSkinFileNodeNameFont) == 0)
		{//font
			bResult = LoadFont(pNode, &pSkinText->skinFont);
		}//font

		pNode = pNode->getNextSibling();
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadTitleBar(void *titlebar, SKINTITLEBAR *pSkinTitleBar)
{
	BOOL bResult = TRUE;
	DOMNamedNodeMap *pAttr;
	DOMNode *pNode, *pTempNode;
	const WCHAR *wstrNodeName;

	pSkinTitleBar->nDefault = 0;

	pAttr = ((DOMNode*)titlebar)->getAttributes();
	if (pAttr)
	{
		//title bar width
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
		if (pTempNode)
			pSkinTitleBar->nWidth = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

		//title bar left size
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameStart));
		if (pTempNode)
			pSkinTitleBar->nStart = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

		//title bar right size
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameEnd));
		if (pTempNode)
			pSkinTitleBar->nEnd = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

		//whether include border
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameIncludeBorder));
		if (pTempNode)
			pSkinTitleBar->nIncBorder = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
	}

	pNode = ((DOMNode*)titlebar)->getFirstChild();
	while (pNode != NULL)
	{
		wstrNodeName = XStringtoWString(pNode->getNodeName());

		if(wcscmp(wstrNodeName, wstrSkinFileNodeNameBackground) == 0)
		{//title bar background
			pAttr = pNode->getAttributes();
			if (pAttr)
			{
				//background draw type
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
				if (pTempNode)
				{
					pSkinTitleBar->skinBkActive.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
					pSkinTitleBar->skinBkInactive.nDrawType = pSkinTitleBar->skinBkActive.nDrawType;
				}
			}

			//loop to load all sub items
			pTempNode = pNode->getFirstChild();
			while(pTempNode != NULL)
			{
				wstrNodeName = XStringtoWString(pTempNode->getNodeName());

				if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
				{//normal
					LoadBackground(pTempNode, &(pSkinTitleBar->skinBkActive));
				}//normal
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
				{//disable
					LoadBackground(pTempNode, &(pSkinTitleBar->skinBkInactive));
				}//disable

				pTempNode = pTempNode->getNextSibling();
			}
		}//title bar background
		else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameClose) == 0)
		{//close button
			pAttr = pNode->getAttributes();
			if (pAttr)
			{
				//close button draw type
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
				if (pTempNode)
				{
					pSkinTitleBar->skinCloseNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
					pSkinTitleBar->skinCloseHover.nDrawType = pSkinTitleBar->skinCloseNormal.nDrawType;
					pSkinTitleBar->skinClosePress.nDrawType = pSkinTitleBar->skinCloseNormal.nDrawType;
					pSkinTitleBar->skinCloseDisable.nDrawType = pSkinTitleBar->skinCloseNormal.nDrawType;
				}

				//close button width
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
				if (pTempNode)
					pSkinTitleBar->nWidthClose = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button height
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameHeight));
				if (pTempNode)
					pSkinTitleBar->nHeightClose = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button margin-x
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameMarginRight));
				if (pTempNode)
					pSkinTitleBar->nXMarginClose = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button margin-y
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameMarginTop));
				if (pTempNode)
					pSkinTitleBar->nYMarginClose = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			}

			//loop to load all sub items
			pTempNode = pNode->getFirstChild();
			while(pTempNode != NULL)
			{
				wstrNodeName = XStringtoWString(pTempNode->getNodeName());

				if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
				{//normal
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinCloseNormal));
				}//normal
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
				{//hover
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinCloseHover));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
				{//hover
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinClosePress));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
				{//disable
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinCloseDisable));
				}//disable

				pTempNode = pTempNode->getNextSibling();
			}

			if (pSkinTitleBar->skinCloseHover.nDefault)
				pSkinTitleBar->skinCloseHover = pSkinTitleBar->skinCloseNormal;

			if (pSkinTitleBar->skinCloseDisable.nDefault)
				pSkinTitleBar->skinCloseDisable = pSkinTitleBar->skinCloseNormal;

			if (pSkinTitleBar->skinClosePress.nDefault)
				pSkinTitleBar->skinClosePress = pSkinTitleBar->skinCloseHover;
		}//close button
		else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameMaximize) == 0)
		{//maximize button
			pAttr = pNode->getAttributes();
			if (pAttr)
			{
				//close button draw type
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
				if (pTempNode)
				{
					pSkinTitleBar->skinMaxNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
					pSkinTitleBar->skinMaxHover.nDrawType = pSkinTitleBar->skinMaxNormal.nDrawType;
					pSkinTitleBar->skinMaxPress.nDrawType = pSkinTitleBar->skinMaxNormal.nDrawType;
					pSkinTitleBar->skinMaxDisable.nDrawType = pSkinTitleBar->skinMaxNormal.nDrawType;
				}

				//close button width
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
				if (pTempNode)
					pSkinTitleBar->nWidthMax = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button height
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameHeight));
				if (pTempNode)
					pSkinTitleBar->nHeightMax = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button margin-x
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameMarginRight));
				if (pTempNode)
					pSkinTitleBar->nXMarginMax = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button margin-y
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameMarginTop));
				if (pTempNode)
					pSkinTitleBar->nYMarginMax = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			}

			//loop to load all sub items
			pTempNode = pNode->getFirstChild();
			while(pTempNode != NULL)
			{
				wstrNodeName = XStringtoWString(pTempNode->getNodeName());

				if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
				{//normal
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinMaxNormal));
				}//normal
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
				{//hover
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinMaxHover));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
				{//hover
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinMaxPress));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
				{//disable
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinMaxDisable));
				}//disable

				pTempNode = pTempNode->getNextSibling();
			}

			if (pSkinTitleBar->skinMaxHover.nDefault)
				pSkinTitleBar->skinMaxHover = pSkinTitleBar->skinMaxNormal;

			if (pSkinTitleBar->skinMaxDisable.nDefault)
				pSkinTitleBar->skinMaxDisable = pSkinTitleBar->skinMaxNormal;

			if (pSkinTitleBar->skinMaxPress.nDefault)
				pSkinTitleBar->skinMaxPress = pSkinTitleBar->skinMaxHover;
		}//maximize button
		else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRestore) == 0)
		{//restore button
			pAttr = pNode->getAttributes();
			if (pAttr)
			{
				//close button draw type
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
				if (pTempNode)
				{
					pSkinTitleBar->skinRestoreNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
					pSkinTitleBar->skinRestoreHover.nDrawType = pSkinTitleBar->skinRestoreNormal.nDrawType;
					pSkinTitleBar->skinRestorePress.nDrawType = pSkinTitleBar->skinRestoreNormal.nDrawType;
					pSkinTitleBar->skinRestoreDisable.nDrawType = pSkinTitleBar->skinRestoreNormal.nDrawType;
				}

				//close button width
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
				if (pTempNode)
					pSkinTitleBar->nWidthMax = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button height
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameHeight));
				if (pTempNode)
					pSkinTitleBar->nHeightMax = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button margin-x
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameMarginRight));
				if (pTempNode)
					pSkinTitleBar->nXMarginMax = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button margin-y
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameMarginTop));
				if (pTempNode)
					pSkinTitleBar->nYMarginMax = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			}

			//loop to load all sub items
			pTempNode = pNode->getFirstChild();
			while(pTempNode != NULL)
			{
				wstrNodeName = XStringtoWString(pTempNode->getNodeName());

				if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
				{//normal
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinRestoreNormal));
				}//normal
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
				{//hover
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinRestoreHover));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
				{//hover
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinRestorePress));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
				{//disable
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinRestoreDisable));
				}//disable

				pTempNode = pTempNode->getNextSibling();
			}

			if (pSkinTitleBar->skinRestoreHover.nDefault)
				pSkinTitleBar->skinRestoreHover = pSkinTitleBar->skinRestoreNormal;

			if (pSkinTitleBar->skinRestoreDisable.nDefault)
				pSkinTitleBar->skinRestoreDisable = pSkinTitleBar->skinRestoreNormal;

			if (pSkinTitleBar->skinRestorePress.nDefault)
				pSkinTitleBar->skinRestorePress = pSkinTitleBar->skinRestoreHover;
		}//restore button
		else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameMinimize) == 0)
		{//minimize button
			pAttr = pNode->getAttributes();
			if (pAttr)
			{
				//close button draw type
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
				if (pTempNode)
				{
					pSkinTitleBar->skinMinNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
					pSkinTitleBar->skinMinHover.nDrawType = pSkinTitleBar->skinMinNormal.nDrawType;
					pSkinTitleBar->skinMinPress.nDrawType = pSkinTitleBar->skinMinNormal.nDrawType;
					pSkinTitleBar->skinMinDisable.nDrawType = pSkinTitleBar->skinMinNormal.nDrawType;
				}

				//close button width
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
				if (pTempNode)
					pSkinTitleBar->nWidthMin = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button height
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameHeight));
				if (pTempNode)
					pSkinTitleBar->nHeightMin = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button margin-x
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameMarginRight));
				if (pTempNode)
					pSkinTitleBar->nXMarginMin = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

				//close button margin-y
				pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameMarginTop));
				if (pTempNode)
					pSkinTitleBar->nYMarginMin = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			}

			//loop to load all sub items
			pTempNode = pNode->getFirstChild();
			while(pTempNode != NULL)
			{
				wstrNodeName = XStringtoWString(pTempNode->getNodeName());

				if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
				{//normal
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinMinNormal));
				}//normal
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
				{//hover
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinMinHover));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
				{//hover
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinMinPress));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
				{//disable
					LoadSysButton(pTempNode, &(pSkinTitleBar->skinMinDisable));
				}//disable

				pTempNode = pTempNode->getNextSibling();
			}

			if (pSkinTitleBar->skinMinHover.nDefault)
				pSkinTitleBar->skinMinHover = pSkinTitleBar->skinMinNormal;

			if (pSkinTitleBar->skinMinDisable.nDefault)
				pSkinTitleBar->skinMinDisable = pSkinTitleBar->skinMinNormal;

			if (pSkinTitleBar->skinMinPress.nDefault)
				pSkinTitleBar->skinMinPress = pSkinTitleBar->skinMinHover;
		}//minimize button

		pNode = pNode->getNextSibling();
	}

	if (pSkinTitleBar->skinBkInactive.nDefault)
		pSkinTitleBar->skinBkInactive = pSkinTitleBar->skinBkActive;

	return bResult;
}