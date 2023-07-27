//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	Algorithm
//	版本:		1.0
//	创建日期:	2008-2-23
//	作者:		荣强
 //	主要功能:	几种常用的算法(BASE64, 快速匹配, CRC32)
//	模块描述:
//	主要函数:	
//**************************************************************** 
#include "stdafx.h"
#include "sys\stat.h"
#include "Algorithm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//*************************** QQTEA算法 ****************************
#include "QQ_Crypt.c"


//*************************** 快速匹配算法 *************************
Search::Search(const char p[MAXCHARS])
{
	InitSearch(p);
}

Search::Search()
{
}

void Search::InitSearch(const char *p)
{
	try
	{
		ASSERT(p);
		ASSERT(&p);
		int len = strlen(p);
		if(len>MAXCHARS-1)
		{
			WriteLog("dce", "InitSearch error StringLen Too Lager, len=%d, String=%s", len, p);
			len = 255;
		}
		
		BYTE idx;
		memset(patt,0,MAXCHARS);
		memcpy(patt,p,len);
		m = strlen(patt);
		for (int i = 0; i < MAXCHARS; i++)
			d[i] = m;
		
		for (int j = 0; j < m - 1; j++)
		{
			idx=patt[j];
			d[idx] = m - j - 1;
		}
	}
	catch (...)
	{
		WriteLog("dce", "InitSearch error len= %d, String=%s", strlen(p), p);
	}

}

int Search::find(const char* text)
{
	ASSERT(text);
	int n = strlen(text);
	if (m > n)
		return -1;
	return find(text, n);
}

int Search::find(const char* text,int n)
{
	try
	{
		unsigned char ch;
		ASSERT(text);
		if (m > n)
			return -1;
		else if (m < 1)
			return -1;
		int k = m - 1;
		while (k < n)
		{
			int j = m - 1;
			int i = k;
			while (j >= 0 && text[i] == patt[j])
			{
				j--;
				i--;
			}
			if (j == -1)
				return i + 1;
			ch = text[k];
			k += d[ch];
		}
	}
	catch (...)
	{
		WriteLog("dce", "find error");
	}
	return -1;

}

//*************************** CRC32算法 *************************
CCrc32::CCrc32()
{
	this->InitCRC32Table();
}

CCrc32::~CCrc32()
{

}

void CCrc32::InitCRC32Table()
{
int error=0;
	try
	{
		ULONG ulPolynomial = 0x04c11db7;
		
		// 256 values representing ASCII character codes.
		for(int i = 0xFF+1; i--; )
		{
error=1;
			m_crc32Table[i]=Reflect(i, 8) << 24;
			for (int j = 8; j--; )
				m_crc32Table[i] = (m_crc32Table[i] << 1) ^ (m_crc32Table[i] & (1 << 31) ? ulPolynomial : 0);
error=2;
			m_crc32Table[i] = Reflect(m_crc32Table[i], 32);
		}
error=3;
	}
	catch (...)
	{
		WriteLog("dce","InitCRC32Table error =%d",error);
	}
}

inline ULONG CCrc32::Reflect(ULONG ref, char ch)
{
	ULONG value(0);

	// Swap bit 0 for bit 7 , bit 1 for bit 6, etc.
	for(int i = 1; i <= ch; i++)
	{
		if(ref & 1)
			value |= 1 << (ch - i);
		ref >>= 1;
	}
	return value;
}

