// GTalk.h: interface for the GTalk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTALK_H__5D1A7AD5_C337_448D_A813_D0D141659B25__INCLUDED_)
#define AFX_GTALK_H__5D1A7AD5_C337_448D_A813_D0D141659B25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\\SocketSession.h"

class GTalkSession  : public SocketSession
{
public:
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
	BOOL  ListToFile(TcpPacketList * pTcpList,BOOL b_request);
	BOOL  OutXmlMessage(CHAR* buf ,int buflen,BOOL b_request);
	void OutData(const char* pData, int datalen);
	void WriteRealFile(const char* pData, int datalen);
public:
	GTalkSession(DATEHEADER& packh, int aT);
	virtual ~GTalkSession();
public:
	TcpPacketList	m_ReqPacketlst		;	//请求包链表
	TcpPacketList	m_ResponsePacketlst	;	//回应包链表
	time_t	m_processtm;
	CString		m_filename;
	CString     m_weiboname;
};


inline int IsGTalkPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_type ==TCPPacket && packh.m_datalen>0x10)
	{

		if(packh.m_dport==0x1466  ) //5222
		{
			if(memcmp(pPacket,"<stream:",8)==0 )
			{
				if(memcmp(pPacket+15,"to=\"gmail.com\"",14)==0)
					return 1;
			}
			else if(memcmp(pPacket,"<message ",9)==0)
			{
				return 1;
			}
		}

	}
	return 0 ;
}


#endif // !defined(AFX_GTALK_H__5D1A7AD5_C337_448D_A813_D0D141659B25__INCLUDED_)
