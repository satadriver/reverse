#ifndef	VOIPQQ_H
#define VOIPQQ_H

#if _MSC_VER > 1000
#pragma once
#endif


#include "..\\src\\media\\MediaSession.h"
#include "..\\src\\media\\media.h"
#include "..\\src\\media\\voip.h"


#define QQ_VOIP_CMD			1
#define MOBILE_QQ_VOIP_DATA	2
#define PC_QQ_VOIP_DATA		3

#define QQ_AUDIO_PACK_FILTER_LEN 16



inline int  IsVoipQQPacket(DATEHEADER & packh,const char * pPacket)
{
	if ( *pPacket == 0x5b )
	{
		if( ntohs(*(unsigned short *)(pPacket + 1)) == packh.m_datalen )
		{
			if (*(unsigned short *)(pPacket + 3) == 0x0101)
			{
				if ( *(unsigned short*)(pPacket + packh.m_datalen - 2) == 0 )
				{
					return MOBILE_QQ_VOIP_DATA;
				}
				else
				{
					return FALSE;
					return PC_QQ_VOIP_DATA;
				}			
			}
		}
	}
	else if( *(unsigned short*)pPacket == 0x0002 && *(pPacket + packh.m_datalen - 1) == 0x03 )
	{
		if( ntohs(*(unsigned short *)(pPacket + 1)) == packh.m_datalen )
		{
			if (*(pPacket + 6) == 1 || *(pPacket + 6) == 2 || *(pPacket + 6) == 3)
			{
				if (memcmp(pPacket + 7,"\x03\x10\x00\x00\x03",5) == 0)
				{
					return QQ_VOIP_CMD;
				}
			}
		}
	}
	
	return FALSE;
}


class VoipQQSession : public MediaSession
{
public:
	VoipQQSession(DATEHEADER& packh, int aT);
	virtual ~VoipQQSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	
	unsigned int srcip;
	unsigned int srcport;
	unsigned int dstip;
	unsigned int dstport;
	unsigned int senderpackcnt;
	unsigned int recverpackcnt;
	unsigned int senderpackindex;
	unsigned int recverpackindex;
	
	unsigned int senderpacknum1;
	unsigned int senderflag;
	unsigned int senderpacktime;
	//00 03 3f 01 05	unknown
	unsigned int senderpacknum2;
	//00 04 04 20		unknown
	unsigned int senderqq;
	unsigned short senderlastlen1;
	unsigned short senderlastlen2;
	
	unsigned int recverpacknum1;
	unsigned int recverflag;
	unsigned int recverpacktime;
	//00 03 3f 01 05	unknown
	unsigned int recverpacknum2;
	//00 04 04 20		unknown
	unsigned int recverqq;
	unsigned short recverlastlen1;
	unsigned short recverlastlen2;
	
	VoipPacketList	m_CallerPackList;
	VoipPacketList	m_CalleePackList;
	VOIPFILEHEADER	VoipFileHdr;

	char senderpackfilter[QQ_AUDIO_PACK_FILTER_LEN];
	char recverpackfilter[QQ_AUDIO_PACK_FILTER_LEN];
};

#endif