int CCrc32::GetCRC(LPCTSTR csData, DWORD dwSize)
{
	// Be sure to use unsigned variables,
	// because negative values introduce high bits
	// where zero bits are required.
	ULONG  crc(0xffffffff);
	int len;
	unsigned char* buffer;

	len = dwSize;
	buffer = (unsigned char*)csData;
	// Perform the algorithm on each character
	// in the string, using the lookup table values.
	while(len--)
		crc = (crc >> 8) ^ m_crc32Table[(crc & 0xFF) ^ *buffer++];

	// Exclusive OR the result with the beginning value.
	return crc^0xffffffff;
}
/*************************************************
  函数名称:      CCrc32::GetEmlCrc
  描述:            获得一个文件内容的crc校验值
  输入参数:        filename
                   文件名
  输出参数:        文件crc校验值串
  其它:            其它说明
*************************************************/
/*CString CCrc32::GetFileCrc(LPCTSTR filename)
{
/*	if (_access(filename, 0) != 0)
	{
		return "失败";
	}
	
	char *ptext;
	UINT flen;
	char buf[10*1024];
	CString str;

	flen=GetFileLength(filename);
	if (flen>sizeof(buf)-10)
	{
		ptext = str.GetBufferSetLength(flen+10);
		memset(ptext+flen,0,9);
	}
	else if (flen > 0)
	{
		ptext = buf;
		memset(ptext+flen,0,9);
	}
	else
		continue;
	
	FILE * fs=0;
	fs=fopen(filename,"rb");
	if(fs==0)
		continue;
	
	UINT readsize=0;
	readsize=fread(ptext,1,flen,fs);
	fclose(fs);
	DWORD dword_filecrc = GetCRC(ptext, flen);
	CString rescrc;
	rescrc.Format("%d",dword_filecrc);
	return rescrc;
}*/
/*************************************************
  函数名称:      CCrc32::GetEmlCrc
  描述:            获得一个邮件内容的crc校验值
  输入参数:        filename
                   文件名
  输出参数:        文件crc校验值串
  其它:            其它说明
*************************************************/
CString CCrc32::GetEmlCrc(LPCTSTR filename)
{
	int error=0;
	try
	{
		CString strhead,strbody;
error=1;
		SplicEmail(filename, strhead, strbody);
		int BodyRcr32;
		if (strbody.IsEmpty())
		{
			BodyRcr32 = rand();
		}
		else
		{
error=2;
			BodyRcr32 = GetCRC(strbody, strbody.GetLength());
		}
error=3;
		CString rescrc;
		rescrc.Format("%08X", BodyRcr32);
		return rescrc;
	}
	catch (...)
	{
		WriteLog("dce","GetEmlCrc error=%d",error);
	}
	return "abcdefgh";
}

/*************************************************
  函数名称:      CCrc32::SplicEmail
  描述:            拆分出邮件的内容部分
  输入参数:        filename，strhead， strbody
                   文件名  头部分内容  内部部分
  输出参数:        成功返回true
  其它:            其它说明
*************************************************/

BOOL CCrc32::SplicEmail(const char *emlFile, CString &strHead, CString &strBody)
{
	UINT flen;
	int pos = -1;
	char* p = 0;
	CString strheader;
	try
	{
		struct _stat buf;
		_stat(emlFile, &buf);
		flen = buf.st_size;
		if(flen > 10 * 1024 * 1024)
		{
			flen = 10 * 1024 * 1024;
		}

		if (flen > 0)
		{
			p = strheader.GetBufferSetLength(flen + 8);
		}
		else
		{
			return FALSE;
		}

		memset(p + flen, 0, 8);
		FILE * fs=0;
		fs=fopen(emlFile, "rb");
		if(fs != NULL)
		{
			UINT readsize = 0;
			readsize = fread(p, 1, flen, fs);
			fclose(fs);
		}
			
		pos = Search2CRLF.find(p);
		if(pos<0)
		{
			pos=Search2CRLFUnix.find(p);
		}
		if(pos<0)
		{
			return FALSE;
		}
		strBody.Format("%s", p + pos + 4);
		strheader.ReleaseBuffer(pos);
		strHead = strheader;
	}
	catch (...)
	{
		strBody.Empty();
		strHead.Empty();
		return FALSE;
	}
	return TRUE;
}


//*************************** BASE64算法 *************************
//编码时候的每行的长度
unsigned int CMailCoder::m_LineWidth = 76;
//------------------------------------------------------------------------------

//Base64解码表
const char CMailCoder::BASE64_ENCODE_TABLE[64] = {
	 65,  66,  67,  68,  69,  70,  71,  72,  // 00 - 07
	 73,  74,  75,  76,  77,  78,  79,  80,  // 08 - 15
	 81,  82,  83,  84,  85,  86,  87,  88,  // 16 - 23
	 89,  90,  97,  98,  99, 100, 101, 102,  // 24 - 31
	103, 104, 105, 106, 107, 108, 109, 110,  // 32 - 39
	111, 112, 113, 114, 115, 116, 117, 118,  // 40 - 47
	119, 120, 121, 122,  48,  49,  50,  51,  // 48 - 55
	 52,  53,  54,  55,  56,  57,  43,  47 };// 56 - 63
	
