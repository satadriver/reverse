#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "VoipMobileG723Session.h"


BOOL VoipMobileG723Session::IsVoipMobileg723(DATEHEADER& packh,const char *pPacket)
{
	if (packh.m_type == UDPPacket)
	{
		if( ( (*(unsigned short *)pPacket == 0x6b80) || (*(unsigned short *)pPacket == 0xeb80) )
			&& (packh.m_datalen == 44) )
		{
			return TRUE;
		}
//		if( ( (*(unsigned short *)pPacket == 0x0480) || (*(unsigned short *)pPacket == 0x8480) )
//			&& (packh.m_datalen == 36) )
//		{
//			return TRUE;
//		}
	}
	return FALSE;
}


VoipMobileG723Session::VoipMobileG723Session(DATEHEADER& packh, int aT, char * pPacket) : MediaSession(packh,aT)
{
	m_processtm				= packh.m_tm;
	m_begfiletm				= packh.m_tm;
	m_Packettype			= packh.m_type;
	m_MediaPro				= _T("media_voip_mobile_g723");	

	m_VoipFileHdr.Protocol	= 0x90;
	m_VoipFileHdr.UserData	= 0;
	m_VoipFileHdr.Version	= 0;
	m_VoipFileHdr.Undefine	= 0;
	m_VoipFileHdr.CallerPackCnt = 0;
	m_VoipFileHdr.CalleePackCnt = 0;
	m_VoipFileHdr.HeaderLen = sizeof(VOIPFILEHEADER) - 16;		//为了兼容以前的数据 在计算头长度时必须减去16字节


	m_CallerPackIdx			= ntohs(*(unsigned short*)(pPacket + 2) );
	m_CallerPackTime		= *(unsigned int*)(pPacket + 4);
	m_CallerPackUnique		= *(unsigned int*)(pPacket + 8);
	m_CallerPackCnt			= 0;

	m_CalleePackCnt			= 0;
	m_CalleePackIdx			= 0;
	m_CalleePackTime		= 0;
	m_CalleePackUnique		= 0;

	m_CallerIP				= *(unsigned int*)(packh.m_sip);
	m_CalleeIP				= *(unsigned int*)(packh.m_dip);
	m_CallerPORT			= packh.m_sport;
	m_CalleePORT			= packh.m_dport;
}



VoipMobileG723Session::~VoipMobileG723Session(){}



int VoipMobileG723Session::InsertPacket(DATEHEADER & packh,const char * pPacket)
{
	if (packh.m_type != UDPPacket)
	{
		return FALSE;
	}
	
	if( ( (*(unsigned short *)pPacket == 0x6b80) || (*(unsigned short *)pPacket == 0xeb80) )
		&& (packh.m_datalen == 44) )
	{
		if( (*(unsigned int*)packh.m_sip == m_CallerIP) && (*(unsigned int*)packh.m_dip == m_CalleeIP) &&
			(packh.m_sport == m_CallerPORT) && (packh.m_dport == m_CalleePORT) )
		{
			if ( *(unsigned int*)(pPacket + 8) != m_CallerPackUnique)
			{
				return FALSE;
			}
			
			m_CallerPackIdx			= ntohs(*(unsigned short*)(pPacket + 2) );
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
			return true;
		}
		else if( (*(unsigned int*)packh.m_sip == m_CalleeIP) && (*(unsigned int*)packh.m_dip == m_CallerIP) &&
			(packh.m_sport == m_CalleePORT) && (packh.m_dport == m_CallerPORT) )
		{
			if( (m_CalleePackCnt == 0) && (m_CalleePackUnique == 0) )
			{
				m_CalleePackUnique	= *(unsigned int*)(pPacket + 8);
			}
			else if ( *(unsigned int*)(pPacket + 8) != m_CalleePackUnique)
			{
				return FALSE;
			}
			
			m_CalleePackIdx		= ntohs(*(unsigned short*)(pPacket + 2) );
			m_CalleePackTime	= *(unsigned int*)(pPacket + 4);
			m_CalleePackCnt ++;
			packh.m_datalen -= 12;
			VOIPPACKET pack(packh, (CHAR *)(pPacket + 12),m_CalleePackIdx);
			m_CalleePackList.push_back(pack);
			m_lastpacktm = packh.m_tm;
			if (abs(packh.m_tm - m_processtm) > 60)
			{
				m_processtm = packh.m_tm - 20;
				ListToFile();
			}
			return true;
		}
	}
	return 0;
}



int VoipMobileG723Session::ListToFile()
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
		m_realfname = CreateMediaFilename("voip_mobile_g723", m_sip, m_dip);
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
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP MOBILE G723 Error!!");
		return -3;
	}
	return 0;
}

