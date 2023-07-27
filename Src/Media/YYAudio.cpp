// YYAudio.cpp: implementation of the YYAudio class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "MediaSession.h"
#include "media.h"
#include "YYAudio.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

YYAudioSession::YYAudioSession(DATEHEADER& packh, int aT)
: MediaSession(packh, aT)
{

	m_processtm = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_Packettype = packh.m_type;
	
	m_MediaPro = _T("media_YY");		// Э������(�������ļ��е�Э���ֶ�ֵ)
	m_MediaDirection = _T("<<==>>");		// ����Ƶ����(�������ļ��еķ����ֶ�ֵ)
	m_IDCSvrType = _T("YY");			// IDC����������(Ӣ�ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
	m_IDCSvrSubType = _T("��������");	// IDC����������(���ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
	m_AvType = 2;	// ��Ƶ

}

YYAudioSession::~YYAudioSession()
{

}


int YYAudioSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{

	MEDIAPACKET packet(packh, (CHAR *)pPacket);
	
	if(packh.m_type==UDPPacket)
	{
		//packet.m_serial=packh.m_ipid;
	}
	packet.m_vtype=2;
	if (packet.m_buf==NULL)
		return 0;
	if (memcmp(packh.m_sip, this->m_cdip, 4) == 0)
		packet.m_vtype |= 0x40;
	m_packetlist.push_back(packet);
	packet.m_buf=NULL;
	m_lastpacktm = packh.m_tm;
	if (abs(packh.m_tm-m_processtm) > 60)
	{
		// ����
		m_processtm = packh.m_tm - 20;
		// д�ļ�
		ListToFile();
	}
	return 0;
}


int YYAudioSession::ListToFile()
{
	m_OpenWarnFlg = v_openwarnUUTongMedia;
	m_OpenCaseFlg = v_opencaseUUTongMedia;
	m_OpenIndxFlg = v_openUUTongMedia;
	
//	m_packetlist.sort();
	MediaPacketListIt item, begin,end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("YY", m_sip, m_dip);
		if (m_realfname.IsEmpty())
		{
			return -2;
		}
	}
	
	if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
	{
		return -2;
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
		time_t Currenttm;
		while(m_packetlist.size() > 0)
		{
			begin = m_packetlist.begin();
			item = begin;
			end= m_packetlist.end();
			item++;
		
			Currenttm = begin->m_tm;
			m_endfiletm = begin->m_tm;
			WriteMediaFile(file, (begin->m_vtype & 0x40)? h2: h1, *begin);
			m_packetlist.erase(begin);
		}
		file.Close();
	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "LstToYYAudioFile() Error!!");
		return -3;
	}
	return 0;
}