//Base64编码表
const unsigned int CMailCoder::BASE64_DECODE_TABLE[256] = {
	255, 255, 255, 255, 255, 255, 255, 255, //  00 -  07
	255, 255, 255, 255, 255, 255, 255, 255, //  08 -  15
	255, 255, 255, 255, 255, 255, 255, 255, //  16 -  23
	255, 255, 255, 255, 255, 255, 255, 255, //  24 -  31
	255, 255, 255, 255, 255, 255, 255, 255, //  32 -  39
	255, 255, 255,  62, 255, 255, 255,  63, //  40 -  47
	 52,  53,  54,  55,  56,  57,  58,  59, //  48 -  55
	 60,  61, 255, 255, 255, 255, 255, 255, //  56 -  63
	255,   0,   1,   2,   3,   4,   5,   6, //  64 -  71
	  7,   8,   9,  10,  11,  12,  13,  14, //  72 -  79
	 15,  16,  17,  18,  19,  20,  21,  22, //  80 -  87
	 23,  24,  25, 255, 255, 255, 255, 255, //  88 -  95
	255,  26,  27,  28,  29,  30,  31,  32, //  96 - 103
	 33,  34,  35,  36,  37,  38,  39,  40, // 104 - 111
	 41,  42,  43,  44,  45,  46,  47,  48, // 112 - 119
	 49,  50,  51, 255, 255, 255, 255, 255, // 120 - 127
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255 };
//------------------------------------------------------------------------------
		
//Quoted编码表
const unsigned char CMailCoder::QUOTED_ENCODE_TABLE[256] = {
	255, 255, 255, 255, 255, 255, 255, 255, // 00 -  07
	255, 255,  10, 255, 255,  13, 255, 255, // 08 - 15
	255, 255, 255, 255, 255, 255, 255, 255, // 16 - 23
	255, 255, 255, 255, 255, 255, 255, 255, // 24 - 31
	255,  33,  34,  35,  36,  37,  38,  39,
	 40,  41,  42,  43,  44,  45,  46,  47,
	 48,  49,  50,  51,  52,  53,  54,  55,
	 56,  57,  58,  59,  60, 255,  62,  63,
	 64,  65,  66,  67,  68,  69,  70,  71,
	 72,  73,  74,  75,  76,  77,  78,  79,
	 80,  81,  82,  83,  84,  85,  86,  87,
	 88,  89,  90,  91,  92,  93,  94,  95,
	 96,  97,  98,  99, 100, 101, 102, 103,
	104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119,
	120, 121, 122, 123, 124, 125, 126, 127,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255 };
//------------------------------------------------------------------------------
			
//获取Base64编码长度
int CMailCoder::Base64EncodeSize(int iSize)
{
	int nSize, nCR;
	nSize = (iSize + 2) / 3 * 4 ;
	nCR = nSize / m_LineWidth; //计算回车数量
	nSize+= nCR * 2;
	return nSize;
}
	
//------------------------------------------------------------------------------
	
//获取Base64解码长度
int CMailCoder::Base64DecodeSize(int iSize)
{
	return (iSize + 3) / 4 * 3;
}

//------------------------------------------------------------------------------

//获取UUCode编码长度
int CMailCoder::UUEncodeSize(int iSize)
{
	int nSize, nCR;
	nSize = (iSize + 2) / 3 * 4 ;
	nCR = nSize / m_LineWidth + 2; //计算回车数量
	nSize+= nCR * 3 + 2;
	return nSize;
}
			
//------------------------------------------------------------------------------

//获取UUCode解码长度
int CMailCoder::UUDecodeSize(int iSize)
{
	return (iSize + 3) / 4 * 3;
}
			
//------------------------------------------------------------------------------

//获取Quoted编码长度
int CMailCoder::QuotedEncodeSize(int iSize)
{
	int nSize = iSize * 3 + 1;
	int nCR = nSize / m_LineWidth;
	nSize+= nCR * 3;
	return nSize;
}

//------------------------------------------------------------------------------

