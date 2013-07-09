//HexBin.cpp
#include "HexBin.h"

const char BintoHexTab[] = "0123456789ABCDEF";

int GetBintoHexDesLen(int nSrcLen, int nMaxLineLen, int nOption)
{
	int nDstLen = 0;             // Output length
    int nLineLen = 0;            // Output line length

	// Get 1 bytes each time and encoding to 2 bytes
    for (int i = 0; i < nSrcLen; i ++)
    {
        nLineLen += 2;
        nDstLen += 2;
 
        // If specified output max line length
		if((nMaxLineLen > 0) && (nOption & ENCODE_OPTION_LIMIT_LINE_LENGTH) && (nLineLen > nMaxLineLen - 2))
		{
			nLineLen = 0;
			nDstLen += 2;   //\r\n
		}
    }

	if (nOption & ENCODE_OPTION_BEGIN_WITH_HEX_TAG)
		nDstLen += 2;      //begin with 0x

	return nDstLen;
}

int BintoHex(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen, int nOption)
{
	int nDstLen = 0;             // Output length
    int nLineLen = 0;            // Output line length
	unsigned char unByte;

	// Get 1 bytes each time and encoding to 2 bytes
    for (int i = 0; i < nSrcLen; i ++)
    {
		//Get 1 byte
        unByte = *pSrc++;

		//Encoding to 2 bytes
		*pDst++ = BintoHexTab[unByte >> 4];
		*pDst++ = BintoHexTab[unByte & 0x0F];
		nLineLen += 2;
        nDstLen += 2;
 
        // If specified output max line length
        if((nMaxLineLen > 0) && (nLineLen > nMaxLineLen - 2))
        {
			*pDst++ = L'\r';
            *pDst++ = L'\n';
            nLineLen = 0;
			nDstLen += 2;
        }
    }

	return nDstLen;
}

const wchar_t BintoHexTabW[] = L"0123456789ABCDEF";
int BintoHexW(const unsigned char* pSrc, unsigned short* pDst, int nSrcLen, int nMaxLineLen, int nOption)
{
	int nDstLen = 0;             // Output length
    int nLineLen = 0;            // Output line length
	unsigned char unByte;

	// Get 1 bytes each time and encoding to 2 bytes
    for (int i = 0; i < nSrcLen; i ++)
    {
		//Get 1 byte
        unByte = *pSrc++;

		//Encoding to 2 bytes
		*pDst++ = BintoHexTabW[unByte >> 4];
		*pDst++ = BintoHexTabW[unByte & 0x0F];
		nLineLen += 2;
        nDstLen += 2;
 
        // If specified output max line length
        if((nMaxLineLen > 0) && (nLineLen > nMaxLineLen - 2))
        {
			*pDst++ = L'\r';
            *pDst++ = L'\n';
            nLineLen = 0;
			nDstLen += 2;
        }
    }

	return nDstLen;
}

const char HextoBinTab[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,       // '0'-'9'
    0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0,       // 'A'-'F'
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0,       // 'a'-'f'
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

int GetHextoBinDesLen(const char* pSrc, int nSrcLen)
{
	int nDstLen;            // Output length
    int i;
 
    i = 0;
    nDstLen = 0;

	if(nSrcLen > 2)
	{
		if((*pSrc == '0') && (*(pSrc + 1) == 'x'))
		{
			pSrc++;
			pSrc++;
			nSrcLen--;
			nSrcLen--;
		}
	}
 
    // Get 2 bytes each time and decode to 1 byte
    while (i < nSrcLen)
    {
		if (pSrc[i]!='\n' && *pSrc!='\r')
        {
            nDstLen++;
            i += 2;
        }
        else        // Skip CRLF
        {
            i++;
        }
     }

 
    return nDstLen;
}

int GetHexToBinDesLenW(const wchar_t *pSrc, int nSrcLen)
{
	int nDstLen;            // Output length
    int i;
 
    i = 0;
    nDstLen = 0;

	if(nSrcLen > 2)
	{
		if((*pSrc == '0') && (*(pSrc + 1) == 'x'))
		{
			pSrc++;
			pSrc++;
			nSrcLen--;
			nSrcLen--;
		}
	}
 
    // Get 2 character each time and decode to 1 byte
    while (i < nSrcLen)
    {
		if (pSrc[i] != '\n' && pSrc[i] !='\r')
        {
            nDstLen++;
            i += 2;
        }
        else        // Skip CRLF
        {
            i++;
        }
     }

 
    return nDstLen;
}

int HextoBin(const char* pSrc, unsigned char* pDst, int nSrcLen)
{
	int nDstLen;            // Output length
	unsigned char unByte;   // Temp char for decode
    int i;
 
    i = 0;
    nDstLen = 0;

	if(nSrcLen > 2)
	{
		if((*pSrc == '0') && (*(pSrc + 1) == 'x'))
		{
			pSrc++;
			pSrc++;
			nSrcLen--;
			nSrcLen--;
		}
	}
 
    // Get 2 bytes each time and decode to 1 byte
    while (i < nSrcLen)
    {
		if (*pSrc!='\n' && *pSrc!='\r')
        {
            unByte = HextoBinTab[(*pSrc++) & 0x7F] << 4;
			unByte |= HextoBinTab[(*pSrc++) & 0x7F];

			*pDst++ = unByte;

			nDstLen++;
            i += 2;
        }
        else        // Skip CRLF
        {
			pSrc++;
            i++;
        }
     }

 
    return nDstLen;
}

int HexToBinW(const wchar_t *pSrc, unsigned char* pDst, int nSrcLen, int nDstLen)
{
	int nRstLen;            // Output length
	unsigned char unByte;   // Temp char for decode
    int i;
 
	if (nDstLen > 0)
	{
		i = 0;
		nRstLen = 0;

		if(nSrcLen > 2)
		{
			if((*pSrc == '0') && (*(pSrc + 1) == 'x' || *(pSrc + 1) == 'X'))
			{
				pSrc++;
				pSrc++;
				nSrcLen--;
				nSrcLen--;
			}
		}
	 
		// Get 2 characters each time and decode to 1 byte
		while (i < nSrcLen && nRstLen < nDstLen)
		{
			if (*pSrc != '\n' && *pSrc != '\r')
			{
				unByte = HextoBinTab[(*pSrc++) & 0x7F] << 4;
				unByte |= HextoBinTab[(*pSrc++) & 0x7F];

				*pDst++ = unByte;

				nRstLen++;
				i += 2;
			}
			else        // Skip CRLF
			{
				pSrc++;
				i++;
			}
		}

		return nRstLen;
	}
	else
	{
		return GetHexToBinDesLenW(pSrc, nSrcLen);
	}
}