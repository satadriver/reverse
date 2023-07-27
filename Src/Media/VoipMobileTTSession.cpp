#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "VoipMobileTTSession.h"


VoipMobileTTSession::~VoipMobileTTSession(){ }


VoipMobileTTSession::VoipMobileTTSession(DATEHEADER& packh, int aT, char * pPacket) : MediaSession(packh,aT)
{
	m_processtm				= packh.m_tm;
	m_begfiletm				= packh.m_tm;
	m_Packettype			= packh.m_type;	
	m_MediaPro				= _T("media_voip_m_tt");	

	m_VoipFileHdr.Protocol	= 0x10;
	m_VoipFileHdr.UserData	= 0;
	m_VoipFileHdr.Version	= 0;
	m_VoipFileHdr.Undefine	= 0;
	m_VoipFileHdr.CallerPackCnt = 0;
	m_VoipFileHdr.CalleePackCnt = 0;
	m_VoipFileHdr.HeaderLen = sizeof(VOIPFILEHEADER) - 16;		//为了兼容以前的数据 在计算头长度时必须减去16字节


	m_CalleePackCnt		= 0;
	m_CalleePackIdx		= 0;
	m_CalleePackTime	= 0;
	m_CalleePackUnique	= 0;

	m_CallerPackTime	= 0;
	m_CallerPackUnique	= 0;
	m_CallerPackIdx		= *(unsigned short*)(pPacket + 8);
	m_CallerPackCnt		= 0;

	m_CallerIP		= *(unsigned int*)(packh.m_sip);
	m_CalleeIP		= *(unsigned int*)(packh.m_dip);
	m_CallerPORT	= packh.m_sport;
	m_CalleePORT	= packh.m_dport;
}


BOOL VoipMobileTTSession::IsVoipMobileTT(DATEHEADER& packh,const char *pPacket)
{
	if (packh.m_type != UDPPacket)
	{
		return FALSE;
	}

//	if ( (packh.m_datalen == 32) &&
//		 (memcmp(pPacket,"\xa2\x80\xfe\xff\x00\x00\x00\x00",8) == 0 ) && 
//		 (*(unsigned short*)(pPacket + 10 ) == 0x0381) )
//	{
//		return TRUE;
//	}
	if ( (packh.m_datalen == 42) &&
		 (*(unsigned short*)pPacket == 0x80a2) && (*(unsigned int*)(pPacket + 4 ) == 0 ) &&
		 (*(unsigned short*)(pPacket + 10 ) == 0x0381) )
	{
		return TRUE;
	}
	return FALSE;
}



int VoipMobileTTSession::InsertPacket(DATEHEADER &packh, const char * pPacket)
{
	if (packh.m_type != UDPPacket)
	{
		return FALSE;
	}

	if ( (packh.m_datalen == 42) &&
		 (*(unsigned short*)pPacket == 0x80a2) && (*(unsigned int*)(pPacket + 4 ) == 0 ) &&
		 (*(unsigned short*)(pPacket + 10 ) == 0x0381) )
	{
		if( (*(unsigned int*)packh.m_sip != m_CallerIP) || (*(unsigned int*)packh.m_dip != m_CalleeIP) ||
			(packh.m_sport != m_CallerPORT) || (packh.m_dport != m_CalleePORT) )
		{
			return FALSE;
		}

		m_CallerPackIdx		= *(unsigned short*)(pPacket + 8);

		m_CallerPackCnt ++;
		packh.m_datalen -= 12;
		VOIPPACKET pack(packh, (CHAR *)(pPacket + 12),m_CallerPackIdx);
		m_CallerPackList.push_back(pack);
		m_lastpacktm = packh.m_tm;
		if (abs(packh.m_tm - m_processtm) > 60)
		{
			m_processtm = packh.m_tm - 20;
			ListToFile();
		}
	}
	return 0;
}



int VoipMobileTTSession::ListToFile()
{
	VoipPacketListIt item, begin,end;
	if (m_CallerPackList.size() <= 100)
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

	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("voip_m_tt", m_sip, m_dip);
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
		WriteLog(MEDIAPRO, "ListToFile() In VOIP MOBILE TT Error!!");
		return -3;
	}
	return 0;
}