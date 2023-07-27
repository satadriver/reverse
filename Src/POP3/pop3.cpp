#include "stdafx.h"
#include "pop3.h"
#include "Pop3Session.h"
#include "ImapSession.h"

// extern "C" PROCOTOL_EXTEND DceProcotol* CreateProcotolObjectv2(void);

// HANDLE			g_Pop3Heap= HeapCreate(HEAP_NO_SERIALIZE, 2048*1024, 0);
DceMission*		g_pop3MissObj = NULL;
DWORD			g_Pop3FileNum = 0;
char*			g_Pop3FILEBUFFER=NULL;
DWORD			g_RubbishPop3Count=0;

// void* ::operator new (unsigned int sz)
// {
// 	ASSERT(g_Pop3Heap!=NULL);
// 	return HeapAlloc(g_Pop3Heap, 0, (sz+15)&~15);
// }/
// 
// void  ::operator delete (void* p)
// {
// 	HeapFree(g_Pop3Heap, 0, p);
// }


Pop3Protocol::Pop3Protocol()
{
	m_ddefiletype = "pop3";
	m_HashSize = 201119;
}

Pop3Protocol::~Pop3Protocol()
{
// 	if (g_Pop3Heap)
// 	{
// 		HeapDestroy(g_Pop3Heap);
// 	}
}

BOOL Pop3Protocol::WriteObjectState(time_t now, CString& strtxt)
{
	strtxt.Format("RubbishPacket: %u\r\n", g_RubbishPop3Count);
	g_RubbishPop3Count=0;
	return FALSE;
}

BOOL Pop3Protocol::InitObject(DceMission* pMiss)
{
	::CoInitialize(NULL);
	WriteLog(POP3PRO, "**************** DCE Module-POP3(ver:%s %s) Start Run!! ****************",
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
// 		pMiss->m_MissHeap = g_Pop3Heap;
// 	}
	if (pMiss->m_logfilename[0] == 0)
	{
		pMiss->m_logfilename = POP3PRO;
	}
	g_Pop3FILEBUFFER = new char [FILEBUFFERSIZE];
	g_pop3MissObj = pMiss;
	return TRUE;
}

BOOL Pop3Protocol::ProcessPacket(DATEHEADER& packh, const char* pPacket, LPCTSTR spyaddr)
{
	int re=0;
	SocketSession *s=0;
#ifdef  _DEBUG
//	增加数据的过滤(仅用于协议调试)
// 	if (packh.sport != 44722 && packh.dport != 44722)
// 		return FALSE;
	
#endif

	try
	{
		char tmppack[16];
		memset(tmppack+12, 0, 4);
		memcpy(tmppack, pPacket, 12);
		strupr(tmppack);
		if((strncmp(tmppack, "USER ", 5)==0 || \
			strncmp(tmppack, "PASS ", 5)==0 || \
			strncmp(tmppack, "RETR ", 5)==0 || \
			strcmp(tmppack,  "LIST\r\n")==0 ) &&  
			strncmp(pPacket+packh.m_datalen-2, "\r\n", 2)==0 )
		{	
			if(packh.m_dport==21)	// 过滤掉FTP
			{
				return FALSE;
			}
			s = (SocketSession*)new Pop3Session(packh, m_PacketFrom, FALSE);
			if(s>0)
			{
				g_pop3MissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
				s->m_spyaddr = spyaddr;
				s->InsertPacket(packh, pPacket);
				return TRUE;
			}
		}
		if(strncmp(tmppack, "+OK", 3)==0 && 
			SearchCRLF.find(pPacket, packh.m_datalen)>2 )
		{
			s = (SocketSession*)new Pop3Session(packh, m_PacketFrom, TRUE);
			if(s>0)
			{
				g_pop3MissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
				s->m_spyaddr = spyaddr;
				s->InsertPacket(packh, pPacket);
			}
			return TRUE;
		}
// 		if(packh.m_sport == 143 || packh.m_dport == 143)
// 		{
// 			Search srhFETCH("FETCH (UID");
// 			Search srhBODY("BODY[] {");
// 			if(srhBODY.find(pPacket, packh.m_datalen) > 0 && srhFETCH.find(pPacket, packh.m_datalen) > 0)
// 			{
// 				s = (SocketSession*)new ImapSession(packh, m_PacketFrom, TRUE);
// 				if(s>0)
// 				{
// 					g_pop3MissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
// 					s->m_spyaddr = spyaddr;
// 					s->InsertPacket(packh, pPacket);
// 				}
// 				return TRUE;
// 			}
// 			if(strncmp(tmppack+5, "LOGIN", 5)==0 && 
// 				SearchCRLF.find(pPacket, packh.m_datalen)>2 )
// 			{
// 				s = (SocketSession*)new ImapSession(packh, m_PacketFrom, FALSE);
// 				if(s>0)
// 				{
// 					g_pop3MissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
// 					s->m_spyaddr = spyaddr;
// 					s->InsertPacket(packh, pPacket);
// 				}
// 				return TRUE;
// 			}
// 		}
		g_RubbishPop3Count++;
	}
	catch (...)
	{
		WriteLog(POP3PRO, "ProcessPacket() Error!!");
	}
	return FALSE;
}
