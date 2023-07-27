#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "VoipYMCallSession.h"


VoipYMCallSession::~VoipYMCallSession(){ }


VoipYMCallSession::VoipYMCallSession(DATEHEADER& packh, int aT, char * pPacket) : MediaSession(packh,aT)
{
	m_processtm				= packh.m_tm;
	m_begfiletm				= packh.m_tm;
	m_Packettype			= packh.m_type;
	m_MediaPro				= _T("media_voip_ymcall");	

	m_VoipFileHdr.Protocol	= 0x01;
	m_VoipFileHdr.UserData	= 0;
	m_VoipFileHdr.Version	= 0;
	m_VoipFileHdr.Undefine	= 0;
	m_VoipFileHdr.CallerPackCnt = 0;
	m_VoipFileHdr.CalleePackCnt = 0;
	m_VoipFileHdr.HeaderLen = sizeof(VOIPFILEHEADER) - 16;		//Ϊ�˼�����ǰ������ �ڼ���ͷ����ʱ�����ȥ16�ֽ�


	m_CalleePackCnt			= 0;
	m_CalleePackIdx			= 0;
	m_CalleePackTime		= 0;
	m_CalleePackUnique		= 0;

	m_CallerPackIdx			= ntohs(*(unsigned short*)(pPacket + 2) ^ 0xb0e7 );
	m_CallerPackTime		= *(unsigned int*)(pPacket + 4) ^ 0xa84e62f2;
	m_CallerPackUnique		= *(unsigned int*)(pPacket + 8) ^ 0x1ebe7fc5;
	m_CallerPackCnt			= 0;

	m_CallerIP				= *(unsigned int*)(packh.m_sip);
	m_CalleeIP				= *(unsigned int*)(packh.m_dip);
	m_CallerPORT			= packh.m_sport;
	m_CalleePORT			= packh.m_dport;
}


BOOL VoipYMCallSession::IsVoipYMCall(DATEHEADER& packh,const char *pPacket)
{
	if (packh.m_type != UDPPacket)
	{
		return FALSE;
	}

	if ( (packh.m_datalen == 45) &&
		 (  (*(unsigned short *)pPacket == 0x6CC8) || (*(unsigned short *)pPacket == 0XECC8)  )  )
	{
		return TRUE;
	}
	return FALSE;
}



int VoipYMCallSession::InsertPacket(DATEHEADER &packh, const char * pPacket)
{
	if (packh.m_type != UDPPacket)
	{
		return FALSE;
	}

	if( (packh.m_datalen == 45) &&
		( (*(unsigned short *)pPacket == 0x6CC8) || (*(unsigned short *)pPacket == 0XECC8) ) )			
	{
		unsigned char MaskBuf[12] ={0x48,0xef,0xe7,0xb0,0xf2,0x62,0x4e,0xa8,0xc5,0x7f,0xbe,0x1e};		
		unsigned char RtpHdr[12] = {0};
		memmove(RtpHdr, pPacket, 12);
		for( int Counter = 12; Counter ; Counter --)
		{
			*(RtpHdr + Counter) ^=  *(MaskBuf+ Counter);
		}

		if( (*(unsigned int*)packh.m_sip == m_CallerIP) && (*(unsigned int*)packh.m_dip == m_CalleeIP) &&
			(packh.m_sport == m_CallerPORT) && (packh.m_dport == m_CalleePORT) )
		{
			if ( *(unsigned int*)(RtpHdr + 8) != m_CallerPackUnique)
			{
				return FALSE;
			}	
			m_CallerPackIdx			= ntohs(*(unsigned short*)(pPacket + 2) ^ 0xb0e7 );

			m_CallerPackCnt ++;
			packh.m_datalen -= 12;
			VOIPPACKET pack(packh, (CHAR *)(pPacket + 12),m_CallerPackIdx);
			m_CallerPackList.push_back(pack);
			m_lastpacktm = packh.m_tm;
			return TRUE;
		}
		else if( (*(unsigned int*)packh.m_sip == m_CalleeIP) && (*(unsigned int*)packh.m_dip == m_CallerIP) &&
			(packh.m_sport == m_CalleePORT) && (packh.m_dport == m_CallerPORT) )
		{
			if( (m_CalleePackCnt == 0) || (m_CalleePackUnique == 0) )
			{
				m_CalleePackUnique	= *(unsigned int*)(RtpHdr + 8);
			}
			else if ( *(unsigned int*)(RtpHdr + 8) != m_CalleePackUnique)
			{
				return FALSE;
			}
			m_CalleePackIdx		= ntohs(*(unsigned short*)(RtpHdr + 2) );
			m_CalleePackTime	= *(unsigned int*)(RtpHdr + 4);			

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



int VoipYMCallSession::ListToFile()
{
	VoipPacketListIt item, begin,end;
	if( (m_CallerPackList.size() <= 100) || (m_CalleePackList.size() <= 100) )
	{
		while(m_CallerPackList.size() )
		{
			begin = m_CallerPackList.begin();
			m_CallerPackList.erase(begin);
		}
		
		while(m_CalleePackList.size() )
		{
			begin = m_CalleePackList.begin();
			m_CalleePackList.erase(begin);
		}	
		return FALSE;
	}
//	m_CallerPackList.sort();
//	m_CalleePackList.sort();

	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("voip_ymcall", m_sip, m_dip);
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
		WriteLog(MEDIAPRO, "ListToFile() In VOIP YMCALL Error!!");
		return -3;
	}
	return 0;
}