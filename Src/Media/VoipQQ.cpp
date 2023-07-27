
#include "StdAfx.h"
#include "VoipQQ.h"

#define PC_QQ_AUDIO_DATA_PACKETPOS	16

VoipQQSession::VoipQQSession(DATEHEADER& packh, int aT): MediaSession(packh, aT)
{
	VoipFileHdr.Protocol	= 0xf0;
	VoipFileHdr.UserData	= 0;
	VoipFileHdr.Version		= 0;
	VoipFileHdr.Undefine	= 0;
	VoipFileHdr.CallerPackCnt = 0;
	VoipFileHdr.CalleePackCnt = 0;
	VoipFileHdr.HeaderLen = sizeof(VOIPFILEHEADER) - 16 + 8;		//为了兼容以前的数据 在计算头长度时必须减去16字节 空出qq号码

	m_MediaPro = _T("media_voip_m_qq");	
	srcip = *(unsigned int*)packh.m_sip;
	dstip = *(unsigned int*)packh.m_dip;
	srcport = packh.m_sport;
	dstport = packh.m_dport;

	senderpackcnt = 0;
	recverpackcnt = 0;
	senderflag = 0;
	recverflag = 0;
	senderqq = 0;
	recverqq = 0;
	m_begfiletm = packh.m_tm;
	m_endfiletm = 0;
	m_closetm = packh.m_tm + 120;
}


VoipQQSession::~VoipQQSession(){}


int VoipQQSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{	
	int result = IsVoipQQPacket(packh,pPacket);
	if (result == 0)
	{
		return FALSE;
	}

	if (result == QQ_VOIP_CMD)
	{
		senderqq = ntohl(*(unsigned int*)(pPacket + 19));
		recverqq = ntohl(*(unsigned int*)(pPacket + 31));	
	}
	else if (result == MOBILE_QQ_VOIP_DATA)
	{
		char buf[4096];
		int buflen = 0;
		if (srcip ==*(unsigned int*)packh.m_sip && dstip == *(unsigned int*)packh.m_dip && srcport == packh.m_sport && dstport == packh.m_dport)
		{
			if (memcmp(pPacket,senderpackfilter,QQ_AUDIO_PACK_FILTER_LEN) == 0)
			{
				return FALSE;
			}
			memmove(senderpackfilter, pPacket, QQ_AUDIO_PACK_FILTER_LEN);

			if (senderflag == 0)
			{
				senderflag = *(unsigned int*)(pPacket + 9);
			}
			else if (senderflag != *(unsigned int*)(pPacket + 9) )
			{
				return FALSE;
			}

			
			buflen = *(unsigned short *)(pPacket + 34);
			if (buflen != ntohs(*(unsigned short *)(pPacket + 32)) - 2)
			{
				return FALSE;
			}

			senderqq = ntohl(*(unsigned int*)(pPacket + 28));
			m_from.Format("%u",ntohl(*(unsigned int*)(pPacket + 28)));

			*(unsigned short*)buf = buflen;
			memmove(buf + 2, pPacket + 36, buflen);
			buflen += 2;
			packh.m_datalen = buflen;

			senderpackcnt ++;
			senderpackindex = ntohl(*(unsigned int *)(pPacket + 5));
			VOIPPACKET pack(packh, buf,senderpackindex);
			m_CallerPackList.push_back(pack);
		}
		else if (dstip==*(unsigned int*)packh.m_sip && srcip==*(unsigned int*)packh.m_dip && dstport==packh.m_sport &&	srcport==packh.m_dport)
		{
			if (memcmp(pPacket,recverpackfilter,QQ_AUDIO_PACK_FILTER_LEN) == 0)
			{
				return FALSE;
			}
			memmove(recverpackfilter, pPacket, QQ_AUDIO_PACK_FILTER_LEN);

			if (recverflag == 0)
			{
				recverflag = *(unsigned int*)(pPacket + 9);
			}
			else if (recverflag != *(unsigned int*)(pPacket + 9) )
			{
				return FALSE;
			}
			
			buflen = *(unsigned short *)(pPacket + 34);
			if (buflen != ntohs(*(unsigned short *)(pPacket + 32)) - 2)
			{
				return FALSE;
			}
			m_to.Format("%u",ntohl(*(unsigned int*)(pPacket + 28)));
			recverqq = ntohl(*(unsigned int*)(pPacket + 28));
			*(unsigned short*)buf = buflen;
			memmove(buf + 2, pPacket + 36, buflen);
			buflen += 2;
			packh.m_datalen = buflen;
			
			recverpackcnt ++;
			recverpackindex = ntohl(*(unsigned int *)(pPacket + 5));
			VOIPPACKET pack(packh, buf,senderpackindex);
			m_CalleePackList.push_back(pack);
		}
	}
	/*
	else if (result == PC_QQ_VOIP_DATA)
	{
		char buf[4096];
		int buflen = 0;
		if (srcip ==*(unsigned int*)packh.m_sip && dstip == *(unsigned int*)packh.m_dip && srcport == packh.m_sport && dstport == packh.m_dport)
		{
			if (senderflag == 0)
			{
				senderflag = *(unsigned int*)(pPacket + 9);
			}
			else if (senderflag != *(unsigned int*)(pPacket + 9) )
			{
				return FALSE;
			}
			
			//15 字节开始是语音还是其他
			buflen = packh.m_datalen - PC_QQ_AUDIO_DATA_PACKETPOS;
			*(unsigned short*)buf = buflen;
			memmove(buf + 2, pPacket + PC_QQ_AUDIO_DATA_PACKETPOS, buflen);
			buflen += 2;
			packh.m_datalen = buflen;
			
			senderpackcnt ++;
			senderpackindex = ntohl(*(unsigned int *)(pPacket + 5));
			VOIPPACKET pack(packh, buf,senderpackindex);
			m_CallerPackList.push_back(pack);
		}
		else if (dstip==*(unsigned int*)packh.m_sip && srcip==*(unsigned int*)packh.m_dip && dstport==packh.m_sport &&	srcport==packh.m_dport)
		{
			if (recverflag == 0)
			{
				recverflag = *(unsigned int*)(pPacket + 9);
			}
			else if (recverflag != *(unsigned int*)(pPacket + 9) )
			{
				return FALSE;
			}
			
			buflen = packh.m_datalen - PC_QQ_AUDIO_DATA_PACKETPOS;
			*(unsigned short*)buf = buflen;
			memmove(buf + 2, pPacket + PC_QQ_AUDIO_DATA_PACKETPOS, buflen);
			buflen += 2;
			packh.m_datalen = buflen;
			
			recverpackcnt ++;
			recverpackindex = ntohl(*(unsigned int *)(pPacket + 5));
			VOIPPACKET pack(packh, buf,senderpackindex);
			m_CalleePackList.push_back(pack);
		}
	}
	*/
	
	m_lastpacktm = packh.m_tm;
	return TRUE;
}




	
int VoipQQSession::ListToFile()
{
//	CString cstrContext;
//	cstrContext.Format("from=%u\r\nto=%u\r\nstate=音视频聊天",senderqq,recverqq);					//注意无符号数
//	m_indexfname = CreateMediaFilename("media_voip_m_qq.indexfile.txt",m_sip, m_dip);
//	WriteIndexFile("media_voip_m_qq","media_voip_m_qq",cstrContext,g_MediaMissObj);	

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
		m_realfname = CreateMediaFilename("qqvoip", m_sip, m_dip);
		if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return -2;
		}
		VoipFileHdr.CallerPackCnt = senderpackcnt ;
		VoipFileHdr.CalleePackCnt = recverpackcnt ;

		file.Write(&VoipFileHdr,sizeof(VOIPFILEHEADER));	
		file.Write(&senderqq,4);	
		file.Write(&recverqq,4);	
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
		VoipPacketListIt begin;
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
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP MOBILE QQ Error!!");
		return -3;
	}
	return 0;
}