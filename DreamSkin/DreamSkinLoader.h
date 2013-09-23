//DreamSkinLoader.h

#ifndef DREAMSKIN_LOADER_H
#define DREAMSKIN_LOADER_H

class CImageHandleList;

//region type
#define RGN_RECT                     0
#define RGN_BITMAP                   1

//draw type
#define DRAWTYPE_FILLCOLOR           0
#define DRAWTYPE_STRETCHBITMAP       1
#define DRAWTYPE_TILEBITMAP          2
#define DRAWTYPE_TRANSPARENT         98
#define DRAWTYPE_CUSTOMIZE           99

//status
#define DRAWSTATUS_NORMAL            0
#define DRAWSTATUS_DISABLE           1
#define DRAWSTATUS_HOVER             2
#define DRAWSTATUS_PRESS             3
#define DRAWSTATUS_DEFAULT           4
#define DRAWSTATUS_MAXCOUNT          5

typedef struct _tag_DRAWCOLOR
{
	COLORREF         clrStart;                        //Start color to draw
}DRAWCOLOR;

typedef struct _tag_DRAWIMAGE
{
	HANDLE           hImage;                          //Handle to the image
	int              x;                               //x coordination for the start point of the image
	int              y;                               //y coordination for the start point of the image
	int              nWidth;                          //Width of the image
	int              nHeight;                         //Height of the image
}DRAWIMAGE;

typedef struct _tag_SKINICON
{
	int              nDefault;                        //Whether the icon was loaded
	int              nDrawType;                       //Icon draw type
	int              nWidth;                          //Width of the icon
	int              nHeight;                         //Height of the icon
	DRAWCOLOR        clrDraw;                         //Color to draw the icon if no image specified
	DRAWIMAGE        imgDraw;                         //Image used to draw the icon
	int              nPadding;                        //Icon padding
}SKINICON;

typedef struct _tag_SKINBACKGROUND
{
	int              nDefault;                        //Whether the background was loaded
	int              nDrawType;                       //Background draw type
	DRAWCOLOR        clrDraw;                         //Background draw color
	DRAWIMAGE        imgDraw;                         //Background draw image
}SKINBACKGROUND;

typedef struct _tag_SKINBORDER
{
	int              nDefault;                        //Whether the border was loaded
	int              nDrawType;                       //Border draw type
	DRAWCOLOR        clrDraw;                         //Border draw color
	DRAWIMAGE        imgDraw;                         //Border draw image
	int              nWidth;                          //Border width
	int              nStart;                          //Border start size
	int              nEnd;                            //Border end size
}SKINBORDER;

typedef struct _tag_SKINSYSBUTTON
{
	int              nDefault;                        //Whether the button was loaded
	int              nDrawType;                       //Button draw type
	DRAWIMAGE        imgDraw;                         //Button image
	DRAWCOLOR        clrBk;                           //Button background
	COLORREF         clrTxt;                          //Button text color;
	COLORREF         clrBd;                           //Button border
	BOOL             bDrawBd;                         //Draw border or not
}SKINSYSBUTTON;

typedef struct _tag_SKINTEXTFONT
{
	int              nFontSize;                       //Text font size
	int              nBold;                           //Whether bold the text
	WCHAR            wstrFontName[LF_FACESIZE];       //Text font name
}SKINTEXTFONT;

typedef struct _tag_SKINTEXT
{
	int              nDefault;                        //Whether the font was loaded
	SKINTEXTFONT     skinFont;                        //Font used to draw text
	COLORREF         clrDraw;                         //Color used to draw text
	BOOL             bDrawShadow;                     //Whether to draw shadow of text
	COLORREF         clrShadow;                       //Shadow of text
}SKINTEXT;

typedef struct _tag_SKINITEM
{
	int              nDefault;                        //Whether the item was loaded
	SKINBACKGROUND   skinBk;                          //Background skin settings
	SKINTEXT         skinTxt;                         //Text skin settings
	SKINBORDER       skinLBorder;                     //Left border skin settings
	SKINBORDER       skinRBorder;                     //Right border skin settings
	SKINBORDER       skinTBorder;                     //Top border skin settings
	SKINBORDER       skinBBorder;                     //Bottom border skin settings
	SKINICON         skinIcon;                        //Icon skin settings
}SKINITEM;

