
#include "stdafx.h"
#include "..\\src\\media\\MediaSession.h"
#include "..\\src\\media\\media.h"
#include "ZelloAudio.h"
#include "..\\src\\media\\voip.h"
#include "..\\src\\http\\VoipUrl.h"



/*
int ZelloLog(DATEHEADER& packh, char * pPacket, char * LogFlag)
{
	try
	{
		char LogContent[4096] = {0};
		int LogLen = 0;
		sprintf(LogContent, "%s",LogFlag);
		LogLen = strlen(LogContent);
		sprintf(LogContent + LogLen, "\r\nthe socket is:%08x_%08x_%04x_%04x\r\n", *(unsigned int*)packh.m_sip, *(unsigned int*)packh.m_dip, packh.m_sport, packh.m_dport);
		LogLen = strlen(LogContent);

		for (int i = 0; i < packh.m_datalen; i ++)
		{
			sprintf(LogContent + LogLen + i, "%02x", *(pPacket + i) );
		}	
		sprintf(LogContent + LogLen + i, "%d", 0x0d0a );
		WriteLog(MEDIAPRO, LogContent);
		return TRUE;
	}
	catch (...)
	{
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP zello Error!!");
		return -3;		
	}
}




int ZelloLog(char * LogFlag)
{
	WriteLog(MEDIAPRO, LogFlag);
	return TRUE;
}
*/


ZelloAudio::ZelloAudio(DATEHEADER& packh, int aT, char * pPacket, int EncodeType): MediaSession(packh, aT)
{
	m_processtm				= packh.m_tm;			//media define
	m_begfiletm				= packh.m_tm;		
	m_closetm				= packh.m_tm + 60;
	m_endfiletm				= packh.m_tm;
	m_lastpacktm			= packh.m_tm;

	m_Packettype			= packh.m_type;
	m_MediaPro				= _T("media_voip_zello");

	m_CallerPackIdx			= *(unsigned short*)(pPacket + 6);
	m_CallerPackUnique		= *(unsigned short*)(pPacket + 2);
	m_CallerPackCnt			= 0;
	m_CalleePackIdx			= 0;
	m_CalleePackCnt			= 0;
	m_CalleePackUnique		= 0;
	m_CallerIP				= *(unsigned int*)(packh.m_sip);
	m_CalleeIP				= *(unsigned int*)(packh.m_dip);
	m_CallerPORT			= packh.m_sport;
	m_CalleePORT			= packh.m_dport;

	memset(m_FromName,0,ZELLO_ITEM_LENTH);
	memset(m_ToName,0,ZELLO_ITEM_LENTH);
	memset(m_PlatForm,0,ZELLO_ITEM_LENTH);
	memset(m_Message,0,ZELLO_ITEM_LENTH * 16);
	memset(m_Codec,0,ZELLO_ITEM_LENTH);
	memset(m_BuddyList,0,ZELLO_ITEM_LENTH * 64);
	memset((char*)&m_VoipFileHdr,0,sizeof(VOIPFILEHEADER));
}


ZelloAudio::~ZelloAudio(){ }




BOOL ZelloAudio::ParseZelloPacket(DATEHEADER & packh, const char * pPacket)
{
	if ( memcmp(pPacket,"LT\nct:",6) == 0 )		//tcp or udp
	{
		return ZELLO_MSG;
	}
	else if( (memcmp(pPacket,"LT\ndo:",6) == 0) || (memcmp(pPacket,"LT\nto:",6) == 0) )		//tcp or udp
	{
		char * AudioData = strstr(pPacket,"\n\n");
		if (AudioData == 0)
		{
			return FALSE;
		}
		
		AudioData += 2;
		if( (*AudioData == 0x58) || (*AudioData == 0x18)  )			//不是包头 包内容数据
		{
			return TCP_OPUS_FRAME;
		}	
		else if (*AudioData == 0x3c || *AudioData == 0x7c)
		{
			return TCP_AMR_FRAME;
		}
		else  if ( (*(AudioData + 8) >= 0x09 && *(AudioData + 8) <= 0x0f) 
			|| (*(AudioData + 8) >= 0x20 && *(AudioData + 8) <= 0x2f) )			//这种格式的数据包没有包头标志，序号等特征
		{
			return TCP_SPEEX_FRAME;
		}	
	}
	else if  (*(unsigned short *)pPacket == packh.m_datalen - 8) 
	{
		if( *(unsigned short*)(pPacket + 4) == 0)
		{
			if( (*(pPacket + 8) == 0x58) || (*(pPacket + 8) == 0x18) )		
			{
				return UDP_OPUS_FRAME;
			}
			else if (*(pPacket + 8) == 0x3c || *(pPacket + 8) == 0x7c)
			{
				return UDP_AMR_FRAME;
			}
			else  if ( *(unsigned short*)pPacket == 0x0113 || *(unsigned short*)pPacket == 0x0177 )	//有包头标志，序号等特征
			{
				return UDP_SPEEX_FRAME;
			}
		}	
	}

	return FALSE;	
}




