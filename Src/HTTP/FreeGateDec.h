// FreeGateDec.h: interface for the FreeGateDec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FREEGATEDEC_H__8839A4F9_0E61_4A81_ACBF_E08D2EEC871E__INCLUDED_)
#define AFX_FREEGATEDEC_H__8839A4F9_0E61_4A81_ACBF_E08D2EEC871E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/***********************************
 *			�ж��Ƿ�Ϊ����֮��
 ***********************************
	1.����referer   ��⺯�� CheckRefer() �� 
		����freegate��Get 
	2.refererΪ��   ��� Get URL ��⺯��Getkey() 
		 ��Decodeurl()���������ǲ���http/��ͷ
 
 ***********************************/


//����֮�� ����ͨ ����
class FreeGateDec  
{
public:
	// �ж��Ƿ�Ϊ����֮�ŵ�referer(6.0��6.1ͨ��)
	BOOL CheckRefer(const char*buf,int len);
	
public:
	// 6.0�Ĵ�����
	//�õ���Կ bufΪ /aaaa/aaaaaaa��ʽ
	BOOL Getkey(const char* buf,INT len,char* key,int &keylen);
	//��ʼ����Կ
	BOOL InitDataKey(const char* key,int keylen);
	BOOL InitDataKey(DWORD key1, DWORD key2);
		
	//�����ݶ�
	void DecodeData(char*buf,int datalen);
	//����refer����  �� +22�������
	BOOL DecodeRefer(char* buf,int& len,char*key,int keylen);
	//url���� bufΪ /aaaaaa
	BOOL Decodeurl(char* buf,int& len,char*key,int keylen);
	//����Կ�õ�DWORD 
	void GetArgKey(const char* key,DWORD* key1 ,DWORD* key2);
public:
//---------------------------------------------------
	// 6.1�Ĵ�����
	// ����URI��Rererer
	WORD DecodeUrlAndReferer(CString& url, CString& Referer, LPCTSTR cookie,
			int cokelen, char* randkey, int& randkeylen);
	
	BOOL AnalyseData(const char* srcdata,int srcdatalen,		// srcdataΪ url ��referer������
			char*dstdata,int& dstdatalen,				// dstdataΪȥ�������ֽ��������Կ������
			char* randkey,int& randkeylen );			// randkeyΪ�����Կ  7>randkey>=3
	BOOL AnalyseDataFor62(const char* srcdata,int srcdatalen,		// srcdataΪ url ��referer������
			char*dstdata,int& dstdatalen);				// dstdataΪȥ�������ֽ��������Կ������
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
//--------------------------����------------------------- 
	//���������Կ
// 	void  sub_40E2E0(DWORD * value1,DWORD *value2);
// 	//����post �ϴ�����
// 	void  sub_40E460(DWORD* arg_0,DWORD* arg_4,DWORD arg_8);
//------------------------------------------------------- 
private:
	//�������Կ�������ݼ�����Կ arg_4Ϊ�����Կ��DWORD arg_8Ϊ+4��DWORD
	void sub_40E2A0(DWORD* arg_0,DWORD arg_4,DWORD arg_8);
	//����post �������� arg_0Ϊ������Կ
	void sub_40E4D0(DWORD* arg_0,DWORD* arg_4,DWORD arg_8);

	BOOL DecodeCookie1 (char* buf,int& len);
	BOOL DecodeCookie2 (char* buf,int& len,char*randkey,int randkeylen);
private:
	//�������ݶ��õ���Կ
	char  m_decbuf[16]; 
public:
	//��url referer ����Կ
	char  m_randkey[12]; //���紫�������Կ
	int	  m_randkeylen;  //�����Կ����
	HttpSession* m_httpsession;
public:		// ����֮��6.3��λ�ж�
	BOOL CheckCookieFor63(CString str_cookie);
	BOOL CheckUrlFor63(CString str_url);
};



//------------------------------------------------
// �޽���ж�
BOOL isUltrasurf6(const char* pCookie, int Cookielen, DWORD& ver);	// 6.8/6.9
BOOL isUltrasurf8(const char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl);			// 8.0

void GetFristKey(const BYTE *fpacket8, unsigned char key[16]);
void sub_makekey_40EB56(BYTE* arg_0, int arg_4, BYTE* arg_8);	// ��չ��Կ
void sub_xorcode_40EBC7(BYTE* arg_0, int arg_4, BYTE* arg_8);	// ����

#define U89_CLIENTHELLO "\x16\x03\x00\x00\x61\x01\x00\x00\x5d\x03\x00"
BOOL CheckU89ClientHello(const BYTE *fpacke);

#endif // !defined(AFX_FREEGATEDEC_H__8839A4F9_0E61_4A81_ACBF_E08D2EEC871E__INCLUDED_)
