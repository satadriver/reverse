#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "voip.h"
#include "Voip97CallSession.h"


Voip97CallSession::Voip97CallSession(DATEHEADER& packh, int aT, char * pPacket) : MediaSession(packh,aT)
{
	m_processtm				= packh.m_tm;
	m_begfiletm				= packh.m_tm;
	m_Packettype			= packh.m_type;
	m_MediaPro				= _T("media_voip_97call");		

	m_VoipFileHdr.Protocol	= 0x02;
	m_VoipFileHdr.UserData	= 0;
	m_VoipFileHdr.Version	= 0;
	m_VoipFileHdr.Undefine	= 0;
	m_VoipFileHdr.CallerPackCnt = 0;
	m_VoipFileHdr.CalleePackCnt = 0;
	m_VoipFileHdr.HeaderLen = sizeof(VOIPFILEHEADER) - 16;		//为了兼容以前的数据 在计算头长度时必须减去16字节

	m_CallerPackCnt			= 0;
	m_CallerPackUnique		= 0;
	m_CallerPackTime		= 0;
	m_CallerPackIdx			= 0;

	m_CalleePackCnt			= 0;
	m_CalleePackUnique		= 0;
	m_CalleePackTime		= 0;
	m_CalleePackIdx			= 0;

	m_CalleeIP				= *(unsigned int*)(packh.m_sip);
	m_CallerIP				= *(unsigned int*)(packh.m_dip);
	m_CalleePORT			= packh.m_sport;
	m_CallerPORT			= packh.m_dport;
}



Voip97CallSession::~Voip97CallSession() { }



BOOL Voip97CallSession::IsVoip97Call(DATEHEADER& packh,const char *pPacket)		//主叫33字节 被叫34字节 过滤器不考虑长度全部放行
{
	if (packh.m_type != UDPPacket)
	{
		return FALSE;
	}

	if( (*(unsigned short *)pPacket == 0x2000) && (packh.m_datalen == 33) )
	{
		return TRUE;
	}
	return FALSE;
}



int Voip97CallSession::InsertPacket(DATEHEADER &packh,const char * pPacket)
{
	if (m_Packettype != UDPPacket)
	{
		return false;
	}

	if( (*(unsigned short *)pPacket == 0x2000) && (packh.m_datalen == 33) )
	{	
		if( (*(unsigned int*)packh.m_sip == m_CalleeIP) && (*(unsigned int*)packh.m_dip == m_CallerIP) &&
			(packh.m_sport == m_CalleePORT) && (packh.m_dport == m_CallerPORT) )
		{
			unsigned char MaskByte = *(pPacket + 2);
			if ( MaskByte & 0x80)
			{
				MaskByte ^= 0x92;
			}
			else
			{
				MaskByte ^= 0x12;
			}
			char RtpHdr[12] = {0};
			memmove(RtpHdr, pPacket + 1, 12);
			for (int Counter = 0; Counter < 12; Counter++)
			{
				RtpHdr[Counter] ^= MaskByte;
			}

			if( (m_CalleePackCnt == 0) && (m_CalleePackUnique == 0) )
			{
				m_CalleePackUnique	= *(unsigned int*)(RtpHdr + 8);
			}
			else if( *(unsigned int*)(RtpHdr + 8) != m_CalleePackUnique)
			{
				return FALSE;
			}
			m_CalleePackIdx		= ntohs(*(unsigned short*)(RtpHdr + 2));
			m_CalleePackTime	= *(unsigned int*)(RtpHdr + 4);

			m_CalleePackCnt ++;
			packh.m_datalen = 20;
			VOIPPACKET pack(packh, (CHAR *)(pPacket + 13),m_CalleePackIdx);
			m_CalleePackList.push_back(pack);

			m_lastpacktm = packh.m_tm;
			m_VoipFileHdr.UserData = (unsigned int)(*(pPacket + 2));
			return TRUE;
		}
	}
	else if( (*(unsigned short *)pPacket == 0x2000) && (packh.m_datalen == 34) )
	{	
		if( (*(unsigned int*)packh.m_sip == m_CallerIP) && (*(unsigned int*)packh.m_dip == m_CalleeIP) &&
			(packh.m_sport == m_CallerPORT) && (packh.m_dport == m_CalleePORT) )
		{
			unsigned char MaskByte = *(pPacket + 2);
			if ( MaskByte & 0x80)
			{
				MaskByte ^= 0x92;
			}
			else
			{
				MaskByte ^= 0x12;
			}
			char RtpHdr[12] = {0};
			memmove(RtpHdr, pPacket + 1, 12);
			for (int Counter = 0; Counter < 12; Counter++)
			{
				RtpHdr[Counter] ^= MaskByte;
			}

			if( (m_CallerPackCnt == 0) && (m_CallerPackUnique == 0) )
			{
				m_CallerPackUnique	= *(unsigned int*)(RtpHdr + 8);
			}
			else if( *(unsigned int*)(RtpHdr + 8) != m_CallerPackUnique)
			{
				return FALSE;
			}
			m_CallerPackIdx		= ntohs(*(unsigned short*)(RtpHdr + 2));
			m_CallerPackTime	= *(unsigned int*)(RtpHdr + 4);

			m_CallerPackCnt ++;
			packh.m_datalen = 20;
			VOIPPACKET pack (packh, (CHAR *)(pPacket + 13),m_CallerPackIdx);
			m_CallerPackList.push_back(pack);
			m_lastpacktm = packh.m_tm;
			m_VoipFileHdr.UserData = (unsigned int)(*(pPacket + 2));
			return TRUE;
		}
	}
	return 0;
}



int Voip97CallSession::ListToFile()
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


	/*
	//voip-call-relate

	SVOIP_CALL_INFO *pVoipCallInfo = GetVoipCallInfo(m_strVoipCallKey);
	if(pVoipCallInfo != NULL)
	{
		m_strText.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\ntype=voip_media\r\n",
			pVoipCallInfo->strUser,pVoipCallInfo->strPass,pVoipCallInfo->strFrom,pVoipCallInfo->strTo);
		delete pVoipCallInfo;
		g_mapVoipCall.erase(m_strVoipCallKey);
	}
	else
	{
		m_strText = "type=voip_media\r\n";
	}
	
	//
	*/


//	m_CallerPackList.sort();
//	m_CalleePackList.sort();	
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("voip_97call", m_sip, m_dip);			//自动加上media
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
		if(!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return -2;
		}
	}
	file.SeekToEnd();
	
	try
	{
		while(m_CallerPackList.size() )
		{
			begin = m_CallerPackList.begin();
			m_endfiletm = begin->m_tm;
			file.Write(begin->m_buf,begin->m_datalen);
			m_CallerPackList.erase(begin);		
		}

		while(m_CalleePackList.size() )
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
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP 97CALL Error!!");
		return -3;
	}
	return 0;
}