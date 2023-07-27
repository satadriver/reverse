#include <StdAfx.h>
#include "VoipUrl.h"		//注意不能写成这种格式#include<UrcCode.h>，否则C++报错
#include "winnls.h"
#include <..\\reverse\\zlib\\zlib.h>
#pragma comment(lib,"..\\reverse\\zlib\\zlib.lib")

#include <windows.h>
#include "stdio.h"
#include <sstream>

//原理: 1.'a---z','A---Z','0---9' keep
//2. ' '---> '+'
//3. '.','_','-','*' keep
//4. char others make change like "%02X"
//调用此函数要注意字符参数必须是以0结尾的字符串,长度也应该稍大
char * DecodeUrl(char * UrlCodeStr)
{
	size_t SrcPtr=0;
	size_t DstPtr=0;
	size_t Counter=0;
	unsigned char CharLow;
	unsigned char CharHigh;

	do
	{
		Counter=0;
		size_t StringLenth=strlen(UrlCodeStr);
		for( SrcPtr=0,DstPtr=0; SrcPtr<StringLenth;  )
		{
			if(UrlCodeStr[SrcPtr]== '%')
			{
				if( isxdigit(UrlCodeStr[SrcPtr+1]) && isxdigit(UrlCodeStr[SrcPtr+2]) )
				{ 
					if(  ( (UrlCodeStr[SrcPtr+1] >='a') && (UrlCodeStr[SrcPtr+1]<= 'z') ) ||	//url编码后必须为大写
						 ( (UrlCodeStr[SrcPtr+2] >='a') && (UrlCodeStr[SrcPtr+2]<= 'z') )  )
					{
						UrlCodeStr[DstPtr] = UrlCodeStr[SrcPtr];
						SrcPtr++;
						DstPtr++;	
					}
					else
					{
						CharHigh = UrlCodeStr[SrcPtr+1];
						CharLow  = UrlCodeStr[SrcPtr+2];
						CharHigh = CharHigh - 0x30 - ( (CharHigh >= 'A') ? 7 : 0);
						CharLow  = CharLow -  0x30 - ( (CharLow  >= 'A') ? 7 : 0);
						UrlCodeStr[DstPtr] = ((CharHigh<<4)+CharLow);
						SrcPtr = SrcPtr+3;
						DstPtr ++;
						Counter ++;	
					}
				}
				else
				{
					UrlCodeStr[DstPtr] = UrlCodeStr[SrcPtr];
					SrcPtr++;
					DstPtr++;
				}
			}
			else if(UrlCodeStr[SrcPtr] == '+')
			{
				UrlCodeStr[DstPtr]= ' ';
				SrcPtr ++;
				DstPtr ++;
				Counter ++;
			}
			else
			{
				UrlCodeStr[DstPtr] = UrlCodeStr[SrcPtr];
				SrcPtr++;
				DstPtr++;
			}
		}
		UrlCodeStr[DstPtr]=0;
	}
	while(Counter);

	return UrlCodeStr;
}
	



