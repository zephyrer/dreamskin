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
#include "DreamSkinEdit.h"
#include "DreamSkinButton.h"
#include "DreamSkinStatic.h"
#include "DreamSkinTab.h"
#include "DreamSkinScrollBar.h"
#include "DreamSkinHeaderCtrl.h"
#include "DreamSkinListBox.h"
#include "DreamSkinListCtrl.h"
#include "DreamSkinComboBox.h"
#include "DreamSkinProgressCtrl.h"
#include "DreamSkinSliderCtrl.h"
#include "DreamSkinSpinCtrl.h"
#include "WinFileEx.h"

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

WCHAR CDreamSkinLoader::wstrSkinFileNodeNameButton[] = L"button";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameCheckBox[] = L"checkbox";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameComboBox[] = L"combobox";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameDialog[] = L"dialog";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameEdit[] = L"edit";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameGroupBox[] = L"groupbox";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameHeaderCtrl[] = L"headerctrl";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameListBox[] = L"listbox";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameListCtrl[] = L"listctrl";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameProgressCtrl[] = L"progressctrl";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameRadio[] = L"radio";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameScrollBar[] = L"scrollbar";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameSliderCtrl[] = L"sliderctrl";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameSpinCtrl[] = L"spinctrl";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameStatic[] = L"static";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameTab[] = L"tab";

WCHAR CDreamSkinLoader::wstrSkinFileNodeNameBackground[] = L"background";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameForeground[] = L"foreground";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameBorder[] = L"border";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameColor[] = L"color";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameFont[] = L"font";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameText[] = L"text";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameIcon[] = L"icon";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameImage[] = L"image";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameItem[] = L"item";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameThumb[] = L"thumb";
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
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameReadWrite[] = L"readwrite";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameReadOnly[] = L"readonly";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameClose[] = L"close";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameMaximize[] = L"maximize";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameRestore[] = L"restore";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameMinimize[] = L"minimize";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameExpand[] = L"expand";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameHorizontal[] = L"horizontal";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameVertical[] = L"vertical";
WCHAR CDreamSkinLoader::wstrSkinFileNodeNameSlider[] = L"slider";

WCHAR CDreamSkinLoader::wstrSkinFileAttrNameWidth[] = L"width";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameHeight[] = L"height";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameType[] = L"type";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameRegion[] = L"region";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameTransparent[] = L"transparent";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameSource[] = L"source";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameStart[] = L"start";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameEnd[] = L"end";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameText[] = L"text";
WCHAR CDreamSkinLoader::wstrSkinFileAttrNameBorder[] = L"border";
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

	memset(m_wstrSkinFilePath, 0, sizeof(WCHAR) * MAX_PATH);
	memset(m_wstrSkinFileDir, 0, sizeof(WCHAR) * MAX_PATH);
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
		wcscpy_s(m_wstrSkinFilePath, MAX_PATH, wstrSkinFilePath);
		::GetDirectoryName(m_wstrSkinFilePath, MAX_PATH, m_wstrSkinFileDir);

		//Load Skin Dialog
		CDreamSkinDialog::GetDefaultSkin(&m_SkinDialog);
		LoadSkinDialog(parser);

		//Load Skin Button
		CDreamSkinButton::GetDefaultButtonSkin(&m_SkinButton);
		LoadSkinButton(parser);

		//Load Skin CheckBox
		CDreamSkinButton::GetDefaultCheckBoxSkin(&m_SkinCheckBox);
		LoadSkinCheckBox(parser);

		//Load Skin Radio
		CDreamSkinButton::GetDefaultRadioSkin(&m_SkinRadio);
		LoadSkinRadio(parser);

		CDreamSkinButton::GetDefaultGroupBoxSkin(&m_SkinGroupBox);
		LoadSkinGroupBox(parser);

		//Load Skin Edit
		CDreamSkinEdit::GetDefaultSkin(&m_SkinEdit);
		LoadSkinEdit(parser);

		//Load Skin ScrollBar
		CDreamSkinScrollBar::GetDefaultSkin(&m_SkinScrollBar);
		LoadSkinScrollBar(parser);

		//Load Skin Static
		CDreamSkinStatic::GetDefaultSkin(&m_SkinStatic);
		LoadSkinStatic(parser);

		//Load Skin Tab
		CDreamSkinTab::GetDefaultSkin(&m_SkinTab);
		LoadSkinTab(parser);

		//Load Skin ListBox
		CDreamSkinListBox::GetDefaultSkin(&m_SkinListBox);
		LoadSkinListBox(parser);

		//Load Skin ListCtrl
		CDreamSkinListCtrl::GetDefaultSkin(&m_SkinListCtrl);
		LoadSkinListCtrl(parser);

		//Load Skin HeaderCtrl
		CDreamSkinHeaderCtrl::GetDefaultSkin(&m_SkinHeaderCtrl);
		LoadSkinHeaderCtrl(parser);

		//Load Skin ComboBox
		CDreamSkinComboBox::GetDefaultSkin(&m_SkinComboBox);
		LoadSkinComboBox(parser);

		//Load Skin ProgressCtrl
		CDreamSkinProgressCtrl::GetDefaultSkin(&m_SkinProgressCtrl);
		LoadSkinProgressCtrl(parser);

		//Load Skin SliderCtrl
		CDreamSkinSliderCtrl::GetDefaultSkin(&m_SkinSliderCtrl);
		LoadSkinSliderCtrl(parser);

		//Load Skin SpinCtrl
		CDreamSkinSpinCtrl::GetDefaultSkin(&m_SkinSpinCtrl);
		LoadSkinSpinCtrl(parser);

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

void CDreamSkinLoader::GetSkinComboBox(SKINCOMBOBOX *pSkinComboBox) const
{
	if (pSkinComboBox)
	{
		memcpy(pSkinComboBox, &m_SkinComboBox, sizeof(SKINCOMBOBOX));
	}
}

void CDreamSkinLoader::GetSkinDialog(SKINDIALOG *pSkinDialog) const
{
	if (pSkinDialog)
	{
		memcpy(pSkinDialog, &m_SkinDialog, sizeof(SKINDIALOG));
	}
}

void CDreamSkinLoader::GetSkinEdit(SKINEDIT *pSkinEdit) const
{
	if (pSkinEdit)
	{
		memcpy(pSkinEdit, &m_SkinEdit, sizeof(SKINEDIT));
	}
}

void CDreamSkinLoader::GetSkinTab(SKINTAB *pSkinTab) const
{
	if (pSkinTab)
	{
		memcpy(pSkinTab, &m_SkinTab, sizeof(SKINTAB));
	}
}

void CDreamSkinLoader::GetSkinGroupBox(SKINGROUPBOX *pSkinGroupBox) const
{
	if (pSkinGroupBox)
	{
		memcpy(pSkinGroupBox, &m_SkinGroupBox, sizeof(SKINGROUPBOX));
	}
}

void CDreamSkinLoader::GetSkinHeaderCtrl(SKINHEADERCTRL *pSkinHeaderCtrl) const
{
	if (pSkinHeaderCtrl)
	{
		memcpy(pSkinHeaderCtrl, &m_SkinHeaderCtrl, sizeof(SKINHEADERCTRL));
	}
}


void CDreamSkinLoader::GetSkinListBox(SKINLISTBOX *pSkinListBox) const
{
	if (pSkinListBox)
	{
		memcpy(pSkinListBox, &m_SkinListBox, sizeof(SKINLISTBOX));
	}
}

void CDreamSkinLoader::GetSkinListCtrl(SKINLISTCTRL *pSkinListCtrl) const
{
	if (pSkinListCtrl)
	{
		memcpy(pSkinListCtrl, &m_SkinListCtrl, sizeof(SKINLISTCTRL));
	}
}

void CDreamSkinLoader::GetSkinProgressCtrl(SKINPROGRESSCTRL *pSkinProgressCtrl) const
{
	if (pSkinProgressCtrl)
	{
		memcpy(pSkinProgressCtrl, &m_SkinProgressCtrl, sizeof(SKINPROGRESSCTRL));
	}
}

void CDreamSkinLoader::GetSkinRadio(SKINRADIO *pSkinRadio) const
{
	if (pSkinRadio)
	{
		memcpy(pSkinRadio, &m_SkinRadio, sizeof(SKINRADIO));
	}
}

void CDreamSkinLoader::GetSkinScrollBar(SKINSCROLLBAR *pSkinScrollBar) const
{
	if (pSkinScrollBar)
	{
		memcpy(pSkinScrollBar, &m_SkinScrollBar, sizeof(SKINSCROLLBAR));
	}
}

void CDreamSkinLoader::GetSkinSliderCtrl(SKINSLIDERCTRL *pSkinSliderCtrl) const
{
	if (pSkinSliderCtrl)
	{
		memcpy(pSkinSliderCtrl, &m_SkinSliderCtrl, sizeof(SKINSLIDERCTRL));
	}
}

