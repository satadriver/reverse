#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "VoipKuBaoSession.h"



VoipKuBaoSession::VoipKuBaoSession(DATEHEADER& packh, int aT, char * pPacket) : MediaSession(packh,aT)
{
	m_processtm				= packh.m_tm;
	m_begfiletm				= packh.m_tm;
	m_Packettype			= packh.m_type;
	m_MediaPro				= _T("media_voip_kubao");		// Э������(�������ļ��е�Э���ֶ�ֵ)

	m_VoipFileHdr.Protocol	= 0x05;
	m_VoipFileHdr.UserData	= 0;
	m_VoipFileHdr.Version	= 0;
	m_VoipFileHdr.Undefine	= 0;
	m_VoipFileHdr.CallerPackCnt = 0;
	m_VoipFileHdr.CalleePackCnt = 0;
	m_VoipFileHdr.HeaderLen = sizeof(VOIPFILEHEADER) - 16;		//Ϊ�˼�����ǰ������ �ڼ���ͷ����ʱ�����ȥ16�ֽ�


	m_CallerPackCnt			= 0;
	m_CallerPackIdx			= 0;
	m_CallerPackTime		= 0;
	m_CallerPackUnique		= 0;

	m_CalleePackIdx			= ntohs(~*(unsigned short*)(pPacket + 10) );
	m_CalleePackTime		= ~*(unsigned int*)(pPacket + 12);
	m_CalleePackUnique		= ~*(unsigned int*)(pPacket + 16);
	m_CalleePackCnt			= 0;

	m_CalleeIP				= *(unsigned int*)(packh.m_sip);
	m_CallerIP				= *(unsigned int*)(packh.m_dip);
	m_CalleePORT			= packh.m_sport;
	m_CallerPORT			= packh.m_dport;
}


VoipKuBaoSession::~VoipKuBaoSession(){ }


BOOL VoipKuBaoSession::IsVoipKuBao(DATEHEADER& packh,const char *pPacket)
{
	if (packh.m_type != UDPPacket)
	{
		return FALSE;
	}

	if( (packh.m_dport == 9000) && (packh.m_datalen == 180) && 
		((*(unsigned short *)(pPacket + 8) == 0x7f7f) || (*(unsigned short *)(pPacket + 8) == 0xff7f) ) )
	{
		return TRUE;
	}
	return FALSE;	
}



int VoipKuBaoSession::InsertPacket(DATEHEADER & packh,const char * pPacket)
{
	if (m_Packettype != UDPPacket)
	{
		return FALSE;
	}

	if( (packh.m_dport == 9000) && (packh.m_datalen == 180) && 
		((*(unsigned short *)(pPacket + 8) == 0x7f7f) || (*(unsigned short *)(pPacket + 8) == 0xff7f) ) )
	{
		char RtpHdr[12] = {0};
		memcpy(RtpHdr, pPacket + 8, 12);
		for (int Counter = 0; Counter < 12; Counter++)
		{
			RtpHdr[Counter] = ~RtpHdr[Counter];
		}

		if( (*(unsigned int*)packh.m_sip == m_CalleeIP) && (*(unsigned int*)packh.m_dip == m_CallerIP) &&
			(packh.m_sport == m_CalleePORT) && (packh.m_dport == m_CallerPORT) )
		{
			if ( *(unsigned int*)(RtpHdr + 8) != m_CalleePackUnique)
			{
				return FALSE;
			}
			m_CalleePackIdx			= ntohs(~*(unsigned short*)(pPacket + 10) );
			
			m_CalleePackCnt ++;
			packh.m_datalen -= 20;
			VOIPPACKET pack(packh, (CHAR *)(pPacket + 20),m_CalleePackIdx);
			m_CalleePackList.push_back(pack);
			m_lastpacktm = packh.m_tm;
			return TRUE;
		}
	}
	else if( (packh.m_sport == 9000) && (packh.m_datalen == 180) && 
		((*(unsigned short *)(pPacket + 8) == 0x7f7f) || (*(unsigned short *)(pPacket + 8) == 0xff7f) ) )
	{
		if( (*(unsigned int*)packh.m_sip == m_CallerIP) && (*(unsigned int*)packh.m_dip == m_CalleeIP) &&
			(packh.m_sport == m_CallerPORT) && (packh.m_dport == m_CalleePORT) )
		{
			char RtpHdr[12] = {0};
			memcpy(RtpHdr, pPacket + 8, 12);
			for (int Counter = 0; Counter < 12; Counter++)
			{
				RtpHdr[Counter] = ~RtpHdr[Counter];
			}

			if( (m_CallerPackCnt == 0) && (m_CallerPackUnique == 0) )
			{
				m_CallerPackUnique	= *(unsigned int*)(RtpHdr + 8);
			}
			else if ( *(unsigned int*)(RtpHdr + 8) != m_CallerPackUnique)
			{
				return FALSE;
			}
			m_CallerPackIdx		= ntohs(*(unsigned short*)(RtpHdr + 2) );
			m_CallerPackTime	= *(unsigned int*)(RtpHdr + 4);

			m_CallerPackCnt ++;
			packh.m_datalen -= 20;
			VOIPPACKET pack(packh, (CHAR *)(pPacket + 20),m_CallerPackIdx);
			m_CallerPackList.push_back(pack);
			m_lastpacktm = packh.m_tm;
			return TRUE;
		}
	}
	return 0;
}



int VoipKuBaoSession::ListToFile()
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
		m_realfname = CreateMediaFilename("voip_kubao", m_sip, m_dip);
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
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP KUBAO Error!!");
		return -3;
	}
	return 0;
}