//u81c7/u8246这样的是UNICODE编码
char * Decode_UrlUnicode(char * UrlUnicodeCodeStr)
{
	size_t SrcPtr	= 0;
	size_t DstPtr	= 0;
	size_t Counter	= 0;
	char CharHigh	= 0;
	char CharLow	= 0;
	char TmpShort[4] = {0};
	
	do
	{
		Counter=0;
		size_t StringLenth=strlen(UrlUnicodeCodeStr);

		for( SrcPtr=0,DstPtr=0; SrcPtr<StringLenth;  )
		{
			if( (UrlUnicodeCodeStr[SrcPtr]== '%') && (UrlUnicodeCodeStr[SrcPtr + 1] == 'u') )
			{
				if( isxdigit(UrlUnicodeCodeStr[SrcPtr + 2]) && isxdigit(UrlUnicodeCodeStr[SrcPtr + 3]) &&
					isxdigit(UrlUnicodeCodeStr[SrcPtr + 4]) && isxdigit(UrlUnicodeCodeStr[SrcPtr + 5]) )
				{ 
					if( ( (UrlUnicodeCodeStr[SrcPtr+2] >='a') && (UrlUnicodeCodeStr[SrcPtr+2]<= 'z') ) ||	//url编码后必须为大写
						( (UrlUnicodeCodeStr[SrcPtr+3] >='a') && (UrlUnicodeCodeStr[SrcPtr+3]<= 'z') ) ||
						( (UrlUnicodeCodeStr[SrcPtr+4] >='a') && (UrlUnicodeCodeStr[SrcPtr+4]<= 'z') ) ||
						( (UrlUnicodeCodeStr[SrcPtr+5] >='a') && (UrlUnicodeCodeStr[SrcPtr+5]<= 'z') )  )
					{
						UrlUnicodeCodeStr[DstPtr] = UrlUnicodeCodeStr[SrcPtr];
						SrcPtr++;
						DstPtr++;
					}
					else
					{
						*(unsigned int*)TmpShort = 0;
						CharHigh = UrlUnicodeCodeStr[SrcPtr+2];
						CharLow  = UrlUnicodeCodeStr[SrcPtr+3];
						CharHigh = CharHigh - 0x30 - ( (CharHigh >= 'A') ? 7 : 0);
						CharLow  = CharLow -  0x30 - ( (CharLow  >= 'A') ? 7 : 0);

						TmpShort[1] = (CharHigh<<4)+CharLow;	//转换成16进制数后要交换两个字节的顺序
						SrcPtr = SrcPtr+4;
						DstPtr ++;

						CharHigh = UrlUnicodeCodeStr[SrcPtr];
						CharLow  = UrlUnicodeCodeStr[SrcPtr+1];
						CharHigh = CharHigh - 0x30 - ( (CharHigh >= 'A') ? 7 : 0);
						CharLow  = CharLow -  0x30 - ( (CharLow  >= 'A') ? 7 : 0);

						TmpShort[0] = (CharHigh<<4)+CharLow;	//转换成16进制数后要交换两个字节的顺序
						SrcPtr = SrcPtr+2;
						DstPtr ++;
						Counter ++;	

						//WideCharToMultiByte输入和输出缓冲区不能相同
						DWORD Result=WideCharToMultiByte(CP_ACP,NULL,(unsigned short*)TmpShort,1,UrlUnicodeCodeStr + DstPtr - 2,
							2,NULL,NULL);	
						if(Result!=2)
						{
//							printf("WideCharToMultiByte Error!\n");
						}
					}
				}
				else
				{
					UrlUnicodeCodeStr[DstPtr] = UrlUnicodeCodeStr[SrcPtr];
					SrcPtr++;
					DstPtr++;
				}
			}
			else if(UrlUnicodeCodeStr[SrcPtr] == '+')
			{
				UrlUnicodeCodeStr[DstPtr]= ' ';
				SrcPtr ++;
				DstPtr ++;
				Counter ++;
			}
			else
			{
				UrlUnicodeCodeStr[DstPtr] = UrlUnicodeCodeStr[SrcPtr];
				SrcPtr++;
				DstPtr++;
			}
		}
		UrlUnicodeCodeStr[DstPtr]=0;
	}
	while(Counter);
	
	return UrlUnicodeCodeStr;
}






// ERROR_INSUFFICIENT_BUFFER
// ERROR_INVALID_FLAGS
// ERROR_INVALID_PARAMETER




