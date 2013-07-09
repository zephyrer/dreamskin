//DreamSkin.h

#ifndef DREAMSKIN_INCLUDE
#define DREAMSKIN_INCLUDE

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

//If use DearmSkin by DLL mode, user should enable the following macro
//or define this macro as pre-defined symbol
//#define DREAMSKINDLL

#ifdef DREAMSKINDLL
	#ifdef DREAMSKINDLL_EXPORTS
		#define DREAMSKIN_API __declspec(dllexport)
	#else
		#define DREAMSKIN_API __declspec(dllimport)
	#endif
#else
	#define DREAMSKIN_API
#endif

//Initial DreamSkin Lib
DREAMSKIN_API BOOL DreamSkinInit();

//Exit DreamSkin
DREAMSKIN_API void DreamSkinExit();

//Get DreamSkin Runtime Status
typedef struct _tag_DREAMSKIN_STATUSA
{
	UINT     nHookedWindowCount;
}DREAMSKIN_STATUSA;
typedef struct _tag_DREAMSKIN_STATUSW
{
	UINT     nHookedWindowCount;
}DREAMSKIN_STATUSW;
DREAMSKIN_API BOOL DreamSkinStatusA(DREAMSKIN_STATUSA* pDreamSkinStatus);
DREAMSKIN_API BOOL DreamSkinStatusW(DREAMSKIN_STATUSW* pDreamSkinStatus);
#ifdef UNICODE
#define DREAMSKIN_STATUS  DREAMSKIN_STATUSW
#define DreamSkinStatus   DreamSkinStatusW
#else
#define DREAMSKIN_STATUS  DREAMSKIN_STATUSA
#define DreamSkinStatus   DreamSkinStatusA
#endif // !UNICODE

//Load a skin
DREAMSKIN_API BOOL DreamSkinLoadA(const char* strSkinFilePath);
DREAMSKIN_API BOOL DreamSkinLoadW(const WCHAR* wstrSkinFilePath);
#ifdef UNICODE
#define DreamSkinLoad  DreamSkinLoadW
#else
#define DreamSkinLoad  DreamSkinLoadA
#endif // !UNICODE

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // DREAMSKIN_INCLUDE