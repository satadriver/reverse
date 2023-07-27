
#include "stdafx.h"

#include "MediaSession.h"
#include "media.h"
#include "VoipRuYiBaoSession.h"

VoipRuYiBaoSession::VoipRuYiBaoSession(DATEHEADER& packh, int aT) : MediaSession(packh,aT)
{
	
	m_processtm = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_Packettype = packh.m_type;
	
	m_MediaPro = _T("media_voip_ryb");		// 协议类型(即对照文件中的协议字段值)
	m_MediaDirection = _T("<<==>>");	// 音视频方向(即对照文件中的方向字段值)
//	m_IDCSvrType = _T("voipruyibao");		// IDC服务器类型(英文表示) 为空或NULL表示不需要输出IDCServer
//	m_IDCSvrSubType = _T("VOIPRUYIBAO");	// IDC服务器类型(中文表示) 为空或NULL表示不需要输出IDCServer
//	m_decode = 
}

VoipRuYiBaoSession::~VoipRuYiBaoSession()
{

}


BOOL VoipRuYiBaoSession::IsVoipRuYiBao(DATEHEADER& packh,const char *pPacket)
{
	if (((*(unsigned short *)pPacket == 0x1280) || 
		(*(unsigned short *)pPacket == 0x9280)) && 
		(32 == packh.m_datalen) && (packh.m_sport == 5000))
	{
		return TRUE;
	}

	return FALSE;
}

int VoipRuYiBaoSession::InsertPacket(DATEHEADER & packh,const char * pPacket)
{
	packh.m_datalen -= 12;
	MEDIAPACKET kcpack(packh, (CHAR *)(pPacket + 12));
	if( (*(unsigned short *)pPacket == 0x1280) || (*(unsigned short *)pPacket == 0x9280) )			
	{
		
		m_packetlist.push_back(kcpack);
		kcpack.m_buf = NULL;
		m_lastpacktm = packh.m_tm;
		if (abs(packh.m_tm - m_processtm) > 60)
		{
			// 写文件
			m_processtm = packh.m_tm - 20;
			ListToFile();
		}
	}

	return false;
}

int VoipRuYiBaoSession::ListToFile()
{
	m_packetlist.sort();
	MediaPacketListIt item, begin,end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("voip_ryb", m_sip, m_dip);
		if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return -2;
		}
		file.Write(m_decode,4);
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
		WriteLog(MEDIAPRO, "LstToMSNFile() Error!!");
		return -3;
	}
	return 0;
}