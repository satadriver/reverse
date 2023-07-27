// UUTong.cpp: implementation of the UUTong class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "UUTong.h"


UUTongSession::UUTongSession(DATEHEADER& packh, int aT)
: MediaSession(packh, aT)
{
	memset(m_serverip,0,4);
	m_processtm = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_Packettype = packh.m_type;

	m_MediaPro = _T("media_UUTong");		// 协议类型(即对照文件中的协议字段值)
	m_MediaDirection = _T("<<==>>");		// 音视频方向(即对照文件中的方向字段值)
	m_IDCSvrType = _T("uutong");			// IDC服务器类型(英文表示) 为空或NULL表示不需要输出IDCServer
	m_IDCSvrSubType = _T("UU通音频");	// IDC服务器类型(中文表示) 为空或NULL表示不需要输出IDCServer
	m_AvType = 2;	// 音频

}

int UUTongSession::InsertPacket(DATEHEADER & packh,const char *pPacket)
{
// 	if(this->CaseID.IsEmpty() && !v_openUUTongMedia)
// 		return 0;	
	MEDIAPACKET packet(packh, (CHAR *)pPacket);
	
	if(packh.m_type==UDPPacket)
	{
		packet.m_serial=packh.m_ipid;
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
		// 排序
		m_processtm = packh.m_tm - 20;
		// 写文件
		ListToFile();
	}
	return 0;

}

int UUTongSession::ListToFile()
{
	m_OpenWarnFlg = v_openwarnUUTongMedia;
	m_OpenCaseFlg = v_opencaseUUTongMedia;
	m_OpenIndxFlg = v_openUUTongMedia;

	m_packetlist.sort();
	MediaPacketListIt item, begin,end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("uut", m_sip, m_dip);
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
#ifdef		_DEBUG
			MEDIAPACKET	*p1, *p2;
			p1 = &(*item);
			p2 = &(*begin);
#endif
			if(this->m_Packettype==UDPPacket){
				for (int i = 1; i < min(m_packetlist.size(), 40); i++)
				{
					if(item==end)
						break;
					if (begin->m_vtype == item->m_vtype)
					{
						if ((begin->m_serial-item->m_serial)==0)
						{
							MediaPacketListIt itemtmp;
							itemtmp=item;
							item++;							
							m_packetlist.erase(itemtmp);
							continue;
							
						}
						else if((WORD)(begin->m_serial-item->m_serial)<0x8000 && abs(begin->m_tm-item->m_tm)<=2 ){
							MediaPacketListIt itemtmp;
							itemtmp=item;
							itemtmp++;
							swap(begin,item);
							item=itemtmp;
							continue;
						}
					}
					item++;
				}
				if (begin->m_tm > m_processtm)
					break;
			}
			Currenttm = begin->m_tm;
			m_endfiletm = begin->m_tm;
			WriteMediaFile(file, (begin->m_vtype & 0x40)? h2: h1, *begin);
			m_packetlist.erase(begin);
		}
		file.Close();
	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "LstToUUTongFile() Error!!");
		return -3;
	}
	return 0;
}

