#pragma once
#include "StdAfx.h"
#include "imsg.h"
#include "VoipIMSG.h"


#define ET_CALL		1
#define ET_LOGIN	2



int ETphoneNumSession::IsETphoneNumPacket(DATEHEADER& packh, const char* pPacket)
{
	if( (packh.m_type == UDPPacket) && (packh.m_dport == 10000) )
	{
		if( (memcmp(pPacket,	"IP Som.",		strlen("IP Som."))==0) ||
			(memcmp(pPacket,	"/2IP Som.",	strlen("/2IP Som."))==0) )  
		{ 
			return ET_CALL;
		}
		else if (memcmp(pPacket," <m:ro\nF0\r2.P/SIm ",strlen(" <m:ro\nF0\r2.P/SIm ")) == 0)
		{
			return ET_LOGIN;
		}
	}
	return FALSE;
}

ETphoneNumSession::ETphoneNumSession(DATEHEADER& packh, int aT) : SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite = FALSE;
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

ETphoneNumSession::~ETphoneNumSession(){ }

int ETphoneNumSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if( (packh.m_type == UDPPacket) && (packh.m_dport == 10000) )
	{
		if( (memcmp(pPacket,"IP Som.",strlen("IP Som."))==0) ||
			(memcmp(pPacket,"/2IP Som.",strlen("/2IP Som."))==0) )  
		{ 
			char * ItemHead = strstr(pPacket,"From: <sip:");
			if (!ItemHead)
			{
				return FALSE;
			}
			ItemHead += strlen("From: <sip:");

			char * ItemEnd = strstr(ItemHead,"@net263.com>");
			if (!ItemEnd)
			{
				return FALSE;
			}
			m_userid = ItemHead;
			m_userid = m_userid.Left(ItemEnd - ItemHead);
			
			ItemHead = strstr(ItemEnd,"To: <sip:");
			if (!ItemHead)
			{
				return FALSE;
			}
			ItemHead += strlen("To: <sip:");

			ItemEnd = strstr(ItemHead,"@net263.com>");
			if (!ItemEnd)
			{
				return FALSE;
			}
			m_to = ItemHead;
			m_to = m_to.Left(ItemEnd - ItemHead);
			WriteETphoneNumIndexFile();
			return TRUE;
		}
		else if (memcmp(pPacket," <m:ro\nF0\r2.P/SIm ",strlen(" <m:ro\nF0\r2.P/SIm ")) == 0)
		{
			char * ItemHead = strstr(pPacket,"sERSTGIREsip:");
			if (!ItemHead)
			{
				return FALSE;
			}
			ItemHead += strlen("sERSTGIREsip:");
			
			char * ItemEnd = strstr(ItemHead,"@net263.com>");
			if (!ItemEnd)
			{
				return FALSE;
			}
			m_userid = ItemHead;
			m_userid.Left(ItemEnd - ItemHead);
			WriteETphoneNumIndexFile();
			return TRUE;
		}
	}
	return FALSE;
}


void ETphoneNumSession::WriteETphoneNumIndexFile()
{
	CString text;
	m_indexfname = CreateFilename(m_sip, m_dip, "et_phonenum.indexfile.txt");
	text.Format("user=%s\r\nto=%s\r\nstate=%s\r\ntype=voip_call\r\n", m_userid,m_to,m_state);
	WriteIndexFile("media_voip_et", "media_voip_et", text, g_pImsgMissObj);
}








VoipFEIYINImsgSession::VoipFEIYINImsgSession(DATEHEADER& packh, int aT): SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite=FALSE;
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipFEIYINImsgSession::~VoipFEIYINImsgSession(){}

int VoipFEIYINImsgSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if( IsVoipFEIYINImsg(packh,pPacket))
	{
		char * StringHead;
		char * StringEnd;
		if( (StringHead = strstr(pPacket,"uid=") ) && (StringEnd = strstr(StringHead,"pwd=") )  )
		{
			StringHead += strlen("uid=");
			StringEnd -- ;
			m_userid = StringHead;
			m_userid = m_userid.Left(StringEnd - StringHead);
			WriteFEIYINIndexFile();
			return TRUE;
		}
	}
	return 0;
}
 