//4c 54 0a 74 6f 3a	udp tcp
//4c 54 0a 64 6f 3a	udp tcp
//10001 10000
//13 01 xx xx 00 00
//77 01 xx xx 00 00
//xx xx xx xx 00 00 xx xx 3c|7c
//xx xx xx xx 00 00 xx xx 58|18
//xx xx xx xx 00 00 xx xx 09-0f/20-2f
//ip.len - 8 - 8 = *(unsigned short*)packet
BOOL ZelloAudio::IsZelloAudio(DATEHEADER& packh, const char *pPacket)
{
	return ParseZelloPacket(packh, pPacket);
	if ( memcmp(pPacket,"LT\nct:",6) == 0 || memcmp(pPacket,"LT\ndo:",6) == 0 || memcmp(pPacket,"LT\nto:",6) == 0 )		//tcp or udp
	{
		return TRUE;
	}
	return FALSE;
}

//只获取每帧都是32字节的帧
int ParseAmrData(char * AmrData, int AmrDataLen, char * TmpData)		
{
	char * AmrDataPtr = AmrData;
	char * TmpDataPtr = TmpData;
	int FrameCnt = 0;
	while (AmrDataPtr < AmrData + AmrDataLen)
	{
		if ( * AmrDataPtr == 0x3c && *(AmrDataPtr + 32) == 0x3c ) 
		{
			memmove(TmpDataPtr, AmrDataPtr,64);
			FrameCnt += 2;
			AmrDataPtr += 64;
			TmpDataPtr += 64;
		}
		else if ( (*AmrDataPtr == 0x3c) && (AmrDataPtr + 32) == (AmrData + AmrDataLen) )
		{
			memmove(TmpDataPtr, AmrDataPtr,32);
			FrameCnt += 1;
			AmrDataPtr += 32;
			TmpDataPtr += 32;
		}
		else
		{
			AmrDataPtr ++;
		}
	}
	return FrameCnt * 32;
}

void ZelloAudio::WriteZelloIndexFile(int Action)
{
	m_indexfname = CreateMediaFilename(m_sip, m_dip, "imsg_zello.indexfile.txt");	
	CString txt;
	if (Action == 1)
	{
		txt.Format("from=%s\r\nto=%s\r\nmsg=%s\r\nstate=发送消息",m_FromName,m_ToName,m_Message);
		WriteIndexFile("im_zello", "im_zello", txt, g_MediaMissObj);
	}/*
	else if (Action == 2)
	{
		txt.Format("from=%s\r\nto=%s\r\nstate=发送语音",m_FromName,m_ToName);
		WriteIndexFile("software_running", "software_running", txt, g_MediaMissObj);
	}	
	
	else if (Action == 3)
	{
		txt.Format("userid=%s\r\nbuddylist=%s\r\n",m_FromName,m_BuddyList);
		WriteIndexFile("imsg_zello", "imsg_zello", txt, g_MediaMissObj);
	}	
	else if (Action == 4)
	{
		txt.Format("userid=%s\r\nloginplatform=%s\r\n",m_FromName,m_PlatForm);
		WriteIndexFile("imsg_zello", "imsg_zello", txt, g_MediaMissObj);
	}	
	*/
}

