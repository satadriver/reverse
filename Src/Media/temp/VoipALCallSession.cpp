
#include "stdafx.h"

#include "MediaSession.h"
#include "media.h"
#include "VoipALCallSession.h"

int __stdcall VoipALCallSession::DecodeALICALL(unsigned int * Packet,int PacketSize)
{	
	for(; PacketSize >= 4; PacketSize -= 4)		
	{
		*Packet ^= 0x57375737;		
		Packet ++ ;
	}
	
	if(PacketSize)
	{
		* Packet ^= 0x57;
		Packet = (unsigned int *)((unsigned int)Packet +1);
		PacketSize --;	
	}
	else
	{
		return false;
	}
	
	if(PacketSize)
	{
		* Packet ^= 0x37;
		Packet = (unsigned int *)((unsigned int)Packet +1);
		PacketSize --;
	}
	else
	{
		return false;
	}
	
	if(PacketSize)
	{
		* Packet ^= 0x57;
		Packet = (unsigned int *)((unsigned int)Packet +1);
		PacketSize --;	
	}
	else
	{
		return false;
	}
	
	return true;
}



VoipALCallSession::VoipALCallSession(DATEHEADER& packh, int aT) : MediaSession(packh,aT)
{

	m_processtm = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_Packettype = packh.m_type;
	
	m_MediaPro = _T("media_voip_alcall");		// 协议类型(即对照文件中的协议字段值)
	m_MediaDirection = _T("<<==>>");	// 音视频方向(即对照文件中的方向字段值)
//	m_IDCSvrType = _T("voipcallmedia");		// IDC服务器类型(英文表示) 为空或NULL表示不需要输出IDCServer
//	m_IDCSvrSubType = _T("VOIPALCALL");	// IDC服务器类型(中文表示) 为空或NULL表示不需要输出IDCServer

	m_type = "voip_media";
	m_decode = 0x06;
	m_flag = 0;

	m_packet_idx = 0;
	m_packet_time = 0;
	m_packet_unique = 0;

	
	m_callerip = *(unsigned int*)packh.m_sip;
	m_callerport = packh.m_sport;
	m_calleeip = *(unsigned int*)packh.m_dip;
	m_calleeport = packh.m_dport;
}

VoipALCallSession::~VoipALCallSession()
{

}

BOOL VoipALCallSession::IsVoipALICall(DATEHEADER& packh,const char *pPacket)
{
	if(/* (packh.m_dport == 2606 ) &&*/
		(packh.m_datalen == 32) &&
		(( (*(unsigned short *)pPacket) == 0xc5b7) || (*(unsigned short *)pPacket == 0x45b7))
		)
	{
		return TRUE;
	}
	return FALSE;
}


int VoipALCallSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if( /*(packh.m_dport == 2606 ) &&*/
		(packh.m_datalen == 32) &&
		(( *(unsigned short *)pPacket == 0xc5b7) || (*(unsigned short *)pPacket == 0x45b7))
		)
	{
		if( (packh.m_sport == m_calleeport) && (packh.m_dport == m_callerport)  &&
			(*(unsigned int*)packh.m_sip == m_calleeip) && (*(unsigned int*)packh.m_dip == m_callerip) )
		{
			char tmp_packet[12] = {0};
			memcpy(tmp_packet,pPacket,12);
			tmp_packet[0] = tmp_packet[0] ^ 0x37;
			tmp_packet[1] = tmp_packet[1] ^ 0x57;
			tmp_packet[2] = tmp_packet[2] ^ 0x37;
			tmp_packet[3] = tmp_packet[3] ^ 0x57;
			tmp_packet[4] = tmp_packet[4] ^ 0x37;
			tmp_packet[5] = tmp_packet[5] ^ 0x57;
			tmp_packet[6] = tmp_packet[6] ^ 0x37;
			tmp_packet[7] = tmp_packet[7] ^ 0x57;
			tmp_packet[8] = tmp_packet[8] ^ 0x37;
			tmp_packet[9] = tmp_packet[9] ^ 0x57;
			tmp_packet[10] = tmp_packet[10] ^ 0x37;
			tmp_packet[11] = tmp_packet[11] ^ 0x57;
			
			unsigned int tmp_unique = ntohl((*(unsigned int *)(tmp_packet + 8)));
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
			
			unsigned short tmpidx = ntohs( (*(unsigned short *) (tmp_packet + 2)));
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
			// 			m_packet_time = ntohl( (*(unsigned int *) (tmp_packet + 4)));
			// 		}
			// 		else
			// 		{
			// 			unsigned int tmp_packet_time = ntohl( (*(unsigned int *)(tmp_packet + 4)));
			// 			if (0 == tmp_packet_time - m_packet_time)
			// 			{
			// 				return 0;
			// 				//重发
			// 			}
			// 			else
			// 			{
			// 				if (0 != ((tmp_packet_time - m_packet_time) % 160) )
			// 				{
			// 					return 0;
			// 					//唯一
			// 				}
			// 			}
			// 			m_packet_time = tmp_packet_time;
			// 		}
			
			packh.m_datalen -= 12;
			MEDIAPACKET pack(packh, (CHAR *)(pPacket + 12));
			m_packetlist.push_back(pack);
			pack.m_buf = NULL;
			m_lastpacktm = packh.m_tm;
			if (abs(packh.m_tm - m_processtm) > 60)
			{
				// 写文件
				m_processtm = packh.m_tm - 20;
				ListToFile();
			}
		}
	}
	
	return 0;

}

int VoipALCallSession::ListToFile()
{
	m_packetlist.sort();
	MediaPacketListIt item, begin,end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("voip_alcall", m_sip, m_dip);
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
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP ALCALL Error!!");
		return -3;
	}
	return 0;
}