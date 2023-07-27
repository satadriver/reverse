#include "StdAfx.h"
#include "MediaSession.h"		//先包括MediaSession.h然后包括Media.h
#include "Media.h"
#include "ISAudioSession.h"



int ISAudioSession::IsISAudio(DATEHEADER & packh, const char * pPacket)
{
	if( (packh.m_type == UDPPacket) && ((packh.m_sport == 7000) || (packh.m_dport == 7000)) && (*(pPacket + 1) == 0x11) )
	{
		if( *(unsigned int*)(pPacket + packh.m_datalen - 9) == 0xe5cccecb )
		{ 
			if( *(unsigned int*)(pPacket + packh.m_datalen - 4) == *(unsigned int*)(pPacket + 40) )
			{
				return IS_MSG;
			}
		}
		else if (*(unsigned short*)(pPacket + packh.m_datalen - 3) == 0xd0c4)
		{
			char * LoginFlag = "status=1;email=";
			if (memcmp(pPacket + 100,pPacket + 234,strlen(LoginFlag)) == 0)
			{
				if (memcmp(pPacket + 100,LoginFlag,strlen(LoginFlag)) == 0)
				{
					return IS_LOGIN;
				}
			}
		}
	}
	return FALSE;
}


ISAudioSession::ISAudioSession(DATEHEADER & packh, int aT) : SocketSession(packh, aT)
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


ISAudioSession::~ISAudioSession(){ }


int ISAudioSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if(m_State = IsISAudio(packh,pPacket))
	{
		char * ItemHead;
		char * ItemEnd;
		if (m_State == IS_MSG)
		{
			m_SenderID.Format("%d",*(unsigned int*)(pPacket + 36) );
			m_RecverID.Format("%d",*(unsigned int *)(pPacket + 44) );

			m_RecverName = pPacket + 94;
			if (m_RecverName.GetLength() >= SENDERNAME_LEN)		//用户名空间最长44字节
			{
				return FALSE;
			}

			m_Message = (char*)pPacket + 139;					//聊天信息空间最长400字节
			if (m_Message.GetLength() > 400)
			{
				return FALSE;
			}
			WriteISAudioIndexFile();	 
			return TRUE;
		}	
		else if (m_State == IS_LOGIN)
		{
			ItemHead = strstr(pPacket + 100 ,"email=");
			if (ItemHead == 0)
			{
				return FALSE;
			}
			ItemHead += strlen("email=");
			ItemEnd = strchr(ItemHead,';');
			if (ItemEnd == 0)
			{
				return FALSE;
			}
			m_SenderAccount = ItemHead;
			m_SenderAccount = m_SenderAccount.Left(ItemEnd - ItemHead);

			ItemHead = strstr(ItemEnd,"nickname=");
			if (ItemHead == 0)
			{
				return FALSE;
			}
			ItemHead += strlen("nickname=");
			ItemEnd = strchr(ItemHead,';');
			if (ItemEnd == 0)
			{
				return FALSE;
			}
			m_SenderName = ItemHead;
			m_SenderName = m_SenderName.Left(ItemEnd - ItemHead);

			ItemHead = strstr(ItemEnd,"userid=");
			if (ItemHead == 0)
			{
				return FALSE;
			}
			ItemHead += strlen("userid=");
			ItemEnd = strchr(ItemHead,';');
			if (ItemEnd == 0)
			{
				return FALSE;
			}
			m_SenderID = ItemHead;
			m_SenderID = m_SenderID.Left(ItemEnd - ItemHead);
			WriteISAudioIndexFile();	 
			return TRUE;
		}
	}
	return TRUE;
}


void ISAudioSession::WriteISAudioIndexFile()
{
	m_indexfname = CreateMediaFilename(m_sip, m_dip, "media_isaudio.indexfile.txt");	
	CString txt;
	if (m_State == IS_MSG)
	{
		txt.Format("senderid=%s\r\nrecverid=%s\r\nrecvernick=%s\r\nmsg=%s\r\nstate=message\r\n",
			m_SenderID,m_RecverID,m_RecverName,m_Message);
		WriteIndexFile("media_isaudio", "media_isaudio", txt, g_MediaMissObj);
	}
	else if (m_State == IS_LOGIN)
	{
		txt.Format("userid=%s\r\nusernick=%s\r\naccount=%s\r\nstate=login\r\n",m_SenderID,m_SenderName,m_SenderAccount);
		WriteIndexFile("media_isaudio", "media_isaudio", txt, g_MediaMissObj);
	}
}