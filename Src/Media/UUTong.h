// UUTong.h: interface for the UUTong class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UUTONG_H__AB67640C_A026_4058_ADDB_497447995380__INCLUDED_)
#define AFX_UUTONG_H__AB67640C_A026_4058_ADDB_497447995380__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//uu通视频
class UUTongSession  : public MediaSession
{
public:
	UUTongSession(DATEHEADER& packh, int aT);
	virtual ~UUTongSession() { }
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
//	virtual BOOL CloseSession(time_t nowtm);
	virtual int ListToFile();
public:
// 	time_t		m_processtm;
	CHAR 		m_sipPacket[2000];
	CHAR 		m_ackPacket[2000];
	CHAR		m_Packettype	;
	UCHAR		m_serverip[4];//服务器ip 
	MediaPacketList m_packetlist;
	CString m_calledphoneno;
	Videotype   m_enumVideotype;
};

inline int isUUTong(DATEHEADER& dateHeader, BYTE* tp)
{
	if(dateHeader.m_type==3 && dateHeader.m_datalen>0x30)//udp
	{
		BYTE len;
		len=tp[0];
		if(len==dateHeader.m_datalen)
		{
			WORD datalen;
			datalen=ntohs(*(WORD*)(tp+0x23));
			if(datalen==(dateHeader.m_datalen-0x25))
			{
				if(memcmp(tp+0x25,"NTFY",4)==0 
					|| memcmp(tp+0x25,"\x80\x04",2)==0
					||memcmp(tp+0x25,"\x80\x62",2)==0)
				{
					return 1;
				}
			}
		
		}
	}
	return 0;
}

#endif // !defined(AFX_UUTONG_H__AB67640C_A026_4058_ADDB_497447995380__INCLUDED_)