void CDreamSkinLoader::GetSkinSpinCtrl(SKINSPINCTRL *pSkinSpinCtrl) const
{
	if (pSkinSpinCtrl)
	{
		memcpy(pSkinSpinCtrl, &m_SkinSpinCtrl, sizeof(SKINSPINCTRL));
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
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pButtonNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameButton);

	if (pButtonNode)
	{
		bResult = LoadButton(pButtonNode, &m_SkinButton);
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

BOOL CDreamSkinLoader::LoadSkinComboBox(void *parser)
{
	BOOL bResult = TRUE;

	DOMNode *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pComboBoxNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameComboBox);
	const WCHAR *wstrNodeName;
	SKINTEXT *pSkinTextList[DRAWSTATUS_MAXCOUNT + 1];
	SKINBORDER *pSkinBorderList[DRAWSTATUS_MAXCOUNT + 1];

	if (pComboBoxNode)
	{
		//loop to load all settings
		pParentNode = pComboBoxNode->getFirstChild();
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
						m_SkinComboBox.skinBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						m_SkinComboBox.skinBkDisable.nDrawType = m_SkinComboBox.skinBkNormal.nDrawType;
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadBackground(pChildNode, &(m_SkinComboBox.skinBkNormal));
					}//normal
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
					{//disable
						LoadBackground(pChildNode, &(m_SkinComboBox.skinBkDisable));
					}//disable

					pChildNode = pChildNode->getNextSibling();
				}

				if (m_SkinComboBox.skinBkDisable.nDefault)
					m_SkinComboBox.skinBkDisable = m_SkinComboBox.skinBkNormal;
			}//background
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
			{//text
				pSkinTextList[DRAWSTATUS_NORMAL] = &m_SkinComboBox.skinTxtNormal;
				pSkinTextList[DRAWSTATUS_DISABLE] = &m_SkinComboBox.skinTxtDisable;
				LoadText(pParentNode, pSkinTextList, DRAWSTATUS_DISABLE + 1);
			}//text
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
			{//border
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
					{//left
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinComboBox.skinLBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinComboBox.skinLBorderDisable;
						pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinComboBox.skinLBorderHover;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_HOVER + 1);
					}//left
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
					{//right
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinComboBox.skinRBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinComboBox.skinRBorderDisable;
						pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinComboBox.skinRBorderHover;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_HOVER + 1);
					}//right
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
					{//top
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinComboBox.skinTBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinComboBox.skinTBorderDisable;
						pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinComboBox.skinTBorderHover;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_HOVER + 1);
					}//top
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
					{//bottom
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinComboBox.skinBBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinComboBox.skinBBorderDisable;
						pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinComboBox.skinBBorderHover;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_HOVER + 1);
					}//bottom

					pChildNode = pChildNode->getNextSibling();
				}
			}//border
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameExpand) == 0)
			{//expand button
				pAttr = pParentNode->getAttributes();
				if (pAttr)
				{
					//left button draw type
					pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
					if (pTempNode)
					{
						m_SkinComboBox.skinBtnNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						m_SkinComboBox.skinBtnDisable.nDrawType = m_SkinComboBox.skinBtnNormal.nDrawType;
						m_SkinComboBox.skinBtnHover.nDrawType = m_SkinComboBox.skinBtnNormal.nDrawType;
						m_SkinComboBox.skinBtnPress.nDrawType = m_SkinComboBox.skinBtnNormal.nDrawType;
					}

					//whether include border
					pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameIncludeBorder));
					if (pTempNode)
						m_SkinComboBox.nBtnIncludeBorder = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadSysButtonItem(pChildNode, &(m_SkinComboBox.skinBtnNormal));
					}//normal
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
					{//hover
						LoadSysButtonItem(pChildNode, &(m_SkinComboBox.skinBtnHover));
					}//hover
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
					{//press
						LoadSysButtonItem(pChildNode, &(m_SkinComboBox.skinBtnPress));
					}//press
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
					{//disable
						LoadSysButtonItem(pChildNode, &(m_SkinComboBox.skinBtnDisable));
					}//disable

					pChildNode = pChildNode->getNextSibling();
				}

				if (m_SkinComboBox.skinBtnHover.nDefault)
					m_SkinComboBox.skinBtnHover = m_SkinComboBox.skinBtnNormal;

				if (m_SkinComboBox.skinBtnDisable.nDefault)
					m_SkinComboBox.skinBtnDisable = m_SkinComboBox.skinBtnNormal;

				if (m_SkinComboBox.skinBtnPress.nDefault)
					m_SkinComboBox.skinBtnPress = m_SkinComboBox.skinBtnHover;
			}//expand button

			pParentNode = pParentNode->getNextSibling();
		}
	} //end load combobox

	m_SkinComboBox.skinListBox = m_SkinListBox;

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinEdit(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pStatusNode, *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pEditNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameEdit);
	const WCHAR *wstrNodeName;
	SKINTEXT *pSkinTextList[DRAWSTATUS_PRESS + 1];
	SKINBORDER *pSkinBorderList[DRAWSTATUS_PRESS + 1];

	if (pEditNode)
	{
		//loop to load all settings
		pStatusNode = pEditNode->getFirstChild();
		while (pStatusNode != NULL)
		{
			wstrNodeName = XStringtoWString(pStatusNode->getNodeName());
			if (wcscmp(wstrNodeName, wstrSkinFileNodeNameReadWrite) == 0)
			{//readwrite
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
								m_SkinEdit.skinBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinEdit.skinBkDisable.nDrawType = m_SkinEdit.skinBkNormal.nDrawType;
								m_SkinEdit.skinBkHover.nDrawType = m_SkinEdit.skinBkNormal.nDrawType;
								m_SkinEdit.skinBkPress.nDrawType = m_SkinEdit.skinBkNormal.nDrawType;
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinEdit.skinBkNormal));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadBackground(pChildNode, &(m_SkinEdit.skinBkDisable));
							}//disable
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
							{//hover
								LoadBackground(pChildNode, &(m_SkinEdit.skinBkHover));
							}//hover
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
							{//press
								LoadBackground(pChildNode, &(m_SkinEdit.skinBkPress));
							}//press

							pChildNode = pChildNode->getNextSibling();
						}
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
					{//text
						pSkinTextList[DRAWSTATUS_NORMAL] = &m_SkinEdit.skinTxtNormal;
						pSkinTextList[DRAWSTATUS_DISABLE] = &m_SkinEdit.skinTxtDisable;
						pSkinTextList[DRAWSTATUS_HOVER] = &m_SkinEdit.skinTxtHover;
						pSkinTextList[DRAWSTATUS_PRESS] = &m_SkinEdit.skinTxtPress;
						LoadText(pParentNode, pSkinTextList, DRAWSTATUS_PRESS + 1);
					}//text
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
					{//border
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
							{//left
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinEdit.skinLBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinEdit.skinLBorderDisable;
								pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinEdit.skinLBorderHover;
								pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinEdit.skinLBorderPress;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_PRESS + 1);
							}//left
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
							{//right
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinEdit.skinRBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinEdit.skinRBorderDisable;
								pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinEdit.skinRBorderHover;
								pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinEdit.skinRBorderPress;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_PRESS + 1);
							}//right
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
							{//top
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinEdit.skinTBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinEdit.skinTBorderDisable;
								pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinEdit.skinTBorderHover;
								pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinEdit.skinTBorderPress;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_PRESS + 1);
							}//top
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
							{//bottom
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinEdit.skinBBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinEdit.skinBBorderDisable;
								pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinEdit.skinBBorderHover;
								pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinEdit.skinBBorderPress;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_PRESS + 1);
							}//bottom

							pChildNode = pChildNode->getNextSibling();
						}
					}//border

					pParentNode = pParentNode->getNextSibling();
				}
			}//readwrite
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameReadOnly) == 0)
			{//readonly
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
								m_SkinEdit.skinBkNormalReadOnly.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinEdit.skinBkHoverReadOnly.nDrawType = m_SkinEdit.skinBkNormalReadOnly.nDrawType;
								m_SkinEdit.skinBkPressReadOnly.nDrawType = m_SkinEdit.skinBkNormalReadOnly.nDrawType;
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinEdit.skinBkNormalReadOnly));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
							{//hover
								LoadBackground(pChildNode, &(m_SkinEdit.skinBkHoverReadOnly));
							}//hover
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
							{//press
								LoadBackground(pChildNode, &(m_SkinEdit.skinBkPressReadOnly));
							}//press

							pChildNode = pChildNode->getNextSibling();
						}
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
					{//text
						pSkinTextList[DRAWSTATUS_NORMAL] = &m_SkinEdit.skinTxtNormalReadOnly;
						pSkinTextList[DRAWSTATUS_DISABLE] = NULL;
						pSkinTextList[DRAWSTATUS_HOVER] = &m_SkinEdit.skinTxtHoverReadOnly;
						pSkinTextList[DRAWSTATUS_PRESS] = &m_SkinEdit.skinTxtPressReadOnly;
						LoadText(pParentNode, pSkinTextList, DRAWSTATUS_PRESS + 1);
					}//text
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
					{//border
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
							{//left
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinEdit.skinLBorderNormalReadOnly;
								pSkinBorderList[DRAWSTATUS_DISABLE] = NULL;
								pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinEdit.skinLBorderHoverReadOnly;
								pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinEdit.skinLBorderPressReadOnly;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_PRESS + 1);
							}//left
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
							{//right
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinEdit.skinRBorderNormalReadOnly;
								pSkinBorderList[DRAWSTATUS_DISABLE] = NULL;
								pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinEdit.skinRBorderHoverReadOnly;
								pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinEdit.skinRBorderPressReadOnly;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_PRESS + 1);
							}//right
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
							{//top
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinEdit.skinTBorderNormalReadOnly;
								pSkinBorderList[DRAWSTATUS_DISABLE] = NULL;
								pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinEdit.skinTBorderHoverReadOnly;
								pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinEdit.skinTBorderPressReadOnly;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_PRESS + 1);
							}//top
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
							{//bottom
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinEdit.skinBBorderNormalReadOnly;
								pSkinBorderList[DRAWSTATUS_DISABLE] = NULL;
								pSkinBorderList[DRAWSTATUS_HOVER] = &m_SkinEdit.skinBBorderHoverReadOnly;
								pSkinBorderList[DRAWSTATUS_PRESS] = &m_SkinEdit.skinBBorderPressReadOnly;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_PRESS + 1);
							}//bottom

							pChildNode = pChildNode->getNextSibling();
						}
					}//border

					pParentNode = pParentNode->getNextSibling();
				}
			}//readonly

			pStatusNode = pStatusNode->getNextSibling();
		}

		if (m_SkinEdit.skinBkDisable.nDefault)
			m_SkinEdit.skinBkDisable = m_SkinEdit.skinBkNormal;

		if (m_SkinEdit.skinBkHover.nDefault)
			m_SkinEdit.skinBkHover = m_SkinEdit.skinBkNormal;

		if (m_SkinEdit.skinBkPress.nDefault)
			m_SkinEdit.skinBkPress = m_SkinEdit.skinBkHover;

		if (m_SkinEdit.skinBkNormalReadOnly.nDefault)
			m_SkinEdit.skinBkNormalReadOnly = m_SkinEdit.skinBkDisable;

		if (m_SkinEdit.skinBkHoverReadOnly.nDefault)
			m_SkinEdit.skinBkHoverReadOnly = m_SkinEdit.skinBkNormalReadOnly;

		if (m_SkinEdit.skinBkPressReadOnly.nDefault)
			m_SkinEdit.skinBkPressReadOnly = m_SkinEdit.skinBkHoverReadOnly;
	} //end load edit

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinGroupBox(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pGroupBoxNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameGroupBox);
	const WCHAR *wstrNodeName;
	SKINTEXT *pSkinText;
	SKINBORDER *pSkinBorder;

	if (pGroupBoxNode)
	{
		//loop to load all settings
		pParentNode = pGroupBoxNode->getFirstChild();
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
						m_SkinGroupBox.skinBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadBackground(pChildNode, &(m_SkinGroupBox.skinBkNormal));
					}//normal

					pChildNode = pChildNode->getNextSibling();
				}
			}//background
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
			{//text
				pSkinText = &m_SkinGroupBox.skinTxtNormal;
				LoadText(pParentNode, &pSkinText, 1);
			}//text
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
			{//border
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
					{//left
						pSkinBorder = &m_SkinGroupBox.skinLBorderNormal;
						LoadBorder(pChildNode, &pSkinBorder, 1);
					}//left
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
					{//right
						pSkinBorder = &m_SkinGroupBox.skinRBorderNormal;
						LoadBorder(pChildNode, &pSkinBorder, 1);
					}//right
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
					{//top
						pSkinBorder = &m_SkinGroupBox.skinTBorderNormal;
						LoadBorder(pChildNode, &pSkinBorder, 1);
					}//top
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
					{//bottom
						pSkinBorder = &m_SkinGroupBox.skinBBorderNormal;
						LoadBorder(pChildNode, &pSkinBorder, 1);
					}//bottom

					pChildNode = pChildNode->getNextSibling();
				}
			}//border

			pParentNode = pParentNode->getNextSibling();
		}
	} //end load groupbox

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinHeaderCtrl(void *parser)
{
	BOOL bResult = TRUE;

	DOMNode *pParentNode;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pHeaderCtrlNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameHeaderCtrl);
	const WCHAR *wstrNodeName;
	SKINITEM *pSkinItemList[DRAWSTATUS_MAXCOUNT];

	if (pHeaderCtrlNode)
	{
		//loop to load all settings
		pParentNode = pHeaderCtrlNode->getFirstChild();
		while (pParentNode != NULL)
		{
			wstrNodeName = XStringtoWString(pParentNode->getNodeName());
			if (wcscmp(wstrNodeName, wstrSkinFileNodeNameItem) == 0)
			{//item
				pSkinItemList[DRAWSTATUS_NORMAL] = &m_SkinHeaderCtrl.skinItemNormal;
				pSkinItemList[DRAWSTATUS_DISABLE] = &m_SkinHeaderCtrl.skinItemDisabled;
				pSkinItemList[DRAWSTATUS_HOVER] = &m_SkinHeaderCtrl.skinItemHover;
				pSkinItemList[DRAWSTATUS_PRESS] = &m_SkinHeaderCtrl.skinItemPress;
				LoadItem(pParentNode, pSkinItemList, DRAWSTATUS_PRESS + 1);
			}//item

			pParentNode = pParentNode->getNextSibling();
		}
	}//headerctrl settings

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinListBox(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pListBoxNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameListBox);
	const WCHAR *wstrNodeName;
	SKINBORDER *pSkinBorderList[DRAWSTATUS_DISABLE + 1];
	SKINITEM *pSkinItemList[DRAWSTATUS_MAXCOUNT];

	if (pListBoxNode)
	{
		//loop to load all settings
		pParentNode = pListBoxNode->getFirstChild();
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
						m_SkinListBox.skinBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						m_SkinListBox.skinBkDisable.nDrawType = m_SkinListBox.skinBkNormal.nDrawType;
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadBackground(pChildNode, &(m_SkinListBox.skinBkNormal));
					}//normal
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
					{//disable
						LoadBackground(pChildNode, &(m_SkinListBox.skinBkDisable));
					}//disable

					pChildNode = pChildNode->getNextSibling();
				}

				if (m_SkinListBox.skinBkDisable.nDefault)
					m_SkinListBox.skinBkDisable = m_SkinListBox.skinBkNormal;
			}//background
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
			{//border
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
					{//left
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinListBox.skinLBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinListBox.skinLBorderDisable;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//left
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
					{//right
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinListBox.skinRBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinListBox.skinRBorderDisable;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//right
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
					{//top
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinListBox.skinTBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinListBox.skinTBorderDisable;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//top
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
					{//bottom
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinListBox.skinBBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinListBox.skinBBorderDisable;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//bottom

					pChildNode = pChildNode->getNextSibling();
				}
			}//border
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameItem) == 0)
			{//item
				pChildNode = pParentNode->getFirstChild();
				while (pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());
					if (wcscmp(wstrNodeName, wstrSkinFileNodeNameUnselected) == 0)
					{//unselected
						pSkinItemList[DRAWSTATUS_NORMAL] = &m_SkinListBox.skinItemNormalUnselected;
						pSkinItemList[DRAWSTATUS_DISABLE] = &m_SkinListBox.skinItemDisableUnselected;
						pSkinItemList[DRAWSTATUS_HOVER] = &m_SkinListBox.skinItemHoverUnselected;
						LoadItem(pChildNode, pSkinItemList, DRAWSTATUS_HOVER + 1);
					}//unselected
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameSelected) == 0)
					{//selected
						pSkinItemList[DRAWSTATUS_NORMAL] = &m_SkinListBox.skinItemNormalSelected;
						pSkinItemList[DRAWSTATUS_DISABLE] = &m_SkinListBox.skinItemDisableSelected;
						pSkinItemList[DRAWSTATUS_HOVER] = &m_SkinListBox.skinItemHoverSelected;
						LoadItem(pChildNode, pSkinItemList, DRAWSTATUS_HOVER + 1);
					}//selected

					pChildNode = pChildNode->getNextSibling();
				}
			}//item

			pParentNode = pParentNode->getNextSibling();
		}
	}//listbox settings

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinListCtrl(void *parser)
{
	BOOL bResult = TRUE;

	DOMNode *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pListCtrlNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameListCtrl);
	const WCHAR *wstrNodeName;
	SKINBORDER *pSkinBorderList[DRAWSTATUS_DISABLE + 1];
	SKINITEM *pSkinItemList[DRAWSTATUS_MAXCOUNT];

	if (pListCtrlNode)
	{
		//loop to load all settings
		pParentNode = pListCtrlNode->getFirstChild();
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
						m_SkinListCtrl.skinBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						m_SkinListCtrl.skinBkDisable.nDrawType = m_SkinListCtrl.skinBkNormal.nDrawType;
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadBackground(pChildNode, &(m_SkinListCtrl.skinBkNormal));
					}//normal
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
					{//disable
						LoadBackground(pChildNode, &(m_SkinListCtrl.skinBkDisable));
					}//disable

					pChildNode = pChildNode->getNextSibling();
				}

				if (m_SkinListCtrl.skinBkDisable.nDefault)
					m_SkinListCtrl.skinBkDisable = m_SkinListCtrl.skinBkNormal;
			}//background
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
			{//border
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
					{//left
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinListCtrl.skinLBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinListCtrl.skinLBorderDisable;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//left
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
					{//right
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinListCtrl.skinRBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinListCtrl.skinRBorderDisable;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//right
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
					{//top
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinListCtrl.skinTBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinListCtrl.skinTBorderDisable;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//top
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
					{//bottom
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinListCtrl.skinBBorderNormal;
						pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinListCtrl.skinBBorderDisable;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
					}//bottom

					pChildNode = pChildNode->getNextSibling();
				}
			}//border
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameItem) == 0)
			{//item
				pChildNode = pParentNode->getFirstChild();
				while (pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());
					if (wcscmp(wstrNodeName, wstrSkinFileNodeNameUnselected) == 0)
					{//unselected
						pSkinItemList[DRAWSTATUS_NORMAL] = &m_SkinListCtrl.skinItemNormalUnselected;
						pSkinItemList[DRAWSTATUS_DISABLE] = &m_SkinListCtrl.skinItemDisableUnselected;
						pSkinItemList[DRAWSTATUS_HOVER] = &m_SkinListCtrl.skinItemHoverUnselected;
						LoadItem(pChildNode, pSkinItemList, DRAWSTATUS_HOVER + 1);
					}//unselected
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameSelected) == 0)
					{//selected
						pSkinItemList[DRAWSTATUS_NORMAL] = &m_SkinListCtrl.skinItemNormalSelected;
						pSkinItemList[DRAWSTATUS_DISABLE] = &m_SkinListCtrl.skinItemDisableSelected;
						pSkinItemList[DRAWSTATUS_HOVER] = &m_SkinListCtrl.skinItemHoverSelected;
						LoadItem(pChildNode, pSkinItemList, DRAWSTATUS_HOVER + 1);
					}//selected

					pChildNode = pChildNode->getNextSibling();
				}
			}//item

			pParentNode = pParentNode->getNextSibling();
		}
	}//listctrl settings

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinProgressCtrl(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pTypeNode, *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pProgressCtrlNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameProgressCtrl);
	const WCHAR *wstrNodeName;
	SKINBORDER *pSkinBorderList[DRAWSTATUS_DISABLE + 1];

	if (pProgressCtrlNode)
	{
		//loop to load all settings
		pTypeNode = pProgressCtrlNode->getFirstChild();
		while (pTypeNode != NULL)
		{
			wstrNodeName = XStringtoWString(pTypeNode->getNodeName());
			if (wcscmp(wstrNodeName, wstrSkinFileNodeNameHorizontal) == 0)
			{//horizontal
				pParentNode = pTypeNode->getFirstChild();
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
								m_SkinProgressCtrl.skinHBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinProgressCtrl.skinHBkNormal));
							}//normal

							pChildNode = pChildNode->getNextSibling();
						}
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameForeground) == 0)
					{//foreground
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//background draw type
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
							if (pTempNode)
							{
								m_SkinProgressCtrl.skinHFrNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinProgressCtrl.skinHFrNormal));
							}//normal

							pChildNode = pChildNode->getNextSibling();
						}
					}//foreground
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
					{//border
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
							{//left
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinProgressCtrl.skinHLBorderNormal;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
							}//left
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
							{//right
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinProgressCtrl.skinHRBorderNormal;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
							}//right
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
							{//top
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinProgressCtrl.skinHTBorderNormal;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
							}//top
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
							{//bottom
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinProgressCtrl.skinHBBorderNormal;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
							}//bottom

							pChildNode = pChildNode->getNextSibling();
						}
					}//border

					pParentNode = pParentNode->getNextSibling();
				}
			}//horizontal
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameVertical) == 0)
			{//vertical
				pParentNode = pTypeNode->getFirstChild();
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
								m_SkinProgressCtrl.skinVBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinProgressCtrl.skinVBkNormal));
							}//normal

							pChildNode = pChildNode->getNextSibling();
						}
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameForeground) == 0)
					{//foreground
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//background draw type
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
							if (pTempNode)
							{
								m_SkinProgressCtrl.skinVFrNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinProgressCtrl.skinVFrNormal));
							}//normal

							pChildNode = pChildNode->getNextSibling();
						}
					}//foreground
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
					{//border
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
							{//left
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinProgressCtrl.skinVLBorderNormal;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
							}//left
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
							{//right
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinProgressCtrl.skinVRBorderNormal;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
							}//right
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
							{//top
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinProgressCtrl.skinVTBorderNormal;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
							}//top
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
							{//bottom
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinProgressCtrl.skinVBBorderNormal;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
							}//bottom

							pChildNode = pChildNode->getNextSibling();
						}
					}//border

					pParentNode = pParentNode->getNextSibling();
				}
			}//vertical

			pTypeNode = pTypeNode->getNextSibling();
		}
	} //end load progress control

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinRadio(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pStatusNode, *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pRadioNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameRadio);
	const WCHAR *wstrNodeName;
	SKINTEXT *pSkinTextList[DRAWSTATUS_PRESS + 1];
	SKINICON *pSkinIconList[DRAWSTATUS_PRESS + 1];

	if (pRadioNode)
	{
		//loop to load all settings
		pStatusNode = pRadioNode->getFirstChild();
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
								m_SkinRadio.skinBkNormalUnchecked.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinRadio.skinBkDisableUnchecked.nDrawType = m_SkinRadio.skinBkNormalUnchecked.nDrawType;
								m_SkinRadio.skinBkHoverUnchecked.nDrawType = m_SkinRadio.skinBkNormalUnchecked.nDrawType;
								m_SkinRadio.skinBkPressUnchecked.nDrawType = m_SkinRadio.skinBkNormalUnchecked.nDrawType;
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinRadio.skinBkNormalUnchecked));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadBackground(pChildNode, &(m_SkinRadio.skinBkDisableUnchecked));
							}//disable
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
							{//hover
								LoadBackground(pChildNode, &(m_SkinRadio.skinBkHoverUnchecked));
							}//hover
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
							{//press
								LoadBackground(pChildNode, &(m_SkinRadio.skinBkPressUnchecked));
							}//press

							pChildNode = pChildNode->getNextSibling();
						}
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
					{//text
						pSkinTextList[DRAWSTATUS_NORMAL] = &m_SkinRadio.skinTxtNormalUnchecked;
						pSkinTextList[DRAWSTATUS_DISABLE] = &m_SkinRadio.skinTxtDisableUnchecked;
						pSkinTextList[DRAWSTATUS_HOVER] = &m_SkinRadio.skinTxtHoverUnchecked;
						pSkinTextList[DRAWSTATUS_PRESS] = &m_SkinRadio.skinTxtPressUnchecked;
						LoadText(pParentNode, pSkinTextList, DRAWSTATUS_PRESS + 1);
					}//text
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameIcon) == 0)
					{//icon
						pSkinIconList[DRAWSTATUS_NORMAL] = &m_SkinRadio.iconNormalUnchecked;
						pSkinIconList[DRAWSTATUS_DISABLE] = &m_SkinRadio.iconDisableUnchecked;
						pSkinIconList[DRAWSTATUS_HOVER] = &m_SkinRadio.iconHoverUnchecked;
						pSkinIconList[DRAWSTATUS_PRESS] = &m_SkinRadio.iconPressUnchecked;
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
								m_SkinRadio.skinBkNormalChecked.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinRadio.skinBkDisableChecked.nDrawType = m_SkinRadio.skinBkNormalChecked.nDrawType;
								m_SkinRadio.skinBkHoverChecked.nDrawType = m_SkinRadio.skinBkNormalChecked.nDrawType;
								m_SkinRadio.skinBkPressChecked.nDrawType = m_SkinRadio.skinBkNormalChecked.nDrawType;
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinRadio.skinBkNormalChecked));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadBackground(pChildNode, &(m_SkinRadio.skinBkDisableChecked));
							}//disable
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
							{//hover
								LoadBackground(pChildNode, &(m_SkinRadio.skinBkHoverChecked));
							}//hover
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
							{//press
								LoadBackground(pChildNode, &(m_SkinRadio.skinBkPressChecked));
							}//press

							pChildNode = pChildNode->getNextSibling();
						}
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
					{//text
						pSkinTextList[DRAWSTATUS_NORMAL] = &m_SkinRadio.skinTxtNormalChecked;
						pSkinTextList[DRAWSTATUS_DISABLE] = &m_SkinRadio.skinTxtDisableChecked;
						pSkinTextList[DRAWSTATUS_HOVER] = &m_SkinRadio.skinTxtHoverChecked;
						pSkinTextList[DRAWSTATUS_PRESS] = &m_SkinRadio.skinTxtPressChecked;
						LoadText(pParentNode, pSkinTextList, DRAWSTATUS_PRESS + 1);
					}//text
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameIcon) == 0)
					{//icon
						pSkinIconList[DRAWSTATUS_NORMAL] = &m_SkinRadio.iconNormalChecked;
						pSkinIconList[DRAWSTATUS_DISABLE] = &m_SkinRadio.iconDisableChecked;
						pSkinIconList[DRAWSTATUS_HOVER] = &m_SkinRadio.iconHoverChecked;
						pSkinIconList[DRAWSTATUS_PRESS] = &m_SkinRadio.iconPressChecked;
						LoadIcon(pParentNode, pSkinIconList, DRAWSTATUS_PRESS + 1);
					}//icon

					pParentNode = pParentNode->getNextSibling();
				}
			}//selected

			pStatusNode = pStatusNode->getNextSibling();
		}

		if (m_SkinRadio.skinBkDisableUnchecked.nDefault)
			m_SkinRadio.skinBkDisableUnchecked = m_SkinRadio.skinBkNormalUnchecked;

		if (m_SkinRadio.skinBkHoverUnchecked.nDefault)
			m_SkinRadio.skinBkHoverUnchecked = m_SkinRadio.skinBkNormalUnchecked;

		if (m_SkinRadio.skinBkPressUnchecked.nDefault)
			m_SkinRadio.skinBkPressUnchecked = m_SkinRadio.skinBkHoverUnchecked;

		if (m_SkinRadio.skinBkNormalChecked.nDefault)
			m_SkinRadio.skinBkNormalChecked = m_SkinRadio.skinBkNormalUnchecked;

		if (m_SkinRadio.skinBkDisableChecked.nDefault)
			m_SkinRadio.skinBkDisableChecked = m_SkinRadio.skinBkNormalChecked;

		if (m_SkinRadio.skinBkHoverChecked.nDefault)
			m_SkinRadio.skinBkHoverChecked = m_SkinRadio.skinBkNormalChecked;

		if (m_SkinRadio.skinBkPressChecked.nDefault)
			m_SkinRadio.skinBkPressChecked = m_SkinRadio.skinBkHoverChecked;
	} //end load checkbox

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinScrollBar(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pTypeNode, *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pScrollBarNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameScrollBar);
	const WCHAR *wstrNodeName;
	SKINBORDER *pSkinBorderList[DRAWSTATUS_DISABLE + 1];
	SKINITEM *pSkinItemList[DRAWSTATUS_MAXCOUNT];

	if (pScrollBarNode)
	{
		//loop to load all settings
		pTypeNode = pScrollBarNode->getFirstChild();
		while (pTypeNode != NULL)
		{
			wstrNodeName = XStringtoWString(pTypeNode->getNodeName());
			if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBackground) == 0)
			{//background
				pAttr = pTypeNode->getAttributes();
				if (pAttr)
				{
					//background draw type
					pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
					if (pTempNode)
					{
						m_SkinScrollBar.skinBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						m_SkinScrollBar.skinBkDisable.nDrawType = m_SkinScrollBar.skinBkNormal.nDrawType;
					}
				}

				//loop to load all sub items
				pChildNode = pTypeNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadBackground(pChildNode, &(m_SkinScrollBar.skinBkNormal));
					}//normal
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
					{//disable
						LoadBackground(pChildNode, &(m_SkinScrollBar.skinBkDisable));
					}//disable

					pChildNode = pChildNode->getNextSibling();
				}

				if (m_SkinScrollBar.skinBkDisable.nDefault)
					m_SkinScrollBar.skinBkDisable = m_SkinScrollBar.skinBkNormal;
			}//background
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameHorizontal) == 0)
			{//horizontal
				pParentNode = pTypeNode->getFirstChild();
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
								m_SkinScrollBar.skinHBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinScrollBar.skinHBkDisable.nDrawType = m_SkinScrollBar.skinHBkNormal.nDrawType;
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinScrollBar.skinHBkNormal));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadBackground(pChildNode, &(m_SkinScrollBar.skinHBkDisable));
							}//disable

							pChildNode = pChildNode->getNextSibling();
						}

						if (m_SkinScrollBar.skinHBkDisable.nDefault)
							m_SkinScrollBar.skinHBkDisable = m_SkinScrollBar.skinHBkNormal;
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
					{//border
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
							{//left
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinScrollBar.skinHLBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinScrollBar.skinHLBorderDisable;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
							}//left
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
							{//right
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinScrollBar.skinHRBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinScrollBar.skinHRBorderDisable;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
							}//right
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
							{//top
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinScrollBar.skinHTBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinScrollBar.skinHTBorderDisable;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
							}//top
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
							{//bottom
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinScrollBar.skinHBBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinScrollBar.skinHBBorderDisable;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
							}//bottom

							pChildNode = pChildNode->getNextSibling();
						}
					}//border
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
					{//left button
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//left button draw type
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
							if (pTempNode)
							{
								m_SkinScrollBar.skinBtnLeftNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinScrollBar.skinBtnLeftDisable.nDrawType = m_SkinScrollBar.skinBtnLeftNormal.nDrawType;
								m_SkinScrollBar.skinBtnLeftHover.nDrawType = m_SkinScrollBar.skinBtnLeftNormal.nDrawType;
								m_SkinScrollBar.skinBtnLeftPress.nDrawType = m_SkinScrollBar.skinBtnLeftNormal.nDrawType;
							}

							//whether include border
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameIncludeBorder));
							if (pTempNode)
								m_SkinScrollBar.nBtnLeftIncludeBorder = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnLeftNormal));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
							{//hover
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnLeftHover));
							}//hover
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
							{//press
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnLeftPress));
							}//press
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnLeftDisable));
							}//disable

							pChildNode = pChildNode->getNextSibling();
						}

						if (m_SkinScrollBar.skinBtnLeftHover.nDefault)
							m_SkinScrollBar.skinBtnLeftHover = m_SkinScrollBar.skinBtnLeftNormal;

						if (m_SkinScrollBar.skinBtnLeftDisable.nDefault)
							m_SkinScrollBar.skinBtnLeftDisable = m_SkinScrollBar.skinBtnLeftNormal;

						if (m_SkinScrollBar.skinBtnLeftPress.nDefault)
							m_SkinScrollBar.skinBtnLeftPress = m_SkinScrollBar.skinBtnLeftHover;
					}//left button
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
					{//right button
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//left button draw type
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
							if (pTempNode)
							{
								m_SkinScrollBar.skinBtnRightNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinScrollBar.skinBtnRightDisable.nDrawType = m_SkinScrollBar.skinBtnRightNormal.nDrawType;
								m_SkinScrollBar.skinBtnRightHover.nDrawType = m_SkinScrollBar.skinBtnRightNormal.nDrawType;
								m_SkinScrollBar.skinBtnRightPress.nDrawType = m_SkinScrollBar.skinBtnRightNormal.nDrawType;
							}

							//whether include border
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameIncludeBorder));
							if (pTempNode)
								m_SkinScrollBar.nBtnRightIncludeBorder = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnRightNormal));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
							{//hover
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnRightHover));
							}//hover
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
							{//press
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnRightPress));
							}//press
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnRightDisable));
							}//disable

							pChildNode = pChildNode->getNextSibling();
						}

						if (m_SkinScrollBar.skinBtnRightHover.nDefault)
							m_SkinScrollBar.skinBtnRightHover = m_SkinScrollBar.skinBtnRightNormal;

						if (m_SkinScrollBar.skinBtnRightDisable.nDefault)
							m_SkinScrollBar.skinBtnRightDisable = m_SkinScrollBar.skinBtnRightNormal;

						if (m_SkinScrollBar.skinBtnRightPress.nDefault)
							m_SkinScrollBar.skinBtnRightPress = m_SkinScrollBar.skinBtnRightHover;
					}//right button
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameThumb) == 0)
					{//thumb
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//whether include border
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameIncludeBorder));
							if (pTempNode)
							{
								m_SkinScrollBar.nHThumbIncludeBorder = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
							}
						}

						pSkinItemList[DRAWSTATUS_NORMAL] = &m_SkinScrollBar.skinHItemNormal;
						pSkinItemList[DRAWSTATUS_DISABLE] = &m_SkinScrollBar.skinHItemDisabled;
						pSkinItemList[DRAWSTATUS_HOVER] = &m_SkinScrollBar.skinHItemHover;
						pSkinItemList[DRAWSTATUS_PRESS] = &m_SkinScrollBar.skinHItemPress;
						LoadItem(pParentNode, pSkinItemList, DRAWSTATUS_PRESS + 1);
					}//thumb

					pParentNode = pParentNode->getNextSibling();
				}
			}//horizontal
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameVertical) == 0)
			{//vertical
				pParentNode = pTypeNode->getFirstChild();
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
								m_SkinScrollBar.skinVBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinScrollBar.skinVBkDisable.nDrawType = m_SkinScrollBar.skinVBkNormal.nDrawType;
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinScrollBar.skinVBkNormal));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadBackground(pChildNode, &(m_SkinScrollBar.skinVBkDisable));
							}//disable

							pChildNode = pChildNode->getNextSibling();
						}

						if (m_SkinScrollBar.skinVBkDisable.nDefault)
							m_SkinScrollBar.skinVBkDisable = m_SkinScrollBar.skinVBkNormal;
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
					{//border
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
							{//left
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinScrollBar.skinVLBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinScrollBar.skinVLBorderDisable;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
							}//left
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
							{//right
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinScrollBar.skinVRBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinScrollBar.skinVRBorderDisable;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
							}//right
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
							{//top
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinScrollBar.skinVTBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinScrollBar.skinVTBorderDisable;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
							}//top
							else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
							{//bottom
								pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinScrollBar.skinVBBorderNormal;
								pSkinBorderList[DRAWSTATUS_DISABLE] = &m_SkinScrollBar.skinVBBorderDisable;
								LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DISABLE + 1);
							}//bottom

							pChildNode = pChildNode->getNextSibling();
						}
					}//border
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
					{//top button
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//left button draw type
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
							if (pTempNode)
							{
								m_SkinScrollBar.skinBtnTopNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinScrollBar.skinBtnTopDisable.nDrawType = m_SkinScrollBar.skinBtnTopNormal.nDrawType;
								m_SkinScrollBar.skinBtnTopHover.nDrawType = m_SkinScrollBar.skinBtnTopNormal.nDrawType;
								m_SkinScrollBar.skinBtnTopPress.nDrawType = m_SkinScrollBar.skinBtnTopNormal.nDrawType;
							}

							//whether include border
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameIncludeBorder));
							if (pTempNode)
								m_SkinScrollBar.nBtnTopIncludeBorder = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnTopNormal));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
							{//hover
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnTopHover));
							}//hover
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
							{//press
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnTopPress));
							}//press
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnTopDisable));
							}//disable

							pChildNode = pChildNode->getNextSibling();
						}

						if (m_SkinScrollBar.skinBtnTopHover.nDefault)
							m_SkinScrollBar.skinBtnTopHover = m_SkinScrollBar.skinBtnTopNormal;

						if (m_SkinScrollBar.skinBtnTopDisable.nDefault)
							m_SkinScrollBar.skinBtnTopDisable = m_SkinScrollBar.skinBtnTopNormal;

						if (m_SkinScrollBar.skinBtnTopPress.nDefault)
							m_SkinScrollBar.skinBtnTopPress = m_SkinScrollBar.skinBtnTopHover;
					}//top button
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
					{//bottom button
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//left button draw type
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
							if (pTempNode)
							{
								m_SkinScrollBar.skinBtnBottomNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinScrollBar.skinBtnBottomDisable.nDrawType = m_SkinScrollBar.skinBtnBottomNormal.nDrawType;
								m_SkinScrollBar.skinBtnBottomHover.nDrawType = m_SkinScrollBar.skinBtnBottomNormal.nDrawType;
								m_SkinScrollBar.skinBtnBottomPress.nDrawType = m_SkinScrollBar.skinBtnBottomNormal.nDrawType;
							}

							//whether include border
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameIncludeBorder));
							if (pTempNode)
								m_SkinScrollBar.nBtnBottomIncludeBorder = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnBottomNormal));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
							{//hover
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnBottomHover));
							}//hover
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
							{//press
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnBottomPress));
							}//press
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadSysButtonItem(pChildNode, &(m_SkinScrollBar.skinBtnBottomDisable));
							}//disable

							pChildNode = pChildNode->getNextSibling();
						}

						if (m_SkinScrollBar.skinBtnBottomHover.nDefault)
							m_SkinScrollBar.skinBtnBottomHover = m_SkinScrollBar.skinBtnBottomNormal;

						if (m_SkinScrollBar.skinBtnBottomDisable.nDefault)
							m_SkinScrollBar.skinBtnBottomDisable = m_SkinScrollBar.skinBtnBottomNormal;

						if (m_SkinScrollBar.skinBtnBottomPress.nDefault)
							m_SkinScrollBar.skinBtnBottomPress = m_SkinScrollBar.skinBtnBottomHover;
					}//bottom button
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameThumb) == 0)
					{//thumb
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//whether include border
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameIncludeBorder));
							if (pTempNode)
							{
								m_SkinScrollBar.nVThumbIncludeBorder = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
							}
						}

						pSkinItemList[DRAWSTATUS_NORMAL] = &m_SkinScrollBar.skinVItemNormal;
						pSkinItemList[DRAWSTATUS_DISABLE] = &m_SkinScrollBar.skinVItemDisabled;
						pSkinItemList[DRAWSTATUS_HOVER] = &m_SkinScrollBar.skinVItemHover;
						pSkinItemList[DRAWSTATUS_PRESS] = &m_SkinScrollBar.skinVItemPress;
						LoadItem(pParentNode, pSkinItemList, DRAWSTATUS_PRESS + 1);
					}//thumb

					pParentNode = pParentNode->getNextSibling();
				}
			}//vertical

			pTypeNode = pTypeNode->getNextSibling();
		}
	} //end load scrollbar

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinSliderCtrl(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pTypeNode, *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pSliderCtrlNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameSliderCtrl);
	const WCHAR *wstrNodeName;
	SKINITEM *pSkinItemList[DRAWSTATUS_MAXCOUNT];

	if (pSliderCtrlNode)
	{
		//loop to load all settings
		pTypeNode = pSliderCtrlNode->getFirstChild();
		while (pTypeNode != NULL)
		{
			wstrNodeName = XStringtoWString(pTypeNode->getNodeName());
			if (wcscmp(wstrNodeName, wstrSkinFileNodeNameHorizontal) == 0)
			{//horizontal
				pParentNode = pTypeNode->getFirstChild();
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
								m_SkinSliderCtrl.skinHBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinSliderCtrl.skinHBkDisable.nDrawType = m_SkinSliderCtrl.skinHBkNormal.nDrawType;
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinSliderCtrl.skinHBkNormal));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadBackground(pChildNode, &(m_SkinSliderCtrl.skinHBkDisable));
							}//disable

							pChildNode = pChildNode->getNextSibling();
						}

						if (m_SkinSliderCtrl.skinHBkDisable.nDefault)
							m_SkinSliderCtrl.skinHBkDisable = m_SkinSliderCtrl.skinHBkNormal;
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameSlider) == 0)
					{//slider
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//whether include border
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
							if (pTempNode)
							{
								m_SkinSliderCtrl.nHSliderWidth = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
							}
						}

						pSkinItemList[DRAWSTATUS_NORMAL] = &m_SkinSliderCtrl.skinHSliderNormal;
						pSkinItemList[DRAWSTATUS_DISABLE] = &m_SkinSliderCtrl.skinHSliderDisable;
						LoadItem(pParentNode, pSkinItemList, DRAWSTATUS_DISABLE + 1);
					}//slider
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameThumb) == 0)
					{//thumb
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//whether include border
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
							if (pTempNode)
							{
								m_SkinSliderCtrl.nHThumbWidth = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
							}
						}

						pSkinItemList[DRAWSTATUS_NORMAL] = &m_SkinSliderCtrl.skinHThumbNormal;
						pSkinItemList[DRAWSTATUS_DISABLE] = &m_SkinSliderCtrl.skinHThumbDisable;
						pSkinItemList[DRAWSTATUS_HOVER] = &m_SkinSliderCtrl.skinHThumbHover;
						pSkinItemList[DRAWSTATUS_PRESS] = &m_SkinSliderCtrl.skinHThumbPress;
						LoadItem(pParentNode, pSkinItemList, DRAWSTATUS_PRESS + 1);
					}//thumb

					pParentNode = pParentNode->getNextSibling();
				}
			}//horizontal
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameVertical) == 0)
			{//vertical
				pParentNode = pTypeNode->getFirstChild();
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
								m_SkinSliderCtrl.skinVBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
								m_SkinSliderCtrl.skinVBkDisable.nDrawType = m_SkinSliderCtrl.skinVBkNormal.nDrawType;
							}
						}

						//loop to load all sub items
						pChildNode = pParentNode->getFirstChild();
						while(pChildNode != NULL)
						{
							wstrNodeName = XStringtoWString(pChildNode->getNodeName());

							if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
							{//normal
								LoadBackground(pChildNode, &(m_SkinSliderCtrl.skinVBkNormal));
							}//normal
							else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
							{//disable
								LoadBackground(pChildNode, &(m_SkinSliderCtrl.skinVBkDisable));
							}//disable

							pChildNode = pChildNode->getNextSibling();
						}

						if (m_SkinSliderCtrl.skinVBkDisable.nDefault)
							m_SkinSliderCtrl.skinVBkDisable = m_SkinSliderCtrl.skinVBkNormal;
					}//background
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameSlider) == 0)
					{//slider
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//whether include border
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
							if (pTempNode)
							{
								m_SkinSliderCtrl.nVSliderWidth = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
							}
						}

						pSkinItemList[DRAWSTATUS_NORMAL] = &m_SkinSliderCtrl.skinVSliderNormal;
						pSkinItemList[DRAWSTATUS_DISABLE] = &m_SkinSliderCtrl.skinVSliderDisable;
						LoadItem(pParentNode, pSkinItemList, DRAWSTATUS_DISABLE + 1);
					}//slider
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameThumb) == 0)
					{//thumb
						pAttr = pParentNode->getAttributes();
						if (pAttr)
						{
							//whether include border
							pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
							if (pTempNode)
							{
								m_SkinSliderCtrl.nVThumbWidth = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
							}
						}

						pSkinItemList[DRAWSTATUS_NORMAL] = &m_SkinSliderCtrl.skinVThumbNormal;
						pSkinItemList[DRAWSTATUS_DISABLE] = &m_SkinSliderCtrl.skinVThumbDisable;
						pSkinItemList[DRAWSTATUS_HOVER] = &m_SkinSliderCtrl.skinVThumbHover;
						pSkinItemList[DRAWSTATUS_PRESS] = &m_SkinSliderCtrl.skinVThumbPress;
						LoadItem(pParentNode, pSkinItemList, DRAWSTATUS_PRESS + 1);
					}//thumb

					pParentNode = pParentNode->getNextSibling();
				}
			}//vertical

			pTypeNode = pTypeNode->getNextSibling();
		}
	} //end load slider control

	return bResult;
}

