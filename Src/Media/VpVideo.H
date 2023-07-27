#ifndef AFX_VPVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#define AFX_VPVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif

class VPSession : public MediaSession
{
public:
	VPSession(DATEHEADER& packh, int aT);
	virtual ~VPSession() { }
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
//	virtual BOOL CloseSession(time_t nowtm);
	virtual int ListToFile();
public:
	MediaPacketList m_packetlist;
// 	time_t		m_processtm;
	int			m_Packettype;
//	int			m_AVtype;
};

inline int isVPVideo(DATEHEADER& dateHeader, BYTE* tp)
{
	//TCP	//ÒôÊÓÆµ  
	if(dateHeader.m_type==2 && dateHeader.m_datalen>=0x218)
	{
		if( memcmp(tp+0x12,"AudioData",9)==0  || 
			memcmp(tp+0x12,"VideoData",9)==0  )
		{
			return 1;
		}
		if( memcmp(tp,"\xf8\x31\xef\x01",4)==0  && 
			memcmp(tp+0x3c,"\xff\xff\xff\xff",4)==0  )
		{
			if(memcmp(tp+0xc, "\x54\x04", 2) == 0)
				return 1;
			else if(memcmp(tp+0xc, "\x4c\x04", 2) == 0)
				return 2;
		}

	}
	return 0;
}

#endif