//原理：
//1B: 0xxxxxxx		//不可能有大于0X80的单字节UTF-8字符
//2B: 110xxxxx,10xxxxxx
//3B: 1110xxxx,10xxxxxx,10xxxxxx
//4B: 11110xxx,10xxxxxx,10xxxxxx, 10xxxxxx	只是理论上有，现在无规定
char * Utf8ToGB2312(char * Utf8String)
{
	size_t			SrcPtr=0;
	size_t			DstPtr=0;
	unsigned char	CharLow = 0;										//C language bit operation operand must be unsigned 
	unsigned char	CharHigh = 0;
	unsigned char	CharMid = 0;
	unsigned char	FlagChar = 0;
	unsigned short	WideChar[2] = {0};

	for ( SrcPtr=0,DstPtr=0; SrcPtr < strlen(Utf8String); )
	{
		if( Utf8String[SrcPtr] < 0 )								//signed char can be plus or negtive
		{
			FlagChar = ( (unsigned char)Utf8String[SrcPtr] & 0xf0);
			if(FlagChar == 0xe0)									//所有汉字都是3个字节
			{
				CharHigh = Utf8String[SrcPtr]		& 0xf;
				CharMid  = Utf8String[SrcPtr+1]		& 0x3f;
				CharLow  = Utf8String[SrcPtr+2]		& 0x3f;
				WideChar[1] = CharMid;
				WideChar[1] = (WideChar[1]<<6) | CharLow;
				WideChar[0] = CharHigh;
				WideChar[0] = (WideChar[0] << 12 ) | WideChar[1] ;	
				//该函数转换指定的字节数，不返回结尾0,结果放在ccMultiBytes中				
				DWORD Result=WideCharToMultiByte(CP_ACP,NULL,WideChar,1,Utf8String+DstPtr,sizeof(wchar_t),NULL,NULL);
				if(Result!=sizeof(wchar_t))
				{
//					printf("3B WideCharToMultiByte Error,ErrorCode is:%d!\n",GetLastError());
				}

				SrcPtr = SrcPtr +3;
				DstPtr = DstPtr +2;
			}
			else if(FlagChar == 0xc0)
			{
				CharHigh	= Utf8String[SrcPtr]	& 0x1f;
				CharLow		= Utf8String[SrcPtr+1]	& 0x3f;
				WideChar[0]	= CharHigh;
				WideChar[0]	= (WideChar[0] <<6) | CharLow ;	//移位运算符比或运算等级高
				//注意此处宽字符的长度为1时不是说是一个字节，表示的是1个宽字符的长度
				DWORD Result=WideCharToMultiByte(CP_ACP,NULL,WideChar,1,Utf8String+DstPtr,sizeof(wchar_t),NULL,NULL);
				if(Result!=sizeof(wchar_t))
				{
//					printf("2B WideCharToMultiByte Error,ErrorCode is:%d!\n",GetLastError());
				}
				SrcPtr = SrcPtr +2;
				DstPtr = DstPtr +2;
			}
			else if(FlagChar== 0xf0)				//没有规定，不可能有四个4字节的UTF-8字符
			{
				unsigned char CharMostHigh = Utf8String[SrcPtr] & 7;
				CharHigh	= Utf8String[SrcPtr+1]	& 0x3f;
				CharMid		= Utf8String[SrcPtr+2]	& 0x3f;
				CharLow		= Utf8String[SrcPtr+3]	& 0x3f;
				unsigned long BigUtf8 = CharMostHigh;
				WideChar[0] = CharHigh;
				WideChar[1] = CharMid;
				WideChar[1] = (WideChar[1]<< 6) | CharLow;
				WideChar[0] = (WideChar[0] << 12) | WideChar[1] ;
				BigUtf8 = (BigUtf8 << 18) | WideChar[0];
				int Result=WideCharToMultiByte(CP_ACP,0,(wchar_t*)&BigUtf8,1,Utf8String+DstPtr,sizeof(BigUtf8),0,0);
				if(Result!= sizeof(BigUtf8))
				{
//					printf("4B WideCharToMultiByte Error,ErrorCode is:%d!\n",GetLastError());
				}
				Utf8String[DstPtr] = Utf8String[SrcPtr];
				SrcPtr =SrcPtr + 4;
				DstPtr =DstPtr + 2;
			}
			else if(FlagChar == 0x80)						//不可能有1字节的大于80的UTF-8字符
			{
				Utf8String[DstPtr] = Utf8String[SrcPtr];
//				printf("1B WideCharToMultiByte Error,ErrorCode is:%d!\n",GetLastError());
				SrcPtr =SrcPtr ++;
				DstPtr =DstPtr ++;
			}
			else
			{
				Utf8String[DstPtr] = Utf8String[SrcPtr];	//正字符说明是一个ASCII字节
				SrcPtr++;
				DstPtr++;			
			}
		}
		else
		{
			Utf8String[DstPtr]=Utf8String[SrcPtr];
			SrcPtr++;
			DstPtr++;
		}
	}

	Utf8String[DstPtr]=0;
	return Utf8String;
}





