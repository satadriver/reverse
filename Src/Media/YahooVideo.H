#ifndef AFX_YahooVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#define AFX_YahooVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif

class YAHOOSession : public MediaSession
{
public:
	YAHOOSession(DATEHEADER& packh, int aT);
	
	virtual ~YAHOOSession()
		{ }

	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);

//	virtual BOOL CloseSession(time_t nowtm);

	virtual int ListToFile();

	virtual BOOL NeedAckSocket()
		{return (m_AvType != 1);}

public:
	MediaPacketList m_packetlist;
// 	time_t		m_processtm;
	int			m_Packettype	;
//	int 		m_AVtype;
};

inline int isYAHOOVideo(DATEHEADER& packh, BYTE* tp)
{
	//TCP	//ÒôÊÓÆµ  
// 	if ((packh.m_type == TCPPacket) && (packh.m_datalen >= 0x400))
// 	{
// 		if(strnicmp("Kakadu", (const char *)(tp + 0x6c), 6) == 0)
// 		{
// 			return 1;		// ÊÓÆµ
// 		}
// 	}
	//UDP   //ÒôÊÓÆµ  //sip
	//else 
	if(packh.m_type == UDPPacket)
	{
			if(memcmp(tp,"\x80\x7b",2)==0  )//ÊÓÆµ
			{

				if(memcmp(tp+15,"\x00\x00",2)==0)
				{
					if(tp[12]==0xc2 || tp[12]==0xc0)
						return 1;
				}
				else if(memcmp(tp+0x14,"\x00\x00",2)==0)
				{
					if(tp[12]==0xc2 || tp[12]==0xc0)
						return 1;
				}


			}
 			else if(
			(((packh.m_datalen == 0x70) || (packh.m_datalen == 0x50)) && (*(WORD *)tp == 0x2280))
// 			|| ((*(WORD *)tp == 0x6480) && (packh.m_datalen >= 0x52))
// 			|| ((*(WORD *)tp == 0x6180) && (packh.m_datalen >= 0x2a))
// 			|| ((*(WORD *)tp == 0x6280) && (packh.m_datalen >= 0x3a))	
			||((*(WORD *)tp == 0x6780) && (packh.m_datalen >= 0x32))
			|| ((*(WORD *)tp == 0xe780) && (packh.m_datalen >= 0x32))
			)
		{	
			return 2;		// ÒôÆµ
		}
	}
	return 0;
}


#endif