BOOL CDreamSkinLoader::LoadSkinSpinCtrl(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pSpinCtrlNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameSpinCtrl);
	const WCHAR *wstrNodeName;

	if (pSpinCtrlNode)
	{
		//loop to load all settings
		pParentNode = pSpinCtrlNode->getFirstChild();
		while (pParentNode != NULL)
		{
			wstrNodeName = XStringtoWString(pParentNode->getNodeName());
			if (wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
			{//left button
				pAttr = pParentNode->getAttributes();
				if (pAttr)
				{
					//left button draw type
					pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
					if (pTempNode)
					{
						m_SkinSpinCtrl.skinBtnLeftNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						m_SkinSpinCtrl.skinBtnLeftDisable.nDrawType = m_SkinSpinCtrl.skinBtnLeftNormal.nDrawType;
						m_SkinSpinCtrl.skinBtnLeftHover.nDrawType = m_SkinSpinCtrl.skinBtnLeftNormal.nDrawType;
						m_SkinSpinCtrl.skinBtnLeftPress.nDrawType = m_SkinSpinCtrl.skinBtnLeftNormal.nDrawType;
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnLeftNormal));
					}//normal
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
					{//hover
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnLeftHover));
					}//hover
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
					{//press
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnLeftPress));
					}//press
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
					{//disable
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnLeftDisable));
					}//disable

					pChildNode = pChildNode->getNextSibling();
				}

				if (m_SkinSpinCtrl.skinBtnLeftHover.nDefault)
					m_SkinSpinCtrl.skinBtnLeftHover = m_SkinSpinCtrl.skinBtnLeftNormal;

				if (m_SkinSpinCtrl.skinBtnLeftDisable.nDefault)
					m_SkinSpinCtrl.skinBtnLeftDisable = m_SkinSpinCtrl.skinBtnLeftNormal;

				if (m_SkinSpinCtrl.skinBtnLeftPress.nDefault)
					m_SkinSpinCtrl.skinBtnLeftPress = m_SkinSpinCtrl.skinBtnLeftHover;
			}//left button
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
			{//right button
				pAttr = pParentNode->getAttributes();
				if (pAttr)
				{
					//left button draw type
					pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
					if (pTempNode)
					{
						m_SkinSpinCtrl.skinBtnRightNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						m_SkinSpinCtrl.skinBtnRightDisable.nDrawType = m_SkinSpinCtrl.skinBtnRightNormal.nDrawType;
						m_SkinSpinCtrl.skinBtnRightHover.nDrawType = m_SkinSpinCtrl.skinBtnRightNormal.nDrawType;
						m_SkinSpinCtrl.skinBtnRightPress.nDrawType = m_SkinSpinCtrl.skinBtnRightNormal.nDrawType;
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnRightNormal));
					}//normal
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
					{//hover
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnRightHover));
					}//hover
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
					{//press
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnRightPress));
					}//press
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
					{//disable
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnRightDisable));
					}//disable

					pChildNode = pChildNode->getNextSibling();
				}

				if (m_SkinSpinCtrl.skinBtnRightHover.nDefault)
					m_SkinSpinCtrl.skinBtnRightHover = m_SkinSpinCtrl.skinBtnRightNormal;

				if (m_SkinSpinCtrl.skinBtnRightDisable.nDefault)
					m_SkinSpinCtrl.skinBtnRightDisable = m_SkinSpinCtrl.skinBtnRightNormal;

				if (m_SkinSpinCtrl.skinBtnRightPress.nDefault)
					m_SkinSpinCtrl.skinBtnRightPress = m_SkinSpinCtrl.skinBtnRightHover;
			}//right button
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
			{//top button
				pAttr = pParentNode->getAttributes();
				if (pAttr)
				{
					//left button draw type
					pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
					if (pTempNode)
					{
						m_SkinSpinCtrl.skinBtnTopNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						m_SkinSpinCtrl.skinBtnTopDisable.nDrawType = m_SkinSpinCtrl.skinBtnTopNormal.nDrawType;
						m_SkinSpinCtrl.skinBtnTopHover.nDrawType = m_SkinSpinCtrl.skinBtnTopNormal.nDrawType;
						m_SkinSpinCtrl.skinBtnTopPress.nDrawType = m_SkinSpinCtrl.skinBtnTopNormal.nDrawType;
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnTopNormal));
					}//normal
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
					{//hover
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnTopHover));
					}//hover
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
					{//press
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnTopPress));
					}//press
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
					{//disable
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnTopDisable));
					}//disable

					pChildNode = pChildNode->getNextSibling();
				}

				if (m_SkinSpinCtrl.skinBtnTopHover.nDefault)
					m_SkinSpinCtrl.skinBtnTopHover = m_SkinSpinCtrl.skinBtnTopNormal;

				if (m_SkinSpinCtrl.skinBtnTopDisable.nDefault)
					m_SkinSpinCtrl.skinBtnTopDisable = m_SkinSpinCtrl.skinBtnTopNormal;

				if (m_SkinSpinCtrl.skinBtnTopPress.nDefault)
					m_SkinSpinCtrl.skinBtnTopPress = m_SkinSpinCtrl.skinBtnTopHover;
			}//top button
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
			{//bottom button
				pAttr = pParentNode->getAttributes();
				if (pAttr)
				{
					//left button draw type
					pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
					if (pTempNode)
					{
						m_SkinSpinCtrl.skinBtnBottomNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
						m_SkinSpinCtrl.skinBtnBottomDisable.nDrawType = m_SkinSpinCtrl.skinBtnBottomNormal.nDrawType;
						m_SkinSpinCtrl.skinBtnBottomHover.nDrawType = m_SkinSpinCtrl.skinBtnBottomNormal.nDrawType;
						m_SkinSpinCtrl.skinBtnBottomPress.nDrawType = m_SkinSpinCtrl.skinBtnBottomNormal.nDrawType;
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnBottomNormal));
					}//normal
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
					{//hover
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnBottomHover));
					}//hover
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
					{//press
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnBottomPress));
					}//press
					else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
					{//disable
						LoadSysButtonItem(pChildNode, &(m_SkinSpinCtrl.skinBtnBottomDisable));
					}//disable

					pChildNode = pChildNode->getNextSibling();
				}

				if (m_SkinSpinCtrl.skinBtnBottomHover.nDefault)
					m_SkinSpinCtrl.skinBtnBottomHover = m_SkinSpinCtrl.skinBtnBottomNormal;

				if (m_SkinSpinCtrl.skinBtnBottomDisable.nDefault)
					m_SkinSpinCtrl.skinBtnBottomDisable = m_SkinSpinCtrl.skinBtnBottomNormal;

				if (m_SkinSpinCtrl.skinBtnBottomPress.nDefault)
					m_SkinSpinCtrl.skinBtnBottomPress = m_SkinSpinCtrl.skinBtnBottomHover;
			}//bottom button

			pParentNode = pParentNode->getNextSibling();
		}
	} //end load scrollbar

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