int Utf8ToGB2312(char * Utf8String, int Lenth)
{
	size_t			SrcPtr=0;
	size_t			DstPtr=0;
	unsigned char	CharLow = 0;										//C language bit operation operand must be unsigned 
	unsigned char	CharHigh = 0;
	unsigned char	CharMid = 0;
	unsigned char	FlagChar = 0;
	unsigned short	WideChar[2] = {0};
	
	for ( SrcPtr=0,DstPtr=0; SrcPtr < Lenth; )
	{
		if( Utf8String[SrcPtr] < 0 )								//signed char can be plus or negtive
		{
			FlagChar = ( (unsigned char)Utf8String[SrcPtr] & 0xf0);
			if(FlagChar == 0xe0)									//所有汉字都是3个字节
			{
				CharHigh = Utf8String[SrcPtr]		& 0xf;
				CharMid  = Utf8String[SrcPtr+1]		& 0x3f;
				CharLow  = Utf8String[SrcPtr+2]		& 0x3f;
				WideChar[1] = CharMid;
				WideChar[1] = (WideChar[1]<<6) | CharLow;
				WideChar[0] = CharHigh;
				WideChar[0] = (WideChar[0] << 12 ) | WideChar[1] ;	
				//该函数转换指定的字节数，不返回结尾0,结果放在ccMultiBytes中				
				DWORD Result=WideCharToMultiByte(CP_ACP,NULL,WideChar,1,Utf8String+DstPtr,sizeof(wchar_t),NULL,NULL);
				if(Result!=sizeof(wchar_t))
				{
//					printf("3B WideCharToMultiByte Error,ErrorCode is:%d!\n",GetLastError());
				}
				
				SrcPtr = SrcPtr +3;
				DstPtr = DstPtr +2;
			}
			else if(FlagChar == 0xc0)
			{
				CharHigh	= Utf8String[SrcPtr]	& 0x1f;
				CharLow		= Utf8String[SrcPtr+1]	& 0x3f;
				WideChar[0]	= CharHigh;
				WideChar[0]	= (WideChar[0] <<6) | CharLow ;	//移位运算符比或运算等级高
				//注意此处宽字符的长度为1时不是说是一个字节，表示的是1个宽字符的长度
				DWORD Result=WideCharToMultiByte(CP_ACP,NULL,WideChar,1,Utf8String+DstPtr,sizeof(wchar_t),NULL,NULL);
				if(Result!=sizeof(wchar_t))
				{
//					printf("2B WideCharToMultiByte Error,ErrorCode is:%d!\n",GetLastError());
				}
				SrcPtr = SrcPtr +2;
				DstPtr = DstPtr +2;
			}
			else if(FlagChar== 0xf0)				//没有规定，不可能有四个4字节的UTF-8字符
			{
				unsigned char CharMostHigh = Utf8String[SrcPtr] & 7;
				CharHigh	= Utf8String[SrcPtr+1]	& 0x3f;
				CharMid		= Utf8String[SrcPtr+2]	& 0x3f;
				CharLow		= Utf8String[SrcPtr+3]	& 0x3f;
				unsigned long BigUtf8 = CharMostHigh;
				WideChar[0] = CharHigh;
				WideChar[1] = CharMid;
				WideChar[1] = (WideChar[1]<< 6) | CharLow;
				WideChar[0] = (WideChar[0] << 12) | WideChar[1] ;
				BigUtf8 = (BigUtf8 << 18) | WideChar[0];
				int Result=WideCharToMultiByte(CP_ACP,0,(wchar_t*)&BigUtf8,1,Utf8String+DstPtr,sizeof(BigUtf8),0,0);
				if(Result!= sizeof(BigUtf8))
				{
//					printf("4B WideCharToMultiByte Error,ErrorCode is:%d!\n",GetLastError());
				}
				Utf8String[DstPtr] = Utf8String[SrcPtr];
				SrcPtr =SrcPtr + 4;
				DstPtr =DstPtr + 2;
			}
			else if(FlagChar == 0x80)						//不可能有1字节的大于80的UTF-8字符
			{
				Utf8String[DstPtr] = Utf8String[SrcPtr];
//				printf("1B WideCharToMultiByte Error,ErrorCode is:%d!\n",GetLastError());
				SrcPtr =SrcPtr ++;
				DstPtr =DstPtr ++;
			}
			else
			{
				Utf8String[DstPtr] = Utf8String[SrcPtr];	//正字符说明是一个ASCII字节
				SrcPtr++;
				DstPtr++;			
			}
		}
		else
		{
			Utf8String[DstPtr]=Utf8String[SrcPtr];
			SrcPtr++;
			DstPtr++;
		}
	}
	
	Utf8String[DstPtr]=0;
	return DstPtr;
}






