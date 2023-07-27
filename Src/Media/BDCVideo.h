#ifndef AFX_BDCVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#define AFX_BDCVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif

#include "MediaSession.h"


class BDCSession : public MediaSession
{
public:
	BDCSession(DATEHEADER& packh, int aT);
	virtual ~BDCSession() { };
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
public:
	MediaPacketList	m_packetlist;
	int				m_Packettype;
};


inline int isBDCVideo(DATEHEADER& packh, BYTE* tp)
{
	//TCP	//ÒôÊÓÆµ  
	if ((packh.m_type == TCPPacket) && (packh.m_datalen >= 80))
	{
		INT numchar = 0;
		INT bdnum = 0;
		while(numchar < packh.m_datalen)
		{
			USHORT len=0;
			if ((strnicmp("BD", (const char *)(tp + numchar), 2) == 0)
				&& (*(USHORT*)(tp + numchar + 2) + 16 + numchar <= packh.m_datalen)
				&& (tp[4] == 0x73 || tp[4] == 0x70 || tp[4] == 0x86  || tp[4] == 0x80)  
				&& (memcmp(tp + numchar + 5, "\0\0\0\0\0\0\0\0\0\0\0", 3) == 0))
			{
				len = *(USHORT*)(tp + numchar + 2);
				bdnum++;
				numchar += 16 + len;
				continue;
			}
			else
			{
				break;
			}
		}
		if (bdnum >= 2)
		{
			return 1;
		}
	}
	return 0;
}


#endif