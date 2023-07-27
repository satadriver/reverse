// DoShowVideo.h: interface for the DoShowVideo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOSHOWVIDEO_H__41C52A6E_7EE4_4398_BDCB_3BDAE6C77C80__INCLUDED_)
#define AFX_DOSHOWVIDEO_H__41C52A6E_7EE4_4398_BDCB_3BDAE6C77C80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DoShowSession : public MediaSession
{
public:
	DoShowSession(DATEHEADER& packh, int aT);
	virtual ~DoShowSession() { }
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
//	virtual BOOL CloseSession(time_t nowtm);
	virtual int ListToFile();
public:
	MediaPacketList m_packetlist;
// 	time_t			m_processtm;
private:
};

struct EPHVIDEOHFMT		// 视频格式
{
	DWORD		ephflg;
	WORD		avflg;
	BYTE		zero;
	BYTE		frame;
	BYTE		serial;
	BYTE		second;
	WORD		provplen;
	WORD		framelen;
	WORD		r;
	WORD		packetlen;
};

//EPH音频数据格式
//udp包格式 
//4字节头 02 01 00 00
//2字节   31  31 //视频
//1字节   00
//1字节	帧序号0---ff循环
//2字节	为该包的数据长度
//...数据 
struct EPHAUDIOHFMT
{
	DWORD		ephflg;
	WORD		avflg;
	BYTE		zero;
	BYTE		frame;
	WORD		packetlen;
};

inline int isEphVideo(DATEHEADER& dateHeader, BYTE* pbuf)
{
	DWORD *tp = (DWORD*)pbuf;
	if ((dateHeader.m_datalen>10) && memcmp(tp,"\x2\x1\x0\0",4)==0)
	{
		// 点对点
		if (memcmp(pbuf+4, "21", 2)==0)
		{
// 			// video
// 			if (dateHeader.m_datalen>20)
// 			{
// 				EPHVIDEOHFMT* pvfmt = (EPHVIDEOHFMT*)pbuf;
// 				if ((pvfmt->serial<16) && (pvfmt->framelen>=pvfmt->packetlen) &&
// 					(pvfmt->packetlen == dateHeader.m_datalen-20))
// 				{
					return 1;
//				}
//			}
		}
		else if (memcmp(pbuf+4, "11", 2) == 0)
		{
			// audio
// 			EPHAUDIOHFMT* pafmt = (EPHAUDIOHFMT*)pbuf;
// 			if (pafmt->packetlen == dateHeader.m_datalen-10)
// 			{
				return 3;
//			}
		}
	}
	else if ((dateHeader.m_datalen>38) && 
		(memcmp(pbuf,"\x0\x1\x0",3)==0 || memcmp(pbuf,"\x0\xa\x0",3)==0 || memcmp(pbuf,"\x0\x14\x0",3)==0)
		)// 聊天室
	{
		
		if (memcmp(pbuf+28, "21", 2) == 0)
		{
			// video
// 			if (dateHeader.m_datalen>48)
// 			{
// 				EPHVIDEOHFMT* pvfmt = (EPHVIDEOHFMT*)(pbuf+24);
// 				if ((pvfmt->serial<16) && (pvfmt->framelen>=pvfmt->packetlen) &&
// 					(pvfmt->packetlen == dateHeader.m_datalen-44))
// 				{
					return 2;
// 				}
// 			}
		}
		else if (memcmp(pbuf+28, "11", 2) == 0)
		{
			// audio
// 			EPHAUDIOHFMT* pafmt = (EPHAUDIOHFMT*)(pbuf+24);
// 			if (pafmt->packetlen == dateHeader.m_datalen-34-2)
// 			{
				return 4;
//			}
		}
	}

	return 0;
}


#endif // !defined(AFX_DOSHOWVIDEO_H__41C52A6E_7EE4_4398_BDCB_3BDAE6C77C80__INCLUDED_)