//Base64编码
int CMailCoder::base64_encode(char *pSrc, unsigned int nSize, char *pDest)
{
int error=0;
	try
	{
		if ((pSrc == NULL) || (nSize <= 0)) return 0;
			
		unsigned int iB, iInMax3, Len;
		char *pInPtr, *pInLimitPtr;
		char *OutPtr ;
error=1;				
		pInPtr = pSrc;
		iInMax3 = nSize / 3 * 3;
		OutPtr = pDest;
		pInLimitPtr = pInPtr + iInMax3;
error=2;					
		while (pInPtr != pInLimitPtr)
		{
			Len = 0;
			while ((Len < m_LineWidth) && (pInPtr != pInLimitPtr))
			{
				iB = (unsigned char) *pInPtr++;
				iB = iB << 8;
					
				iB = iB | (unsigned char) *pInPtr++;
				iB = iB << 8;
							
				iB = iB | (unsigned char) *pInPtr++;
							
				//以4 byte倒序写入输出缓冲
				OutPtr[3] = BASE64_ENCODE_TABLE[iB & 0x3F];
				iB = iB >> 6;
				OutPtr[2] = BASE64_ENCODE_TABLE[iB & 0x3F];
				iB = iB >> 6;
				OutPtr[1] = BASE64_ENCODE_TABLE[iB & 0x3F];
				iB = iB >> 6;
				OutPtr[0] = BASE64_ENCODE_TABLE[iB];
				OutPtr+=4;
				Len+=4;
			}
			if (Len >= m_LineWidth)
			{
				*OutPtr++ = '\r'; //加上回车换行符
				*OutPtr++ = '\n';
			}
		}
error=3;
		//设置尾部
		switch (nSize - iInMax3)
		{
			case 1:
				iB = (unsigned char) *pInPtr;
				iB = iB << 4;
				OutPtr[1] = BASE64_ENCODE_TABLE[iB & 0x3F];
				iB = iB >> 6;
				OutPtr[0] = BASE64_ENCODE_TABLE[iB];
				OutPtr[2] = '='; //用'='也就是64码填充剩余部分
				OutPtr[3] = '=';
				OutPtr+=4;
				break;
			case 2:
				iB = (unsigned char) *pInPtr++;
				iB = iB << 8;
				iB = iB | (unsigned char) *pInPtr;
				iB = iB << 2;
				OutPtr[2] = BASE64_ENCODE_TABLE[iB & 0x3F];
				iB = iB >> 6;
				OutPtr[1] = BASE64_ENCODE_TABLE[iB & 0x3F];
				iB = iB >> 6;
				OutPtr[0] = BASE64_ENCODE_TABLE[iB];
				OutPtr[3] = '='; // Fill remaining byte.
				OutPtr+=4;
				break;
		}
error=4;
		return (unsigned int) (OutPtr - pDest);
	}
	catch (...)
	{
		WriteLog("dce","base64_encode error =%d",error);
	}
}
			
//------------------------------------------------------------------------------
//Base64解码

int CMailCoder::base64_decode(char *pSrc, unsigned int nSize, char *pDest)
{
	if ((pSrc == NULL) || (pDest == NULL) || (nSize <= 0)) return 0;
			
	unsigned int lByteBuffer, lByteBufferSpace;
	unsigned int C; //临时阅读变量
	int reallen;
	char *InPtr, *InLimitPtr;
	char *OutPtr;
				
	lByteBuffer = 0; lByteBufferSpace = 4;
				
	InPtr = pSrc;
	InLimitPtr= InPtr + nSize;
	OutPtr = pDest;
				
	while (InPtr != InLimitPtr)
	{
		C = BASE64_DECODE_TABLE[*InPtr]; // Read from InputBuffer.
		InPtr++;
		if (C == 0xFF) continue; //读到255非法字符
		lByteBuffer = lByteBuffer << 6 ;
		lByteBuffer = lByteBuffer | C ;
		lByteBufferSpace--;
		if (lByteBufferSpace != 0) continue; //一次读入4个字节
		//到序写入3个字节到缓冲
		OutPtr[2] = lByteBuffer;
		lByteBuffer = lByteBuffer >> 8;
		OutPtr[1] = lByteBuffer;
		lByteBuffer = lByteBuffer >> 8;
		OutPtr[0] = lByteBuffer;
		//准备写入后3位
		OutPtr+= 3; lByteBuffer = 0; lByteBufferSpace = 4;
	}
	reallen = (unsigned int)OutPtr - (unsigned int)pDest;
	//处理尾部 返回实际长度
	switch (lByteBufferSpace)
	{
		case 1:
			lByteBuffer = lByteBuffer >> 2;
			OutPtr[1] = lByteBuffer;
			lByteBuffer = lByteBuffer >> 8;
			OutPtr[0] = lByteBuffer;
			return reallen + 2;
		case 2:
			lByteBuffer = lByteBuffer >> 4;
			OutPtr[0] = lByteBuffer;
			return reallen + 1;
		default:
			return reallen;
	}
}

