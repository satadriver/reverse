// ftp.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ftp.h"
#include "FtpSession.h"
#include "TelnetSession.h"
#include "eMule.h"
#include "Vnc.h"
#include "terminal.h"
#include "Radmin.h"
#include "IrcSession.h"

#pragma comment( lib, "ws2_32.lib" )

DceMission*		g_pFtpMissObj	= NULL;
char*			g_FtpFILEBUFFER	= NULL;
char*			g_PROCESSBUFFER	= NULL;
DWORD			g_FtpSion		= 0;
DWORD			g_TelnetSion	= 0;
DWORD			g_VncSion		= 0;
DWORD			g_FtpFileNum	= 0;
DWORD			g_TelnetFileNum	= 0;
DWORD			g_VncFileNum	= 0;
DWORD			g_EmuleFileNum	= 0;
DWORD			g_TerminalSion	= 0;
DWORD			g_RadminSion	= 0;

// LPCTSTR pFTPSubDir[] =\
// {
// 	_T("ftp"),
// 	_T("telnet"),
// 	_T("p2p_eMule"),
// 	_T("rc_vnc"),
// 	NULL
// };
// 
// void* ::operator new (unsigned int sz)
// {
// 	ASSERT(g_FtpHeap!=NULL);
// 	return HeapAlloc(g_FtpHeap, 0, (sz+15)&~15);
// }
// 
// void  ::operator delete (void* p)
// {
// 	HeapFree(g_FtpHeap, 0, p);
// }


FtpProtocol::FtpProtocol()
{
	m_ddefiletype = "ftp";
	m_HashSize = 60003;
//	m_outdirlist = pFTPSubDir;
}

FtpProtocol::~FtpProtocol()
{
	
}

BOOL FtpProtocol::InitObject(DceMission* pMiss)
{
	WriteLog(FTPPRO, "**************** DCE Module-FTP(ver:%s %s) Start Run!! ****************",
		__DATE__, __TIME__);

	m_pDseFileInfo = new IpDataStruct[3];
	m_pDseFileInfo[0].m_filenum = 0;
	m_pDseFileInfo[1].m_filenum = 0;
	m_pDseFileInfo[2].m_filenum = 0;
	m_pDseFileInfo[0].m_realflen = 0;
	m_pDseFileInfo[1].m_realflen = 0;
	m_pDseFileInfo[2].m_realflen = 0;
	m_pDseFileInfo[0].m_realflen = 0;
	m_pDseFileInfo[1].m_realflen = 0;
	m_pDseFileInfo[2].m_realflen = 0;
	m_pDseFileInfo[0].m_pindexfile = NULL;
	m_pDseFileInfo[1].m_pindexfile = NULL;
	m_pDseFileInfo[2].m_pindexfile = NULL;
	m_pDseFileInfo[0].m_prealfile = NULL;
	m_pDseFileInfo[1].m_prealfile = NULL;
	m_pDseFileInfo[2].m_prealfile = NULL;
	m_DseInfoCount = 3;

	g_pFtpMissObj = pMiss;
	g_FtpFILEBUFFER = new char [FILEBUFFERSIZE];
	g_PROCESSBUFFER = new char [PROCESSBUFFERLEN];
	if (pMiss->m_logfilename[0] == 0)
		pMiss->m_logfilename = FTPPRO;
//	g_pFtpMissObj->m_MissHeap = g_FtpHeap;
	
	g_EMuleMap.InitHashTable(200867, FALSE);

	return TRUE;
}