typedef struct _tag_SKINTITLEBAR
{
	int              nDefault;                        //Whether the title bar was loaded
	int              nIncBorder;                      //Whether include border
	int              nWidth;                          //Title bar width
	int              nStart;                          //Title bar left size
	int              nEnd;                            //Title bar right size
	SKINBACKGROUND   skinBkActive;                    //Title bar background in active status
	COLORREF         clrTxtActive;                    //Title bar text color in active status;
	SKINBACKGROUND   skinBkInactive;                  //Title bar background in inactive status
	COLORREF         clrTxtInactive;                  //Title bar text color in inactive status;

	int              nXMarginClose;                   //Close button X margin;
	int              nYMarginClose;                   //Close button Y margin;
	int              nWidthClose;                     //Close button width;
	int              nHeightClose;                    //Close button height;
	SKINSYSBUTTON    skinCloseNormal;                 //Close button in normal status
	SKINSYSBUTTON    skinCloseHover;                  //Close button in hover status
	SKINSYSBUTTON    skinClosePress;                  //Close button in press status
	SKINSYSBUTTON    skinCloseDisable;                //Close button in press status

	int              nXMarginMax;                     //Maximize button X margin;
	int              nYMarginMax;                     //Maximize button Y margin;
	int              nWidthMax;                       //Maximize button width;
	int              nHeightMax;                      //Maximize button height;
	SKINSYSBUTTON    skinMaxNormal;                   //Maximize button in normal status
	SKINSYSBUTTON    skinMaxHover;                    //Maximize button in hover status
	SKINSYSBUTTON    skinMaxPress;                    //Maximize button in press status
	SKINSYSBUTTON    skinMaxDisable;                  //Maximize button in press status
	SKINSYSBUTTON    skinRestoreNormal;               //Restore button in normal status
	SKINSYSBUTTON    skinRestoreHover;                //Restore button in hover status
	SKINSYSBUTTON    skinRestorePress;                //Restore button in press status
	SKINSYSBUTTON    skinRestoreDisable;              //Restore button in press status

	int              nXMarginMin;                     //Minimize button X margin;
	int              nYMarginMin;                     //Minimize button Y margin;
	int              nWidthMin;                       //Minimize button width;
	int              nHeightMin;                      //Minimize button height;
	SKINSYSBUTTON    skinMinNormal;                   //Minimize button in normal status
	SKINSYSBUTTON    skinMinHover;                    //Minimize button in hover status
	SKINSYSBUTTON    skinMinPress;                    //Minimize button in press status
	SKINSYSBUTTON    skinMinDisable;                  //Minimize button in press status
}SKINTITLEBAR;

//skin settings for dialog
typedef struct _tag_SKINDIALOG
{
	int              nRgnType;                        //Dialog region type
	COLORREF         clrTansparent;                   //Transparent color for creating windows region

	SKINTITLEBAR     skinTitleBar;                    //Title bar

	SKINBACKGROUND   skinBkActive;                    //Background in active status
	SKINBACKGROUND   skinBkInactive;                  //Background in active status

	SKINBORDER       skinLBorderActive;               //Left border in active status
	SKINBORDER       skinTBorderActive;               //Top border in active status
	SKINBORDER       skinRBorderActive;               //Right border in active status
	SKINBORDER       skinBBorderActive;               //Bottom border in active status

	SKINBORDER       skinLBorderInactive;             //Left border in inactive status
	SKINBORDER       skinTBorderInactive;             //Top border in inactive status
	SKINBORDER       skinRBorderInactive;             //Right border in inactive status
	SKINBORDER       skinBBorderInactive;             //Bottom border in inactive status

	HFONT            hTitleFont;                      //Title text font
	int              nTitleMargin;                    //Title text margin
	int              nIconMargin;                     //Title icon margin

	int              nBtnPadding;                     //Padding for button
}SKINDIALOG;

//skin settings for button
typedef struct _tag_SKINBUTTON
{
	int              nRgnType;                        //Button region type
	COLORREF         clrTansparent;                   //Transparent color for creating windows region

	SKINBACKGROUND   skinBkNormal;                    //Background in normal status
	SKINBACKGROUND   skinBkDisable;                   //Background in disable status
	SKINBACKGROUND   skinBkHover;                     //Background in hover status
	SKINBACKGROUND   skinBkPress;                     //Background in press status
	SKINBACKGROUND   skinBkDefault;                   //Background in default status

	SKINTEXT         skinTxtNormal;                   //Text in normal status
	SKINTEXT         skinTxtDisable;                  //Text in disable status
	SKINTEXT         skinTxtHover;                    //Text in hover status
	SKINTEXT         skinTxtPress;                    //Text in press status
	SKINTEXT         skinTxtDefault;                  //Text in default status

	SKINBORDER       skinLBorderNormal;               //Left border in normal status
	SKINBORDER       skinRBorderNormal;               //Right border in normal status
	SKINBORDER       skinTBorderNormal;               //Top border in normal status
	SKINBORDER       skinBBorderNormal;               //Bottom border in normal status

	SKINBORDER       skinLBorderDisable;              //Left border in disable status
	SKINBORDER       skinRBorderDisable;              //Right border in disable status
	SKINBORDER       skinTBorderDisable;              //Top border in disable status
	SKINBORDER       skinBBorderDisable;              //Bottom border in disable status

	SKINBORDER       skinLBorderHover;                //Left border in hover status
	SKINBORDER       skinRBorderHover;                //Right border in hover status
	SKINBORDER       skinTBorderHover;                //Top border in hover status
	SKINBORDER       skinBBorderHover;                //Bottom border in hover status

	SKINBORDER       skinLBorderPress;                //Left border in press status
	SKINBORDER       skinRBorderPress;                //Right border in press status
	SKINBORDER       skinTBorderPress;                //Top border in press status
	SKINBORDER       skinBBorderPress;                //Bottom border in press status

	SKINBORDER       skinLBorderDefault;              //Left border in default status
	SKINBORDER       skinRBorderDefault;              //Right border in default status
	SKINBORDER       skinTBorderDefault;              //Top border in default status
	SKINBORDER       skinBBorderDefault;              //Bottom border in default status
}SKINBUTTON;