void VoipFEIYINImsgSession::WriteFEIYINIndexFile()
{
	m_indexfname = CreateFilename(m_sip, m_dip, "media_voip_kc.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_kc", "media_voip_kc", txt, g_pImsgMissObj);
	m_begfiletm = 0;
	m_closetm += 10;
}






VoipSKYImsgSession::VoipSKYImsgSession(DATEHEADER& packh, int aT): SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite=FALSE;
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipSKYImsgSession::~VoipSKYImsgSession(){	}

int VoipSKYImsgSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	char * StringHead;
	char * StringEnd;
	int Value = IsVoipSKYImsg(packh,pPacket);
	if( Value == VOIP_CALL)
	{

		if( (StringHead = strstr(pPacket,"uid=") ) && (StringEnd = strstr(StringHead,"pwd=") )  )
		{
			StringHead += strlen("uid=");
			StringEnd -- ;
			m_userid = StringHead;
			m_userid = m_userid.Left(StringEnd - StringHead);
			m_state = "call";
			WriteSKYIndexFile();
			return TRUE;
		}
	}
	else if (Value == VOIP_LOGIN)
	{
		if( (StringHead = strstr(pPacket,"<account>") ) && (StringEnd = strstr(StringHead,"</account>") )  )
		{
			StringHead += strlen("<account>");
			m_userid = StringHead;
			m_userid = m_userid.Left(StringEnd - StringHead);
			m_state = "login";
			WriteSKYIndexFile();
			return TRUE;
		}
	}
	return 0;
}


void VoipSKYImsgSession::WriteSKYIndexFile()
{
	m_indexfname = CreateFilename(m_sip, m_dip, "media_voip_kc.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"",m_state);
	WriteIndexFile("media_voip_kc", "media_voip_kc", txt, g_pImsgMissObj);
	m_begfiletm = 0;
	m_closetm += 10;
} 






VoipTUXINImsgSession::VoipTUXINImsgSession(DATEHEADER& packh, int aT): SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite=FALSE;
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipTUXINImsgSession::~VoipTUXINImsgSession(){}

int VoipTUXINImsgSession::InsertPacket(DATEHEADER & packh, const char * pPacket)
{
	if( IsVoipTUXINImsg(packh,pPacket))
	{
		char * StringHead;
		char * StringEnd;
		if( (StringHead = strstr(pPacket, "uid=")) && (StringEnd = strstr(StringHead,"pwd=")) )
		{
			StringHead += strlen("uid=");
			StringEnd -- ;
			m_userid = StringHead;
			m_userid = m_userid.Left(StringEnd - StringHead);
			WriteTUXINIndexFile();
			return TRUE;
		}	
	}
	return 0;
}

void VoipTUXINImsgSession::WriteTUXINIndexFile()
{
	m_indexfname = CreateFilename(m_sip, m_dip, "media_voip_kc.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_kc", "media_voip_kc", txt, g_pImsgMissObj);
	m_begfiletm = 0;
	m_closetm += 10;
}







VoipYLTImsgSession::VoipYLTImsgSession(DATEHEADER& packh, int aT): SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite=FALSE;
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipYLTImsgSession::~VoipYLTImsgSession(){}

int VoipYLTImsgSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if(IsVoipYLTImsg(packh,pPacket))
	{
		char * StringHead;
		char * StringEnd;
		if (StringHead = strstr(pPacket ,"<kc-number>") )
		{
			StringHead += strlen("<kc-number>");
			if( StringEnd = strstr(StringHead , "</kc-number>") )
			{
				m_userid = StringHead;
				m_userid = m_userid.Left(StringEnd - StringHead);
				WriteYLTIndexFile();
				return TRUE;
			}
		}
		else if (StringHead = strstr(pPacket ,"<uid>") )
		{
			StringHead += strlen("<uid>");
			if( StringEnd = strstr(StringHead , "</uid>") )
			{
				m_userid = StringHead;
				m_userid = m_userid.Left(StringEnd - StringHead);
				WriteYLTIndexFile();
				return TRUE;
			}
		}	
	}
	return FALSE;
}
 
void VoipYLTImsgSession::WriteYLTIndexFile()
{
	m_indexfname = CreateFilename(m_sip, m_dip, "media_voip_kc.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_kc", "media_voip_kc", txt, g_pImsgMissObj);
	m_begfiletm = 0;
	m_closetm += 10;
}













