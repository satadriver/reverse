#include "StdAfx.h"
#include "MediaSession.h"		//先包括MediaSession.h然后包括Media.h
#include "Media.h"
#include "KKSession.h"



int KKSession::IsKKpacket(DATEHEADER & packh, const char * pPacket)
{
	if( (packh.m_type == UDPPacket) && ((packh.m_dport == 8021) || (packh.m_sport == 8021) ) )
	{
		if( (*(unsigned int*)pPacket == 0x00000008) && (*(unsigned short*)(pPacket + 4) == 0) )
		{
			return KK_LOGIN;
		}
	}
	return FALSE;
}


KKSession::KKSession(DATEHEADER & packh, int aT) : SocketSession(packh, aT)
{
	m_State = 0;
	m_SenderName.Empty();
	m_RecverName.Empty();
	m_SenderAccount.Empty();
	m_Message.Empty();
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite = FALSE;
}


KKSession::~KKSession(){ }


int KKSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if(m_State = IsKKpacket(packh,pPacket))
	{
		if (m_State == KK_LOGIN)
		{
			m_SenderID.Format("%d",*(unsigned int*)(pPacket + 6) );
			m_SenderPsw = pPacket + 26;

			if (packh.m_datalen > 100)
			{
				m_RecverName = pPacket + 109;
				WriteKKIndexFile();	 
				return TRUE;
			}
		}	
	}
	return TRUE;
}


void KKSession::WriteKKIndexFile()
{
	m_indexfname = CreateMediaFilename(m_sip, m_dip, "media_kk.indexfile.txt");	
	CString txt;
	if (m_State == KK_LOGIN)
	{
		txt.Format("userid=%s\r\npassword=%s\r\nusernick=%s\r\nstate=login\r\n",m_SenderID,m_SenderPsw,m_SenderName);
		WriteIndexFile("media_kk", "media_kk", txt, g_MediaMissObj);
	}
}