int ZelloAudio::InsertPacket(DATEHEADER& packh, const char *pPacket)			
{
	try
	{
		//ZelloLog("Begin insertpacket\r\n");

		char TmpData[4096];
		int Result = ParseZelloPacket(packh,pPacket);						
		if (Result == UDP_AMR_FRAME )				//每个数据包包含多个帧
		{
			packh.m_datalen = ParseAmrData((char*)pPacket + 8, packh.m_datalen - 8, TmpData);	
		}
		else if (Result == UDP_OPUS_FRAME || Result == UDP_SPEEX_FRAME)			//每个数据包一帧
		{
			*(unsigned short*)TmpData = packh.m_datalen - 8;
			memmove(TmpData + 2,pPacket + 8, packh.m_datalen -8);
			packh.m_datalen = packh.m_datalen - 8 + 2;
		}
		else if( (Result == TCP_OPUS_FRAME) || (Result == TCP_AMR_FRAME) || (Result == TCP_SPEEX_FRAME) )		//每个数据包一帧
		{
			char * AudioData = strstr(pPacket,"\n\n");
			if (AudioData == 0)
			{
				return FALSE;
			}
			
			AudioData += 2;
			if( (*(AudioData) == 0x58) || (*(AudioData) == 0x18) )
			{
				packh.m_datalen = packh.m_datalen - (AudioData - pPacket);
				*(unsigned short*)TmpData = packh.m_datalen;
				memmove(TmpData + 2, AudioData, packh.m_datalen);
				packh.m_datalen += 2;
			}
			else if (*(AudioData) == 0x3c || *(AudioData) == 0x7c)
			{
				packh.m_datalen = ParseAmrData(AudioData, packh.m_datalen - (AudioData - pPacket), TmpData);	
			}
			else  if ( (*(AudioData + 8) >= 0x09 && *(AudioData + 8) <= 0x0f) || (*(AudioData + 8) >= 0x20 && *(AudioData + 8) <= 0x2f) )
			{
				packh.m_datalen = packh.m_datalen - (AudioData - pPacket);
				*(unsigned short*)TmpData = packh.m_datalen;
				memmove(TmpData + 2, AudioData, packh.m_datalen);
				packh.m_datalen += 2;
			}
		}
		else if (Result == ZELLO_MSG)
		{
			int Result = TruncateStr("\nun:","\n", pPacket, m_FromName, ZELLO_ITEM_LENTH);
			if (Result)
			{
				//MD5签名
				/*
				char Sign[2048]= {0};
				Result = TruncateStr("sign:","\n", pPacket, Sign, 2048);
				if (Result)
				{
					DecodeBase64(Sign,strlen(Sign));
					Utf8ToGB2312(Sign);
				}
				*/
				char * Head = strstr(pPacket,"\n\n");
				if (Head == 0)
				{
					return FALSE;
				}
				Head += 2;
				
				int Action = 0;
				char *  End = 0;
				if (End = strstr(Head,"{\"command\":\"alert\""))
				{
					Result = TruncateStr("\"message\":\"","\"", End, m_Message, ZELLO_ITEM_LENTH * 16);
					Action = 1;
					Utf8ToGB2312(m_Message);
					Result = TruncateStr("\"to\":\"","\"", End, m_ToName, ZELLO_ITEM_LENTH);
					Utf8ToGB2312(m_FromName);
					Utf8ToGB2312(m_ToName);
					WriteZelloIndexFile(Action);	
				}
				else if (End = strstr(Head,"{\"command\":\"message\""))
				{
					Action = 2;
					Result = TruncateStr("\"codec\":\"","\"", End, m_Codec, ZELLO_ITEM_LENTH);
					Result = TruncateStr("\"to\":\"","\"", End, m_ToName, ZELLO_ITEM_LENTH);	
					Utf8ToGB2312(m_FromName);
					Utf8ToGB2312(m_ToName);
					WriteZelloIndexFile(Action);

					/*
					if(memcmp(m_Codec,"speex",5) == 0)
					{
						m_EncodeType			= UDP_SPEEX_FRAME;
					}
					else if(memcmp(m_Codec,"opus",45) == 0)
					{
						m_EncodeType			= UDP_OPUS_FRAME;
					}
					else if(memcmp(m_Codec,"amr",3) == 0)
					{
						m_EncodeType			= UDP_AMR_FRAME;
					}

					char Sign[1024];
					Result = TruncateStr("\"codec_header\":\"","\"", pPacket, Sign, 2048);
					if (Result)
					{
						int SignLen = DecodeBase64(Sign,strlen(Sign));
						SignLen = Utf8ToGB2312(Sign,SignLen);
					}
					*/
				}
				
/*
				else if (End = strstr(Head,"{\"command\":\"logon_init\""))
				{
					Action = 4;
					Result = TruncateStr("\"platform\":\"","\"", End, m_FromName, ZELLO_ITEM_LENTH);	
					Utf8ToGB2312(m_PlatForm);
					Utf8ToGB2312(m_FromName);
					WriteZelloIndexFile(Action);
				}
				else if( (End = strstr(Head,"{\"command\":\"channel\""))  ||  (End = strstr(Head,"{\"command\":\"subscribe\"") ) )
				{
					int BuddyLen = TruncateStr("\"users\":[\"","]", End, m_BuddyList, ZELLO_ITEM_LENTH * 64);
					int ChannelLen = TruncateStr("\"channels\":[\"","]", End,  m_BuddyList + BuddyLen, ZELLO_ITEM_LENTH * 64 - BuddyLen);
					if (BuddyLen + ChannelLen == 0)
					{
						return FALSE;
					}

					Result = BuddyLen + ChannelLen;
					for (int i = 0; i < Result; i ++)
					{
						if (m_BuddyList[i] == '\"' || m_BuddyList[i] == ',' || m_BuddyList[i] == 0)
						{
							m_BuddyList[i] = 0x20;
						}
					}

					m_BuddyList[Result] = 0;
					Utf8ToGB2312(m_FromName);
					Utf8ToGB2312(m_BuddyList);
					Action = 3;
					WriteZelloIndexFile(Action);	
				}
				
*/
			}
			
			return FALSE;
		}
		else
		{
			return FALSE;
		}

		int PackNum = *(unsigned short*)(pPacket + 6);
		VOIPPACKET pack(packh, TmpData,PackNum);
		if( (*(unsigned int*)packh.m_sip == m_CallerIP) && (*(unsigned int*)packh.m_dip == m_CalleeIP) &&
			(packh.m_sport == m_CallerPORT) && (packh.m_dport == m_CalleePORT) )
		{
			if (*(unsigned short*)(pPacket + 2) != m_CallerPackUnique)
			{
		//		return FALSE;
			}
			m_CallerPackList.push_back(pack);
			m_CallerPackCnt ++;
		}
		else if( (*(unsigned int*)packh.m_sip == m_CalleeIP) && (*(unsigned int*)packh.m_dip == m_CallerIP) &&
			(packh.m_sport == m_CalleePORT) && (packh.m_dport == m_CallerPORT) )
		{
			if (m_CalleePackUnique == 0 && m_CalleePackCnt == 0)
			{
				m_CalleePackUnique = *(unsigned short*)(pPacket + 2);
			}
			else if (*(unsigned short*)(pPacket + 2) != m_CalleePackUnique)
			{
	//			return FALSE;
			}
			m_CalleePackList.push_back(pack);
			m_CalleePackCnt ++;
		}

		m_lastpacktm = pack.m_tm;
		//ZelloLog("end insertpacket\r\n");
		return TRUE;
	}
	catch (...)
	{
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP zello Error!!");
		return -3;	
	}
}

