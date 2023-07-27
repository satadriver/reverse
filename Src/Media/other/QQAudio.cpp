
#include "stdafx.h"
#include "..\\src\\media\\MediaSession.h"
#include "..\\src\\media\\media.h"
#include "..\\src\\media\\QQAudio.h"


QQAudio::QQAudio(DATEHEADER& packh, int aT, char * pPacket) : MediaSession(packh,aT)
{
	m_processtm				= packh.m_tm;
	m_begfiletm				= packh.m_tm;
	AudioBufLen				= 0;
	FlagUid					= 0;
	FlagEnd					= 0;
	FlagHead				= 0;
	PackCnt					= 0;
	memset(SenderQQ,0,16);
	memset(RecverQQ,0,16);
	memmove((unsigned char*)&SenderIP,packh.m_sip,4);
	memmove((unsigned char*)&RecverIP,packh.m_dip,4);
	SenderPort				=packh.m_sport;
	RecverPort				=packh.m_dport;
}





QQAudio::~QQAudio() { }



int QQAudio::IsQQAudio(DATEHEADER& packh,const char *pPacket)
{

	if( (packh.m_sport != 80) && (packh.m_dport != 80) )
	{
		return FALSE;
	}
	
	if( memcmp(pPacket,"POST /cgi-bin/httpconn?", strlen("POST /cgi-bin/httpconn?")) == 0)
	{
		return TRUE;
	}
	return FALSE;
}



int QQAudio::InsertPacket(DATEHEADER &packh,const char * pPacket)
{
	char * iHead, *iEnd;
	if( (memcmp((char*)&SenderIP,packh.m_sip,4) == 0) && (memcmp((char*)&RecverIP,packh.m_dip,4) == 0) &&
		(SenderPort == packh.m_sport) && (RecverPort == packh.m_dport) )
	{
		if( ( FlagUid == 0) && (AudioBufLen == 0) && (PackCnt == 0) )
		{
			iHead = strstr(pPacket,"uin=");
			if (iHead == 0)
			{
				return FALSE;
			}
			iHead += strlen("uin=");
			
			iEnd = strchr(iHead,'&');
			if (iEnd == 0)
			{
				return FALSE;
			}
			memmove(SenderQQ,iHead, iEnd - iHead);
			
			iHead = strstr(pPacket,"groupcode=");
			if (iHead == 0)
			{
				return FALSE;
			}
			iHead += strlen("groupcode=");
			iEnd = strchr(iHead,' ');
			if (iEnd == 0)
			{
				return FALSE;
			}
			memmove(RecverQQ,iHead, iEnd - iHead);
			FlagUid = TRUE;
		}
			
		if (FlagHead == 0)
		{
			iHead = strstr(pPacket,"#!AMR\n");
			if (iHead == 0)
			{
				return FALSE;		
			}
			else
			{
				FlagHead = TRUE;
			}
		}
		else
		{
			iHead = (char*)pPacket;
		}
		
		if (iHead)
		{
			iEnd = (char*)pPacket + packh.m_datalen;
			memmove(AudioBuf + AudioBufLen,iHead,iEnd - iHead);
			AudioBufLen += (iEnd - iHead);
		}
	}
	else if( (memcmp((char*)&SenderIP,packh.m_dip,4) == 0) && (memcmp((char*)&RecverIP,packh.m_sip,4) == 0) &&
		(SenderPort == packh.m_dport) && (RecverPort == packh.m_sport) )
	{
		if ( (packh.m_datalen > 0) && (memcmp(pPacket,"HTTP/1.1 200 OK\r\n",strlen("HTTP/1.1 200 OK\r\n")) == 0) )
		{
			FlagHead = 0;
			FlagUid = 0;
			ListToFile();	
		}	
	}
	return 0;
}	
	
	
	
QQAudio:: ListToFile()
{
	// 	m_indexfname = CreateFilename(m_sip, m_dip, "imsg_moicq.indexfile.txt");	
	// 	CString txt;
	// 	txt.Format("senderqq=%s\r\nrecverqq=%s\r\nstate=qunaudio\r\n",SenderQQ,RecverQQ);
	// 	WriteIndexFile("imsg_moicq", "imsg_moicq", txt, g_pImsgMissObj);
	// 	m_begfiletm = 0;
	// 	m_closetm += 10;
	
	CFile	file;
	m_realfname = CreateMediaFilename("imsg_moicq", m_sip, m_dip);	//自动加上media
	if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
	{
		return -2;
	}	
	file.Write(AudioBuf,AudioBufLen);

	file.Close();
	return FALSE;	
	
}
	
