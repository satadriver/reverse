// FreeGateDec.h: interface for the FreeGateDec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FREEGATEDEC_H__8839A4F9_0E61_4A81_ACBF_E08D2EEC871E__INCLUDED_)
#define AFX_FREEGATEDEC_H__8839A4F9_0E61_4A81_ACBF_E08D2EEC871E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/***********************************
 *			判断是否为自由之门
 ***********************************
	1.存在referer   检测函数 CheckRefer() 否 
		不是freegate的Get 
	2.referer为空   检测 Get URL 检测函数Getkey() 
		 在Decodeurl()后检测内容是不是http/开头
 
 ***********************************/


//自由之门 动网通 解码
class FreeGateDec  
{
public:
	// 判断是否为自由之门的referer(6.0和6.1通用)
	BOOL CheckRefer(const char*buf,int len);
	
public:
	// 6.0的处理函数
	//得到密钥 buf为 /aaaa/aaaaaaa形式
	BOOL Getkey(const char* buf,INT len,char* key,int &keylen);
	//初始化密钥
	BOOL InitDataKey(const char* key,int keylen);
	BOOL InitDataKey(DWORD key1, DWORD key2);
		
	//解数据段
	void DecodeData(char*buf,int datalen);
	//解密refer数据  是 +22后的数据
	BOOL DecodeRefer(char* buf,int& len,char*key,int keylen);
	//url数据 buf为 /aaaaaa
	BOOL Decodeurl(char* buf,int& len,char*key,int keylen);
	//由密钥得到DWORD 
	void GetArgKey(const char* key,DWORD* key1 ,DWORD* key2);
public:
//---------------------------------------------------
	// 6.1的处理函数
	// 处理URI和Rererer
	WORD DecodeUrlAndReferer(CString& url, CString& Referer, LPCTSTR cookie,
			int cokelen, char* randkey, int& randkeylen);
	
	BOOL AnalyseData(const char* srcdata,int srcdatalen,		// srcdata为 url 或referer的数据
			char*dstdata,int& dstdatalen,				// dstdata为去掉垃圾字节与随机密钥的数据
			char* randkey,int& randkeylen );			// randkey为随机密钥  7>randkey>=3
	BOOL AnalyseDataFor62(const char* srcdata,int srcdatalen,		// srcdata为 url 或referer的数据
			char*dstdata,int& dstdatalen);				// dstdata为去掉垃圾字节与随机密钥的数据
	BOOL DecodeCookie(char* buf,int& len,char*randkey,int randkeylen);
	BOOL GetGlobalkey (char* buf,int len,char*g_key);
	BOOL Decode1 (char* srcdata,int srcdatalen,char* dstdata,int& dstdatalen);
	BOOL Decode2 (char*buf,int& len,char*g_key);
	BOOL Decode3 (char*buf,int& len);
	BOOL Decode4 (char*buf,int&len,char*randkey,int randkeylen);
//	BOOL ConvertgKey(char*buf,char*g_key);
	void DecodeCookieBlock(char*buf,int len);
	BOOL Convert2to1(char* pSrc,char *pDst);
	BOOL Convert3to2(char* pSrc,char *pDst);
	BOOL Convert4to3(char* pSrc,char *pDst);
	BOOL GetOffsetTable (char c,DWORD&offset);
public:
	FreeGateDec();
	~FreeGateDec();
public:
//--------------------------加密------------------------- 
	//产生随机密钥
// 	void  sub_40E2E0(DWORD * value1,DWORD *value2);
// 	//加密post 上传数据
// 	void  sub_40E460(DWORD* arg_0,DWORD* arg_4,DWORD arg_8);
//------------------------------------------------------- 
private:
	//由随机密钥生成数据加密密钥 arg_4为随机密钥的DWORD arg_8为+4的DWORD
	void sub_40E2A0(DWORD* arg_0,DWORD arg_4,DWORD arg_8);
	//解码post 下载数据 arg_0为数据密钥
	void sub_40E4D0(DWORD* arg_0,DWORD* arg_4,DWORD arg_8);

	BOOL DecodeCookie1 (char* buf,int& len);
	BOOL DecodeCookie2 (char* buf,int& len,char*randkey,int randkeylen);
private:
	//解密数据段用的密钥
	char  m_decbuf[16]; 
public:
	//解url referer 的密钥
	char  m_randkey[12]; //网络传输随机密钥
	int	  m_randkeylen;  //随机密钥长度
	HttpSession* m_httpsession;
public:		// 自由之门6.3定位判断
	BOOL CheckCookieFor63(CString str_cookie);
	BOOL CheckUrlFor63(CString str_url);
};



//------------------------------------------------
// 无界的判断
BOOL isUltrasurf6(const char* pCookie, int Cookielen, DWORD& ver);	// 6.8/6.9
BOOL isUltrasurf8(const char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl);			// 8.0

void GetFristKey(const BYTE *fpacket8, unsigned char key[16]);
void sub_makekey_40EB56(BYTE* arg_0, int arg_4, BYTE* arg_8);	// 扩展密钥
void sub_xorcode_40EBC7(BYTE* arg_0, int arg_4, BYTE* arg_8);	// 编码

#define U89_CLIENTHELLO "\x16\x03\x00\x00\x61\x01\x00\x00\x5d\x03\x00"
BOOL CheckU89ClientHello(const BYTE *fpacke);

#endif // !defined(AFX_FREEGATEDEC_H__8839A4F9_0E61_4A81_ACBF_E08D2EEC871E__INCLUDED_)