BOOL CDreamSkinLoader::LoadSkinTab(void *parser)
{
	BOOL bResult = TRUE;
	DOMNode *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *docRootNode = ((XercesDOMParser*)parser)->getDocument()->getDocumentElement();
	DOMNode *pTabNode = GetNamedChild(docRootNode, wstrSkinFileNodeNameTab);
	const WCHAR *wstrNodeName;
	SKINBORDER *pSkinBorderList[DRAWSTATUS_NORMAL + 1];

	if (pTabNode)
	{
		//loop to load all settings
		pParentNode = pTabNode->getFirstChild();
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
						m_SkinTab.skinBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
					}
				}

				//loop to load all sub items
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
					{//normal
						LoadBackground(pChildNode, &(m_SkinTab.skinBkNormal));
					}//normal

					pChildNode = pChildNode->getNextSibling();
				}
			}//background
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
			{//border
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
					{//left
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinTab.skinLBorderNormal;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
					}//left
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
					{//right
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinTab.skinRBorderNormal;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
					}//right
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
					{//top
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinTab.skinTBorderNormal;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
					}//top
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
					{//bottom
						pSkinBorderList[DRAWSTATUS_NORMAL] = &m_SkinTab.skinBBorderNormal;
						LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_NORMAL + 1);
					}//bottom

					pChildNode = pChildNode->getNextSibling();
				}
			}//border
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameButton) == 0)
			{//button
				LoadButton(pParentNode, &m_SkinTab.skinTabButton);
			}//button

			pParentNode = pParentNode->getNextSibling();
		}
	}//tab settings

	return bResult;
}

