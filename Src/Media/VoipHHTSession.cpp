#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "VoipHHTSession.h"



VoipHHTSession::VoipHHTSession(DATEHEADER& packh, int aT, char * pPacket) : MediaSession(packh,aT)
{
	m_processtm		= packh.m_tm;
	m_begfiletm		= packh.m_tm;
	m_Packettype	= packh.m_type;
	m_MediaPro		= _T("media_voip_hht");

	m_VoipFileHdr.Protocol = 0x20;
	m_VoipFileHdr.UserData = 0;
	m_VoipFileHdr.Version	= 0;
	m_VoipFileHdr.Undefine	= 0;
	m_VoipFileHdr.CallerPackCnt = 0;
	m_VoipFileHdr.CalleePackCnt = 0;
	m_VoipFileHdr.HeaderLen = sizeof(VOIPFILEHEADER) - 16;		//为了兼容以前的数据 在计算头长度时必须减去16字节
	

	m_CallerPackCnt		= 0;
	m_CallerPackUnique	= 0;
	m_CallerPackIdx		= 0;
	m_CallerPackTime	= 0;

	m_CalleePackCnt		= 0;
	m_CalleePackUnique	= 0;
	m_CalleePackIdx		= *(unsigned short*)(pPacket + 1) ;
	m_CalleePackTime	= *(unsigned int*)(pPacket + 3);

	m_CalleeIP			= *(unsigned int*)(packh.m_sip);
	m_CallerIP			= *(unsigned int*)(packh.m_dip);
	m_CalleePORT		= packh.m_sport;
	m_CallerPORT		= packh.m_dport;
}



VoipHHTSession::~VoipHHTSession(){ }



BOOL VoipHHTSession::IsVoipHHT(DATEHEADER& packh,const char *pPacket)
{
	if (packh.m_type != UDPPacket)
	{
		return FALSE;
	}

	if ( (packh.m_datalen == 31) && (packh.m_dport == 4000) && (*pPacket == 1) )
	{
		return TRUE;
	}
	return FALSE;
}



int VoipHHTSession::InsertPacket(DATEHEADER & packh,const char * pPacket)
{
	if (m_Packettype != UDPPacket)
	{
		return FALSE;
	}

	if ( (packh.m_datalen == 31) && (packh.m_sport == 4000) && (*pPacket == 1) )
	{
		if( (*(unsigned int*)packh.m_sip == m_CallerIP) && (*(unsigned int*)packh.m_dip == m_CalleeIP) &&
			(packh.m_sport == m_CallerPORT) && (packh.m_dport == m_CalleePORT) )
		{
			m_CallerPackIdx = *(unsigned short*)(pPacket + 1);
			m_CallerPackTime = *(unsigned int*)(pPacket + 3);
			m_CallerPackCnt ++;
			packh.m_datalen -= 7;
			VOIPPACKET pack(packh, (CHAR *)(pPacket + 7),m_CallerPackIdx);
			m_CallerPackList.push_back(pack);
			m_lastpacktm = packh.m_tm;
			return TRUE;
		}
	}
	else if ( (packh.m_datalen == 31) && (packh.m_dport == 4000) && (*pPacket == 1) )
	{
		if( (*(unsigned int*)packh.m_sip == m_CalleeIP) && (*(unsigned int*)packh.m_dip == m_CallerIP) &&
			(packh.m_sport == m_CalleePORT) && (packh.m_dport == m_CallerPORT) )
		{	
			m_CalleePackIdx		= *(unsigned short*)(pPacket + 1) ;
			m_CalleePackTime = *(unsigned int*)(pPacket + 3);

			m_CalleePackCnt ++;
			packh.m_datalen -= 7;
			VOIPPACKET pack(packh, (CHAR *)(pPacket + 7),m_CalleePackIdx);
			m_CalleePackList.push_back(pack);
			m_lastpacktm = packh.m_tm;
			return TRUE;
		}
	}
	return 0;
}



int VoipHHTSession::ListToFile()
{
	VoipPacketListIt item, begin,end;
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
		m_realfname = CreateMediaFilename("voip_hht", m_sip, m_dip);
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
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP HHT Error!!");
		return -3;
	}
	return 0;
}