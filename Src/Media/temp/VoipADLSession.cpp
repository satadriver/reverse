
#include "stdafx.h"

#include "MediaSession.h"
#include "media.h"
#include "VoipADLSession.h"

VoipADLSession::VoipADLSession(DATEHEADER& packh, int aT) : MediaSession(packh,aT)
{
	
	m_processtm = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_Packettype = packh.m_type;
	
	m_MediaPro = _T("media_voip_adl");		// Э������(�������ļ��е�Э���ֶ�ֵ)
	m_MediaDirection = _T("<<==>>");	// ����Ƶ����(�������ļ��еķ����ֶ�ֵ)
//	m_IDCSvrType = _T("voipadlmedia");		// IDC����������(Ӣ�ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
//	m_IDCSvrSubType = _T("VOIPADL");	// IDC����������(���ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer

	m_type = "voip_media";
	m_decode = 0x12;
	m_flag = 0;
	m_packet_idx = 0;
	m_packet_time = 0;
	m_packet_unique = 0;

	m_callerip = *(unsigned int *)packh.m_sip;
	m_callerport = packh.m_sport;
	m_calleeip = *(unsigned int *)packh.m_dip;
	m_calleeport = packh.m_dport;
}

VoipADLSession::~VoipADLSession()
{

}


BOOL VoipADLSession::IsVoipADL(DATEHEADER& packh,const char *pPacket)
{
	if( ((*(unsigned short *)pPacket == 0x1280) || (*(unsigned short *)pPacket == 0x9280) )
		&& (packh.m_datalen == 32) && 
		(packh.m_dport != 15004) && (packh.m_sport != 15004) &&
		(packh.m_sport != 7078) && (packh.m_sport != 5000) && 
		(packh.m_sport != 40000) && (packh.m_sport != 40002) &&
		(packh.m_sport != 40004) && (packh.m_sport != 40006))
	{
		return TRUE;
	}
	return FALSE;
}

int VoipADLSession::InsertPacket(DATEHEADER & packh,const char * pPacket)
{
	
	if( ((*(unsigned short *)pPacket == 0x1280) || (*(unsigned short *)pPacket == 0x9280) )
		&& (packh.m_datalen == 32) && 
		(packh.m_dport != 15004) && (packh.m_sport != 15004) &&
		(packh.m_sport != 7078) && (packh.m_sport != 5000) && 
		(packh.m_sport != 40000) && (packh.m_sport != 40002) &&
		(packh.m_sport != 40004) && (packh.m_sport != 40006) )			
	{
		
		if( (packh.m_sport == m_callerport) && (packh.m_dport == m_calleeport)  &&
			(*(unsigned int*)packh.m_sip == m_callerip) && (*(unsigned int*)packh.m_dip == m_calleeip) )
		{
			unsigned int tmp_unique = ntohl((*(unsigned int *)(pPacket + 8)));
			if ( 0 == m_packet_unique)
			{
				m_packet_unique = tmp_unique;
			}
			else
			{
				if (m_packet_unique != tmp_unique)
				{
					return 0;
				}
			}
			
			
			
			unsigned short tmpidx = ntohs( (*(unsigned short *) (pPacket + 2)));
			if (  tmpidx == m_packet_idx)
			{
				return 0;
			}
			else
			{
				m_packet_idx = tmpidx;
			}
			
			// 		if (0 == m_packet_time)
			// 		{
			// 			m_packet_time = ntohl( (*(unsigned int *) (pPacket + 4)));
			// 		}
			// 		else
			// 		{
			// 			unsigned int tmp_packet_time = ntohl( (*(unsigned int *)(pPacket + 4)));
			// 			if (0 == tmp_packet_time - m_packet_time)
			// 			{
			// 				return 0;
			// 				//�ط�
			// 			}
			// // 			else
			// // 			{
			// // 				if (0 != ((tmp_packet_time - m_packet_time) % 80) )
			// // 				{
			// // 					return 0;
			// // 					//Ψһ
			// // 				}
			// //  			}
			// 			m_packet_time = tmp_packet_time;
			// 		}
			
			
			
			
			packh.m_datalen -= 12;
			MEDIAPACKET pack(packh, (CHAR *)(pPacket + 12));
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
	}
	return 0;
}

int VoipADLSession::ListToFile()
{
	m_packetlist.sort();
	MediaPacketListIt item, begin,end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("voip_adl", m_sip, m_dip);
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
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP ADL Error!!");
		return -3;
	}
	return 0;
}