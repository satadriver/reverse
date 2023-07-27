
#include "stdafx.h"

#include "MediaSession.h"
#include "media.h"
#include "VoipYouXinSession.h"

VoipYouXinSession::VoipYouXinSession(DATEHEADER& packh, int aT) : MediaSession(packh,aT)
{
	
	m_processtm = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_Packettype = packh.m_type;
	
	m_MediaPro = _T("media_voip_yx");		// Э������(�������ļ��е�Э���ֶ�ֵ)
	m_MediaDirection = _T("<<==>>");	// ����Ƶ����(�������ļ��еķ����ֶ�ֵ)
//	m_IDCSvrType = _T("voipyouxinmedia");		// IDC����������(Ӣ�ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
//	m_IDCSvrSubType = _T("VOIPYOUXIN");	// IDC����������(���ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
	m_type = "voip_media";
	m_decode = 0x13;
	m_flag = 0;
	m_packet_idx = 0;
	m_packet_time = 0;
	m_packet_unique = 0;
}

VoipYouXinSession::~VoipYouXinSession()
{

}


BOOL VoipYouXinSession::IsVoipYouXin(DATEHEADER& dataheader,const char *pPacket)
{
	if( (*(unsigned short *)pPacket == 0x00c0) && 
		(dataheader.m_datalen == 24) && 
		(dataheader.m_dport == 15004))
	{
		return TRUE;
	}
	return FALSE;
}

int VoipYouXinSession::InsertPacket(DATEHEADER & packh,const char * pPacket)
{

	if( (*(unsigned short *)pPacket == 0x00c0) && 
		(packh.m_datalen == 24) && 
		(packh.m_dport == 15004) )			
	{
		unsigned short tmpidx = ntohs( (*(unsigned short *) (pPacket + 2)));
		if (  tmpidx == m_packet_idx)
		{
			return 0;
		}
		else
		{
			m_packet_idx = tmpidx;
		}
		
		packh.m_datalen -= 4;
		MEDIAPACKET pack(packh, (CHAR *)(pPacket + 4));
		m_packetlist.push_back(pack);
		pack.m_buf = NULL;
		m_lastpacktm = packh.m_tm;
		if (abs(packh.m_tm - m_processtm) > 60)
		{
			// д�ļ�
			m_processtm = packh.m_tm - 20;
			ListToFile();
		}
	}

	return 0;
}

int VoipYouXinSession::ListToFile()
{
	m_packetlist.sort();
	MediaPacketListIt item, begin,end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("voip_yx", m_sip, m_dip);
		if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return -2;
		}
		file.Write(&m_decode,4);
		file.Write(&m_flag,4);
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
		OLDDATEHEADER h1, h2;
		memset(&h1, 0, sizeof(h1));
		memset(&h2, 0, sizeof(h2));
		h1.m_type = h2.m_type = this->m_Packettype;
		memcpy(h1.m_sip, this->m_csip, 4);
		memcpy(h1.m_dip, this->m_cdip, 4);
		memcpy(h2.m_dip, this->m_csip, 4);
		memcpy(h2.m_sip, this->m_cdip, 4);
		h1.m_sport = h2.m_dport = atoi(this->m_sip.Right(5));
		h1.m_dport = h2.m_sport = atoi(this->m_dip.Right(5));
		while(m_packetlist.size()>10)
		{
			begin = m_packetlist.begin();
			item = begin;
			end=m_packetlist.end();
			item++;
			
			m_endfiletm = begin->m_tm;
			//	WriteMediaFile(file, (begin->m_vtype & 0x40)? h2: h1, *begin);
			//	WriteToFile(file,(begin->m_vtype & 0x40)? h2: h1, *begin);
			file.Write(begin->m_buf,begin->m_datalen);
			m_packetlist.erase(begin);
			
		}
		file.Close();
	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP YX Error!!");
		return -3;
	}
	return 0;
}