BOOL CDreamSkinLoader::LoadBackground(void *bkgnd, SKINBACKGROUND **pSkinBackgroundList, int nCount)
{
	BOOL bResult = TRUE;
	DOMNode *pBackgroundNode = (DOMNode*)bkgnd;
	const WCHAR *wstrNodeName;
	DOMNode *pTempNode;
	DOMNamedNodeMap *pAttr;
	int nTemp;

	if (nCount > 0 && pSkinBackgroundList)
	{
		pAttr = pBackgroundNode->getAttributes();

		//draw type
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
		if (pTempNode)
		{
			nTemp = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
			if (pSkinBackgroundList[DRAWSTATUS_NORMAL])
				pSkinBackgroundList[DRAWSTATUS_NORMAL]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_DISABLE && pSkinBackgroundList[DRAWSTATUS_DISABLE])
				pSkinBackgroundList[DRAWSTATUS_DISABLE]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_HOVER && pSkinBackgroundList[DRAWSTATUS_HOVER])
				pSkinBackgroundList[DRAWSTATUS_HOVER]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_PRESS && pSkinBackgroundList[DRAWSTATUS_PRESS])
				pSkinBackgroundList[DRAWSTATUS_PRESS]->nDrawType = nTemp;

			if (nCount > DRAWSTATUS_DEFAULT && pSkinBackgroundList[DRAWSTATUS_DEFAULT])
				pSkinBackgroundList[DRAWSTATUS_DEFAULT]->nDrawType = nTemp;
		}

		pTempNode = pBackgroundNode->getFirstChild();
		while (pTempNode != NULL)
		{
			wstrNodeName = XStringtoWString(pTempNode->getNodeName());

			if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
			{//normal
				if (pSkinBackgroundList[DRAWSTATUS_NORMAL])
					LoadBackgrounditem(pTempNode, pSkinBackgroundList[DRAWSTATUS_NORMAL]);
			}//normal
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
			{//disable
				if (nCount > DRAWSTATUS_DISABLE && pSkinBackgroundList[DRAWSTATUS_DISABLE])
					LoadBackgrounditem(pTempNode, pSkinBackgroundList[DRAWSTATUS_DISABLE]);
			}//disable
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
			{//hover
				if (nCount > DRAWSTATUS_HOVER && pSkinBackgroundList[DRAWSTATUS_HOVER])
					LoadBackgrounditem(pTempNode, pSkinBackgroundList[DRAWSTATUS_HOVER]);
			}//hover
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
			{//press
				if (nCount > DRAWSTATUS_PRESS && pSkinBackgroundList[DRAWSTATUS_PRESS])
					LoadBackgrounditem(pTempNode, pSkinBackgroundList[DRAWSTATUS_PRESS]);
			}//press
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameDefault) == 0)
			{//default
				if (nCount > DRAWSTATUS_DEFAULT && pSkinBackgroundList[DRAWSTATUS_DEFAULT])
					LoadBackgrounditem(pTempNode, pSkinBackgroundList[DRAWSTATUS_DEFAULT]);
			}//default
			pTempNode = pTempNode->getNextSibling();
		}

		if (nCount > DRAWSTATUS_DISABLE && pSkinBackgroundList[DRAWSTATUS_DISABLE] && pSkinBackgroundList[DRAWSTATUS_DISABLE]->nDefault && pSkinBackgroundList[DRAWSTATUS_NORMAL])
			*(pSkinBackgroundList[DRAWSTATUS_DISABLE]) = *(pSkinBackgroundList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_HOVER && pSkinBackgroundList[DRAWSTATUS_HOVER] && pSkinBackgroundList[DRAWSTATUS_HOVER]->nDefault && pSkinBackgroundList[DRAWSTATUS_NORMAL])
			*(pSkinBackgroundList[DRAWSTATUS_HOVER]) = *(pSkinBackgroundList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_DEFAULT && pSkinBackgroundList[DRAWSTATUS_DEFAULT] && pSkinBackgroundList[DRAWSTATUS_DEFAULT]->nDefault && pSkinBackgroundList[DRAWSTATUS_NORMAL])
			*(pSkinBackgroundList[DRAWSTATUS_DEFAULT]) = *(pSkinBackgroundList[DRAWSTATUS_NORMAL]);

		if (nCount > DRAWSTATUS_PRESS && pSkinBackgroundList[DRAWSTATUS_PRESS] && pSkinBackgroundList[DRAWSTATUS_PRESS]->nDefault)
			if (pSkinBackgroundList[DRAWSTATUS_HOVER])
				*(pSkinBackgroundList[DRAWSTATUS_PRESS]) = *(pSkinBackgroundList[DRAWSTATUS_HOVER]);
			else if (pSkinBackgroundList[DRAWSTATUS_NORMAL])
				*(pSkinBackgroundList[DRAWSTATUS_PRESS]) = *(pSkinBackgroundList[DRAWSTATUS_HOVER]);
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadBackgrounditem(void *bkitem, SKINBACKGROUND *pSkinBackground)
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