//skin settings for check box
typedef struct _tag_SKINCHECKBOX
{
	int              nRgnType;                        //Check box region type
	COLORREF         clrTansparent;                   //Transparent color for creating windows region

	SKINBACKGROUND   skinBkNormalUnchecked;           //Background in normal and unchecked status
	SKINBACKGROUND   skinBkDisableUnchecked;          //Background in disable and unchecked status
	SKINBACKGROUND   skinBkHoverUnchecked;            //Background in hover and unchecked status
	SKINBACKGROUND   skinBkPressUnchecked;            //Background in press and unchecked status

	SKINBACKGROUND   skinBkNormalChecked;             //Background in normal and checked status
	SKINBACKGROUND   skinBkDisableChecked;            //Background in disable and checked status
	SKINBACKGROUND   skinBkHoverChecked;              //Background in hover and checked status
	SKINBACKGROUND   skinBkPressChecked;              //Background in press and checked status

	SKINBACKGROUND   skinBkNormalPartChecked;         //Background in normal and partially checked status
	SKINBACKGROUND   skinBkDisablePartChecked;        //Background in disable and partially checked status
	SKINBACKGROUND   skinBkHoverPartChecked;          //Background in hover and partially checked status
	SKINBACKGROUND   skinBkPressPartChecked;          //Background in press and partially checked status

	SKINTEXT         skinTxtNormalUnchecked;          //Text in normal and unchecked status
	SKINTEXT         skinTxtDisableUnchecked;         //Text in disable and unchecked status
	SKINTEXT         skinTxtHoverUnchecked;           //Text in hover and unchecked status
	SKINTEXT         skinTxtPressUnchecked;           //Text in press and unchecked status

	SKINTEXT         skinTxtNormalChecked;            //Text in normal and checked status
	SKINTEXT         skinTxtDisableChecked;           //Text in disable and checked status
	SKINTEXT         skinTxtHoverChecked;             //Text in hover and checked status
	SKINTEXT         skinTxtPressChecked;             //Text in press and checked status

	SKINTEXT         skinTxtNormalPartChecked;        //Text in normal and partially checked status
	SKINTEXT         skinTxtDisablePartChecked;       //Text in disable and partially checked status
	SKINTEXT         skinTxtHoverPartChecked;         //Text in hover and partially checked status
	SKINTEXT         skinTxtPressPartChecked;         //Text in press and partially checked status

	SKINICON         iconNormalUnchecked;             //Icon in normal and unchecked status
	SKINICON         iconDisableUnchecked;            //Icon in disable and unchecked status
	SKINICON         iconHoverUnchecked;              //Icon in hover and unchecked status
	SKINICON         iconPressUnchecked;              //Icon in press and unchecked status

	SKINICON         iconNormalChecked;               //Icon in normal and checked status
	SKINICON         iconDisableChecked;              //Icon in disable and checked status
	SKINICON         iconHoverChecked;                //Icon in hover and checked status
	SKINICON         iconPressChecked;                //Icon in press and checked status

	SKINICON         iconNormalPartChecked;           //Icon in normal and partially checked status
	SKINICON         iconDisablePartChecked;          //Icon in disable and partially checked status
	SKINICON         iconHoverPartChecked;            //Icon in hover and partially checked status
	SKINICON         iconPressPartChecked;            //Icon in press and partially checked status
}SKINCHECKBOX;

//skin settings for radio
typedef struct _tag_SKINRADIO
{
	int              nRgnType;                        //Check box region type
	COLORREF         clrTansparent;                   //Transparent color for creating windows region

	SKINBACKGROUND   skinBkNormalUnchecked;           //Background in normal and unchecked status
	SKINBACKGROUND   skinBkDisableUnchecked;          //Background in disable and unchecked status
	SKINBACKGROUND   skinBkHoverUnchecked;            //Background in hover and unchecked status
	SKINBACKGROUND   skinBkPressUnchecked;            //Background in press and unchecked status

	SKINBACKGROUND   skinBkNormalChecked;             //Background in normal and checked status
	SKINBACKGROUND   skinBkDisableChecked;            //Background in disable and checked status
	SKINBACKGROUND   skinBkHoverChecked;              //Background in hover and checked status
	SKINBACKGROUND   skinBkPressChecked;              //Background in press and checked status

	SKINTEXT         skinTxtNormalUnchecked;          //Text in normal and unchecked status
	SKINTEXT         skinTxtDisableUnchecked;         //Text in disable and unchecked status
	SKINTEXT         skinTxtHoverUnchecked;           //Text in hover and unchecked status
	SKINTEXT         skinTxtPressUnchecked;           //Text in press and unchecked status

	SKINTEXT         skinTxtNormalChecked;            //Text in normal and checked status
	SKINTEXT         skinTxtDisableChecked;           //Text in disable and checked status
	SKINTEXT         skinTxtHoverChecked;             //Text in hover and checked status
	SKINTEXT         skinTxtPressChecked;             //Text in press and checked status

	SKINICON         iconNormalUnchecked;             //Icon in normal and unchecked status
	SKINICON         iconDisableUnchecked;            //Icon in disable and unchecked status
	SKINICON         iconHoverUnchecked;              //Icon in hover and unchecked status
	SKINICON         iconPressUnchecked;              //Icon in press and unchecked status

	SKINICON         iconNormalChecked;               //Icon in normal and checked status
	SKINICON         iconDisableChecked;              //Icon in disable and checked status
	SKINICON         iconHoverChecked;                //Icon in hover and checked status
	SKINICON         iconPressChecked;                //Icon in press and checked status
}SKINRADIO;

