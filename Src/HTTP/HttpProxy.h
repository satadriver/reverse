// HttpProxy.h: interface for the HttpProxy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPPROXY_H__56C1E9C3_1E72_4C35_9225_4C94CF754212__INCLUDED_)
#define AFX_HTTPPROXY_H__56C1E9C3_1E72_4C35_9225_4C94CF754212__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "httpsession.h"

//class ImsgPacketInfo
//{
//public:
//	time_t			starttm;
//	CString			strInfo;
//};
//bool AFXAPI operator<(const ImsgPacketInfo& s1, const ImsgPacketInfo& s2);
//// Define a template class list of int
//typedef list<ImsgPacketInfo, allocator<ImsgPacketInfo> > ImsgPacketInfoList ;
////Define an iterator for template class list of ints
//typedef ImsgPacketInfoList::iterator ImsgPacketInfoListIt ;

class HttpImsgProxy : public SocketSession
{
public:
	int				m_ProxyType ;
	LPCTSTR			m_imsgtype	;	//msn ymsg icqaol	
	CString			m_linkman	;	// ��ϵ��
	CString			m_msgtext	;
	HttpProtocol*	m_dceprocotol;

	HttpImsgProxy();

	virtual ~HttpImsgProxy()
	{
		if (!m_realfname.IsEmpty()) DeleteFile(m_realfname);
	};

	virtual BOOL CloseSession(time_t nowtm);
private:
	void WriteImsgFile();
};

//*************************************************
//	��������:		IsImsgProxy
//	����:			�ж�һ��Http�Ự�Ƿ���IMSG����
//	�������:		pPacketInfo Httpͷ��Ϣ
//					pPacket Http���ص�����
//					len		Http���ص����ݳ���
//	�������:		
//	����ֵ:			��Http����ͷ���TRUE, ���򷵻�FALSE
//	����:			
//*************************************************
BOOL IsImsgProxy(HttpCommInfo* pPacketInfo, const char* pPacket, int len);

//inline void AFXAPI DestructElements(HttpImsgProxy** ppElements, int nCount)
//{
//	delete ppElements;
//}
//
//typedef CMap<CString, LPCSTR, class HttpImsgProxy*, class HttpImsgProxy*&>ClassHashHttpImsgProxy;
//
//extern ClassHashHttpImsgProxy HttpImsgProxySocketMap;
//�� ������Ϣ���뵽hash
//void InsertMsnProxy(IMSGPROXY &imsgproxy , CString caseid,int limittype,CString strObjid  ,CString LimitAddr,int AllWatchFlg,CString UserAccount,CString Phone,CString spyaddr);
//
//void InsertYahooProxy(IMSGPROXY &imsgproxy , CString caseid,int limittype,CString strObjid  ,CString LimitAddr,int AllWatchFlg,CString UserAccount,CString Phone,CString spyaddr);

extern Search SerMsn;

class TaoBaoFriend : public SocketSession
{
public:
	TaoBaoFriend(DATEHEADER& packh, int aT);
	virtual ~TaoBaoFriend();
public:
	int				m_ProxyType ;
	LPCTSTR			m_imsgtype	;	//msn ymsg icqaol	
	CString			m_linkman	;	// ��ϵ��
	CString			m_msgtext	;
	CString			m_idxtext   ;
	HttpProtocol*	m_dceprocotol;
	HttpImsgProxy();

// 	virtual ~TaoBaoFriend()
// 	{
// 		if (!m_realfname.IsEmpty()) DeleteFile(m_realfname);
// 	};

	virtual BOOL CloseSession();
private:
	void WriteImsgFile();
};
BOOL FormatTaobaoFrient(DATEHEADER& packh,CString url,char* pSplitxt,LPCTSTR spyaddr,CString host,HttpSession* pSion);
BOOL FormatTaobaoLogin(DATEHEADER& packh,CString url,char* pSplitxt,LPCTSTR spyaddr);
BOOL FormatTaobaoLogin(CString filename,CString &taobaotxt,LPCTSTR spyaddr);
BOOL testtaobaopass();

#endif