VoipKCImsgSession::VoipKCImsgSession(DATEHEADER& packh, int aT) : SocketSession(packh,aT)
{
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite=FALSE;
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipKCImsgSession::~VoipKCImsgSession(){ }

VoipKCImsgSession::InsertPacket(DATEHEADER & packh, const char * pPacket)
{
	int Value = IsVoipKCImsg(packh,pPacket);
	char * StringHead;
	char * StringEnd;

	if (Value == VOIP_CALL)
	{
		if( (StringHead = strstr(pPacket, "\"uid\": ")) && (StringEnd = strchr(StringHead  + strlen("\"uid\": "), ',') ) )
		{
			StringHead += strlen("\"uid\": ");
			m_userid = StringHead;
			m_userid = m_userid.Left(StringEnd - StringHead);

			if(StringHead = strstr(StringEnd, "\"phone\": \""))
			{
				StringEnd = strchr(StringHead  + strlen("\"phone\": \""), '"');
				if (StringEnd == 0)
				{
					return FALSE;
				}
				StringHead += strlen("\"phone\": \"");
				m_from = StringHead ;
				m_from = m_from.Left(StringEnd - StringHead);
				WriteKCIndexFile();
				return TRUE;
			}
		}
	}
	else if (Value == VOIP_PHONE_CALL)
	{
		if( (StringHead = strstr(pPacket, "uid=")) && (StringEnd = strstr(StringHead,"v=") ) )
		{
			StringHead += strlen("uid=");
			StringEnd --;
			m_userid = StringHead;
			m_userid = m_userid.Left(StringEnd - StringHead);
			
			WriteKCIndexFile();
			return TRUE;
		}
	}
	return 0;
}

void VoipKCImsgSession::WriteKCIndexFile()
{
	m_indexfname = CreateFilename(m_sip, m_dip, "media_voip_kc.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_kc", "media_voip_kc", txt, g_pImsgMissObj);	
	m_begfiletm = 0;
	m_closetm += 10;
}












VoipWEIWEIImsgSession::VoipWEIWEIImsgSession(DATEHEADER& packh, int aT): SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite=FALSE;
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipWEIWEIImsgSession::~VoipWEIWEIImsgSession(){}

int VoipWEIWEIImsgSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if(IsVoipWEIWEIImsg(packh,pPacket) )
	{
		char * StringHead;
		char * StringEnd;
		if( (StringHead = strstr(pPacket,"<user_id>") ) && (StringEnd = strstr(StringHead,"</user_id>") )  )
		{
				StringHead += strlen("<user_id>");
				m_userid = StringHead;
				m_userid = m_userid.Left(StringEnd - StringHead);

			 if( (StringHead = strstr(pPacket,"<phone_no>") ) && (StringEnd = strstr(StringHead,"</phone_no>") ) )
			 {
				StringHead += strlen("<phone_no>");
				m_from = StringHead;
				m_from = m_from.Left(StringEnd - StringHead);
			 }
			WriteWEIWEIIndexFile();	
			return TRUE;
		}
	}
	return 0;
}
 
void VoipWEIWEIImsgSession::WriteWEIWEIIndexFile()
{
	m_indexfname = CreateFilename(m_sip, m_dip, "media_voip_weiwei.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_weiwei", "media_voip_weiwei", txt, g_pImsgMissObj);	
	m_begfiletm = 0;
	m_closetm += 10;
}



















VoipUxinImsgSession::VoipUxinImsgSession(DATEHEADER& packh, int aT): SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite=FALSE;
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipUxinImsgSession::~VoipUxinImsgSession(){}

int VoipUxinImsgSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if(int Value = IsVoipUxinImsg(packh,pPacket) )
	{
		char * ItemHead;
		char * ItemEnd;
		if (Value == VOIP_LOGIN)
		{
			ItemHead = strstr(pPacket,"account=");
			if (ItemHead == 0)
			{
				return FALSE;
			}
			ItemHead += strlen("account=");
			
			ItemEnd = strstr(ItemHead,"imei=");
			if (ItemEnd == 0)
			{
				return FALSE;
			}
			ItemEnd --;
			m_userid = ItemHead;
			m_userid = m_userid.Left(ItemEnd - ItemHead);
			WriteUxinIndexFile();	
			return TRUE;
		}
		else if (Value == VOIP_CALL)
		{
			ItemHead = strstr(pPacket,"caller_no%22%3A%22");
			if (ItemHead == 0)		//注意4个字段的顺序不能乱，否则出错
			{
				return FALSE;
			}
			ItemHead += strlen("caller_no%22%3A%22");
			
			ItemEnd = strstr(ItemHead,"%22%2C%22caller_uid%22%3A%22");
			if(ItemEnd == 0)
			{
				return FALSE;
			}
			m_from = ItemHead;
			m_from = m_from.Left(ItemEnd - ItemHead);

			
			ItemHead = ItemEnd + strlen("%22%2C%22caller_uid%22%3A%22");
			ItemEnd = strstr(ItemHead,"%22%2C%22callee%22%3A%22");
			if(ItemEnd == 0)
			{
				return FALSE;
			}
			m_userid = ItemHead;
			m_userid = m_userid.Left(ItemEnd - ItemHead);
			
			ItemHead = ItemEnd + strlen("%22%2C%22callee%22%3A%22");
			ItemEnd = strstr(ItemHead,"%22%2C%22");
			if(ItemEnd == 0)
			{
				return FALSE;
			}
			m_to = ItemHead;
			m_to = m_to.Left(ItemEnd - ItemHead);
			WriteUxinIndexFile();
		}
		else if (Value == VOIP_PHONE_CALL)
		{	
			ItemHead = strstr(pPacket,"\"caller_uid\":\"");
			if (ItemHead == 0)	
			{
				return FALSE;
			}
			ItemHead += strlen("\"caller_uid\":\"");
			
			ItemEnd = strchr(ItemHead,'"');
			if(ItemEnd == 0)
			{
				return FALSE;
			}
			m_userid = ItemHead;
			m_userid = m_userid.Left(ItemEnd - ItemHead);
			
			ItemHead = strstr(pPacket,"\"caller_no\":\"");
			if (ItemHead == 0)	
			{
				return FALSE;
			}
			ItemHead += strlen("\"caller_no\":\"");
			
			ItemEnd = strchr(ItemHead,'"');
			if(ItemEnd == 0)
			{
				return FALSE;
			}
			m_from = ItemHead;
			m_from = m_from.Left(ItemEnd - ItemHead);
			
			ItemHead = strstr(pPacket,"\"callee\":\"");
			if (ItemHead == 0)	
			{
				return FALSE;
			}
			ItemHead += strlen("\"callee\":\"");
			
			ItemEnd = strchr(ItemHead,'"');
			if(ItemEnd == 0)
			{
				return FALSE;
			}
			m_to = ItemHead;
			m_to = m_to.Left(ItemEnd - ItemHead);
			WriteUxinIndexFile();
		}
	}
	return FALSE;
}

void VoipUxinImsgSession::WriteUxinIndexFile()
{
	m_indexfname = CreateFilename(m_sip, m_dip, "media_voip_yx.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_yx", "media_voip_yx", txt, g_pImsgMissObj);	
	m_begfiletm = 0;
	m_closetm += 10;
}







VoipAdlImsgSession::VoipAdlImsgSession(DATEHEADER& packh, int aT): SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite=FALSE;
	m_userid.Empty();
	m_from.Empty();
	m_to.Empty();
}

VoipAdlImsgSession::~VoipAdlImsgSession(){}

int VoipAdlImsgSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if(IsVoipAdlImsg(packh,pPacket) )
	{
		char * StringHead;
		char * StringEnd;
		if( (StringHead = strstr(pPacket,"uid=") ) && (StringEnd = strstr(StringHead,"sign=") )  )
		{
			StringHead += strlen("uid=");
			StringEnd --;
			m_userid = StringHead;
			m_userid = m_userid.Left(StringEnd - StringHead);
			WriteAdlIndexFile();	
			return TRUE;
		}
	}
	return 0;
}

void VoipAdlImsgSession::WriteAdlIndexFile()
{
	m_indexfname = CreateFilename(m_sip, m_dip, "media_voip_kc.indexfile.txt");	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=call\r\ntype=voip_call\r\n",m_userid,"",m_from,m_to,"");
	WriteIndexFile("media_voip_kc", "media_voip_kc", txt, g_pImsgMissObj);	
	m_begfiletm = 0;
	m_closetm += 10;
}

