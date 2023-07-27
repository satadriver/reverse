#ifndef AFX_MsnVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#define AFX_MsnVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class MSNSession : public MediaSession
{
public:
	MSNSession(DATEHEADER& packh, int aT);
	virtual ~MSNSession() { }
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	virtual BOOL NeedAckSocket()
	{
		return (m_SubAvType != 1 || m_intmsnver != 0);
	}

public:
	Videotype   m_enumVideotype;
	MediaPacketList m_packetlist;
// 	time_t		m_processtm;
	int			m_Packettype;
	int			m_intmsnver;
	BOOL		m_bhascheck;
	int			m_checknum;
	int			m_SubAvType;
};

inline int isMSNVideo(DATEHEADER& packh, BYTE* tp)
{
// 	if ((packh.m_type == TCPPacket) && (packh.m_datalen >= 24))		//TCP	//音视频  
// 	{
// 		//\x18\x0\x40\x01\xf0\x00  或 \x18\x0\xa0\x0\x78\x00  在第12位置标出 ML20  
// 		if ((memcmp(tp, "\x56\x20\x01\x05", 4) == 0)
// 			&& (packh.m_datalen == 88))
// 		{
// 			return 1;
// 		}
// 		
// 		if ((memcmp("ML20", tp + 12, 4) == 0)
// 			&& ((memcmp("\x18\x0\x40\x01\xf0\x00", tp, 6) == 0)
// 				|| (memcmp("\x18\x0\xa0\x0\x78\x00", tp, 6) == 0)))
// 		{
// 			return 1;
// 		}
// 			
// 		if ((memcmp(tp + 3, "\x18\x0\x40\x01\xf0\x0", 6) == 0)
// 			&& (memcmp(tp + 3 + 12, "\x57\x4d\x56\x33", 4) == 0))
// 		{
// 			return 1;
// 		}
// 	}
// 	else 
	if(packh.m_type == UDPPacket)	//UDP   //音视频  //sip
	{
// 		if ((memcmp(tp + 6, "\x0\x0\x0", 3) == 0)
// 			&& ((memcmp(tp, "\x4a\x0\x0a\x01", 4) == 0)
// 				|| (memcmp(tp, "\x4a\x0\x14\x01", 4) == 0)))
// 		{
// 			return 1;
// 		}
// 	 
// 		if ((memcmp(tp + 12 + 9, "ML20", 4) == 0)
// 			&& ((memcmp(tp + 9, "\x18\x0\x40\x01\xf0\x00", 6) == 0)
// 				|| (memcmp(tp + 9, "\x18\x0\xa0\x0\x78\x00", 6) == 0)))
// 		{
// 			return 1;
// 		} 
// 		else if ((packh.m_datalen == 172) && (*(WORD*)tp == 0x0080))
// 		{	
// 			return 2;
// 		}

		//视频
		if( memcmp(tp,"\x80\x79",2)==0 || memcmp(tp,"\x80\xf9",2)==0 )
		{

			if(tp[12]==0xcf || tp[12]==0xe9 || tp[12]==0x89)
			{
				if(tp[13]==0)
					return 1;
			}
			
		}
		//音频
		else if((memcmp(tp,"\x80\x61",2)==0 && memcmp(tp+0x10,"\x72\x3f",2)==0)
				|| ((memcmp(tp,"\x80\x72",2)==0 || memcmp(tp,"\x80\xf2",2)==0) && tp[12]==0x3f)
			)
		{
			return 2;
		}
	}
	return 0;
}



inline int isMSNVideo1(DATEHEADER& packh, BYTE* tp)
{
	if ((packh.m_type == TCPPacket) && (packh.m_datalen >= 24))		//TCP	//音视频  
	{
		//\x18\x0\x40\x01\xf0\x00  或 \x18\x0\xa0\x0\x78\x00  在第12位置标出 ML20  
		if ((memcmp(tp, "\x56\x20\x01\x05", 4) == 0)
			&& (packh.m_datalen == 88))
		{
			return 1;
		}
		
		if ((memcmp("ML20", tp + 12, 4) == 0)
			&& ((memcmp("\x18\x0\x40\x01\xf0\x00", tp, 6) == 0)
				|| (memcmp("\x18\x0\xa0\x0\x78\x00", tp, 6) == 0)))
		{
			return 1;
		}
			
		if ((memcmp(tp + 3, "\x18\x0\x40\x01\xf0\x0", 6) == 0)
			&& (memcmp(tp + 3 + 12, "\x57\x4d\x56\x33", 4) == 0))
		{
			return 1;
		}
	}
	else if(packh.m_type == UDPPacket)	//UDP   //音视频  //sip
	{
		if ((memcmp(tp + 6, "\x0\x0\x0", 3) == 0)
			&& ((memcmp(tp, "\x4a\x0\x0a\x01", 4) == 0)
				|| (memcmp(tp, "\x4a\x0\x14\x01", 4) == 0)))
		{
			return 1;
		}
		if(tp[0]==0x62)
			return 1;
	 
		if ((memcmp(tp + 12 + 9, "ML20", 4) == 0)
			&& ((memcmp(tp + 9, "\x18\x0\x40\x01\xf0\x00", 6) == 0)
				|| (memcmp(tp + 9, "\x18\x0\xa0\x0\x78\x00", 6) == 0)))
		{
			return 1;
		} 
		else if ((packh.m_datalen == 172) && (*(WORD*)tp == 0x0080))
		{	
			return 2;
		}
		else if(memcmp(tp,"\x80\x61",2)==0 	|| memcmp(tp,"\x80\x72",2)==0 || memcmp(tp,"\x80\xf2",2)==0	)
		{
			return 2;
		}
	}
	return 0;
}
#endif