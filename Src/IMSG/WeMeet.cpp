
#include "StdAfx.h"

#include "imsg.h"
#include "WeMeet.h"


WeMeet::WeMeet(DATEHEADER& packh, int aT) : SocketSession(packh,aT)
{
	m_begfiletm = packh.m_tm;
	m_closetm = packh.m_tm + 120;
}

WeMeet::~WeMeet()
{
	
}

BOOL WeMeet::CloseSession(time_t nowtm)
{
	return FALSE;
}

int WeMeet::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	int offset = IsConv(packh,pPacket);
	if (offset != 0)
	{
		char *tmp = (char *)pPacket + offset;
		char *tmpb = tmp;
		char *tmpe = tmpb;
		int tmplen = 0;
		while(1)
		{
			tmpb--;
			if ( ('0' <= char(*tmpb)) && (char(*tmpb) <= '9'))
			{
				tmplen++;
			}
			else
			{
				tmpb++;
				break;
			}
			if (tmplen > 15)
			{
				return 0;
			}
		}

		char tmpfrom[16] = {0};
		memcpy(tmpfrom,tmpb,tmplen);
		CString strFrom(tmpfrom);

		if (strFrom.GetLength() < 7)
		{
			return 0;
		}

		//--
		tmpb = tmp + 6;
		tmpe = tmpb;

		tmplen = 0;
		char tmpto[16] = {0};
		CString strTo;
		unsigned char tmpc = '\0';
		
		while(1)
		{
			if ( ('0' <= char(*tmpe)) && (char(*tmpe) <= '9'))
			{
				tmpe++;
				tmplen++;
			}
			else
			{
				if (memcmp(tmpe,"\x18\x00",2) == 0)
				{
					tmpc = (unsigned char)*(tmpe + 2);
					if ( ('0' <= tmpc) && (tmpc <= '9'))
					{
						break;
					}
					else
					{
						return 0;
					}
				}
				break;
			}
			if (tmplen > 15)
			{
				return 0;
			}
		}

		
		memcpy(tmpto,tmpb,tmplen);

		
		if (tmpc == '\0')
		{
			strTo.Format("%s",tmpto);
		}
		else
		{
			strTo.Format("%s%c",tmpto,tmpc);
		}

	


		//--
		int tmpoffset = 0;
		tmpb = NULL;
		while(1)
		{
			tmpb = strstr(tmpe + tmpoffset,tmpto);
			if (tmpb)
			{
			
				break;
			}
			if (tmpoffset > 32)
			{
				break;
			}
			tmpoffset++;
		}
		
		if (tmpb)
		{
			
			unsigned char msglen = 0;
			
			msglen = *(tmpb + strTo.GetLength());
			tmpb = tmpb + strTo.GetLength() + 1;
			
		
			char *tmpmsg = new char[msglen + 1];
			memset(tmpmsg,0,msglen + 1);
			memcpy(tmpmsg,tmpb,msglen);
			
			
			CString strMsg(tmpmsg);
			
			delete [] tmpmsg;
			ConvertUtf8toGbk(strMsg,strMsg.GetLength(),strMsg);
			
			if (strFrom.IsEmpty() || strTo.IsEmpty() || strMsg.IsEmpty())
			{
				return FALSE;
			}
			CString strSuffix = "wemeet.indexfile.txt";
			m_indexfname = CreateFilename(m_sip, m_dip, strSuffix);
			CString txt;
			txt.Format("user=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=发送信息\r\n",strFrom,strFrom,strTo,strMsg);
			WriteIndexFile("imsg_wemeet", "imsg_wemeet", txt, g_pImsgMissObj);
			return TRUE;
		}
		
	}/*
	if (IsConv(packh,pPacket))
	{

		char *tmpbeg = (char *)pPacket + 23;
		
		
		char *tmpend = strstr(tmpbeg,"-");
		if (!tmpend)
		{
			return 0;
		}
		char FromId[16] = {0};
		memcpy(FromId,tmpbeg,tmpend - tmpbeg);
		
		CString strFrom(FromId);
		
		tmpend = tmpend + 6;
		tmpbeg = tmpend;
		
		int tmplen = 0;
		while(1)
		{
			if ( ('0' <= char(*tmpend)) && (char(*tmpend) <= '9'))
			{
				tmpend++;
				tmplen++;
			}
			else
			{
				break;
			}
			if (tmplen > 15)
			{
				return 0;
			}
		}
		
		char ToId[16] = {0};
		memcpy(ToId,tmpbeg,tmplen);
		
		CString strTo(ToId);
		tmpbeg = (char *)pPacket + 98;
		
		CString strMsg(tmpbeg);
		strMsg = strMsg.Left(strMsg.GetLength() - 4);
		ConvertUtf8toGbk(strMsg,strMsg.GetLength(),strMsg);

		CString strSuffix = "wemeet.indexfile.txt";
		m_indexfname = CreateFilename(m_sip, m_dip, strSuffix);
		CString txt;
		txt.Format("user=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=发送信息\r\n",strFrom,strFrom,ToId,strMsg);
		WriteIndexFile("imsg_wemeet", "imsg_wemeet", txt, g_pImsgMissObj);
		
		m_closetm += 10;
		return TRUE;

	}*/

	/*
	if (IsRoom(packh,pPacket))
	{
		char *tmpbeg = (char *)pPacket + 23;


		char *tmpend = strstr(tmpbeg,"-");
		if (!tmpend)
		{
			return 0;
		}
		char FromId[16] = {0};
		memcpy(FromId,tmpbeg,tmpend - tmpbeg);

		CString strFrom(FromId);

		tmpend = tmpend + 6;
		tmpbeg = tmpend;

		int tmplen = 0;
		while(1)
		{
			if ( ('0' <= char(*tmpend)) && (char(*tmpend) <= '9'))
			{
				tmpend++;
				tmplen++;
			}
			else
			{
				break;
			}
			if (tmplen > 15)
			{
				return 0;
			}
		}

		char RoomId[16] = {0};
		memcpy(RoomId,tmpbeg,tmplen);

		CString strRoom(RoomId);
		tmpbeg = (char *)pPacket + 98;

		CString strMsg(tmpbeg);
		strMsg = strMsg.Left(strMsg.GetLength() - 4);
		ConvertUtf8toGbk(strMsg,strMsg.GetLength(),strMsg);

		CString strSuffix = "wemeet.indexfile.txt";
		m_indexfname = CreateFilename(m_sip, m_dip, strSuffix);
		CString txt;
		txt.Format("user=%s\r\nfrom=%s\r\nmsgtype=%s\r\nmsg=%s\r\nstate=聊天室信息\r\n",strFrom,strFrom,strRoom,strMsg);
		WriteIndexFile("imsg_wemeet", "imsg_wemeet", txt, g_pImsgMissObj);

		m_closetm += 10;
		return TRUE;
	}*/
	return FALSE;
}

int WeMeet::IsConv(DATEHEADER& packh, const char* pPacket)
{

	for ( int offset = 80; offset > 20; offset--)
	{
		if ( memicmp(pPacket + offset,"-conv-",6) == 0)
		{
			return offset;
		}
	}

/*
	if ( memicmp(pPacket + 32,"-conv-",6) == 0)
	{
		return 32;
	}

	if ( memicmp(pPacket + 29,"-conv-",6) == 0)
	{
		return 29;
	}*/
	return 0;
}

int WeMeet::IsRoom(DATEHEADER& packh, const char* pPacket)
{
	if ( strstr(pPacket + 23,"-room-"))
	{
		return 1;
	}
	return 0;
}