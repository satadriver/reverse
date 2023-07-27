// Dzh.h: interface for the CDzh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DZH_H__56EE2112_DCA7_4329_8B7E_4F90F7198CAE__INCLUDED_)
#define AFX_DZH_H__56EE2112_DCA7_4329_8B7E_4F90F7198CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\SocketSession.h"

inline int IsDzh60(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_type ==TCPPacket)
	{
		if(packh.m_datalen  > 100)
		{
			if(memcmp(pPacket,"L=",2)==0)
			{
				return 1;
			}			
		}	
	}
	
	return 0;
}

class CDzh60Session  : public SocketSession
{
public:
	CDzh60Session(DATEHEADER& packh, int aT);
	virtual ~CDzh60Session();

public:
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

private:
	DWORD		m_balreadyout;		// 已经输出过, 同一会话仅输出一次

};

inline int IsThsPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_type ==TCPPacket)
	{
		if(packh.m_datalen  > 500)
		{
			if(memcmp(pPacket,"\xfd\xfd\xfd\xfd",4)==0 
				&& memcmp(pPacket+16,"thsuser",7)==0)
			{
				return 1;
			}			
		}	
	}
	
	return 0;
}

class CThsSession  : public SocketSession
{
public:
	CThsSession(DATEHEADER& packh, int aT);
	virtual ~CThsSession();

public:
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

private:
	DWORD		m_balreadyout;		// 已经输出过, 同一会话仅输出一次

};



#endif // !defined(AFX_DZH_H__56EE2112_DCA7_4329_8B7E_4F90F7198CAE__INCLUDED_)
