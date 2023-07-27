#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "VoipMedia.h"

VoipYltMediaSession::VoipYltMediaSession(DATEHEADER& packh, int aT): MediaSession(packh, aT)
{
	m_processtm		= packh.m_tm;
	m_begfiletm		= packh.m_tm;
	m_Packettype	= packh.m_type;
	m_MediaPro		= _T("media_voip_kc");
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipYltMediaSession::~VoipYltMediaSession(){}

int VoipYltMediaSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (IsVoipYltMedia(packh,pPacket))
	{
		m_userid = pPacket;
		int Offset = m_userid.Find("From: <sip:");	
		if (Offset == -1)
		{
			return FALSE;
		}
		m_userid = m_userid.Mid(Offset + strlen("From: <sip:"));
		Offset = m_userid.Find("@");
		if (Offset == -1)
		{
			return FALSE;
		}
		m_userid = m_userid.Left(Offset);
		
		m_to = pPacket;
		Offset = m_to.Find("To: <sip:");	
		if (Offset == -1)
		{
			return FALSE;
		}
		m_to = m_to.Mid(Offset + strlen("To: <sip:"));
		Offset = m_to.Find("@");
		if (Offset == -1)
		{
			return FALSE;
		}
		m_to = m_to.Left(Offset);
	//	ListToFile();
		
		CString strKey = GetVoipCallKey("ylt",packh);
		SVOIP_CALL_INFO *pVoipCallInfo = GetVoipCallInfo(strKey);
		
		if (pVoipCallInfo != NULL)
		{
			return TRUE;
		}
		else
		{
			pVoipCallInfo = new SVOIP_CALL_INFO();
			pVoipCallInfo->strUser = m_userid;
			pVoipCallInfo->strTo = m_to;
			SetVoipCallInfo(strKey,pVoipCallInfo);
		}

		return TRUE;
	}
	return 0;
}

int VoipYltMediaSession::ListToFile()
{
	return FALSE;
	/*
	m_indexfname = CreateMediaFilename(m_sip, m_dip, "voip_kc.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_kc", "media_voip_kc", txt, g_MediaMissObj);
	return TRUE;*/

	
}


int Voip97CallPhoneNum(DATEHEADER& packh, const char *pPacket,int g_PacketFrom)
{
	char  Flag97CALL[] = {0x4b,0x53,0x00,0x00,0x00,0x01};
	if( ( (packh.m_dport == 5060 ) || ( packh.m_dport == 5070 ) ) && (memcmp(pPacket,Flag97CALL,6) == 0) )
	{
		CString m_userid;
		m_userid = pPacket + 10;
		short PhoneNumLen = ntohs(*(short*)(pPacket + 8));		 //ÍøÂç×Ö½ÚË³Ðò
		if (PhoneNumLen > 13)
		{
			return FALSE;
		}
		m_userid = m_userid.Left(PhoneNumLen);

		/**/

		CString strKey = GetVoipCallKey("97call",packh);
		SVOIP_CALL_INFO *pVoipCallInfo = GetVoipCallInfo(strKey);
		
		if (pVoipCallInfo != NULL)
		{
			return TRUE;
		}
		else
		{
			pVoipCallInfo = new SVOIP_CALL_INFO();
			pVoipCallInfo->strUser = m_userid;
			SetVoipCallInfo(strKey,pVoipCallInfo);
		}
		

		
		

		/*

		SocketSession vgatesession(packh, g_PacketFrom);
	
		CString text;	
		text.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"","","","");
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmpdir = g_MediaMissObj->GetTmpPath();		
		vgatesession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_vgate.indexfile.txt",
			tmpdir, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, vgatesession.m_sip, vgatesession.m_dip, ++g_MediaFileNum);
		vgatesession.m_spyaddr = "";
		vgatesession.WriteIndexFile("media_voip_97call", "media_voip_97call", text, g_MediaMissObj);
*/
		return TRUE;
	}
	return FALSE;
}