int ZelloAudio::ListToFile()
{
	try
	{
		//ZelloLog("Begin listtofile\r\n");

		if( (m_CallerPackList.size() <= 50) && (m_CalleePackList.size() <= 50) )
		{
			m_CallerPackList.clear();
			m_CalleePackList.clear();
			return FALSE;
		}
		//m_CallerPackList.sort();		//数据包不按顺序来
		//m_CalleePackList.sort();
		VoipPacketListIt begin;
		CFile	file;
		if (m_realfname.IsEmpty())
		{
			m_realfname = CreateMediaFilename("media_voip_zello", m_sip, m_dip);
			if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
			{
				return -2;
			}

			m_VoipFileHdr.Protocol	= 0x09;								//3种编码容易混淆，在一个函数中判断
			m_VoipFileHdr.Version	= 0;
			m_VoipFileHdr.Undefine	= 0;
			m_VoipFileHdr.HeaderLen = sizeof(VOIPFILEHEADER) - 16;		//为了兼容以前的数据 在计算头长度时必须减去16字节
			m_VoipFileHdr.UserData	= 0;
			m_VoipFileHdr.CallerPackCnt	= m_CallerPackCnt ;
			m_VoipFileHdr.CalleePackCnt	= m_CalleePackCnt ;

			file.Write((char*)&m_VoipFileHdr,sizeof(VOIPFILEHEADER));	
		}
		else
		{
			if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
			{
				return -2;
			}
		}
		file.SeekToEnd();
	
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

		//ZelloLog("end listtofile\r\n");
	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "ListToFile() IN VOIP zello Error!!");
		return -3;
	}
	return 0;
}
