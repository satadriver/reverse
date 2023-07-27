// Https.h: interface for the Https class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPS_H__EDA09FF1_43FA_45CC_B8E4_7C1A950D5E58__INCLUDED_)
#define AFX_HTTPS_H__EDA09FF1_43FA_45CC_B8E4_7C1A950D5E58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// #include "httpsession.h"

// class Https  
// {
// public:
// 	Https();
// 	virtual ~Https();
// 
// };
class HttpsSession : public SocketSession
{
public:
	HttpsSession(DATEHEADER& packh, int aT, BOOL bC2S);
	virtual ~HttpsSession();
public:
	char			*m_certext	;		// 服务器证书数据缓冲
	int				m_cerlen	;		// 应有数据
	int				m_havelen	;		// 已有数据
	CString			m_idxtext   ;
	DWORD			m_needseq	;		// 需要的下一包的序号
	
	HttpProtocol*	m_dceprocotol;

	BOOL	SetCerBuf(unsigned char *buf, int len);
	virtual int InsertPacket(DATEHEADER& packh,const char *pPacket);
	virtual BOOL CloseSession(time_t now);

private:
	BOOL DealOneCerBuf();
	BOOL Ready(){
		return (m_cerlen>0 && m_cerlen<=m_havelen);
	};



	static BOOL WhatsApp(const char* pPacket, DATEHEADER& packh);
};

void Parsehttps(LPCTSTR strLine, char* text);
void Parissuer(LPCTSTR strLine, char* text);
BOOL DealOneCerBuf(unsigned char *buf, int len, DATEHEADER& packh);

BOOL isHttps(const char* pPacket, DATEHEADER& packh);



#endif // !defined(AFX_HTTPS_H__EDA09FF1_43FA_45CC_B8E4_7C1A950D5E58__INCLUDED_)