BOOL CDreamSkinLoader::LoadButton(void *button, SKINBUTTON *pSkinButton)
{
	DOMNode *pParentNode, *pChildNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	DOMNode *pButtonNode = (DOMNode*)button;
	const WCHAR *wstrNodeName;
	SKINBORDER *pSkinBorderList[DRAWSTATUS_DEFAULT + 1];
	SKINTEXT *pSkinTextList[DRAWSTATUS_DEFAULT + 1];

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
					pSkinButton->skinBkNormal.nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
					pSkinButton->skinBkHover.nDrawType = pSkinButton->skinBkNormal.nDrawType;
					pSkinButton->skinBkDisable.nDrawType = pSkinButton->skinBkNormal.nDrawType;
					pSkinButton->skinBkPress.nDrawType = pSkinButton->skinBkNormal.nDrawType;
				}
			}

			//loop to load all sub items
			pChildNode = pParentNode->getFirstChild();
			while(pChildNode != NULL)
			{
				wstrNodeName = XStringtoWString(pChildNode->getNodeName());

				if(wcscmp(wstrNodeName, wstrSkinFileNodeNameNormal) == 0)
				{//normal
					LoadBackground(pChildNode, &(pSkinButton->skinBkNormal));
				}//normal
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
				{//disable
					LoadBackground(pChildNode, &(pSkinButton->skinBkDisable));
				}//disable
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
				{//hover
					LoadBackground(pChildNode, &(pSkinButton->skinBkHover));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
				{//press
					LoadBackground(pChildNode, &(pSkinButton->skinBkPress));
				}//press
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDefault) == 0)
				{//default
					LoadBackground(pChildNode, &(pSkinButton->skinBkDefault));
				}//default

				pChildNode = pChildNode->getNextSibling();
			}

			if (pSkinButton->skinBkDisable.nDefault)
				pSkinButton->skinBkDisable = pSkinButton->skinBkNormal;

			if (pSkinButton->skinBkHover.nDefault)
				pSkinButton->skinBkHover = pSkinButton->skinBkNormal;

			if (pSkinButton->skinBkPress.nDefault)
				pSkinButton->skinBkPress = pSkinButton->skinBkHover;

			if (pSkinButton->skinBkDefault.nDefault)
				pSkinButton->skinBkDefault = pSkinButton->skinBkNormal;
		}//background
		else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
		{//border
			pChildNode = pParentNode->getFirstChild();
			while(pChildNode != NULL)
			{
				wstrNodeName = XStringtoWString(pChildNode->getNodeName());

				if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
				{//left
					pSkinBorderList[DRAWSTATUS_NORMAL] = &pSkinButton->skinLBorderNormal;
					pSkinBorderList[DRAWSTATUS_DISABLE] = &pSkinButton->skinLBorderDisable;
					pSkinBorderList[DRAWSTATUS_HOVER] = &pSkinButton->skinLBorderHover;
					pSkinBorderList[DRAWSTATUS_PRESS] = &pSkinButton->skinLBorderPress;
					pSkinBorderList[DRAWSTATUS_DEFAULT] = &pSkinButton->skinLBorderDefault;
					LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DEFAULT + 1);
				}//left
				else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
				{//right
					pSkinBorderList[DRAWSTATUS_NORMAL] = &pSkinButton->skinRBorderNormal;
					pSkinBorderList[DRAWSTATUS_DISABLE] = &pSkinButton->skinRBorderDisable;
					pSkinBorderList[DRAWSTATUS_HOVER] = &pSkinButton->skinRBorderHover;
					pSkinBorderList[DRAWSTATUS_PRESS] = &pSkinButton->skinRBorderPress;
					pSkinBorderList[DRAWSTATUS_DEFAULT] = &pSkinButton->skinRBorderDefault;
					LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DEFAULT + 1);
				}//right
				else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
				{//top
					pSkinBorderList[DRAWSTATUS_NORMAL] = &pSkinButton->skinTBorderNormal;
					pSkinBorderList[DRAWSTATUS_DISABLE] = &pSkinButton->skinTBorderDisable;
					pSkinBorderList[DRAWSTATUS_HOVER] = &pSkinButton->skinTBorderHover;
					pSkinBorderList[DRAWSTATUS_PRESS] = &pSkinButton->skinTBorderPress;
					pSkinBorderList[DRAWSTATUS_DEFAULT] = &pSkinButton->skinTBorderDefault;
					LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DEFAULT + 1);
				}//top
				else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
				{//bottom
					pSkinBorderList[DRAWSTATUS_NORMAL] = &pSkinButton->skinBBorderNormal;
					pSkinBorderList[DRAWSTATUS_DISABLE] = &pSkinButton->skinBBorderDisable;
					pSkinBorderList[DRAWSTATUS_HOVER] = &pSkinButton->skinBBorderHover;
					pSkinBorderList[DRAWSTATUS_PRESS] = &pSkinButton->skinBBorderPress;
					pSkinBorderList[DRAWSTATUS_DEFAULT] = &pSkinButton->skinBBorderDefault;
					LoadBorder(pChildNode, pSkinBorderList, DRAWSTATUS_DEFAULT + 1);
				}//bottom

				pChildNode = pChildNode->getNextSibling();
			}
		}//border
		else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
		{//text
			pSkinTextList[DRAWSTATUS_NORMAL] = &pSkinButton->skinTxtNormal;
			pSkinTextList[DRAWSTATUS_DISABLE] = &pSkinButton->skinTxtDisable;
			pSkinTextList[DRAWSTATUS_HOVER] = &pSkinButton->skinTxtHover;
			pSkinTextList[DRAWSTATUS_PRESS] = &pSkinButton->skinTxtPress;
			pSkinTextList[DRAWSTATUS_DEFAULT] = &pSkinButton->skinTxtDefault;
			LoadText(pParentNode, pSkinTextList, DRAWSTATUS_DEFAULT + 1);
		}//text

		pParentNode = pParentNode->getNextSibling();
	}

	return TRUE;
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
			{
				const WCHAR *wstrImagePath = XStringtoWString(pTempNode->getNodeValue());
				WCHAR wstrFullImagePath[MAX_PATH];
				if (::GetFullPathName(wstrImagePath, MAX_PATH, wstrFullImagePath, NULL))
				{
					if (wcscmp(wstrFullImagePath, wstrImagePath))
					{
						if ((wcslen(m_wstrSkinFileDir) + wcslen(wstrImagePath)) < MAX_PATH)
							wsprintf(wstrFullImagePath, L"%s%s", m_wstrSkinFileDir, wstrImagePath);
					}

					pDrawImage->hImage = m_pImageHandleList->LoadImage(wstrFullImagePath);
				}
				else
				{
					pDrawImage->hImage = m_pImageHandleList->LoadImage(wstrImagePath);
				}
			}
		}

		bResult = TRUE;
	}

	return bResult;
}

