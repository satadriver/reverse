
#ifndef AFX_UCVIDEO_H__8745554E_0847_437A_B24A_22255FDCBA68__INCLUDED_
#define AFX_UCVIDEO_H__8745554E_0847_437A_B24A_22255FDCBA68__INCLUDED_

/***************************  新浪UC视频处理  ******************************/

#if _MSC_VER > 1000
#pragma once
#endif


class UcSession : public MediaSession
{
public:
	UcSession(DATEHEADER& packh, int aT);
	virtual ~UcSession() { }
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
//	virtual BOOL CloseSession(time_t nowtm);
	virtual int ListToFile();
public:
	MediaPacketList m_packetlist;

	int m_sinatype;  // 1为sinashow  2为sinauc 
// 	time_t			m_processtm;
private:
};

// inline int isUcVideo(DATEHEADER& packh, char* tp)
// {
// 	if (packh.m_datalen > 12)
// 	{
// 		if(memcmp(tp, "\x0\x0\x0\x1", 4) == 0)
// 		{
// 			tp += 5;
// 		}
// 			
// 		if (memcmp(tp,"\x80\x0a",2)==0 && memcmp(tp+0x10,"\x00\x01\x41\x9a",4)==0)
// 		{
// 			return 1;
// 		}
// 		else if (memcmp(tp,"\x80\x00",2)==0  && (packh.m_datalen == 36))
// 		{
// 			return 2;
// 		}
// 		else if (memcmp(tp,"\x80\x06",2)==0  && (packh.m_datalen == 72))
// 		{
// 			return 2;
// 		}
// 	}
// 	return 0;
// }

//90 81 为uc   90 8a 为sinashow

inline int isUcVideo1(DATEHEADER& packh, char* tp)
{
	if (packh.m_type == UDPPacket && packh.m_datalen > 50)
	{
	/////////////////////////sinashow  ucv1/////////////////////////////////////////////////
		if (memcmp(tp,"\x90\x8a",2)==0 && 
			memcmp(tp+0x12,"\x00\x00\x00\x01",4)==0)
		{
			return 1;
		}
		else if (memcmp(tp,"\x80\x0a",2)==0 && memcmp(tp+0x10,"\x00\x01\x41\x9a",4)==0)
		{
			return 1;
		}
	/////////////////////////sina uc ucv2/////////////////////////////////////////////////
		if (memcmp(tp,"\x90\x81",2)==0 && 
			memcmp(tp+0x10,"\x00\x00\x00\x01",4)==0)
		{
			return 2;
		}
		else if (memcmp(tp,"\x80\x81",2)==0 && memcmp(tp+14,"\x00\x01\x41\x9a",4)==0)
		{
			return 2;
		}




// 		else if(memcmp(tp,"\x90\x0a",2)==0 || memcmp(tp,"\x90\x8a",2)==0
// 			 )
// 		{
// 			return 1;
// 		}
// 		else if(memcmp(tp,"\x90\x01",2)==0 || memcmp(tp,"\x90\x81",2)==0)
// 		{
// 
// 			return 1;
// 		}
// 		else if (memcmp(tp,"\x80\x00",2)==0  && (packh.m_datalen == 36))
// 		{
// 			return 2;
// 		}
// 		else if (memcmp(tp,"\x80\x06",2)==0  && (packh.m_datalen == 72))
// 		{
// 			return 2;
// 		}
// 		else if(memcmp(tp,"\x80\x02",2)==0)
// 		{
// 			return 3;
// 		}
	}
	return 0;
}

#endif