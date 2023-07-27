#include "StdAfx.h"
#include "Telegram.h"


CMap< CString,LPCSTR,TELEGRAMINFO *,TELEGRAMINFO * &>cmapTelegram;







#define TELEGRAM_INIT_PACKET		1

#define TELEGRAM_FIRST_PACKET		2

#define TELEGRAM_NEXT_PACKET		3

#define TELEGRAM_PACKET_CNT			16




int IsTelegramInitPacket(DATEHEADER & packh, unsigned char * pPacket, int Start)
{
	if (memcmp(pPacket + Start, "\x00\x00\x00\x00\x00\x00\x00\x00", 8) == 0)		//authkey  msgid
	{
		if (*(unsigned int*)(pPacket  + Start + 16) == 0x00000014)
		{
			if (*(unsigned int*)(pPacket + Start + 20) == 0x60469778)
			{
				return TELEGRAM_INIT_PACKET;
			}
		}
	}

	return FALSE;
}



int IsTelegramPacket(DATEHEADER & packh, const char * pPacket)
{
	int Result = 0;
	if (packh.m_sport == 443 || packh.m_dport == 443)
	{
		if (*(unsigned int*)pPacket == packh.m_datalen && *(unsigned int*)(pPacket + 4) == 0)
		{
			Result = IsTelegramInitPacket(packh, (unsigned char *)pPacket, 8);
			return Result;
		}
		else if (*(unsigned char*)pPacket == 0xef)
		{
			if (*(unsigned char*)(pPacket + 1)*4 + 2 == packh.m_datalen)
			{
				Result = IsTelegramInitPacket(packh, (unsigned char *)pPacket , 2);
				if (Result == 0)
				{
					return TELEGRAM_FIRST_PACKET;
				}
				return Result;
			}
		}
		else if (*(unsigned char*)pPacket == 0x7f)
		{
			unsigned int Size = *(unsigned int*)(pPacket +1) & 0x00ffffff;
			if (Size * 4 + 4 == packh.m_datalen)
			{
				Result = IsTelegramInitPacket(packh, (unsigned char *)pPacket, 4);
				if (Result == 0)
				{
					return TELEGRAM_FIRST_PACKET;
				}
				return Result;
			}
		}
		else if (*(unsigned char*)pPacket * 4 + 1 == packh.m_datalen)
		{
			return TELEGRAM_NEXT_PACKET;
		}
		/*
		else if (*(unsigned int*)pPacket == packh.m_datalen && *(unsigned int*)pPacket% 4 == 0)
		{
			return TELEGRAM_NEXT_PACKET;
		}
		*/
	}

	return FALSE;
}

int ProcessTelegramPacket(DATEHEADER & packh, const char * pPacket, HttpProtocol * pHttpPro)
{
	char SocketKey[32];
	LPTELEGRAMINFO TeleInfo = 0;

	int Result = IsTelegramPacket(packh,pPacket);


	if (Result == TELEGRAM_INIT_PACKET )
	{
		WriteTelegramIndexFile(packh,pHttpPro);
	}
	else if (Result == TELEGRAM_FIRST_PACKET )
	{
		sprintf(SocketKey, "%08x_%04x_%08x_%04x", *(unsigned int*)packh.m_sip, packh.m_sport, *(unsigned int*)packh.m_dip, packh.m_dport);
		Result = cmapTelegram.Lookup(SocketKey, TeleInfo);
		if (Result)
		{
			TeleInfo->Cnt ++;
			cmapTelegram.SetAt(SocketKey,TeleInfo);
			if (TeleInfo->Cnt >= TELEGRAM_PACKET_CNT)
			{
				WriteTelegramIndexFile(packh,pHttpPro);
				cmapTelegram.RemoveKey(SocketKey);
				delete TeleInfo;
			}
		}
		else
		{
			TeleInfo = new TELEGRAMINFO;
			TeleInfo->Cnt = 1;
			TeleInfo->Time = time(0);
			if (*(unsigned char *)pPacket == 0xef)
			{
				memmove(TeleInfo->AuthKey, pPacket + 2, 8);
				memmove(TeleInfo->MsgId, pPacket + 10, 8);
			}
			else if (*(unsigned char *)pPacket == 0x7f)
			{
				memmove(TeleInfo->AuthKey, pPacket + 4, 8);
				memmove(TeleInfo->MsgId, pPacket + 12, 8);
			}
			else
			{
				memmove(TeleInfo->AuthKey, pPacket + 8, 8);
				memmove(TeleInfo->MsgId, pPacket + 16, 8);
			}
			cmapTelegram.SetAt(SocketKey,TeleInfo);
		}
	}
	else if (Result == TELEGRAM_NEXT_PACKET)
	{
		sprintf(SocketKey, "%08x_%04x_%08x_%04x", *(unsigned int*)packh.m_sip, packh.m_sport, *(unsigned int*)packh.m_dip, packh.m_dport);
		Result = cmapTelegram.Lookup(SocketKey, TeleInfo);
		if (Result)
		{
			TeleInfo->Cnt ++;
			cmapTelegram.SetAt(SocketKey,TeleInfo);
			if (TeleInfo->Cnt >= TELEGRAM_PACKET_CNT)
			{
				WriteTelegramIndexFile(packh,pHttpPro);
				cmapTelegram.RemoveKey(SocketKey);
				delete TeleInfo;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}





void WriteTelegramIndexFile(DATEHEADER & packh, HttpProtocol * pHttpPro)
{
	HttpSession clsHttpSession(packh, g_httpFrom);		
	clsHttpSession.m_dceprocotol=pHttpPro;

	CString cstrTxt;
	cstrTxt.Format("state=µÇÂ¼\r\n");

	SYSTEMTIME stNow;
	GetLocalTime(&stNow);
	CString tmppath = clsHttpSession.m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	clsHttpSession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_imsg_telegram.indexfile.txt", 
		tmppath, stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour,
		stNow.wMinute, stNow.wSecond, clsHttpSession.m_sip, clsHttpSession.m_dip, rand());
	clsHttpSession.WriteIndexFile("imsg_telegram","imsg_telegram",cstrTxt,clsHttpSession.m_dceprocotol->m_httpMissObj);
	return;
}