BOOL CDreamSkinLoader::LoadItem(void *item, SKINITEM **pSkinItemList, int nCount)
{
	BOOL bResult = TRUE;
	DOMNode *pItemNode = (DOMNode*)item;
	const WCHAR *wstrNodeName;
	DOMNode *pParentNode, *pChildNode;
	int i;
	SKINBACKGROUND *pSkinBackgroundList[DRAWSTATUS_MAXCOUNT];
	SKINTEXT *pSkinTextList[DRAWSTATUS_MAXCOUNT];
	SKINBORDER *pSkinBorderList[DRAWSTATUS_MAXCOUNT];
	SKINICON *pSkinIconList[DRAWSTATUS_MAXCOUNT];

	if (nCount > DRAWSTATUS_MAXCOUNT)
		nCount = DRAWSTATUS_MAXCOUNT;

	if (nCount > 0 && pSkinItemList)
	{
		//loop to load all settings for item
		pParentNode = pItemNode->getFirstChild();
		while (pParentNode != NULL)
		{
			wstrNodeName = XStringtoWString(pParentNode->getNodeName());
			if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBackground) == 0)
			{//background
				for (i = 0; i < nCount; i++)
				{
					if (pSkinItemList[i])
						pSkinBackgroundList[i] = &pSkinItemList[i]->skinBk;
					else
						pSkinBackgroundList[i] = NULL;
				}

				for (i = nCount; i < DRAWSTATUS_MAXCOUNT; i++)
					pSkinBackgroundList[i] = NULL;

				LoadBackground(pParentNode, pSkinBackgroundList, nCount);
				
			}//background
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameText) == 0)
			{//text
				for (i = 0; i < nCount; i++)
				{
					if (pSkinItemList[i])
						pSkinTextList[i] = &pSkinItemList[i]->skinTxt;
					else
						pSkinTextList[i] = NULL;
				}

				for (i = nCount; i < DRAWSTATUS_MAXCOUNT; i++)
					pSkinTextList[i] = NULL;

				LoadText(pParentNode, pSkinTextList, nCount);
			}//text
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBorder) == 0)
			{//border
				pChildNode = pParentNode->getFirstChild();
				while(pChildNode != NULL)
				{
					wstrNodeName = XStringtoWString(pChildNode->getNodeName());

					if(wcscmp(wstrNodeName, wstrSkinFileNodeNameLeft) == 0)
					{//left
						for (i = 0; i < nCount; i++)
						{
							if (pSkinItemList[i])
								pSkinBorderList[i] = &pSkinItemList[i]->skinLBorder;
							else
								pSkinBorderList[i] = NULL;
						}

						for (i = nCount; i < DRAWSTATUS_MAXCOUNT; i++)
							pSkinBorderList[i] = NULL;

						LoadBorder(pChildNode, pSkinBorderList, nCount);
					}//left
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameRight) == 0)
					{//right
						for (i = 0; i < nCount; i++)
						{
							if (pSkinItemList[i])
								pSkinBorderList[i] = &pSkinItemList[i]->skinRBorder;
							else
								pSkinBorderList[i] = NULL;
						}

						for (i = nCount; i < DRAWSTATUS_MAXCOUNT; i++)
							pSkinBorderList[i] = NULL;

						LoadBorder(pChildNode, pSkinBorderList, nCount);
					}//right
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameTop) == 0)
					{//top
						for (i = 0; i < nCount; i++)
						{
							if (pSkinItemList[i])
								pSkinBorderList[i] = &pSkinItemList[i]->skinTBorder;
							else
								pSkinBorderList[i] = NULL;
						}

						for (i = nCount; i < DRAWSTATUS_MAXCOUNT; i++)
							pSkinBorderList[i] = NULL;

						LoadBorder(pChildNode, pSkinBorderList, nCount);
					}//top
					else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameBottom) == 0)
					{//bottom
						for (i = 0; i < nCount; i++)
						{
							if (pSkinItemList[i])
								pSkinBorderList[i] = &pSkinItemList[i]->skinBBorder;
							else
								pSkinBorderList[i] = NULL;
						}

						for (i = nCount; i < DRAWSTATUS_MAXCOUNT; i++)
							pSkinBorderList[i] = NULL;

						LoadBorder(pChildNode, pSkinBorderList, nCount);
					}//bottom

					pChildNode = pChildNode->getNextSibling();
				}
			}//border
			else if (wcscmp(wstrNodeName, wstrSkinFileNodeNameIcon) == 0)
			{//icon
				for (i = 0; i < nCount; i++)
				{
					if (pSkinItemList[i])
						pSkinIconList[i] = &pSkinItemList[i]->skinIcon;
					else
						pSkinIconList[i] = NULL;
				}

				for (i = nCount; i < DRAWSTATUS_MAXCOUNT; i++)
					pSkinIconList[i] = NULL;

				LoadIcon(pParentNode, pSkinIconList, nCount);
			}//icon

			pParentNode = pParentNode->getNextSibling();
		}//end of loop to load all settings for item
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
	DOMNode *pNode, *pTempNode;
	DOMNamedNodeMap *pAttr;
	const WCHAR *wstrNodeName;
	
	pSkinBorder->nDefault = 0;

	pAttr = ((DOMNode*)bditem)->getAttributes();
	if (pAttr)
	{
		//border width
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameWidth));
		if (pTempNode)
			pSkinBorder->nWidth = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

		//draw type
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameType));
		if (pTempNode)
			pSkinBorder->nDrawType = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

		//start size
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameStart));
		if (pTempNode)
			pSkinBorder->nStart = _wtoi(XStringtoWString(pTempNode->getNodeValue()));

		//end size
		pTempNode = pAttr->getNamedItem(WStringtoXString(wstrSkinFileAttrNameEnd));
		if (pTempNode)
			pSkinBorder->nEnd = _wtoi(XStringtoWString(pTempNode->getNodeValue()));
	}

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

