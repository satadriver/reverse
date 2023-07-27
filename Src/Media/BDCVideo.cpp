#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "BDCVideo.h"
#include "winsock.h"

BOOL WriteIndexFile(BDCSession* s, time_t fendtm);

BDCSession::BDCSession(DATEHEADER& packh, int aT)
: MediaSession(packh, aT)
{
	m_processtm = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_Packettype = packh.m_type;

	m_MediaPro = _T("media_讯通");		// 协议类型(即对照文件中的协议字段值)
	m_MediaDirection = _T("<<==>>");	// 音视频方向(即对照文件中的方向字段值)
	m_AvType = 3;						// 实体文件类型(1:视频 / 2:音频 / 3:音视频)
	m_IDCSvrType = _T("bdc");			// IDC服务器类型(英文表示) 为空或NULL表示不需要输出IDCServer
	m_IDCSvrSubType = _T("讯通音视频");	// IDC服务器类型(中文表示) 为空或NULL表示不需要输出IDCServer
}

int BDCSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	ASSERT(packh.m_type == TCPPacket);
	MEDIAPACKET bdcpack(packh, (CHAR *)pPacket);
	if (bdcpack.m_buf == NULL)
	{
		return 0;
	}

	if (memcmp(packh.m_sip, this->m_cdip, 4) == 0)
	{
		bdcpack.m_vtype |= 0x40;
	}
	m_packetlist.push_back(bdcpack);
	bdcpack.m_buf = NULL;
	m_lastpacktm = packh.m_tm;
	if (abs(packh.m_tm - m_processtm) > 60)
	{
		m_processtm = packh.m_tm - 20;
		ListToFile();	// 写文件
	}
	return 0;
}

int BDCSession::ListToFile()
{
	m_OpenIndxFlg = v_openflagBDCMedia;
	m_OpenCaseFlg = v_opencaseBDCMedia;
	m_OpenWarnFlg = v_openwarnBDCMedia;

	m_packetlist.sort();
	MediaPacketListIt item, begin, end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("bdc", m_sip, m_dip);
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
		begin = m_packetlist.begin();
		end=m_packetlist.end();
		while(begin != end)
		{
			item = begin;
			begin++;
#ifdef		_DEBUG
			MEDIAPACKET	*p1, *p2;
			p1 = &(*item);
			p2 = &(*begin);
			printf("tcpseq=%08x\ttcpack=%08x\tnseq=%08x\n",p1->m_TCPSeq,p1->m_TCPAck,p1->m_TCPSeq+p1->m_datalen);
#endif
			m_endfiletm = item->m_tm;
			WriteMediaFile(file, (item->m_vtype & 0x40)? h2: h1, *item);
			m_packetlist.erase(item);
		}
		file.Close();
	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "LstToBDCFile() Error!!");
		return -3;
	}
	return 0;
}

