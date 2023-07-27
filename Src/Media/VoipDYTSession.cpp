#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "VoipDYTSession.h"



VoipDYTSession::VoipDYTSession(DATEHEADER& packh, int aT, char * pPacket): MediaSession(packh, aT)
{
	m_processtm				= packh.m_tm;
	m_begfiletm				= packh.m_tm;
	m_Packettype			= packh.m_type;
	
	m_MediaPro				= _T("media_voip_dyt");
	m_VoipFileHdr.Protocol	= 0x07;
	m_VoipFileHdr.UserData	= 0;
	m_VoipFileHdr.Version	= 0;
	m_VoipFileHdr.Undefine	= 0;
	m_VoipFileHdr.CallerPackCnt = 0;
	m_VoipFileHdr.CalleePackCnt = 0;
	m_VoipFileHdr.HeaderLen = sizeof(VOIPFILEHEADER) - 16;		//为了兼容以前的数据 在计算头长度时必须减去16字节


	m_CalleePackCnt			= 0;
	m_CalleePackIdx			= 0;
	m_CalleePackTime		= 0;
	m_CalleePackUnique		= 0;

	m_CallerPackIdx			= ntohs(*(unsigned short*)(pPacket + 2) ^ 0xdf3f);
	m_CallerPackTime		= *(unsigned int*)(pPacket + 4) ^ 0xaf4b3f2c;
	m_CallerPackUnique		= *(unsigned int*)(pPacket + 8) ^ 0x8b108120;
	m_CallerPackCnt			= 0;

	m_CallerIP				= *(unsigned int*)(packh.m_sip);
	m_CalleeIP				= *(unsigned int*)(packh.m_dip);
	m_CallerPORT			= packh.m_sport;
	m_CalleePORT			= packh.m_dport;
}


VoipDYTSession::~VoipDYTSession(){ }



BOOL VoipDYTSession::IsVoipDYT(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type != UDPPacket)
	{
		return FALSE;
	}

	if ( ( (*(unsigned short *)pPacket == 0x3bab) || (*(unsigned short *)pPacket == 0xbbab) ) && 
		(32 == packh.m_datalen))
	{
		return TRUE;
	}
	return FALSE;
}



int VoipDYTSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (m_Packettype != UDPPacket)
	{
		return FALSE;
	} 

	if ( ( (*(unsigned short *)pPacket == 0x3bab) || (*(unsigned short *)pPacket == 0xbbab) ) && 
		(32 == packh.m_datalen))
	{
		char RtpHdr[12] = {0};
		memmove(RtpHdr, pPacket, 12);
		unsigned char MaskBuf[12] = {0x2b,0xa9,0x3f,0xdf,0x2c,0x3f,0x4b,0xaf,0x20,0x81,0x10,0x8b};
		for (int Counter = 0; Counter < 12 ;Counter ++)
		{
			*(RtpHdr + Counter) ^= *(MaskBuf + Counter);
		}

		if( (*(unsigned int*)packh.m_sip == m_CallerIP) && (*(unsigned int*)packh.m_dip == m_CalleeIP) &&
			(packh.m_sport == m_CallerPORT) && (packh.m_dport == m_CalleePORT) )
		{
			if ( *(unsigned int*)(RtpHdr + 8) != m_CallerPackUnique)
			{
				return FALSE;
			}
			m_CallerPackIdx			= ntohs(*(unsigned short*)(pPacket + 2) ^ 0xdf3f);
	
			m_CallerPackCnt ++;
			packh.m_datalen -= 12;
			VOIPPACKET pack(packh, (CHAR *)(pPacket + 12), m_CallerPackIdx);
			m_CallerPackList.push_back(pack);
			m_lastpacktm = packh.m_tm;
			return TRUE;
		}
		else if( (*(unsigned int*)packh.m_sip == m_CalleeIP) && (*(unsigned int*)packh.m_dip == m_CallerIP) &&
			(packh.m_sport == m_CalleePORT) && (packh.m_dport == m_CallerPORT) )
		{
			if( (m_CalleePackUnique == 0) && (m_CalleePackCnt == 0) )
			{
				m_CalleePackUnique = *(unsigned int *)(RtpHdr + 8);
			}
			else if ( *(unsigned int*)(RtpHdr + 8) != m_CalleePackUnique)
			{
				return FALSE;
			}

			m_CalleePackTime = *(unsigned int *)(RtpHdr + 4);
			m_CalleePackIdx = ntohs(*(unsigned short *)(RtpHdr + 2));
			
			m_CalleePackCnt ++;
			packh.m_datalen -= 12;
			VOIPPACKET pack(packh, (CHAR *)(pPacket + 12),m_CalleePackIdx);
			m_CalleePackList.push_back(pack);
			m_lastpacktm = packh.m_tm;
			return TRUE;
		}
	}
	return 0;
}




int VoipDYTSession::ListToFile()
{
	VoipPacketListIt begin;
	if( (m_CallerPackList.size() <= 100) && (m_CalleePackList.size() <= 100) )
	{
		if(m_CallerPackList.size() )
		{
			m_CallerPackList.clear();
		}
		
		if(m_CalleePackList.size() )
		{
			m_CalleePackList.clear();
		}	
		return FALSE;
	}

//	m_CallerPackList.sort();
//	m_CalleePackList.sort();

	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("voip_dyt", m_sip, m_dip);
		if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return -2;
		}
		m_VoipFileHdr.CallerPackCnt = m_CallerPackCnt ;
		m_VoipFileHdr.CalleePackCnt = m_CalleePackCnt ;
		file.Write(&m_VoipFileHdr,sizeof(VOIPFILEHEADER));
	}
	else
	{
		if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return -2;
		}
	}
	file.SeekToEnd();

	try
	{
		while(m_CallerPackList.size())
		{
			begin = m_CallerPackList.begin();
			m_endfiletm = begin->m_tm;
			file.Write(begin->m_buf,begin->m_datalen);
			m_CallerPackList.erase(begin);
		}

		while(m_CalleePackList.size())
		{
			begin = m_CalleePackList.begin();
			m_endfiletm = begin->m_tm;
			file.Write(begin->m_buf,begin->m_datalen);
			m_CalleePackList.erase(begin);
		}
		file.Close();
	}	
	catch(...)
	{
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP DYT Error!!");
		return -3;
	}
	return 0;
}

