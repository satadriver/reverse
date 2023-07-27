
#include "stdafx.h"
#include "..\\src\\media\\MediaSession.h"
#include "..\\src\\media\\media.h"
#include "..\\src\\media\\UXinAudio.h"


UXinAudio::UXinAudio(DATEHEADER& packh, int aT, char * pPacket) : MediaSession(packh,aT)
{
	m_processtm				= packh.m_tm;
	m_begfiletm				= packh.m_tm;
	AudioBufLen				= 0;
	FlagUid					= 0;
	FlagHead				= 0;
	FlagEnd					= 0;
	PackCnt					= 0;
	memset(SenderID,0,16);
	memset(RecverID,0,16);
	memmove((unsigned char*)&SenderIP,packh.m_sip,4);
	memmove((unsigned char*)&RecverIP,packh.m_dip,4);
	SenderPort				=packh.m_sport;
	RecverPort				=packh.m_dport;
}





UXinAudio::~UXinAudio() { }



int UXinAudio::IsUXinAudio(DATEHEADER& packh,const char *pPacket)
{

	if( (packh.m_sport != 8887) && (packh.m_dport != 8887) )
	{
		return FALSE;
	}
	
	if( memcmp(pPacket,"POST /v2/upload_multimedia?", strlen("POST /v2/upload_multimedia?")) == 0)
	{
		return TRUE;
	}
	return FALSE;
}



int UXinAudio::InsertPacket(DATEHEADER &packh,const char * pPacket)
{

	char * iHead, *iEnd;
	char * EndFlag = "\r\n--ITzBdf9w31HyTtoSCwBAT5wwmVM2Ka8Cy2EVW--\r\n";
	int EndFlagLen = strlen(EndFlag);
	
	if( (FlagUid == 0) && (AudioBufLen == 0) && (PackCnt == 0) )
	{
		iHead = strstr(pPacket,"uid=");
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
		memmove(SenderID,iHead, iEnd - iHead);
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

	if (memcmp(pPacket + packh.m_datalen - EndFlagLen, EndFlag, EndFlagLen) == 0)
	{
		iEnd = (char*)pPacket + packh.m_datalen - EndFlagLen;
		FlagEnd = TRUE;	
	}
	else
	{
		iEnd = (char*)pPacket + packh.m_datalen;
	}

	if (FlagHead)
	{
		memmove(AudioBuf + AudioBufLen,iHead,iEnd - iHead);
		AudioBufLen += (iEnd - iHead);
	}

	if( (FlagHead) && (FlagEnd) )
	{
		FlagEnd = FALSE;
		FlagHead = FALSE;
		FlagUid = FALSE;
		ListToFile();
	}

	return 0;
}	
	
	
	
UXinAudio:: ListToFile()
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
	