typedef struct _tag_SKINGROUPBOX
{
	SKINBACKGROUND   skinBkNormal;                    //Background in normal status

	SKINBORDER       skinLBorderNormal;               //Left border in normal status
	SKINBORDER       skinRBorderNormal;               //Right border in normal status
	SKINBORDER       skinTBorderNormal;               //Top border in normal status
	SKINBORDER       skinBBorderNormal;               //Bottom border in normal status

	SKINTEXT         skinTxtNormal;                   //Text in normal status
	int              nMarginTxt;                      //Text margin
}SKINGROUPBOX;

typedef struct _tag_SKINSTATIC
{
	SKINBACKGROUND   skinBkNormal;                    //Background in normal status
	SKINTEXT         skinTxtNormal;                   //Text in normal status
}SKINSTATIC;

typedef struct _tag_SKINEDIT
{
	SKINBACKGROUND   skinBkNormal;                    //Background in normal status
	SKINBACKGROUND   skinBkDisable;                   //Background in disable status
	SKINBACKGROUND   skinBkHover;                     //Background in hover status
	SKINBACKGROUND   skinBkPress;                     //Background in focus status

	SKINBACKGROUND   skinBkNormalReadOnly;            //Background in normal status
	SKINBACKGROUND   skinBkHoverReadOnly;             //Background in hover status
	SKINBACKGROUND   skinBkPressReadOnly;             //Background in focus status

	SKINTEXT         skinTxtNormal;                   //Text in normal and unchecked status
	SKINTEXT         skinTxtDisable;                  //Text in disable and unchecked status
	SKINTEXT         skinTxtHover;                    //Text in hover and unchecked status
	SKINTEXT         skinTxtPress;                    //Text in press and unchecked status

	SKINTEXT         skinTxtNormalReadOnly;           //Text in normal and checked status
	SKINTEXT         skinTxtHoverReadOnly;            //Text in hover and checked status
	SKINTEXT         skinTxtPressReadOnly;            //Text in press and checked status

	SKINBORDER       skinLBorderNormal;               //Left border in normal status
	SKINBORDER       skinRBorderNormal;               //Right border in normal status
	SKINBORDER       skinTBorderNormal;               //Top border in normal status
	SKINBORDER       skinBBorderNormal;               //Bottom border in normal status

	SKINBORDER       skinLBorderDisable;              //Left border in disable status
	SKINBORDER       skinRBorderDisable;              //Right border in disable status
	SKINBORDER       skinTBorderDisable;              //Top border in disable status
	SKINBORDER       skinBBorderDisable;              //Bottom border in disable status

	SKINBORDER       skinLBorderHover;                //Left border in hover status
	SKINBORDER       skinRBorderHover;                //Right border in hover status
	SKINBORDER       skinTBorderHover;                //Top border in hover status
	SKINBORDER       skinBBorderHover;                //Bottom border in hover status

	SKINBORDER       skinLBorderPress;                //Left border in focus status
	SKINBORDER       skinRBorderPress;                //Right border in focus status
	SKINBORDER       skinTBorderPress;                //Top border in focus status
	SKINBORDER       skinBBorderPress;                //Bottom border in focus status

	SKINBORDER       skinLBorderNormalReadOnly;       //Left border in normal status for read only edit
	SKINBORDER       skinRBorderNormalReadOnly;       //Right border in normal status for read only edit
	SKINBORDER       skinTBorderNormalReadOnly;       //Top border in normal status for read only edit
	SKINBORDER       skinBBorderNormalReadOnly;       //Bottom border in normal status for read only edit

	SKINBORDER       skinLBorderHoverReadOnly;        //Left border in hover status for read only edit
	SKINBORDER       skinRBorderHoverReadOnly;        //Right border in hover status for read only edit
	SKINBORDER       skinTBorderHoverReadOnly;        //Top border in hover status for read only edit
	SKINBORDER       skinBBorderHoverReadOnly;        //Bottom border in hover status for read only edit

	SKINBORDER       skinLBorderPressReadOnly;        //Left border in focus status for read only edit
	SKINBORDER       skinRBorderPressReadOnly;        //Right border in focus status for read only edit
	SKINBORDER       skinTBorderPressReadOnly;        //Top border in focus status for read only edit
	SKINBORDER       skinBBorderPressReadOnly;        //Bottom border in focus status for read only edit
}SKINEDIT;

typedef struct _tag_SKINTAB
{
	SKINBACKGROUND   skinBkNormal;                    //Background in normal status

	SKINBORDER       skinLBorderNormal;               //Left border in normal status
	SKINBORDER       skinRBorderNormal;               //Right border in normal status
	SKINBORDER       skinTBorderNormal;               //Top border in normal status
	SKINBORDER       skinBBorderNormal;               //Bottom border in normal status

	SKINBUTTON       skinTabButton;                   //Tab button
	int              nButtonWidth;                    //Button width
	int              nButtonMargin;                   //Button margin
	int              nButtonPadding;                  //Button padding
	int              nDrawOrder;                      //Button draw order
}SKINTAB;

