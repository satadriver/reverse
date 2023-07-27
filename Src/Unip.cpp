//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	Unip
//	版本:		1.0
//	创建日期:	2008-2-25
//	作者:		荣强
//	主要功能:	未知协议IP处理
//	模块描述:
//	主要函数:	
//**************************************************************** 

#include "stdafx.h"
#include "ReadDDeFile.h"
#include "SocketSession.h"
#include "DceMission.h"
#include "Unip.h"
#include "unipsession.h"

DceMission*			g_pUnipMissObj = NULL;
UnipSocketMap		g_unippackmap(256);
time_t				g_lastcleartime = 0;
IPMap				g_ipMap(1024);	

void ClearIpMap();


UnipProtocol::UnipProtocol()
{
	m_ddefiletype = "unip";
}

UnipProtocol::~UnipProtocol()
{
}

void UnipProtocol::ProcessBuffer(DATABUFFER& unipdata, time_t& packtm)
{
int error=0;

	try
	{
		DATEHEADER	packh; // lyb begin1
		packh.m_tm = packtm;
		int datapos=0;
		DWORD DataSrc = DATASRC_OTHER;
		for (int i = 0; i < unipdata.packetnum; i++)
		{
			char packettype = unipdata.pBuffer[datapos];
			this->m_PacketFrom = packettype;
			BOOL isCDMA = FALSE;
			switch(packettype)
			{
			case CDMATCP:
			case CdmaUnipTCP:
				packettype = UnipTCP;
				DataSrc = DATASRC_CDMA;
				isCDMA = TRUE;
				break;
			case GPRSTCP:
			case GprsUnipTCP:
				packettype = UnipTCP;
				DataSrc = DATASRC_GPRS;
				break;
			case CDMAUDP:
			case CdmaUnipUDP:
				packettype = UnipUDP;
				DataSrc = DATASRC_CDMA;
				isCDMA = TRUE;
				break;
			case GPRSUDP:
			case GprsUnipUDP:
				packettype = UnipUDP;
				DataSrc = DATASRC_GPRS;
				break;
			}
error=1;
			if ((unipdata.bufsize-datapos) >= NEWHEADERLEN)
			{
				memcpy(&packh, unipdata.pBuffer+datapos, NEWHEADERLEN);
				ASSERT(packh.m_datalen < MAXLEN);
				datapos += NEWHEADERLEN;
				if(packettype==TCPPacket || packettype==UDPPacket)
				{
					datapos += packh.m_datalen;
					packettype += 8;   //TCPPacket --> UnipTCP
				}
			}
			else
			{
				ASSERT(FALSE);
				break;
			}
error=2;
			if (packh.m_datalen>0)
			{
				SOCKETKEY skey;
				memcpy(&skey.sip, &packh.m_sip, 4);
				memcpy(&skey.dip, &packh.m_dip, 4);
				skey.sport = packh.m_sport;
				skey.dport = packh.m_dport;
				CString strISP;
				if ((v_programtype == SYSTEM_002) && (DataSrc == DATASRC_OTHER))
				{
error=3;
					// 首先处理IDCSvrPort
					BOOL isTcp = (packettype == UnipTCP);
					if (v_IdcIpLib.QueryIdcIP(Swap4Byte(skey.dip), strISP))
					{
						CString str =v_IDCSvrPort.GetSvrType(skey.dport, isTcp);
						if (v_statIdc && !str.IsEmpty())
						{
							// SIP\1SUBPRO\11PRO\1PORT\1ClassType\1明确标识\1交互\1DOMAIN 
							CString dstr;
							dstr.Format("%03u.%03u.%03u.%03u\1%s\1%s\1%s\1\1\1", 
								packh.m_dip[0], packh.m_dip[1],
								packh.m_dip[2], packh.m_dip[3],
								strISP, str, TYPESTATE_POSSIBLE);
							v_IdcServerStat[UNIP_PROTOCOL].AddServer(dstr, 0);
						}
						if (DataSrc == DATASRC_OTHER)
						{
							DataSrc = DATASRC_IDC;
						}
						else
						{
							// 如果不标识IDC数据源则不写运营商
							strISP.Empty();
						}
					}
					else if (v_IdcIpLib.QueryIdcIP(Swap4Byte(skey.sip), strISP))
					{
error=4;
						CString str =v_IDCSvrPort.GetSvrType(skey.sport, isTcp);
						if (v_statIdc && !str.IsEmpty())
						{
							CString dstr;
							dstr.Format("%03u.%03u.%03u.%03u\1%s\1%s\1%s\1\1\1", 
								packh.m_sip[0], packh.m_sip[1],
								packh.m_sip[2], packh.m_sip[3],
								strISP, str, TYPESTATE_POSSIBLE);
							v_IdcServerStat[UNIP_PROTOCOL].AddServer(dstr, 0);
						}
						if (DataSrc == DATASRC_OTHER)
						{
							DataSrc = DATASRC_IDC;
						}
						else
						{
							// 如果不标识IDC数据源则不写运营商
							strISP.Empty();
						}
					}
				}
error=5;

				UnipSession* s = 0;
				if (g_unippackmap.Lookup(skey, s))
				{
					ASSERT(s!=NULL);
					s->AddPacket(packh);
				}
				else
				{
					packh.m_type = m_PacketFrom;
					s = UnipSession::CreateUnipSession(packh, isCDMA);
					if (s)
					{
						s->m_spyaddr = unipdata.posaddr;
						s->m_DataSrc = DataSrc;
						s->m_ISP = strISP;
						// 增加到哈希表
						g_unippackmap.SetAt(skey, s);
					}
				}
			}
			g_pUnipMissObj->m_packetnum++;
		}
error=6;
		packtm = packh.m_tm;
		if (abs(packh.m_tm-g_lastcleartime)>=60)
		{
			g_lastcleartime = packh.m_tm;
	// #ifdef		_DEBUG
	// 		PROCESS_HEAP_ENTRY heapinfo;
	// 		memset(&heapinfo, 0, sizeof(heapinfo));   
	// 		heapinfo.cbData = sizeof(heapinfo);
	// 		heapinfo.wFlags = PROCESS_HEAP_REGION;
	// /		HeapWalk(g_UnipHeap, &heapinfo);
	// #endif
			ClearSocket();
		}
		return;
	}
	catch (...)
	{
		WriteLog(UNIPPRO, "UnipProtocol::ProcessBuffer Error=%d!!", error);
	}

}