int GzipDecode(char *source,int len,char **dest,int gzip) 
{ 
	int ret; 
	unsigned have; 
	z_stream strm; 
	unsigned char out[1024]; 
	int totalsize = 0; 
	
	/* allocate inflate state */ 
	strm.zalloc		= Z_NULL; 
	strm.zfree		= Z_NULL; 
	strm.opaque		= Z_NULL; 
	strm.avail_in	= 0; 
	strm.next_in	= Z_NULL; 
	
	if(gzip) 
		ret = inflateInit2(&strm, 47); 
	else 
		ret = inflateInit(&strm); 
	
	if (ret != Z_OK) 
		return ret; 
	
	strm.avail_in = len; 
	strm.next_in = (unsigned char *)source; 
	
	/* run inflate() on input until output buffer not full */ 
	do { 
		strm.avail_out = 1024; 
		strm.next_out = out; 
		ret = inflate(&strm, Z_NO_FLUSH); 
//		assert(ret != Z_STREAM_ERROR); /* state not clobbered */ 
		switch (ret) 
		{ 
		case Z_NEED_DICT: 
			ret = Z_DATA_ERROR;				/* and fall through */ 
		case Z_DATA_ERROR: 
		case Z_MEM_ERROR: 
			inflateEnd(&strm); 
			return ret; 
		} 
		have = 1024 - strm.avail_out; 
		totalsize += have; 
		*dest = (char*)realloc(*dest,totalsize); 
		memcpy(*dest + totalsize - have,out,have); 
	} while (strm.avail_out == 0); 
	
	/* clean up and return */ 
	(void)inflateEnd(&strm); 
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR; 
} 





