#ifndef AFX_SQQVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#define AFX_SQQVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif


class SQQSession : public MediaSession
{
public:
	SQQSession(DATEHEADER& packh, int aT);
	virtual ~SQQSession() { }
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
// 	virtual BOOL CloseSession(time_t nowtm);
	virtual int ListToFile();
public:
	MediaPacketList m_packetlist;
// 	time_t		m_processtm;
	int			m_Packettype	;
private:
};

inline int isSQQVideo(DATEHEADER& packh, BYTE* tp)
{
	if((packh.m_type == TCPPacket) && (packh.m_datalen >= 0x20))	//TCP	//ÒôÊÓÆµ  
	{
		if (memcmp("\x12\x7\x1\x0\x0\x0", tp, 6) == 0)
		{
			if (*(WORD *)(tp+17) == 0x1409)
			{
				return 1;
			}
			else if (*(WORD *)(tp+18) == 0x140a)
			{
				return 1;
			}
			else if (*(WORD *)(tp+17)==0x1209)
			{
				return 2;
			}
			else if (*(WORD *)(tp+18)==0x120a)
			{
				return 2;
			}
		}
	}
	else if(packh.m_type == UDPPacket)	//UDP   //ÒôÊÓÆµ  //sip
	{
		if((packh.m_datalen == 0x80) && (*(WORD *)tp == 0xb180))
		{
			return 2;
		}
		else if((packh.m_datalen > 80) && (*(WORD *)tp == 0xb380))
		{
			return 1;
		}
	}
	return 0;
}

#endif