typedef struct _tag_SKINLISTBOX
{
	SKINBACKGROUND   skinBkNormal;                    //Background in normal status
	SKINBACKGROUND   skinBkDisable;                   //Background in disable status

	SKINBORDER       skinLBorderNormal;               //Left border in normal status
	SKINBORDER       skinRBorderNormal;               //Right border in normal status
	SKINBORDER       skinTBorderNormal;               //Top border in normal status
	SKINBORDER       skinBBorderNormal;               //Bottom border in normal status

	SKINBORDER       skinLBorderDisable;              //Left border in disable status
	SKINBORDER       skinRBorderDisable;              //Right border in disable status
	SKINBORDER       skinTBorderDisable;              //Top border in disable status
	SKINBORDER       skinBBorderDisable;              //Bottom border in disable status

	SKINITEM         skinItemNormalUnselected;        //Unselected item in normal status
	SKINITEM         skinItemDisableUnselected;       //Unselected item in disable status
	SKINITEM         skinItemHoverUnselected;         //Unselecetd item in disable status

	SKINITEM         skinItemNormalSelected;          //Selected item in normal status
	SKINITEM         skinItemDisableSelected;         //Selected item in disable status
	SKINITEM         skinItemHoverSelected;           //Selected item in hover status
}SKINLISTBOX;

typedef struct _tag_SKINLISTCTRL
{
	SKINBACKGROUND   skinBkNormal;                    //Background in normal status
	SKINBACKGROUND   skinBkDisable;                   //Background in disable status

	SKINBORDER       skinLBorderNormal;               //Left border in normal status
	SKINBORDER       skinRBorderNormal;               //Right border in normal status
	SKINBORDER       skinTBorderNormal;               //Top border in normal status
	SKINBORDER       skinBBorderNormal;               //Bottom border in normal status

	SKINBORDER       skinLBorderDisable;              //Left border in disable status
	SKINBORDER       skinRBorderDisable;              //Right border in disable status
	SKINBORDER       skinTBorderDisable;              //Top border in disable status
	SKINBORDER       skinBBorderDisable;              //Bottom border in disable status

	SKINITEM         skinItemNormalUnselected;        //Unselected item in normal status
	SKINITEM         skinItemDisableUnselected;       //Unselected item in disable status
	SKINITEM         skinItemHoverUnselected;         //Unselecetd item in disable status

	SKINITEM         skinItemNormalSelected;          //Selected item in normal status
	SKINITEM         skinItemDisableSelected;         //Selected item in disable status
	SKINITEM         skinItemHoverSelected;           //Selected item in hover status
}SKINLISTCTRL;

typedef struct _tag_SKINCOMBOBOX
{
	SKINBACKGROUND   skinBkNormal;                    //Background in normal status
	SKINBACKGROUND   skinBkDisable;                   //Background in disable status

	SKINTEXT         skinTxtNormal;                   //Text in normal status
	SKINTEXT         skinTxtDisable;                  //Text in disable status

	SKINBORDER       skinLBorderNormal;               //Left border in normal status
	SKINBORDER       skinRBorderNormal;               //Right border in normal status
	SKINBORDER       skinTBorderNormal;               //Top border in normal status
	SKINBORDER       skinBBorderNormal;               //Bottom border in normal status

	SKINBORDER       skinLBorderDisable;              //Left border in disable status
	SKINBORDER       skinRBorderDisable;              //Right border in disable status
	SKINBORDER       skinTBorderDisable;              //Top border in disable status
	SKINBORDER       skinBBorderDisable;              //Bottom border in disable status

	SKINBORDER       skinLBorderHover;                //Left border in hover status
	SKINBORDER       skinRBorderHover;                //Right border in hover status
	SKINBORDER       skinTBorderHover;                //Top border in hover status
	SKINBORDER       skinBBorderHover;                //Bottom border in hover status

	SKINSYSBUTTON    skinBtnNormal;                   //Dropdown button in normal status
	SKINSYSBUTTON    skinBtnDisable;                  //Dropdown button in disable status
	SKINSYSBUTTON    skinBtnHover;                    //Dropdown button in hover status
	SKINSYSBUTTON    skinBtnPress;                    //Dropdown button in press status
	int              nBtnIncludeBorder;               //Whether button include border

	SKINLISTBOX      skinListBox;                     //Skin list box
}SKINCOMBOBOX;

