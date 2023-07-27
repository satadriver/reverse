#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "VoipMobileAiliaoSession.h"



BOOL VoipMobileAILIAOSession::IsVoipMobileAILIAO(DATEHEADER& packh,const char *pPacket)
{
	if (packh.m_type != UDPPacket)
	{
		return FALSE;
	}
	
	if( (packh.m_datalen == 47) && ( *(unsigned int*)pPacket == 0x00000087 || *(unsigned int*)pPacket == 0x00000086) 
		&& (*(pPacket + 6) == 0x14 ) )
	{
		return TRUE;
	}
	return FALSE;
}


VoipMobileAILIAOSession::VoipMobileAILIAOSession(DATEHEADER& packh, int aT, char * pPacket): MediaSession(packh,aT)
{
	m_processtm				= packh.m_tm;
	m_begfiletm				= packh.m_tm;
	m_Packettype			= packh.m_type;
	m_MediaPro				= _T("media_voip_m_ailiao");		

	m_VoipFileHdr.Protocol	= 0x08;
	m_VoipFileHdr.UserData	= *(unsigned char *) pPacket;		//87 86
	m_VoipFileHdr.Version	= 0;
	m_VoipFileHdr.Undefine	= 0;
	m_VoipFileHdr.CallerPackCnt = 0;
	m_VoipFileHdr.CalleePackCnt = 0;
	m_VoipFileHdr.HeaderLen = sizeof(VOIPFILEHEADER) - 16;		//为了兼容以前的数据 在计算头长度时必须减去16字节


	m_CalleePackCnt			= 0;
	m_CalleePackIdx			= 0;
	m_CalleePackTime		= 0;
	m_CalleePackUnique		= 0;

	m_CallerPackTime		= 0;
	m_CallerPackUnique		= 0;
	m_CallerPackIdx			= ntohs(*(unsigned short*)(pPacket + 4) );
	m_CallerPackCnt			= 0;

	m_CallerIP				= *(unsigned int*)(packh.m_sip);
	m_CalleeIP				= *(unsigned int*)(packh.m_dip);
	m_CallerPORT			= packh.m_sport;
	m_CalleePORT			= packh.m_dport;
}




VoipMobileAILIAOSession::~VoipMobileAILIAOSession() {}



int VoipMobileAILIAOSession::InsertPacket(DATEHEADER & packh,const char * pPacket)
{
	if (m_Packettype != UDPPacket)
	{
		return FALSE;
	}

	if( (packh.m_datalen != 47) || ( *(unsigned int*)pPacket != 0x00000087 && *(unsigned int*)pPacket != 0x00000086)
		|| (*(pPacket + 6) != 0x14 ) )		//注意书写的逻辑
	{
		return FALSE;
	}

	int Useless = 0;
	int Len = 0;
	if (m_VoipFileHdr.UserData == 0x86)
	{
		Useless =  7;
		Len = 20;
	}
	else if (m_VoipFileHdr.UserData == 0x87)
	{
		Useless =  27;
		Len = 20;
	}
	else
	{
		return FALSE;
	}

	if( (*(unsigned int*)packh.m_sip == m_CallerIP) && (*(unsigned int*)packh.m_dip == m_CalleeIP) &&
		(packh.m_sport == m_CallerPORT) && (packh.m_dport == m_CalleePORT) )
	{	
		m_CallerPackIdx			= ntohs(*(unsigned short*)(pPacket + 4) );

		m_CallerPackCnt ++;
		packh.m_datalen = Len;
		VOIPPACKET pack(packh, pPacket + Useless,m_CallerPackIdx);
		m_CallerPackList.push_back(pack);
		m_lastpacktm = packh.m_tm;
		if (abs(packh.m_tm - m_processtm) > 60)
		{
			m_processtm = packh.m_tm - 20;
			ListToFile();
		}
		return TRUE;
	}
	else if( (*(unsigned int*)packh.m_sip == m_CalleeIP) && (*(unsigned int*)packh.m_dip == m_CallerIP) &&
		(packh.m_sport == m_CalleePORT) && (packh.m_dport == m_CallerPORT) )
	{
		m_CalleePackIdx			= ntohs(*(unsigned short*)(pPacket + 4) );

		m_CalleePackCnt ++;
		packh.m_datalen = Len;
		VOIPPACKET pack(packh, pPacket + Useless,m_CalleePackIdx);
		m_CalleePackList.push_back(pack);
		m_lastpacktm = packh.m_tm;
		if (abs(packh.m_tm - m_processtm) > 60)
		{
			m_processtm = packh.m_tm - 20;
			ListToFile();
		}
		return TRUE;
	}
	return 0;
}



int VoipMobileAILIAOSession::ListToFile()
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
		m_realfname = CreateMediaFilename("media_voip_m_ailiao", m_sip, m_dip);
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
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP MOBILE ALICALL Error!!");
		return -3;
	}
	return 0;
}