Voip97CallMediaSession::Voip97CallMediaSession(DATEHEADER& packh, int aT): MediaSession(packh, aT)
{
	m_processtm		= packh.m_tm;
	m_begfiletm		= packh.m_tm;
	m_Packettype	= packh.m_type;
	m_MediaPro		= _T("media_voip_97call");
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

Voip97CallMediaSession::~Voip97CallMediaSession(){}

int Voip97CallMediaSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (IsVoip97CallMedia(packh,pPacket))
	{
		m_userid = pPacket + 10;
		short PhoneNumLen = ntohs(*(short*)(pPacket + 8));		 //ÍøÂç×Ö½ÚË³Ðò
		if (PhoneNumLen > 13)
		{
			return FALSE;
		}
		m_userid = m_userid.Left(PhoneNumLen);
		ListToFile();
		return TRUE;
	}
	return 0;
}

int Voip97CallMediaSession::ListToFile()
{
	m_indexfname = CreateMediaFilename(m_sip, m_dip, "voip_97call.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
//	WriteIndexFile("media_voip_97call", "media_voip_97call", txt, g_MediaMissObj);
	return TRUE;
}


// 	CString txt;
// 	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=ºô½Ð\r\ntype=voip_msg\r\n",
// 			m_CallerID,m_CallerPSW,m_CallerPhone,m_CalleePhone,"");
// 				
// 	SYSTEMTIME now;
// 	GetLocalTime(&now);
// 	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
// 	ASSERT(!tmppath.IsEmpty());
// 	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_alcall.indexfile.txt", 
// 		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
// 		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
// 	pHttpSession->WriteIndexFile("media_voip_alcall","media_voip_alcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
// 	return true;


VoipRYBMediaSession::VoipRYBMediaSession(DATEHEADER& packh, int aT) : MediaSession(packh,aT)
{
	m_processtm		= packh.m_tm;
	m_begfiletm		= packh.m_tm;
	m_Packettype	= packh.m_type;
	m_MediaPro		= _T("media_voip_kc");
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipRYBMediaSession::~VoipRYBMediaSession(){ }

VoipRYBMediaSession::InsertPacket(DATEHEADER & datahdr, const char * pPacket)
{
	if( IsVoipRYBMedia(datahdr,pPacket) )
	{
		m_userid = pPacket;
		int Offset = m_userid.Find("From: <sip:");	
		if (Offset == -1)
		{
			return FALSE;
		}
		m_userid = m_userid.Mid(Offset + strlen("From: <sip:"));
		Offset = m_userid.Find("@");
		if (Offset == -1)
		{
			return FALSE;
		}
		m_userid = m_userid.Left(Offset);
	
		m_to = pPacket;
		Offset = m_to.Find("To: <sip:");	
		if (Offset == -1)
		{
			return FALSE;
		}
		m_to = m_to.Mid(Offset + strlen("To: <sip:"));
		Offset = m_to.Find("@");
		if (Offset == -1)
		{
			return FALSE;
		}
		m_to = m_to.Left(Offset);
		ListToFile();	
		return TRUE;
	}
	return 0;
}

int VoipRYBMediaSession::ListToFile()
{
	m_indexfname = CreateMediaFilename(m_sip, m_dip, "voip_kc.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_kc", "media_voip_kc", txt, g_MediaMissObj);	
	return TRUE;
}






VoipADLMediaSession::VoipADLMediaSession(DATEHEADER& packh, int aT): MediaSession(packh, aT)
{
	m_processtm		= packh.m_tm;
	m_begfiletm		= packh.m_tm;
	m_Packettype	= packh.m_type;
	m_MediaPro		= _T("media_voip_kc");
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipADLMediaSession::~VoipADLMediaSession(){	}

int VoipADLMediaSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (IsVoipADLMedia(packh,pPacket))
	{
		m_userid = pPacket;
		int Offset = m_userid.Find("From: <sip:");	
		if (Offset == -1)
		{
			return FALSE;
		}
		m_userid = m_userid.Mid(Offset + strlen("From: <sip:"));
		Offset = m_userid.Find("@");
		if (Offset == -1)
		{
			return FALSE;
		}
		m_userid = m_userid.Left(Offset);
		
		m_to = pPacket;
		Offset = m_to.Find("To: <sip:");	
		if (Offset == -1)
		{
			return FALSE;
		}
		m_to = m_to.Mid(Offset + strlen("To: <sip:"));
		Offset = m_to.Find("@");
		if (Offset == -1)
		{
			return FALSE;
		}
		m_to = m_to.Left(Offset);
		ListToFile();	
		return TRUE;
	}
	return 0;
}

int VoipADLMediaSession::ListToFile()
{
	m_indexfname = CreateMediaFilename(m_sip, m_dip, "voip_kc.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_kc", "media_voip_kc", txt, g_MediaMissObj);
	return TRUE;
}






VoipDuoLiaoMediaSession::VoipDuoLiaoMediaSession(DATEHEADER& packh, int aT): MediaSession(packh, aT)
{
	m_processtm		= packh.m_tm;
	m_begfiletm		= packh.m_tm;
	m_Packettype	= packh.m_type;
	m_MediaPro		= _T("media_voip_kc");
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipDuoLiaoMediaSession::~VoipDuoLiaoMediaSession(){	}

int VoipDuoLiaoMediaSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (IsVoipDuoLiaoMedia(packh,pPacket))
	{
		m_userid = pPacket;
		int Offset = m_userid.Find("From: <sip:");	
		if (Offset == -1)
		{
			return FALSE;
		}
		m_userid = m_userid.Mid(Offset + strlen("From: <sip:"));
		Offset = m_userid.Find("@");
		if (Offset == -1)
		{
			return FALSE;
		}
		m_userid = m_userid.Left(Offset);
		
		m_to = pPacket;
		Offset = m_to.Find("To: <sip:");	
		if (Offset == -1)
		{
			return FALSE;
		}
		m_to = m_to.Mid(Offset + strlen("To: <sip:"));
		Offset = m_to.Find("@");
		if (Offset == -1)
		{
			return FALSE;
		}
		m_to = m_to.Left(Offset);
		ListToFile();	
		return TRUE;
	}
	return 0;
}

int VoipDuoLiaoMediaSession::ListToFile()
{
	m_indexfname = CreateMediaFilename(m_sip, m_dip, "voip_kc.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_kc", "media_voip_kc", txt, g_MediaMissObj);
	return TRUE;
}






//hhtµÇÂ½Ê±ÓÃTCP 4000¶Ë¿Ú
// GET /account/login/LoginXML.jsp?hht_alias_name=satadriver&hht_password=7679f6284f28d5006fbb7ecc695f138a8206 HTTP/1.1
// Accept: */*		 
// Accept-Encoding: gzip, deflate
// User-Agent: Mozilla/4.0 (AckOutTime; BindOk; SocketInit; CardNotInit; Compatible win32 72.121.2.166; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727)
// Host: 219.232.228.78:4000
// Connection: Keep-Alive


VoipHHTMediaSession::VoipHHTMediaSession(DATEHEADER& packh, int aT): MediaSession(packh, aT)
{
	m_processtm		= packh.m_tm;
	m_begfiletm		= packh.m_tm;
	m_Packettype	= packh.m_type;
	m_MediaPro		= _T("media_voip_hht");
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipHHTMediaSession::~VoipHHTMediaSession(){}

int VoipHHTMediaSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{	
	/*
	char NumHHT[] = {0x00,0x06,0x00,0x15,0x00,0x00};
	char NameHHT[] = {0x00,0x02,0x00,0x02,0x00,0x00};
	if(IsVoipHHTMedia(packh,pPacket))
	{
		char * StringHead;
		char * StringEnd;
		if( memcmp(pPacket + packh.m_datalen - 8 ,"|8+10+13",8) == 0) 
		{
			m_userid = pPacket + packh.m_datalen - 19 ;
			m_userid = m_userid.Left(11);
			ListToFile();
			return TRUE;
		}
		else if(memcmp(pPacket,NumHHT ,6) == 0)
		{
			StringHead = (char*)pPacket + 10;
			StringEnd = strchr(pPacket,'|');
			m_userid = pPacket+ 10 ;
			m_userid = m_userid.Left(StringEnd - StringHead);
			ListToFile();
			return TRUE;
		}
	}*/
	return FALSE;
}

int VoipHHTMediaSession::ListToFile()
{

	m_indexfname = CreateMediaFilename(m_sip, m_dip, "voip_hht.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_hht", "media_voip_hht", txt, g_MediaMissObj);	
	return TRUE;
}







