#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "UcVideo.h"

UcSession::UcSession(DATEHEADER& packh, int aT)
: MediaSession(packh, aT)
{
	m_begfiletm = packh.m_tm;
	m_processtm = packh.m_tm;

	m_MediaPro = _T("media_UC");		// 协议类型(即对照文件中的协议字段值)
	m_MediaDirection = _T("<<==>>");		// 音视频方向(即对照文件中的方向字段值)
	m_IDCSvrType = _T("ucmedia");			// IDC服务器类型(英文表示) 为空或NULL表示不需要输出IDCServer
	m_IDCSvrSubType = _T("UC音视频");	// IDC服务器类型(中文表示) 为空或NULL表示不需要输出IDCServer
	m_AvType = 2;	// 音频
}


int UcSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	MEDIAPACKET ucpack(packh, pPacket);
	if (ucpack.m_buf==NULL)
		return 0;
 
	m_AvType = 3;		// 音视频
	 
	
	if (memcmp(packh.m_sip, m_cdip, 4) == 0)
	{
		ucpack.m_vtype |= 0x40;
	}
	m_packetlist.push_back(ucpack);
	ucpack.m_buf=NULL;
	m_lastpacktm = packh.m_tm;
	if (abs(packh.m_tm-m_processtm) > 60)
	{
		// 写文件
		m_processtm = packh.m_tm - 20;
		ListToFile();
	}
	return 0;
}

int UcSession::ListToFile()
{
	m_packetlist.sort();			// 排序

	MediaPacketListIt item, begin,end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		if(m_sinatype==1)//sinashow 
			m_realfname = CreateMediaFilename("ucv1", m_sip, m_dip);
		else  //sinauc 
			m_realfname = CreateMediaFilename("ucv2", m_sip, m_dip);

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
		h1.m_type = h2.m_type = UDPPacket;
		memcpy(h1.m_sip, this->m_csip, 4);
		memcpy(h1.m_dip, this->m_cdip, 4);
		memcpy(h2.m_dip, this->m_csip, 4);
		memcpy(h2.m_sip, this->m_cdip, 4);
		h1.m_sport = h2.m_dport = atoi(this->m_sip.Right(5));
		h1.m_dport = h2.m_sport = atoi(this->m_dip.Right(5));

		m_OpenWarnFlg = v_openwarnUCMedia;
		m_OpenCaseFlg = v_opencaseUCMedia;
		m_OpenIndxFlg = v_openflagUCMedia;

		while(m_packetlist.size()>10)
		{
			begin = m_packetlist.begin();
			item = begin;
			end=m_packetlist.end();
			item++;
// 			for (int i = 1; i < min(m_packetlist.size(), 40); i++)
// 			{
// 				if(item==end)
// 					break;
// #ifdef		_DEBUG
// 				MEDIAPACKET	*p1, *p2;
// 				p1 = &(*item);
// 				p2 = &(*begin);
// #endif
// 				if (begin->m_vtype == item->m_vtype)
// 				{
// 					if(begin->m_serial ==0 && item->m_serial==0)
// 					{
// 						break;
// 					}
// 					else
// 					{
// 						if ((begin->m_serial-item->m_serial)==0)
// 						{
// 							MediaPacketListIt itemtmp;
// 							itemtmp=item;
// 							item++;						
// 							m_packetlist.erase(itemtmp);
// 
// 							continue;
// 							
// 						}
// 						else if((WORD)(begin->m_serial-item->m_serial)<0x8000 && abs(begin->m_tm-item->m_tm)<=2 ){
// 							MediaPacketListIt itemtmp;
// 							itemtmp=item;
// 							itemtmp++;
// 							swap(begin,item);
// 							item=itemtmp;
// 							continue;
// 						}
// 					}
// 				}
// 				item++;
// 			}
// 			if (begin->m_tm>m_processtm)
// 				break;
			m_endfiletm = begin->m_tm;
			WriteMediaFile(file, (begin->m_vtype&0x40)? h2: h1, *begin);
			m_packetlist.erase(begin);
		}

		file.Close();
	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "LstToUcFile() Error!!");
		return -3;
	}
	return 0;
}