typedef struct _tag_SKINSCROLLBAR
{
	SKINBACKGROUND   skinBkNormal;                     //Background in normal status
	SKINBACKGROUND   skinBkDisable;                   //Background in disable status

	SKINBACKGROUND   skinHBkNormal;                   //Background in normal status for horizontal scroll bar
	SKINBACKGROUND   skinHBkDisable;                  //Background in disable status for horizontal scroll bar

	SKINBACKGROUND   skinVBkNormal;                   //Background in normal status for vertical scroll bar
	SKINBACKGROUND   skinVBkDisable;                  //Background in disable status for vertical scroll bar

	SKINSYSBUTTON    skinBtnLeftNormal;               //Left button in normal status
	SKINSYSBUTTON    skinBtnLeftDisable;              //Left button in disable status
	SKINSYSBUTTON    skinBtnLeftHover;                //Left button in hover status
	SKINSYSBUTTON    skinBtnLeftPress;                //Left button in press status

	SKINSYSBUTTON    skinBtnRightNormal;              //Right button in normal status
	SKINSYSBUTTON    skinBtnRightDisable;             //Right button in disable status
	SKINSYSBUTTON    skinBtnRightHover;               //Right button in hover status
	SKINSYSBUTTON    skinBtnRightPress;               //Right button in press status

	SKINSYSBUTTON    skinBtnTopNormal;                //Top button in normal status
	SKINSYSBUTTON    skinBtnTopDisable;               //Top button in disable status
	SKINSYSBUTTON    skinBtnTopHover;                 //Top button in hover status
	SKINSYSBUTTON    skinBtnTopPress;                 //Top button in press status

	SKINSYSBUTTON    skinBtnBottomNormal;             //Bottom button in normal status
	SKINSYSBUTTON    skinBtnBottomDisable;            //Bottom button in disable status
	SKINSYSBUTTON    skinBtnBottomHover;              //Bottom button in hover status
	SKINSYSBUTTON    skinBtnBottomPress;              //Bottom button in press status

	SKINITEM         skinHItemNormal;                 //Item in normal status for horizontal scroll bar
	SKINITEM         skinHItemDisabled;               //Item in disable status for horizontal scroll bar
	SKINITEM         skinHItemHover;                  //Item in hover status for horizontal scroll bar
	SKINITEM         skinHItemPress;                  //Item in press status for horizontal scroll bar

	SKINITEM         skinVItemNormal;                 //Item in normal status for vertical scroll bar
	SKINITEM         skinVItemDisabled;               //Item in disable status for vertical scroll bar
	SKINITEM         skinVItemHover;                  //Item in hover status for vertical scroll bar
	SKINITEM         skinVItemPress;                  //Item in press status for vertical scroll bar

	SKINBORDER       skinHLBorderNormal;              //Left border in normal status for horizontal scroll bar
	SKINBORDER       skinHRBorderNormal;              //Right border in normal status for horizontal scroll bar
	SKINBORDER       skinHTBorderNormal;              //Top border in normal status for horizontal scroll bar
	SKINBORDER       skinHBBorderNormal;              //Bottom border in normal status for horizontal scroll bar

	SKINBORDER       skinHLBorderDisable;             //Left border in disable status for horizontal scroll bar
	SKINBORDER       skinHRBorderDisable;             //Right border in disable status for horizontal scroll bar
	SKINBORDER       skinHTBorderDisable;             //Top border in disable status for horizontal scroll bar
	SKINBORDER       skinHBBorderDisable;             //Bottom border in disable status for horizontal scroll bar

	SKINBORDER       skinVLBorderNormal;              //Left border in normal status for vertical scroll bar
	SKINBORDER       skinVRBorderNormal;              //Right border in normal status for vertical scroll bar
	SKINBORDER       skinVTBorderNormal;              //Top border in normal status for vertical scroll bar
	SKINBORDER       skinVBBorderNormal;              //Bottom border in normal status for vertical scroll bar

	SKINBORDER       skinVLBorderDisable;             //Left border in disable status for vertical scroll bar
	SKINBORDER       skinVRBorderDisable;             //Right border in disable status for vertical scroll bar
	SKINBORDER       skinVTBorderDisable;             //Top border in disable status for vertical scroll bar
	SKINBORDER       skinVBBorderDisable;             //Bottom border in disable status for vertical scroll bar

	int              nBtnLeftIncludeBorder;           //Whether left button include border       
	int              nBtnRightIncludeBorder;          //Whether right button include border
	int              nBtnTopIncludeBorder;            //Whether top button include border
	int              nBtnBottomIncludeBorder;         //Whether bottom button include border
	int              nHThumbIncludeBorder;            //Whether thumb include border
	int              nVThumbIncludeBorder;            //Whether thumb include border
}SKINSCROLLBAR;

typedef struct _tag_SKINHEADERCTRL
{
	SKINITEM         skinItemNormal;                  //Item in normal status
	SKINITEM         skinItemDisabled;                //Item in disable status
	SKINITEM         skinItemHover;                   //Item in hover status
	SKINITEM         skinItemPress;                   //Item in press status
}SKINHEADERCTRL;

typedef struct _tag_SKINPROGRESSCTRL
{
	SKINBACKGROUND   skinHBkNormal;                    //Background in normal status
	SKINBACKGROUND   skinHFrNormal;                    //Foreground in normal status

	SKINBACKGROUND   skinVBkNormal;                    //Background in normal status
	SKINBACKGROUND   skinVFrNormal;                    //Foreground in normal status

	SKINBORDER       skinHLBorderNormal;               //Left border in normal status
	SKINBORDER       skinHRBorderNormal;               //Right border in normal status
	SKINBORDER       skinHTBorderNormal;               //Top border in normal status
	SKINBORDER       skinHBBorderNormal;               //Bottom border in normal status

	SKINBORDER       skinVLBorderNormal;               //Left border in normal status
	SKINBORDER       skinVRBorderNormal;               //Right border in normal status
	SKINBORDER       skinVTBorderNormal;               //Top border in normal status
	SKINBORDER       skinVBBorderNormal;               //Bottom border in normal status

	int              nHBlockWidth;                     //Width of foreground block
	int              nHBlockSpacing;                   //Space width between foreground blocks

	int              nVBlockWidth;                     //Width of foreground block
	int              nVBlockSpacing;                   //Space width between foreground blocks
}SKINPROGRESSCTRL;