BOOL CDreamSkinLoader::LoadSysButtonItem(void *sysbtn, SKINSYSBUTTON *pSkinSysButton)
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
			bResult = LoadColorItem(pNode, wstrSkinFileAttrNameText, &(pSkinSysButton->clrTxt));
			bResult = LoadColorItem(pNode, wstrSkinFileAttrNameBorder, &(pSkinSysButton->clrBd));
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
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinCloseNormal));
				}//normal
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
				{//hover
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinCloseHover));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
				{//hover
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinClosePress));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
				{//disable
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinCloseDisable));
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
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinMaxNormal));
				}//normal
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
				{//hover
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinMaxHover));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
				{//hover
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinMaxPress));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
				{//disable
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinMaxDisable));
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
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinRestoreNormal));
				}//normal
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
				{//hover
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinRestoreHover));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
				{//hover
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinRestorePress));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
				{//disable
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinRestoreDisable));
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
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinMinNormal));
				}//normal
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameHover) == 0)
				{//hover
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinMinHover));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNamePress) == 0)
				{//hover
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinMinPress));
				}//hover
				else if(wcscmp(wstrNodeName, wstrSkinFileNodeNameDisable) == 0)
				{//disable
					LoadSysButtonItem(pTempNode, &(pSkinTitleBar->skinMinDisable));
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