BOOL UnipProtocol::InitObject(DceMission* pMiss)
{
	WriteLog(UNIPPRO, "**************** DCE Module-UNIP(ver:%s %s) Start Run!! ****************",
		__DATE__, __TIME__);

	m_pDseFileInfo = new IpDataStruct[3];
	m_pDseFileInfo[0].m_filenum = 0;
	m_pDseFileInfo[1].m_filenum = 0;
	m_pDseFileInfo[2].m_filenum = 0;
	m_pDseFileInfo[0].m_pindexfile = NULL;
	m_pDseFileInfo[1].m_pindexfile = NULL;
	m_pDseFileInfo[2].m_pindexfile = NULL;
	m_pDseFileInfo[0].m_prealfile = NULL;
	m_pDseFileInfo[1].m_prealfile = NULL;
	m_pDseFileInfo[2].m_prealfile = NULL;
	m_DseInfoCount = 3;

	g_unippackmap.InitHashTable(201119, TRUE);
	g_ipMap.InitHashTable(201119, TRUE);
//	g_fileMap.InitHashTable(1997, TRUE);
	g_pUnipMissObj = pMiss;
// 	if (pMiss->m_MissHeap==NULL)
// 		pMiss->m_MissHeap = g_UnipHeap;
	if (pMiss->m_logfilename[0] == 0)
		pMiss->m_logfilename = UNIPPRO;

	return TRUE;
}

BOOL UnipProtocol::WriteObjectState(time_t packtm, CString& strtxt)
{
// 	DWORD heapsize=0;
// 	DWORD emptysize=0;
// 	int   blockcount=0;
// 	if(!GetHeapInfo(g_UnipHeap, heapsize, emptysize, blockcount))
// 		heapsize = emptysize = blockcount = 0;
	char ptm[40];
	GetTimeString(packtm, "%Y-%m-%d %H:%M:%S", ptm, 40);
	WriteLog(UNIPPRO, "当前状态:packtm:%s  loop:%u  packnum:%u  MapSize:%u",
		ptm, g_pUnipMissObj->m_loopnum, g_pUnipMissObj->m_packetnum, g_unippackmap.GetCount());
	return TRUE;
}

void UnipProtocol::ClearSocket()
{
	int err = 0;
	if (v_programtype == SYSTEM_SLEUTH)
	{
		ClearIpMap();
	}
	time_t closetm = g_lastcleartime - 180;
	UINT idxipnum = 0;
	UINT csipnum = 0;
	CString indexfilename;
	CString casefilename;
	SYSTEMTIME nowtm;
	CString tmpdir = g_pUnipMissObj->GetTmpPath();
	GetLocalTime(&nowtm);
	CFile Csfile;
	CFile Idxfile;
	CStringArray attfiles;
	SOCKETKEY skey;
	POSITION pos = g_unippackmap.GetStartPosition();
	while(pos)
	{
		UnipSession* s = NULL;
		g_unippackmap.GetNextAssoc(pos, skey, s);
		if (s->m_begtime <= closetm)
		{
			s->WriteUnipFile(tmpdir, this);
			g_unippackmap.RemoveKey(skey);
		}
	}

// 	if (g_fileMap.GetCount()>0)
// 	{
// 		g_fileMap.RemoveAll();
// 	}

}

void ClearIpMap()
{
	static DWORD pwr = 100;
	int ipcount = g_ipMap.GetCount();
	int m = 0;
	if (ipcount > 50000)
	{
		POSITION pos=NULL;
		DWORD IP=0;
		DWORD power;
		pos = g_ipMap.GetStartPosition();
		while (pos != NULL)
		{
			power = 0;
			g_ipMap.GetNextAssoc(pos, IP, power);
			if (power < pwr)
			{
				g_ipMap.RemoveKey(IP);
				m++;
			}
		}
		 
		if (m < 5000)
			pwr += 100;
	}
}