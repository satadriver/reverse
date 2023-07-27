
#ifndef VOIP_URL_H_H_H
#define VOIP_URL_H_H_H


typedef struct  
{
	unsigned char		Identifier[4];
	unsigned short		Ver;
	unsigned short		Flag;
	unsigned short		Method;
	unsigned short		ModifyTime;
	unsigned short		ModifyDate;
	unsigned int		CRC;
	unsigned int		CompressedSize;
	unsigned int		DecomressSize;
	unsigned short		NameLen;
	unsigned short		ExtraLen;
	unsigned char		Name[256];

}GZIPHEADER,*LPGZIPHEADER;

char * DecodeUrl(char * UrlCodeStr);

char * Decode_UrlUnicode(char * Url_UCodestr);

char * Utf8ToGB2312(char * StringUtf8);
int Utf8ToGB2312(char * Utf8String, int Lenth);

int GzipUncompress(BYTE *zdata, ULONG nzdata, BYTE *data, ULONG *ndata);
int	GzipDecode(char * GzipBuf,int GzipSize, char * DecodeBuf, int * DecodeSize, int Flag);

int TruncateStr(LPCSTR SeperatorHead, LPCSTR SeperatorEnd, LPCSTR SrcStr, char DstStr[],unsigned int DstStrLen);

char * EncodeBase64(char * Base64Buf,int Base64Len);
int DecodeBase64(char * Base64Buf,int Base64Len);

int zdecompress(BYTE *zdata, ULONG nzdata,BYTE *data, ULONG *ndata);
int Mygzdecompress(BYTE *zdata, ULONG nzdata,BYTE *data, ULONG *ndata);
string DecodeBase62(string str) ;
#endif