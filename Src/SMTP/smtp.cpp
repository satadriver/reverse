
#include "stdafx.h"
#include "smtp.h"
#include "SmtpSession.h"

//#define			new		smtpnew
//#define			delete	smtpdelete

//#include <Objbase.h>

//HANDLE			g_SmtpHeap = HeapCreate(HEAP_NO_SERIALIZE, 2048*1024, 0);
DceMission*		g_smtpMissObj = NULL;
char*			g_SmtpFileBuffer=NULL;
DWORD			g_smtpFileNum = 0;

//调试用的公共变量
DWORD			g_RubbishCount=0;
DWORD			g_ReUseCount = 0;
BOOL			g_RubPacketCatch = FALSE;
// CatchPacketList	g_RubPacketList;


// void* operator new (unsigned int sz)
// {
// 	ASSERT(g_SmtpHeap!=NULL);
// 	return HeapAlloc(g_SmtpHeap, 0, (sz+15)&~15);
// }
// 
// void AFX_CDECL delete (void* p)
// {
// 	HeapFree(g_SmtpHeap, 0, p);
// }

SmtpProtocol::SmtpProtocol()
{
	m_ddefiletype = "smtp";
	m_HashSize = 201119;
}

SmtpProtocol::~SmtpProtocol()
{
// 	if (g_SmtpHeap)
// 	{
// 		HeapDestroy(g_SmtpHeap);
// 	}
}

BOOL SmtpProtocol::WriteObjectState(time_t now, CString& strtxt)
{
	strtxt.Format("RubbishPacket: %u_%u\r\n",
		g_RubbishCount, g_ReUseCount);
	g_RubbishCount=0;
	g_ReUseCount = 0;
	return FALSE;
}

BOOL SmtpProtocol::InitObject(DceMission* pMiss)
{
	::CoInitialize(NULL);
	WriteLog(SMTPPRO, "**************** DCE Module-SMTP(ver: %s %s) Start Run!! ****************",
		__DATE__, __TIME__);

	m_DseInfoCount = g_DseServer.m_count*3;
	m_pDseFileInfo = new IpDataStruct[m_DseInfoCount];
	for (int i=0; i<m_DseInfoCount; i++)
	{
		m_pDseFileInfo[i].m_filenum = 0;
		m_pDseFileInfo[i].m_realflen = 0;
		m_pDseFileInfo[i].m_pindexfile = NULL;
		m_pDseFileInfo[i].m_prealfile = NULL;
	}

// 	if (pMiss->m_MissHeap==NULL)
// 	{
// 		pMiss->m_MissHeap = g_SmtpHeap;
// 	}
	if (pMiss->m_logfilename[0] == 0)
	{
		pMiss->m_logfilename = SMTPPRO;
	}
	g_SmtpFileBuffer = new char [FILEBUFFERSIZE];
	g_smtpMissObj = pMiss;
	return TRUE;
	if (_access(v_WorkPath+"\\flg\\smtpcah.flg", 0) == 0)
	{
		WriteLog(SMTPPRO, "协议打开包缓存机制!");
		g_RubPacketCatch = TRUE;
	}
	return TRUE;
}

// void CheckCatchList(DWORD now)
// {
// 	if ((now - g_RubPacketList.m_LastChecktm) > 10*1000)	// 10s
// 	{
// 		g_RubPacketList.m_LastChecktm = now;
// 		char key[100];
// 		RubbPacketListIt item;
// 		RubbPacketListIt end = g_RubPacketList.m_PacketList.end();
// 		RubbPacketListIt tmpitem = g_RubPacketList.m_PacketList.begin();
// 		while(tmpitem != end)
// 		{
// 			item = tmpitem;
// #ifdef	_DEBUG
// 			RUBBPACKET *pp = &*item;
// #endif
// 			tmpitem++;
// 			if ((now - item->m_packtm) >= 30*1000)
// 			{
// 				memset(key, 0, sizeof(key));
// 				SocketSession* s = NULL;
// 				sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d",
// 					item->m_packh.sip[0], item->m_packh.sip[1], item->m_packh.sip[2], item->m_packh.sip[3], item->m_packh.sport,
// 					item->m_packh.dip[0], item->m_packh.dip[1], item->m_packh.dip[2], item->m_packh.dip[3], item->m_packh.dport);
// 				if (!g_smtpMissObj->m_SocketMap.Lookup(key, s))
// 				{
// 					sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d",
// 						item->m_packh.dip[0], item->m_packh.dip[1], item->m_packh.dip[2], item->m_packh.dip[3], item->m_packh.dport,
// 						item->m_packh.sip[0], item->m_packh.sip[1], item->m_packh.sip[2], item->m_packh.sip[3], item->m_packh.sport);
// 					g_smtpMissObj->m_SocketMap.Lookup(key, s);
// 				}
// 
// 				if (s != NULL)
// 				{
// 					// 将包插入会话
// 					s->InsertPacket(item->m_packh, item->m_buf);
// 					g_ReUseCount++;
// 					delete[] item->m_buf;
// 					item->m_buf = NULL;
// 					g_RubPacketList.m_PacketList.erase(item);
// 				}
// 				else if ((now-item->m_packtm) >= 60*1000)
// 				{
// 					// 丢弃长时间没有会话的包
// 					delete[] item->m_buf;
// 					item->m_buf = NULL;
// 					g_RubPacketList.m_PacketList.erase(item);
// 				}
// 			}
// 			else
// 				break;
// 		}
// 	}
// }

BOOL SmtpProtocol::ProcessPacket(DATEHEADER& packh, const char* pPacket, LPCTSTR spyaddr)
{
	int re=0;
	SmtpSession *s=0;
#ifdef  _DEBUG
//	增加数据的过滤(仅用于协议调试)
// 	if (packh.sip[3]==139 || packh.dip[3]==139){
// 	}
// 	else
// 		return FALSE;
// 	if (packh.m_sport != 50633)
// 		return FALSE;
#endif

	try
	{
		DWORD now = GetTickCount();
		char tmppack[16];
		memset(tmppack+12, 0, 4);
		memcpy(tmppack, pPacket, 12);
		strupr(tmppack);
		if((memcmp(tmppack, "HELO ",5)==0 || \
			memcmp(tmppack, "EHLO ",5)==0 || \
			memcmp(tmppack, "AUTH LOGIN",10)==0 || \
			memcmp(tmppack, "MAIL FROM:",10)==0 ) && 
			memcmp(pPacket+packh.m_datalen-2, "\r\n", 2)==0 )
		{	
			s = new SmtpSession(packh, m_PacketFrom);
			if(s>0)
			{
				// 对照认证信息
				g_smtpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
				s->m_spyaddr = spyaddr;
				s->InsertPacket(packh, pPacket);
			}
			return TRUE;
		}
		g_RubbishCount++;
	}
	catch (...)
	{
		WriteLog(SMTPPRO, "ProcessPacket() Error!!");
	}
	return FALSE;
}

