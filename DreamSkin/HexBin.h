//HexBin.h

#ifndef HEX_BIN_TRANS_H
#define HEX_BIN_TRANS_H

#define DEFAULT_BINTOHEX_MAXLINE_LENGTH    72

#define ENCODE_OPTION_LIMIT_LINE_LENGTH                  0x01
#define ENCODE_OPTION_BEGIN_WITH_HEX_TAG                 0x02

int GetBinToHexDesLen(int nSrcLen, int nMaxLineLen = DEFAULT_BINTOHEX_MAXLINE_LENGTH, int nOption = 0);
int BinToHexA(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen = DEFAULT_BINTOHEX_MAXLINE_LENGTH, int nOption = 0);
int BinToHexW(const unsigned char* pSrc, wchar_t* pDst, int nSrcLen, int nMaxLineLen = DEFAULT_BINTOHEX_MAXLINE_LENGTH, int nOption = 0);

int GetHexToBinDesLenA(const char* pSrc, int nSrcLen);
int GetHexToBinDesLenW(const wchar_t *pSrc, int nSrcLen);
int HexToBinA(const char* pSrc, unsigned char* pDst, int nSrcLen, int nDstLen);
int HexToBinW(const wchar_t *pSrc, unsigned char* pDst, int nSrcLen, int nDstLen);

#ifdef UNICODE
#define BinToHex  BinToHexW
#define GetHexToBinDesLen GetHexToBinDesLenW
#define HextoBin HextoBinW
#else
#define BinToHex  BinToHexA
#define GetHexToBinDesLen GetHexToBinDesLenA
#define HexToBin HexToBinA
#endif // !UNICODE


#endif // HEX_BIN_TRANS_H