#ifndef AFX_popo_VIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#define AFX_popo_VIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif


class POPOSession : public MediaSession
{
public:
	POPOSession(DATEHEADER& packh, int aT);
	virtual ~POPOSession() { };
	virtual int InsertPacket(DATEHEADER& dataheader, const char *pPacket);
	virtual int ListToFile();

	virtual BOOL NeedAckSocket()
	{
		return m_AvType != 1;
	}
public:
	MediaPacketList m_packetlist;
// 	time_t		m_processtm;
	int			m_Packettype;
private:
};


inline int isPOPOVideo(DATEHEADER& dateHeader, BYTE* tp)
{
//	11 10 64 00 11 0a 05 02 00 00  .m......d.......
//		0040   00 00 02 11 00 00 01 b6
	//TCP	//   ”∆µ
	if ((dateHeader.m_type == TCPPacket)
		&& (dateHeader.m_datalen > 0x400))
	{
		if ((*(WORD*)(tp+2) == 0x0064 )
			&& (memcmp(tp+8, "\x0\x0\x0", 3) == 0)
			&& (*(DWORD *)(tp + 0xe) == 0x00010000) 
			&& (*(DWORD *)(tp + 0x21) == 0xb6010000))
		{
			return 1;
		}
	}
	//UDP   //“Ù∆µ  //sip
	else if(dateHeader.m_type == UDPPacket)
	{
		if(((dateHeader.m_datalen == 145) && (*(WORD *)(tp + 2) == 0x64)
				&& (memcmp(tp + 6, "\x4\x0\x0\x0\x85\x0", 6) == 0)
				&& (*(WORD *)(tp + 0x17) == 0x0078))
			|| ((dateHeader.m_datalen == 181)  
				&& (memcmp(tp + 0x2a, "\x4\x0\x0\x0\x85\x0", 6) == 0)
				&& (*(WORD *)(tp + 0x3b) == 0x0078))) 
		{
			return 2;
		}
	}
	return 0;
}

#endif