typedef struct _tag_SKINSLIDERCTRL
{
	SKINBACKGROUND   skinHBkNormal;                    //Background in normal status
	SKINBACKGROUND   skinHBkDisable;                   //Background in disable status

	SKINBACKGROUND   skinVBkNormal;                    //Background in normal status
	SKINBACKGROUND   skinVBkDisable;                   //Foreground in normal status

	SKINITEM         skinHSliderNormal;                //Slider in normal status
	SKINITEM         skinHSliderDisable;               //Slider in disable status

	SKINITEM         skinVSliderNormal;                //Slider in normal status
	SKINITEM         skinVSliderDisable;               //Slider in disable status

	SKINITEM         skinHThumbNormal;                 //Thumb in normal status
	SKINITEM         skinHThumbDisable;                //Thumb in disable status
	SKINITEM         skinHThumbHover;                  //Thumb in hover status
	SKINITEM         skinHThumbPress;                  //Thumb in press status

	SKINITEM         skinVThumbNormal;                 //Thumb in normal status
	SKINITEM         skinVThumbDisable;                //Thumb in disable status
	SKINITEM         skinVThumbHover;                  //Thumb in hover status
	SKINITEM         skinVThumbPress;                  //Thumb in press status

	int              nHSliderWidth;                    //Slider width
	int              nVSliderWidth;                    //Slider width

	int              nHThumbWidth;                     //Thumb width
	int              nVThumbWidth;                     //Thumb width
}SKINSLIDERCTRL;

class CDreamSkinLoader
{
public:
	CDreamSkinLoader(CImageHandleList *pImageHandleList);
	~CDreamSkinLoader();

protected:
	static WCHAR wstrSkinFileNodeNameButton[];
	static WCHAR wstrSkinFileNodeNameCheckBox[];
	static WCHAR wstrSkinFileNodeNameComboBox[];
	static WCHAR wstrSkinFileNodeNameDialog[];
	static WCHAR wstrSkinFileNodeNameEdit[];
	static WCHAR wstrSkinFileNodeNameGroupBox[];
	static WCHAR wstrSkinFileNodeNameHeaderCtrl[];
	static WCHAR wstrSkinFileNodeNameListBox[];
	static WCHAR wstrSkinFileNodeNameListCtrl[];
	static WCHAR wstrSkinFileNodeNameProgressCtrl[];
	static WCHAR wstrSkinFileNodeNameRadio[];
	static WCHAR wstrSkinFileNodeNameScrollBar[];
	static WCHAR wstrSkinFileNodeNameSliderCtrl[];
	static WCHAR wstrSkinFileNodeNameStatic[];
	static WCHAR wstrSkinFileNodeNameTab[];

	static WCHAR wstrSkinFileNodeNameBackground[];
	static WCHAR wstrSkinFileNodeNameForeground[];
	static WCHAR wstrSkinFileNodeNameBorder[];
	static WCHAR wstrSkinFileNodeNameColor[];
	static WCHAR wstrSkinFileNodeNameFont[];
	static WCHAR wstrSkinFileNodeNameText[];
	static WCHAR wstrSkinFileNodeNameIcon[];
	static WCHAR wstrSkinFileNodeNameImage[];
	static WCHAR wstrSkinFileNodeNameItem[];
	static WCHAR wstrSkinFileNodeNameThumb[];
	static WCHAR wstrSkinFileNodeNameLeft[];
	static WCHAR wstrSkinFileNodeNameRight[];
	static WCHAR wstrSkinFileNodeNameTop[];
	static WCHAR wstrSkinFileNodeNameBottom[];
	static WCHAR wstrSkinFileNodeNameTitle[];
	static WCHAR wstrSkinFileNodeNameNormal[];
	static WCHAR wstrSkinFileNodeNameHover[];
	static WCHAR wstrSkinFileNodeNamePress[];
	static WCHAR wstrSkinFileNodeNameDisable[];
	static WCHAR wstrSkinFileNodeNameDefault[];
	static WCHAR wstrSkinFileNodeNameSelected[];
	static WCHAR wstrSkinFileNodeNameUnselected[];
	static WCHAR wstrSkinFileNodeNameIndeterminate[];
	static WCHAR wstrSkinFileNodeNameReadWrite[];
	static WCHAR wstrSkinFileNodeNameReadOnly[];
	static WCHAR wstrSkinFileNodeNameClose[];
	static WCHAR wstrSkinFileNodeNameMaximize[];
	static WCHAR wstrSkinFileNodeNameRestore[];
	static WCHAR wstrSkinFileNodeNameMinimize[];
	static WCHAR wstrSkinFileNodeNameExpand[];
	static WCHAR wstrSkinFileNodeNameHorizontal[];
	static WCHAR wstrSkinFileNodeNameVertical[];
	static WCHAR wstrSkinFileNodeNameSlider[];

	static WCHAR wstrSkinFileAttrNameWidth[];
	static WCHAR wstrSkinFileAttrNameHeight[];
	static WCHAR wstrSkinFileAttrNameType[];
	static WCHAR wstrSkinFileAttrNameRegion[];
	static WCHAR wstrSkinFileAttrNameTransparent[];
	static WCHAR wstrSkinFileAttrNameSource[];
	static WCHAR wstrSkinFileAttrNameStart[];
	static WCHAR wstrSkinFileAttrNameEnd[];
	static WCHAR wstrSkinFileAttrNameText[];
	static WCHAR wstrSkinFileAttrNameBorder[];
	static WCHAR wstrSkinFileAttrNameBold[];
	static WCHAR wstrSkinFileAttrNameX[];
	static WCHAR wstrSkinFileAttrNameY[];
	static WCHAR wstrSkinFileAttrNameMarginLeft[];
	static WCHAR wstrSkinFileAttrNameMarginTop[];
	static WCHAR wstrSkinFileAttrNameMarginBottom[];
	static WCHAR wstrSkinFileAttrNameMarginRight[];
	static WCHAR wstrSkinFileAttrNameIncludeBorder[];

protected:
	static COLORREF TextToColor(const WCHAR *wstrColor);

public:
	BOOL Load(const WCHAR *wstrSkinFilePath);

