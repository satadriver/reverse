// YYAudio.h: interface for the YYAudio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYAUDIO_H__1B033C70_9D1B_4350_974A_B9CA231A4CD2__INCLUDED_)
#define AFX_YYAUDIO_H__1B033C70_9D1B_4350_974A_B9CA231A4CD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class YYAudioSession   : public MediaSession
{
public:
	YYAudioSession(DATEHEADER& packh, int aT);
	virtual ~YYAudioSession();

	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
public:
	MediaPacketList m_packetlist;

	int			m_Packettype;

};


//ÍáÍáÓïÒô
inline int isYYAudio(DATEHEADER& dateHeader, BYTE* tp)
{
	if(dateHeader.m_type==3 && dateHeader.m_datalen>=198)//udp
	{
		DWORD packetlen=*(DWORD*)(tp);
		if(packetlen==dateHeader.m_datalen)
		{

			if(memcmp(tp+2,"\x00\x00\x02",3)==0 
				&& memcmp(tp+6,"\x00\x00\xc8\x00",4)==0)
				return 1; 

		}	
	}
	return 0;
}


#endif // !defined(AFX_YYAUDIO_H__1B033C70_9D1B_4350_974A_B9CA231A4CD2__INCLUDED_)
