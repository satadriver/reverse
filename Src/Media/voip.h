#ifndef AFX_VOIPVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#define AFX_VOIPVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif


#include "..\\src\\Media\\MediaSession.h"
#include "..\\src\\Media\\Media.h"

typedef struct _OLDVOIPFILEHEADER
{
	unsigned int Protocol;
	unsigned int UserData;
	unsigned int Reserved;
	unsigned int Unused;
}OLDVOIPFILEHEADER,*LPOLDVOIPFILEHEADER;






typedef struct _VOIPFILEHEADER
{
	unsigned char Protocol;
	unsigned char Version;
	unsigned char HeaderLen;		//为了兼容实际数值为头长度减去16
	unsigned char Undefine;

	unsigned int UserData;
	unsigned int CallerPackCnt;
	unsigned int CalleePackCnt;
//	unsigned char CallerPhone[16];
//	unsigned char CalleePhone[16];
}VOIPFILEHEADER, * LPVOIPFILEHEADER;






typedef struct  _RTPHEADER
{
	unsigned char	CSRCcount:4;
	unsigned char	HeadExtention:1;
	unsigned char	Padding:1;
	unsigned char	Version:2;
	
	unsigned char	PayloadType:7;
	unsigned char	Mark:1;
	
	unsigned short	SeqNum;
	unsigned int	TimeStamp;
	unsigned int	SSRC;			//synchrounous source identifier(SSRC) and contribute source identifier(CSRC)
}RTPHEADER,*LPRTPHEADER;



#define VOIP_PACKET_MAX_SIZE 1024
class VOIPPACKET
{
public:
	VOIPPACKET(DATEHEADER& packh, const char* pbuf, unsigned int RTPNO)
	{
		m_tm = packh.m_tm;
		m_datalen = packh.m_datalen;
		memcpy(m_buf, pbuf, m_datalen);
		m_RTPNO = RTPNO;
	}

	~VOIPPACKET(){ }

	time_t			m_tm;
	unsigned int	m_RTPNO;								// rtp sequence
	unsigned int	m_datalen;								// 数据长度
	char 			m_buf[VOIP_PACKET_MAX_SIZE];			// 缓冲区地址

};



inline BOOL AFXAPI operator< (const VOIPPACKET& vp1, const VOIPPACKET& vp2)
{
	return vp1.m_RTPNO < vp2.m_RTPNO;
}

typedef list<VOIPPACKET, allocator<VOIPPACKET> > VoipPacketList;
typedef VoipPacketList::iterator VoipPacketListIt;		//iterator 代表类指针 operator代表分配内存







class VOIPSession : public MediaSession
{
public:
	VOIPSession(DATEHEADER& packh, int aT);
	virtual ~VOIPSession() { }
	virtual int InsertPacket(DATEHEADER& dataheader, const char *pPacket);
//	virtual BOOL CloseSession(time_t nowtm);
	virtual int ListToFile();
public:
	MediaPacketList m_packetlist;
	time_t		m_processtm;
	int			m_Packettype	;
//	int			m_AVtype;
};

inline int isVOIPVideo(DATEHEADER& packh, BYTE* tp)
{
	if(packh.m_type == UDPPacket)
	{
		if ((*(WORD *)tp == 0x0880) && (packh.m_datalen == 172))
		{
			return 2;
		}
		else if(*(WORD *)tp == 0x0480 
			&& ((((packh.m_datalen - 12) % 24) == 0)
				|| (((packh.m_datalen - 12) % 20) == 0))
			&& (packh.m_datalen <= 108))
		{
			return 2;
		}
	}
	return 0;
}

#endif