//------------------------------------------------------------------------------

//UUCode编码
int CMailCoder::UU_encode(char *pSrc, unsigned int nSize, char *pDest)
{
	if ((pSrc == NULL) || (pDest == NULL) || (nSize <= 0)) return 0;
				
	unsigned int lByteBufferSpace, Len;
	unsigned char B[3]; //临时阅读变量
	char *InPtr, *InLimitPtr;
	char *OutPtr;
			
	InPtr = pSrc;
	InLimitPtr= InPtr + nSize;
	OutPtr = pDest;
				
	while (InPtr < InLimitPtr)
	{
		Len = 0;
					
		while ((InPtr < InLimitPtr) && (Len < m_LineWidth))
		{
			lByteBufferSpace = (unsigned int) InLimitPtr - (unsigned int) InPtr;
			if (lByteBufferSpace > 3) lByteBufferSpace = 3; //设置步长
			//取值
			for (unsigned int i = 0; i < lByteBufferSpace; i++ )
			{
				B[i] = *InPtr++;
			}
			if (Len == 0)
			{
				*OutPtr++ = 'M';
				Len++;
			}
			//编码运算
			OutPtr[0] = B[0] >> 2;
			OutPtr[1] = (unsigned int) (B[0] << 4 & 0x30) + (unsigned int) (B[1] >> 4 & 0x0F);
			OutPtr[2] = (unsigned int) (B[1] << 2 & 0x3C) + (unsigned int) (B[2] >> 6 & 0x03);
			OutPtr[3] = B[2] & 0x3F;
			for (i = 0; i < 4; i++)
			{
				if (OutPtr[i] == NULL)
					OutPtr[i] = '`';//(unsigned char) (OutPtr[i] + 0x60);
				else 
					OutPtr[i] = ' ' + OutPtr[i];//(unsigned char) (OutPtr[i] + 0x20);
			}	
			OutPtr+=4;
			Len+=4;
		}
					
		*OutPtr++ = '\r'; //设置回车
		*OutPtr++ = '\n';
	}
	return (unsigned int) (OutPtr - pDest);
}

//------------------------------------------------------------------------------

//UUCode解码
int CMailCoder::UU_decode(char *pSrc, unsigned int nSize, char *pDest)
{
	char C[4]; //临时阅读变量
	char Tmp;
	int CurrIndex, Index;
	char *InPtr, *InLimitPtr;
	char *OutPtr;
		
	if ((pSrc == NULL) || (pDest == NULL) || (nSize <= 0)) return 0;
		
	CurrIndex = 0;
				
	InPtr = pSrc;
	InLimitPtr= InPtr + nSize;
	OutPtr = pDest;
				
	while (InPtr != InLimitPtr) //读取4个字符
	{
		memset(C, 0, sizeof(C));
		Index = 0;
		do
		{
			Tmp = *InPtr++;
			if (Tmp == 0x60) 
			{
				Tmp = 0x20;       //为了兼容OutLook Express
			}
			else if (Tmp =='\r')  //首个字母不处理
			{
				InPtr++;
				CurrIndex = 0;
			}
			
			if ((Tmp > 0x20) || ( CurrIndex > 0 ))
			{
				Tmp = Tmp - 0x20;
				if (CurrIndex == 0) 
				{
					CurrIndex = Tmp;
				}
				else
				{
					C[Index] = Tmp;  // 向数组中追加字符
					Index++;
				}
			}
		}
		while ((InPtr < InLimitPtr) && (Index < sizeof(C)));
					
		OutPtr[0] = (char) ((C[0] << 2) + (C[1] >> 4));
		OutPtr[1] = (char) ((C[1] << 4) + (C[2] >> 2));
		OutPtr[2] = (char) ((C[2] << 6) + C[3]);
		OutPtr+=3; //设置起始位置
	}
	return (unsigned int)OutPtr - (unsigned int)pDest;
}

//------------------------------------------------------------------------------

