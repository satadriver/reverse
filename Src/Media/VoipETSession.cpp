#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "VoipETSession.h"


VoipETSession::VoipETSession(DATEHEADER& packh, int aT, char * pPacket): MediaSession(packh, aT)
{
	m_processtm				= packh.m_tm;
	m_begfiletm				= packh.m_tm;
	m_Packettype			= packh.m_type;
	m_MediaPro				= _T("media_voip_et");		

	m_VoipFileHdr.Protocol	= 0x10;
	m_VoipFileHdr.UserData	= 0;
	m_VoipFileHdr.Version	= 0;
	m_VoipFileHdr.Undefine	= 0;
	m_VoipFileHdr.CallerPackCnt = 0;
	m_VoipFileHdr.CalleePackCnt = 0;
	m_VoipFileHdr.HeaderLen = sizeof(VOIPFILEHEADER) - 16;		//为了兼容以前的数据 在计算头长度时必须减去16字节


	m_CalleePackIdx			= ntohs(*(unsigned short*)(pPacket + 28) );
	m_CalleePackTime		= *(unsigned int*)(pPacket + 24);
	m_CalleePackUnique		= *(unsigned int*)(pPacket + 20);
	m_CalleePackCnt			= 0;

	m_CallerPackIdx			= 0;
	m_CallerPackTime		= 0;
	m_CallerPackCnt			= 0;
	m_CallerPackUnique		= 0;

	m_CalleeIP				= *(unsigned int*)(packh.m_sip);
	m_CallerIP				= *(unsigned int*)(packh.m_dip);
	m_CalleePORT			= packh.m_sport;
	m_CallerPORT			= packh.m_dport;
}


VoipETSession::~VoipETSession(){ }



BOOL VoipETSession::IsVoipET(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type != UDPPacket)
	{
		return FALSE;
	}

	if ( ( (*(unsigned short *)(pPacket + 30) == 0x1280) || (*(unsigned short *)(pPacket + 30) == 0x9280) ) && 
		 (32 == packh.m_datalen) )
	{
		return TRUE;
	}
	return FALSE;
}


int VoipETSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (m_Packettype != UDPPacket)
	{
		return FALSE;
	}

	if ( ( (*(unsigned short *)(pPacket + 30) == 0x1280) || (*(unsigned short *)(pPacket + 30) == 0x9280) ) && 
		 (32 == packh.m_datalen) )		
	{	
		if( (*(unsigned int*)packh.m_dip == m_CallerIP) && (*(unsigned int*)packh.m_sip == m_CalleeIP) &&
			(packh.m_dport == m_CallerPORT) && (packh.m_sport == m_CalleePORT) )
		{
			if ( *(unsigned int*)(pPacket + 20) != m_CalleePackUnique)
			{
				return FALSE;
			}

			unsigned short TmpBuf[16] = {0};
			unsigned short * TmpBufPtr = TmpBuf;
			unsigned short * pPacketPtr = (unsigned short *)(pPacket + 30);
			int Cnt = 16;
			for(; Cnt ; Cnt --)
			{
				*TmpBufPtr = *pPacketPtr;
				TmpBufPtr ++;
				pPacketPtr --;
			}
			m_CalleePackIdx			= ntohs(*(unsigned short*)(pPacket + 28) );

			m_CalleePackCnt ++;
			packh.m_datalen = 20;
			VOIPPACKET pack(packh, (const char*)((unsigned int)TmpBuf + 12),m_CalleePackIdx);
			m_CalleePackList.push_back(pack);
			m_lastpacktm = packh.m_tm;
			return TRUE;
		}
	}
	else if ( ( (*(unsigned short *)(pPacket + 38) == 0x1280) || (*(unsigned short *)(pPacket + 38) == 0x9280) ) && 
		(42 == packh.m_datalen) )		
	{	
		if( (*(unsigned int*)packh.m_sip == m_CallerIP) && (*(unsigned int*)packh.m_dip == m_CalleeIP) &&
			(packh.m_sport == m_CallerPORT) && (packh.m_dport == m_CalleePORT) )
		{
			if( (m_CallerPackCnt == 0) && (m_CallerPackUnique == 0) )
			{
				m_CallerPackUnique	= *(unsigned int*)(pPacket + 28);
			}
			else if ( *(unsigned int*)(pPacket + 28) != m_CallerPackUnique)
			{
				return FALSE;
			}
			m_CallerPackIdx		= ntohs(*(unsigned short*)(pPacket + 36) );
			m_CallerPackTime	= *(unsigned int*)(pPacket + 32);
			
			unsigned short TmpBuf[21] = {0};
			unsigned short * TmpBufPtr = TmpBuf;
			unsigned short * pPacketPtr = (unsigned short *)(pPacket + 38);
			int Cnt = 16;
			for(; Cnt ; Cnt --)
			{
				*TmpBufPtr = *pPacketPtr;
				TmpBufPtr ++;
				pPacketPtr --;
			}

			m_CallerPackCnt ++;
			packh.m_datalen = 20;
			VOIPPACKET pack(packh, (CHAR *)((unsigned int)TmpBuf + 12),m_CallerPackIdx);
			m_CallerPackList.push_back(pack);
			m_lastpacktm = packh.m_tm;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}


int VoipETSession::ListToFile()
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
		m_realfname = CreateMediaFilename("voip_et", m_sip, m_dip);
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
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP ET Error!!");
		return -3;
	}
	return 0;
}
