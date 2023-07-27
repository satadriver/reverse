#ifndef AFX_BLUESKYVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#define AFX_BLUESKYVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif

#include "MediaSession.h"

class BLUESKYSession : public MediaSession
{
public:
	BLUESKYSession(DATEHEADER& packh, BYTE* pPacket, int aT);
	virtual ~BLUESKYSession() { }
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual int ListToFile();

	// 是否需要处理反向Socket的包
	virtual BOOL NeedAckSocket()
	{
		return FALSE;
	}
	
public:
	MediaPacketList m_packetlist;
	int				m_Packettype;
};


inline int isBLUESKYVideo(DATEHEADER& dateHeader, BYTE* tp)
{
// VMPBEGIN
	//TCP	//音视频  
	if(dateHeader.m_type==2 && dateHeader.m_datalen>=40)
	{
		if(dateHeader.m_sport==80 || dateHeader.m_dport==80)
		{
			return 0;
		}
		if(memcmp((const char *)(tp),"EI",2)==0 &&
			memcmp((const char *)(tp),"EIGHT:",6) !=0)//视频
		{
			char buf[30];
			memset(buf,0,20);
			memcpy(buf,tp,20);

			char* pdst=NULL;
			pdst=strstr((const char *)(tp),"WATCH");
			if(pdst)
			{
				return 1;
			}
			pdst=strstr((const char *)(tp),"SIZE:");
			if(pdst)
				return 1;
		}
		else if( memcmp((const char *)(tp),"EH",2)==0)	 
		{
			char buf[30];
			memset(buf,0,20);
			memcpy(buf,tp,20);

			char* pdst=NULL;
			pdst=strstr((const char *)(tp),"WATCH");
			if(pdst)
			{
				return 2;
			}	
			pdst=strstr((const char *)(tp),"CH:");
			if(pdst)
				return 2;
		}

	}
	//UDP   //音视频  //sip
	else if(dateHeader.m_type==3)
	{
		if(	memcmp("SIZE:00",(const char *)(tp),7)==0 ){
			return 1;
		}
		else if(memcmp("CH:000",(const char *)(tp),6)==0){
			return 2;
		}
	}
// VMPEND
	return 0;
}

#endif