//Quoted编码
int CMailCoder::quoted_encode(char *pSrc, unsigned int nSize, char *pDest)
{
	unsigned int Len;
	unsigned char B; //临时阅读变量
	char *InPtr, *InLimitPtr;
	char *OutPtr;
			
	if ((pSrc == NULL) || (pDest == NULL) || (nSize <= 0)) return 0;
			
	InPtr = pSrc;
	InLimitPtr= InPtr + nSize;
	OutPtr = pDest;
				
	while (InPtr < InLimitPtr)
	{
		Len = 0;
		while ((InPtr < InLimitPtr) && (Len < m_LineWidth))
		{
			B = *InPtr++;
			if (QUOTED_ENCODE_TABLE[ B ]  == 255)
			{
				*OutPtr++ = '=';
				*OutPtr++ = DecToHex(B >> 4);
				*OutPtr++ = DecToHex(B & 0x0f);
				Len+=3;
			}
			else
			{
				*OutPtr++ = B;
				Len++;
			}
		}
		if (Len >= m_LineWidth)
		{
			*OutPtr++ = '=';
			*OutPtr++ = '\r'; //设置新行
			*OutPtr++ = '\n';
		}
	}
	*OutPtr = '\0';
	return (unsigned int) (OutPtr - pDest);
}

//------------------------------------------------------------------------------

//Quoted解码
int CMailCoder::quoted_decode(char *pSrc, unsigned int nSize, char *pDest)
{
	if ((pSrc == NULL) || (pDest == NULL) || (nSize <= 0)) return 0;
				
	unsigned char nA, nB;
	char C[2]; //临时阅读变量
				
	char *InLimitPtr= pSrc + nSize;
	char *pDestOrg = pDest;
				
	while (pSrc < InLimitPtr)
	{
		C[0] = *pSrc++; //先取第一字符
		if (C[0] == '=') //如果后面是经过编码的
		{
			C[0] = *pSrc++;  //取出两个编码字符
			C[1] = *pSrc++;

			if (C[0] != '\r')
			{
				nA = HexToDec(C[0]); //解码变换
				nB = HexToDec(C[1]);
				*pDest++ = (nA << 4) + nB;
			}
		}
		else //否则直接输出
		{
			*pDest++ = C[0];
		}
	}
	return (unsigned int)(pDest - pDestOrg);
}

//------------------------------------------------------------------------------

//十进制-->十六进制
unsigned char CMailCoder::DecToHex( unsigned char B)
{
	return B < 10 ? '0' + B:'A' + B - 10;
}

//------------------------------------------------------------------------------

//十六进制-->十进制
unsigned char CMailCoder::HexToDec( unsigned char C )
{
	return C <= '9' ? C - '0' : C - 'A' + 10;
}

//------------------------------------------------------------------------------
//16进制字符串转换成10进制 如61 61-》aa

CString hexstrtoint10str(const char *s, char *outbuf)
{
	const char* p = s;
	int len = strlen(s);
	CString res;
	char* tp = outbuf; 
	int i = 0;
	int hh = 0;
	while (i < len)
	{
		int temp;
		char tmpchar[4];
		memset(tmpchar, 0, 4);
		memcpy(tmpchar, p+i, 2);
		sscanf(tmpchar, "%x", &temp);
//  		CString overcver;
//  		overcver.Format("%c", temp);
// 		char overcver[2];
// 		sprintf(overcver,"%c",temp);
// 		CString aaa;//不定义此句话时返回值是乱码
// 		memcpy(tp+hh, (char*)overcver, 1);
		tp[hh] = temp;
		i+=2;
		hh++;
	}

	return "ok";
}

//字符转换成16进制，如 aa-》61 61

CString int10str_to_hexstr(const char *s,int len)
{
	const char* p = s;
	if (len == 0)
	{
		len = strlen(s);
	}
	CString res;
	char* tp = res.GetBufferSetLength(len*2); 
	int i = 0;
	while (i < len)
	{
		CString converstr;
		//02x是为了处理\8之类的转换后只有一个8
		//只取两位是为了汉字%02x之后有8字节，只取最后的
		converstr.Format("%02x",*(p+i));
		converstr = converstr.Right(2);
		memcpy(tp+2*i,(char*)(LPCTSTR)converstr,2);
//		memcpy(tp+4*i+2,"  ",2);
		i++;
	}
	return res;
}