	void GetSkinButton(SKINBUTTON *pSkinButton) const;
	void GetSkinCheckBox(SKINCHECKBOX *pSkinCheckBox) const;
	void GetSkinComboBox(SKINCOMBOBOX *pSkinComboBox) const;
	void GetSkinDialog(SKINDIALOG *pSkinDialog) const;
	void GetSkinEdit(SKINEDIT *pSkinEdit) const;
	void GetSkinGroupBox(SKINGROUPBOX *pSkinGroupBox) const;
	void GetSkinHeaderCtrl(SKINHEADERCTRL *pSkinHeaderCtrl) const;
	void GetSkinListBox(SKINLISTBOX *pSkinListBox) const;
	void GetSkinListCtrl(SKINLISTCTRL *pSkinListCtrl) const;
	void GetSkinProgressCtrl(SKINPROGRESSCTRL *pSkinProgressCtrl) const;
	void GetSkinRadio(SKINRADIO *pSkinRadio) const;
	void GetSkinScrollBar(SKINSCROLLBAR *pSkinScrollBar) const;
	void GetSkinSliderCtrl(SKINSLIDERCTRL *pSkinSliderCtrl) const;
	void GetSkinStatic(SKINSTATIC *pSkinStatic) const;
	void GetSkinTab(SKINTAB *pSkinTab) const;

protected:
	BOOL LoadSkinDialog(void *parser);
	BOOL LoadSkinButton(void *parser);
	BOOL LoadSkinCheckBox(void *parser);
	BOOL LoadSkinComboBox(void *parser);
	BOOL LoadSkinEdit(void *parser);
	BOOL LoadSkinGroupBox(void *parser);
	BOOL LoadSkinHeaderCtrl(void *parser);
	BOOL LoadSkinListBox(void *parser);
	BOOL LoadSkinListCtrl(void *parser);
	BOOL LoadSkinProgressCtrl(void *parser);
	BOOL LoadSkinRadio(void *parser);
	BOOL LoadSkinScrollBar(void *parser);
	BOOL LoadSkinSliderCtrl(void *parser);
	BOOL LoadSkinStatic(void *parser);
	BOOL LoadSkinTab(void *parser);

	BOOL LoadBackground(void *bkgnd, SKINBACKGROUND **pSkinBackgroundList, int nCount);
	BOOL LoadBackgrounditem(void *bkitem, SKINBACKGROUND *pSkinBackground);
	BOOL LoadButton(void *button, SKINBUTTON *pSkinButton);
	BOOL LoadColor(void *color, DRAWCOLOR *pDrawColor);
	BOOL LoadColorItem(void *color, WCHAR* wstrColorItemName, COLORREF *pColor);
	BOOL LoadFont(void *font, SKINTEXTFONT *pSkinFont);
	BOOL LoadIcon(void *icon, SKINICON **pSkinIconList, int nCount);
	BOOL LoadIconItem(void *iconitem, SKINICON *pSkinIcon);
	BOOL LoadImage(void *image, DRAWIMAGE *pDrawImage);
	BOOL LoadItem(void *item, SKINITEM **pSkinItemList, int nCount);
	BOOL LoadBackground(void *bkitem, SKINBACKGROUND *pSkinBackground);
	BOOL LoadBorder(void *border, SKINBORDER **pSkinBorderList, int nCount);
	BOOL LoadBorderItem(void *bditem, SKINBORDER *pSkinBorder);
	//BOOL LoadSysButton(void *sysbtn, SKINSYSBUTTON *pSkinSysButton);
	BOOL LoadSysButtonItem(void *sysbtn, SKINSYSBUTTON *pSkinSysButton);
	BOOL LoadText(void *text, SKINTEXT **pSkinTextList, int nCount);
	BOOL LoadTextItem(void *txtitem, SKINTEXT *pSkinText);
	BOOL LoadTitleBar(void *titlebar, SKINTITLEBAR *pSkinTitleBar);

protected:
	SKINDIALOG       m_SkinDialog;
	SKINBUTTON       m_SkinButton;
	SKINCHECKBOX     m_SkinCheckBox;
	SKINCOMBOBOX     m_SkinComboBox;
	SKINEDIT         m_SkinEdit;
	SKINGROUPBOX     m_SkinGroupBox;
	SKINHEADERCTRL   m_SkinHeaderCtrl;
	SKINLISTBOX      m_SkinListBox;
	SKINLISTCTRL     m_SkinListCtrl;
	SKINPROGRESSCTRL m_SkinProgressCtrl;
	SKINRADIO        m_SkinRadio;
	SKINSCROLLBAR    m_SkinScrollBar;
	SKINSLIDERCTRL   m_SkinSliderCtrl;
	SKINSTATIC       m_SkinStatic;
	SKINTAB          m_SkinTab;
	WCHAR            m_wstrSkinFilePath[MAX_PATH];
	WCHAR            m_wstrSkinFileDir[MAX_PATH];
	
	CImageHandleList *m_pImageHandleList;
};

#endif // DREAMSKIN_LOADER_H