/* Uncompress gzip data */
int GzipUncompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
{
    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */
    static char dummy_head[2] = 
    {
        0x8 + 0x7 * 0x10,
			(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = zdata;
    d_stream.avail_in = 0;
    d_stream.next_out = data;
    if(inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) return -1;
    //if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
    while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
        if(err != Z_OK )
        {
            if(err == Z_DATA_ERROR)
            {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
                {
                    return -1;
                }
            }
            else return -1;
        }
    }
    if(inflateEnd(&d_stream) != Z_OK) return -1;
    *ndata = d_stream.total_out;
    return 0;
}

//DecodeSize is output variable
int	GzipDecode(char * GzipBuf,int GzipSize, char * DecodeBuf, int * DecodeSize, int Flag)	//无效	
{
	if (GzipSize == 0)
	{
		return FALSE;
	}

	if (Flag)
	{
		if (memcmp(GzipBuf,"\x1f\x8b\x08\x00\x00\x00\x00\x00",8))
		{
			return false;
		}
		GzipBuf += 10;
		GzipSize -= 10;
	}

	int err = 0;
	z_stream d_stream = {0};			
	static char dummy_head[2] = {0x8 + 0x7 * 0x10,(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,};
	d_stream.zalloc = (alloc_func)0;
	d_stream.zfree = (free_func)0;
	d_stream.opaque = (voidpf)0;
	d_stream.next_in  = (unsigned char *)GzipBuf;
	d_stream.avail_in = 0;
	d_stream.next_out = (unsigned char *)DecodeBuf;
	if(inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) 
	{
		return -1;
	}
	if(inflateInit2(&d_stream, 47) != Z_OK) 
	{
		return -1;
	}
	while( ((int)d_stream.total_out < (int)*DecodeSize ) && ( (int)d_stream.total_in < (int)GzipSize) )
	{
		d_stream.avail_in = d_stream.avail_out = 1;				/* force small buffers */
		if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) 
		{
			break;
		}
		if(err != Z_OK )
		{
			if(err == Z_DATA_ERROR)
			{
				d_stream.next_in = (Bytef*) dummy_head;
				d_stream.avail_in = sizeof(dummy_head);
				if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
		}
	}
	if(inflateEnd(&d_stream) != Z_OK) 
	{
		return -1;
	}
	* DecodeSize = d_stream.total_out;
	return TRUE;
}







//Base64Buf的长度必须要足够大
char * EncodeBase64(char * Base64Buf,int Base64Len)
{
	if (Base64Len == 0)
	{
		return FALSE;
	}
	
	char * Base64TmpBuf = new char [Base64Len * 2 + 4];
	if (Base64TmpBuf == 0)
	{
		return FALSE;
	}
	
	char * Base64Ptr = Base64Buf;
	char Base[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int Count = Base64Len/3;
	int Mod	 = Base64Len%3;
	for (; Count; Count --)
	{
		unsigned int Tmp = *(unsigned int*)Base64Ptr;
		Tmp &= 0x00ffffff;
		unsigned char LowChar		= Tmp & 0x0000003f;
		unsigned char MidLowChar	= (Tmp & 0x00000fc0) >> 6;
		unsigned char MIdHighChar	= (Tmp & 0x0003f000) >> 12;
		unsigned char HighCHar		= (Tmp & 0x00fc0000) >> 18;
		* Base64TmpBuf = *(HighCHar + Base);
		Base64TmpBuf ++;
		* Base64TmpBuf = *(MIdHighChar + Base);
		Base64TmpBuf ++;
		* Base64TmpBuf = *(MidLowChar + Base);
		Base64TmpBuf ++;		
		* Base64TmpBuf = *(LowChar + Base);
		Base64TmpBuf ++;

		Base64Ptr += 3;
	}
	

	if (Mod)
	{
		//处理==
	}
	* Base64Ptr = 0;
	memmove(Base64Buf,Base64TmpBuf,Base64Ptr - Base64Buf);
	return Base64Buf;
}







//长度必定为4的整数倍，所以不用考虑处理'=',直接设置为0即可
int DecodeBase64(char * Base64Buf,int  Base64Len)
{
	if (Base64Len == 0)  
	{
		return FALSE;
	}

	if (Base64Len % 4)
	{
		Base64Len -= (Base64Len % 4);
	}
	
	char * Base64Ptr = Base64Buf;
	char * DecodeBuf = Base64Buf;
	char Base[128] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int Count = 0;
	for (int Circle = Base64Len / 4 ; Circle ; Circle --)
	{
		unsigned int LowChar		= 0;
		unsigned int MidLowChar		= 0;
		unsigned int MidHighChar	= 0;
		unsigned int HighChar		= 0;

// 		if( (*Base64Ptr == '/') || (*Base64Ptr == '\') )
// 		{
// 			HighChar = 0;
// 		}
		if (*Base64Ptr == '=')
		{
			HighChar = 0;
		}
		else
		{
			for (Count = 0; Count < 64; Count ++)
			{
				if (*(Base + Count) == *Base64Ptr)
				{
					HighChar = Count;
					break;
				}
			}

			if( (Count == 64) && (HighChar == 0) )
			{
				return FALSE;
			}
		}
		Base64Ptr ++;

		if (*Base64Ptr == '=')
		{
			MidHighChar = 0;
		}
		else
		{
			for (Count = 0; Count < 64; Count ++)
			{	if (*(Base + Count) == *Base64Ptr)
				{
					MidHighChar = Count;
					break;
				}
			}

			if( (MidHighChar == 0) && (Count == 64) )
			{
				return FALSE;
			}
		}
		Base64Ptr ++;

		if (*Base64Ptr == '=')
		{
			MidLowChar = 0;
		}
		else
		{
			for (Count = 0; Count < 64; Count ++)
			{
				if (*(Base + Count) == *Base64Ptr)
				{
					MidLowChar = Count;
					break;
				}
			}
			if( (MidLowChar == 0) && (Count == 64) )
			{
				return FALSE;
			}
		}
		Base64Ptr ++;

		if (*Base64Ptr == '=')
		{
			LowChar = 0;
		}
		else
		{
			for (Count = 0; Count < 64; Count ++)
			{
				if (*(Base + Count) == *Base64Ptr)
				{
					LowChar = Count;
					break;
				}
			}
			if( (LowChar == 0) && (Count == 64) )
			{
				return FALSE;
			}
		}
		Base64Ptr ++;

		unsigned int Tmp = 0;
		Tmp |= LowChar;
		Tmp |= (MidLowChar << 6);
		Tmp |= (MidHighChar << 12);
		Tmp |= (HighChar << 18);

		char * strTmp = (char*)&Tmp;		//必须在解密后才能交换字节的顺序
		char cTmp = strTmp[0];
		strTmp[0] = strTmp[2];
		strTmp[2] = cTmp;
		memmove(DecodeBuf,strTmp,3);
		DecodeBuf += 3;
	}

	*DecodeBuf = 0;
	return (DecodeBuf - Base64Buf);
}






int TruncateStr(LPCSTR SeperatorHead, LPCSTR SeperatorEnd, LPCSTR SrcStr, char DstStr[],unsigned int DstStrLen)
{
	
	char * iHead, * iEnd;
	iHead = strstr(SrcStr, SeperatorHead);
	if (iHead == 0)
	{
		return FALSE;
	}
	iHead += strlen(SeperatorHead);
	
	iEnd = strstr(iHead, SeperatorEnd);
	if (iEnd == 0)
	{
		return FALSE;
	}

	if ((unsigned int)(iEnd - iHead) >= DstStrLen )
	{
		return FALSE;
	}

	memmove(DstStr, iHead, iEnd - iHead);
	return TRUE;
}



/* Uncompress data */
int zdecompress(BYTE *zdata, ULONG nzdata,BYTE *data, ULONG *ndata)
{
	int err = 0;
	z_stream d_stream; /* decompression stream */
	
	d_stream.zalloc = (alloc_func)0;
	d_stream.zfree = (free_func)0;
	d_stream.opaque = (voidpf)0;
    d_stream.next_in  = zdata;
	d_stream.avail_in = 0;
	d_stream.next_out = data;
	if(inflateInit(&d_stream) != Z_OK) return -1;
	while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
		d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
		if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
		if(err != Z_OK) return -1;
	}
	if(inflateEnd(&d_stream) != Z_OK) return -1;
	*ndata = d_stream.total_out;
	return 0;
}




/* Uncompress gzip data */
int Mygzdecompress(BYTE *zdata, ULONG nzdata,BYTE *data, ULONG *ndata)
{
    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */
    static char dummy_head[2] = 
    {
        0x8 + 0x7 * 0x10,
			(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = zdata;
    d_stream.avail_in = 0;
    d_stream.next_out = data;
    if(inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) return -1;
    //if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
    while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
        if(err != Z_OK )
        {
            if(err == Z_DATA_ERROR)
            {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
                {
                    return -1;
                }
            }
            else return -1;
        }
    }
    if(inflateEnd(&d_stream) != Z_OK) return -1;
    *ndata = d_stream.total_out;
    return 0;
}



string DecodeBase62(string str) 
{  
	char decodes[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	if(str== "")
	{
		return "";
	}
	//  char[] data=string.toCharArray();
	//   ByteArrayOutputStream baos = new ByteArrayOutputStream(string.toCharArray().length);  
	
	string data = str;
	ostringstream result;
	int pos = 0, val = 0;  
	for (int i = 0; i < data.length(); i++) 
	{  
		char c = data[i];  
		if (c == 'i')
		{  
			c = data[++i];  
			c =  
				/**/c == 'a' ? 'i' :  
				/**/c == 'b' ? '+' :  
				/**/c == 'c' ? '/' : data[--i];  
		}  
		val = (val << 6) | decodes[c];  
		pos += 6;  
		while (pos > 7)
		{  
			result << (val >> (pos -= 8));  
			val &= ((1 << pos) - 1);  
		}  
	}  
	return result.str();  
}