#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "MsnVideo.h"

BOOL WriteIndexFile(MSNSession* s, time_t fendtm);

MSNSession::MSNSession(DATEHEADER& packh, int aT)
: MediaSession(packh, aT)
{
	m_processtm = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_Packettype=packh.m_type;
	m_intmsnver=0;
	m_checknum=0;
	m_bhascheck=FALSE;
	m_SubAvType = 0;

	if (packh.m_type == UDPPacket)
	{// UDP
		if (isMSNVideo1(packh, (BYTE*)g_Packet) == 1)
		{
			m_SubAvType = 1;
		}
		else
		{
			m_SubAvType = 2;
		}
		if(memcmp(g_Packet, "\x4a\x0", 2) == 0)
		{
			m_intmsnver = 2;
			m_AvType = 3;
		}
		else
		{
			m_AvType = m_SubAvType;
		}
	}
	else
	{// TCP
		if(memcmp(g_Packet, "\x56\x20", 2) == 0)
		{
			m_intmsnver = 2;
			m_AvType = 3;
			m_SubAvType = 1;
		}
		else
		{
			m_SubAvType = m_AvType = 1;
		}
	}
	
	m_MediaPro = _T("media_MSN");		// 协议类型(即对照文件中的协议字段值)
	m_MediaDirection = _T("<<==>>");	// 音视频方向(即对照文件中的方向字段值)
	m_IDCSvrType = _T("msnmedia");		// IDC服务器类型(英文表示) 为空或NULL表示不需要输出IDCServer
	m_IDCSvrSubType = _T("MSN音视频");	// IDC服务器类型(中文表示) 为空或NULL表示不需要输出IDCServer
}

int MSNSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	MEDIAPACKET msnpack(packh, (CHAR *)pPacket);
	if(packh.m_type == UDPPacket)
	{
		int ret = isMSNVideo1(packh, (BYTE *)pPacket);
		if ((m_intmsnver == 2) && !m_bhascheck)
		{
			if ((packh.m_datalen > 20) 
				&& (memcmp(pPacket, "\x4a\x0", 2) == 0))
			{
				m_checknum++;
			}

			if (m_packetlist.size() > 30)
			{
				if (m_checknum < 10)
				{
					return -3;
				}
				else
				{
					m_bhascheck = TRUE;
				}
			}
		}

		if (m_SubAvType == 1)
		{
			msnpack.m_serial =0;
			msnpack.m_vtype = ret;
		}
		else if(m_SubAvType == 2 && ret == 2)
		{
			msnpack.m_serial = 0;
			msnpack.m_vtype = ret;
		}
		else
		{
			msnpack.m_serial = 0;
			msnpack.m_vtype = ret;
		}
	}
	else
	{
		if ((m_intmsnver == 2) && !m_bhascheck)
		{
			if ((packh.m_datalen > 20)
				&& (memcmp(pPacket, "\x56\x20", 2) == 0))
			{
				m_checknum++;
			}
			if (m_packetlist.size() > 30)
			{
				if (m_checknum < 10)
				{
					return -3;
				}
				else
				{
					m_bhascheck = TRUE;
				}
			}
		}
		msnpack.m_vtype = 1;
	}

	if (msnpack.m_buf==NULL)
	{
		return 0;
	}
	if (memcmp(packh.m_sip, m_cdip, 4) == 0)
	{
		msnpack.m_vtype |= 0x40;
	}
	m_packetlist.push_back(msnpack);
	msnpack.m_buf = NULL;
	m_lastpacktm = packh.m_tm;
	if (abs(packh.m_tm - m_processtm) > 60)
	{
		// 写文件
		m_processtm = packh.m_tm - 20;
		ListToFile();
	}
	return 0;
}

int MSNSession::ListToFile()
{
	m_OpenIndxFlg = v_openflagMSNMedia;
	m_OpenCaseFlg = v_opencaseMSNMedia;
	m_OpenWarnFlg = v_openwarnMSNMedia;

	m_packetlist.sort();
	MediaPacketListIt item, begin,end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("msn", m_sip, m_dip);
		if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return -2;
		}
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
			WriteMediaFile(file, (begin->m_vtype & 0x40)? h2: h1, *begin);
			m_packetlist.erase(begin);



// 
// #ifdef		_DEBUG
// 			MEDIAPACKET	*p1, *p2;
// 			p1 = &(*item);
// 			p2 = &(*begin);
// #endif
// 			if(m_Packettype == UDPPacket)
// 			{
// 				for (int i = 1; i < min(m_packetlist.size(), 40); i++)
// 				{
// 					if(item == end)
// 						break;
// 					if (begin->m_vtype == item->m_vtype)
// 					{
// 						if ((begin->m_serial - item->m_serial)==0)
// 						{
// 							MediaPacketListIt itemtmp;
// 							itemtmp=item;
// 							item++;							
// 							m_packetlist.erase(itemtmp);
// 							continue;
// 						}
// 						else if(((WORD)(begin->m_serial - item->m_serial) < 0x8000)
// 							&& (abs(begin->m_tm - item->m_tm) <= 2))
// 						{
// 							MediaPacketListIt itemtmp;
// 							itemtmp = item;
// 							itemtmp++;
// 							swap(begin, item);
// 							item=itemtmp;
// 							continue;
// 						}
// 					}
// 					item++;
// 				}
// 				if (begin->m_tm > m_processtm)
// 				{
// 					break;
// 				}
// 			}
// 			m_endfiletm = begin->m_tm;
// 			WriteMediaFile(file, (begin->m_vtype & 0x40)? h2: h1, *begin);
// 			m_packetlist.erase(begin);
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