BOOL FtpProtocol::ProcessPacket(DATEHEADER& packh, const char* pPacket, LPCTSTR spyaddr)
{
#ifdef  _DEBUG
	// 增加数据的过滤(仅用于协议调试)
#endif
int error=0;	
	////查是否在Map里////////////////////////////////////////////////////////
	try
	{
		// ftp
		if((packh.m_type == TCPPacket) && (packh.m_sport!=110) && (packh.m_dport!=110))
		{
			char tmppack[12];
			memset(tmppack+8, 0, 4);
			memcpy(tmppack, pPacket, 8);
error=1;
			if (memcmp(pPacket+packh.m_datalen-2, "\r\n", 2)==0 )
			{
				if(memicmp(tmppack, "USER ", 5) == 0)
				{	//
					// 对照认证信息
error=2;
					FtpSession *s = new FtpSession(packh, m_PacketFrom, FALSE);
					if(s>0)
					{
						g_pFtpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
						s->m_spyaddr = spyaddr;
error=3;
						s->InsertPacket(packh, pPacket);
					}
					return TRUE;
				}
				else if (memicmp(tmppack, "220 ", 4) == 0)
				{
					// 对照认证信息
					FtpSession *s = new FtpSession(packh, m_PacketFrom, TRUE);
error=4;
					if(s>0)
					{
						g_pFtpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
						s->m_spyaddr = spyaddr;
error=5;
						s->InsertPacket(packh, pPacket);
					}
					return TRUE;
				}
			}

		}
		//360 mac  
		if((packh.m_type == UDPPacket) && packh.m_dport==0x0050)
		{
			if(packh.m_datalen>=0x1C)
			{
				Deal360Mac((const unsigned char*)pPacket, packh);
			}
			
			return TRUE;
		}



error=6;
		// p2p
		if(ProcessEMulePacket(packh, m_PacketFrom, (UCHAR*)pPacket, spyaddr))
			return TRUE;
error=7;
		// VNC
		BOOL caseflg = TRUE;
		if (IsVNCPacket(packh, pPacket))
		{
			VncSession *s = new VncSession(packh, m_PacketFrom);
			if(s>0)
			{
				g_pFtpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
				s->m_spyaddr = spyaddr;
 				s->InsertPacket(packh, pPacket);
				return TRUE;
			}
		}
error=8;
		// telnet
		if ((packh.m_type == TCPPacket) &&
			((packh.m_sport==23) || (packh.m_dport==23)) &&
			(memicmp(pPacket, "YMSG", 4) != 0))
		{
			TelnetSession* s = new TelnetSession(packh, m_PacketFrom);
			if(s>0)
			{
				// 对照认证信息
				g_pFtpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
				s->m_spyaddr = spyaddr;
				s->InsertPacket(packh, pPacket);
				return TRUE;
			}
		}
error=9;
		//remote
		if(packh.m_type == TCPPacket && IsRadminPacket(packh, pPacket))
		{
			RadminSession* s = new RadminSession(packh, m_PacketFrom);
			if(s>0)
			{
				// 对照认证信息
				g_pFtpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
				s->m_spyaddr = spyaddr;
				s->InsertPacket(packh, pPacket);
				return TRUE;
			}
		}
error=10;
		//terminal
		if(packh.m_type == TCPPacket && IsTerminalPacket(packh, pPacket))
		{
			TerminalSession* s = new TerminalSession(packh, m_PacketFrom);
			if(s>0)
			{
				// 对照认证信息
				g_pFtpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
				s->m_spyaddr = spyaddr;
				s->InsertPacket(packh, pPacket);
				return TRUE;
			}
		}
error=11;
// 		if(packh.m_type == TCPPacket && 
// 			(memicmp(pPacket, "NICK ", 5) == 0 
// 			||memicmp(pPacket, "JOIN ", 5) == 0 
// 			|| memicmp(pPacket, "PRIVMSG ", 8) == 0))
// 		{
// 			CIrcSession* s = new CIrcSession(packh, m_PacketFrom);
// 			if(s>0)
// 			{
// 				// 对照认证信息
// 				g_pFtpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
// 				s->m_spyaddr = spyaddr;
// 				s->InsertPacket(packh, pPacket);
// 				return TRUE;
// 			}
// 		}
		if(packh.m_type == TCPPacket && packh.m_sport == 0 && packh.m_datalen > 50)
		{
			if(DealPPTPVpn(packh, pPacket, m_PacketFrom))
				return TRUE;
		}
		if(packh.m_type == TCPPacket  && packh.m_datalen > 50
			&& packh.m_sport == 1701 && packh.m_dport == 1701)
		{
			if(DealL2TPVpn(packh, pPacket, m_PacketFrom))
				return TRUE;
		}
		//openvpn 
		if(	packh.m_type == UDPPacket&&
			packh.m_datalen>0x60 &&
			memcmp(pPacket+10,"\x0\x0\x0\x01",4)==0 &&
			memcmp(pPacket+14,"\x16\x03\x01\x00\x59\x01",6)==0  ) //client hello
		{
			if(Dealopenvpn((const unsigned char*)pPacket, packh))
			{		
				return TRUE;
			}
		}	
		//openssh 
		if(	packh.m_type == TCPPacket&&
			packh.m_dport==0x0016 &&
			packh.m_datalen>0x10 &&
			memcmp(pPacket,"SSH-2.0",7)==0 
			) //client hello
		{
			if(Dealopenssh((const unsigned char*)pPacket, packh))
			{			 
				return TRUE;
			}
		}
	}
	catch (...)
	{
		WriteLog(FTPPRO, "ProcessPacket() Error!!=%d",error);
	}
	return FALSE;
}

BOOL FtpProtocol::WriteObjectState(time_t packtm, CString& strtxt)
{
	// 打印状态日志
// 	DWORD heapsize=0;
// 	DWORD emptysize=0;
// 	int   blockcount=0;
// 	if(!GetHeapInfo(g_FtpHeap, heapsize, emptysize, blockcount))
// 	{
// 		heapsize = emptysize = blockcount = 0;
// 	}

	char ptm[40];
	GetTimeString(packtm, "%Y-%m-%d %H:%M:%S", ptm, 40);
	WriteLog(FTPPRO,
		"当前状态:packtm:%s  loop:%u  packnum:%u\r\n"
		"ftp:%u_%u    telnet:%u_%u    vnc:%u_%u    eMule:%u_%u\r\n",
		ptm, g_pFtpMissObj->m_loopnum, g_pFtpMissObj->m_packetnum, g_FtpSion, g_FtpFileNum,
		g_TelnetSion, g_TelnetFileNum, g_VncSion, g_VncFileNum, g_EMuleMap.GetCount(), g_EmuleFileNum);

	// 检查EMule哈希表，清除2小时前的数据
	POSITION pos;
	pos = g_EMuleMap.GetStartPosition();
	CString key;
	time_t val;
	time_t tm = packtm-7200;
	while (pos)
	{
		g_EMuleMap.GetNextAssoc(pos, key, val);
		if (val <= tm)
		{
			g_EMuleMap.RemoveKey(key);
		}
	}
	return TRUE;
}

