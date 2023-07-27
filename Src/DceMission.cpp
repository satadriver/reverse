#include "Stdafx.h"
#include "ReadDDeFile.h"
#include "SocketSession.h"
#include "DceMission.h"

//int ReadDDeData(DDEFILEINFO* pDDEInfo, DATABUFFER& databuf, LPCTSTR pro);

DceMission::DceMission()
:m_SocketMap(128)
{
	m_loopnum = m_packetnum = 0;
	m_MissHeap = NULL;
	m_logfilename="";
	m_ddebakflg = 0;
}

CString DceMission::GetTmpPath()
{
	CString path;
	if (CheckZDrive())
	{
		path.Format("%s:\\dce\\tmp\\%s\\", g_ddepath,m_logfilename);
	}
	else
	{
		path.Format("%s\\tmp\\%s\\", v_datapath, m_logfilename);
	}
	CreateDir(path);
	return path;
}

// CString DceMission::QueryIpAddr(LPCTSTR strip)
// {
// 	ASSERT(FALSE);
// 	return k_IpQueryTable.QueryIpAddr(strip);
// }

// CString DceMission::QueryIpAddr(DWORD uip)
// {
// 	return  k_IpQueryTable.QueryIpAddr(uip);
// }

DWORD WINAPI DceMission::MissRunProc(LPVOID p)
{
	DceMission* pMiss = (DceMission*)p;
	DceProcotol* pSubpro = pMiss->m_pSubPro;
	ASSERT(pSubpro!=NULL);

	if (pSubpro->InitObject(pMiss))
	{
		// 删除各协议的临时数据
		char cmd[200];
		sprintf(cmd, "del %s\\tmp\\%s\\*.* /s /q", v_datapath, pMiss->m_logfilename);
		system(cmd);
		sprintf(cmd, "del %s:\\dce\\tmp\\%s\\*.* /s /q", g_ddepath,pMiss->m_logfilename);
		system(cmd);
	}
	
//	pMiss->m_SocketMap.m_memheap = pMiss->m_MissHeap;
	if (pSubpro->m_HashSize == 0)
	{
		pMiss->RunForNoHash();
	}
	else
	{
		pMiss->m_SocketMap.InitHashTable(pSubpro->m_HashSize, FALSE);
		pMiss->RunForOneSubPro();
	}
	return 0;
}

SocketSession* DceMission::LocateSession(DATEHEADER& packh, char* key)
{
	/*
	SocketSession *session=NULL;
	sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d_%d",
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
		packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_sequence);
	if (m_SocketMap.Lookup(key,session))
		return session;
	
	sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d_%d",
		packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,packh.m_sequence);
	if(m_SocketMap.Lookup(key,session))
		return session;
	else
		return NULL;
	*/
	SocketSession *session=NULL;
	sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d",
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
		packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport);
	if (m_SocketMap.Lookup(key,session))
		return session;
	
	sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d",
		packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport);
	if(m_SocketMap.Lookup(key,session))
		return session;
	else
		return NULL;
}

void DceMission::RunForNoHash()
{
	ASSERT(this->m_pSubPro!=NULL);
	DATABUFFER ddedata; // lyb begin
	srand(time(0)+GetTickCount());	// 创建本协议的随机因子
	// 初始化DDE线程
	DDEFILEINFO* pDDeFile = this->m_pDdeFileInfo;
	::Sleep(1000);
	time_t now=time(0);
	time_t CurOutTime = now / 60;	// 当前文件输出的时间(精确到分钟)
	m_pSubPro->RenameTmpDir();
	time_t logtm=now;
	time_t cleartm=now;
	time_t packtm=0;
	time_t clrtm = 0;
	DWORD packnum=0;
	// 进入主循环
	int err=0;
	while(v_DceRunFlg)
	{
		try
		{
err=0;
			if ((++m_loopnum&15) == 0)
			{
err=1;
				now = time(0);
				if (packnum!=m_packetnum)
				{
					packnum = m_packetnum;
					cleartm = now;
				}
				else if(now-cleartm>=30)
				{
err=2;
					cleartm = now;
					packtm += 30;
					ddedata.packetnum = 0;
					clrtm = packtm;
					if (m_SocketMap.GetCount()>0)
					{
						DWORD begtm = GetTickCount();
						int bcount = m_SocketMap.GetCount();
						ClearSocketMap(packtm);
						DWORD endtm = GetTickCount();
						if ((endtm - begtm) > 20*1000)
						{
							WriteLog(m_logfilename, "清除哈希表(%d --> %d)用了(%d秒)",
								bcount, m_SocketMap.GetCount(), (endtm - begtm)/1000);
						}
					}
					else
						m_pSubPro->ProcessBuffer(ddedata, packtm);
				}
				if (CurOutTime != (now / 60))
				{
					// 需要输出文件了
					CurOutTime = now / 60;
err=3;
					m_pSubPro->RenameTmpDir();
				}
				if (abs(now-logtm)>=5*60)
				{
err=4;
					srand(now+rand());
					logtm = now;
					CString strtxt;
					if (!m_pSubPro->WriteObjectState(packtm, strtxt))
					{
err=5;
// 						DWORD heapsize=0;
// 						DWORD emptysize=0;
// 						int   blockcount=0;
// 						if (m_MissHeap!=NULL)
// 						{
// 							if(!GetHeapInfo(m_MissHeap, heapsize, emptysize, blockcount))
// 								heapsize = emptysize = blockcount = 0;
// 						}
						char ptm[40];
						GetTimeString(packtm, "%Y-%m-%d %H:%M:%S", ptm, 40);
						if (m_SocketMap.GetCount()>0)
						{
// 							WriteLog(m_logfilename, "当前状态:packtm:%s\r\nloop:%u  packnum:%u  mapsize:%d  v_ProbeAAAMapsize:%d heapco:%d  heapsz:%u(%u)\r\n%s",
// 								ptm, m_loopnum, m_packetnum, m_SocketMap.GetCount(), v_ProbeAAAMap.GetCount(),blockcount, heapsize, emptysize, strtxt);
							WriteLog(m_logfilename, "当前状态:packtm:%s  loop:%u  packnum:%u  mapsize:%d  %s",
								ptm, m_loopnum, m_packetnum, m_SocketMap.GetCount(), strtxt);
						}
						else
						{
// 							WriteLog(m_logfilename, "当前状态:packtm:%s  loop:%u  packnum:%u  v_ProbeAAAMapsize:%d  heapco:%d heapsize:%u(%u)\r\n%s",
// 								ptm, m_loopnum, m_packetnum, v_ProbeAAAMap.GetCount(), blockcount, heapsize,emptysize, strtxt);
							WriteLog(m_logfilename, "当前状态:packtm:%s  loop:%u  packnum:%u  %s",
								ptm, m_loopnum, m_packetnum, strtxt);
						}
					}
#ifndef		PACKETUNM
					m_loopnum = m_packetnum = 0;					
#endif
				}
			}
err=6;
			int wait = ReadDDeData(pDDeFile, ddedata, this->m_ddefiletype);
err=7;
			if (ddedata.packetnum>0)
			{
err=8;
				m_pSubPro->ProcessBuffer(ddedata, packtm);
err=9;
				if (abs(packtm-clrtm) >=60)
				{
err=10;
					clrtm = packtm;
					if (m_SocketMap.GetCount()>0)
					{
						DWORD begtm = GetTickCount();
						int bcount = m_SocketMap.GetCount();
						ClearSocketMap(packtm);
						DWORD endtm = GetTickCount();
						if ((endtm - begtm) > 10*1000)
						{
							WriteLog(m_logfilename, "清除哈希表(%d --> %d)用了(%d秒)",
								bcount, m_SocketMap.GetCount(), (endtm - begtm)/1000);
						}
					}
				}
				else
				{
					Sleep(wait);
				}
			}
			else
			{
				Sleep(wait);
			}
		}
		catch(...)
		{
			WriteLog(m_logfilename, "RunForNoHash() Error=%d!!", err);
		}
	}
}

void DceMission::RunForOneSubPro()
{
	ASSERT(this->m_pSubPro!=NULL);
	DATABUFFER ddedata;
	char Packet[MAXLEN];
	DATEHEADER dataheader;
	memset(&dataheader, 0, sizeof(DATEHEADER));
	srand(time(0)+GetTickCount());	// 创建本协议的随机因子
	// 初始化DDE线程
	DDEFILEINFO* pDDeFile = this->m_pDdeFileInfo;
	::Sleep(1000);
	time_t now=time(0);
	time_t CurOutTime = now / 60;
	m_pSubPro->RenameTmpDir();
	time_t logtm=now;
	time_t cleartm=0;
	//char strtm[20];
	// 进入主循环
	int datapos;
	DWORD packnum = 0;
	time_t cltm = now;
	int err = 0;
	int isClear=0;
	// 增加http的运行轨迹跟踪
#ifdef		ADD_RUNSTATE_INFO
	BOOL bDumpRunState;
	if (stricmp(this->m_ddefiletype, "http") == 0)
		bDumpRunState = TRUE;
	else
		bDumpRunState = FALSE;
#endif

	DATABUFFER oneBuf;		// HttpTeSpread命令
//	DATEHEADER oneHdr;		

//	DWORD bufcount = 0;
	while(v_DceRunFlg)
	{
		try
		{
err = 0;
			if ((++m_loopnum & 15) == 0)
			{
err = 1;
				now = time(0);
				if (packnum!=m_packetnum)
				{
					packnum = m_packetnum;
					cltm = now;
				}
				else if(now-cltm >= 60)
				{
					dataheader.m_tm += 60;
					cltm = now;
				}
				if (CurOutTime != (now / 60))
				{
err=100;
					CurOutTime = now / 60;
					m_pSubPro->RenameTmpDir();
				}
				if (abs(logtm - now)>= 5*60)
				{
err=101;
					srand(now+rand());
					logtm = now;
// #ifndef		_DEBUG
// 					if (!CheckDogResult())
// 					{
// err=102;
// 						WriteLog(m_logfilename, "ClearSocketMap()");
// 						ClearSocketMap(logtm+60);
// 					}
// 					else
// #endif
					{
err=120;
						CString strtxt;
						if (!m_pSubPro->WriteObjectState(now, strtxt))
						{
err=121;
// 							DWORD heapsize=0;
// 							DWORD emptysize=0;
// 							int   blockcount=0;
// 							if (m_MissHeap!=NULL)
// 							{
// 								if(!GetHeapInfo(m_MissHeap, heapsize, emptysize, blockcount))
// 									heapsize = emptysize = blockcount = 0;
// 							}
							char ptm[40];
							GetTimeString(dataheader.m_tm, "%Y-%m-%d %H:%M:%S", ptm, 40);
							WriteLog(m_logfilename, "当前状态:packettime:%s\r\nloop:%u  packnum:%u  mapsize:%u  %s",
								ptm, m_loopnum, m_packetnum, m_SocketMap.GetCount(), strtxt);
						}
					}
#ifndef		PACKETUNM
					m_loopnum = m_packetnum = 0;
#endif
				}
			}
err=2;
// 			if(stricmp(pDDeFile->m_ddefiletype, "http") == 0)
// 			{
// 				// 处理RecSpreadPacket线程收到的扩展命令
// 				oneBuf.packetnum = 0;
// 				while(GetHttpSpread(oneBuf))
// 				{
// 					m_pSubPro->ProcessPacket(oneHdr, oneBuf.pBuffer, "@REC&C");
// 					oneBuf.packetnum = 0;
// 				}
// 			}

	//		memset(&ddedata,0,sizeof(ddedata));
			int wait = ReadDDeData(pDDeFile, ddedata, this->m_ddefiletype);
//			if (ddedata.packetnum>0)
//				bufcount++;
err=3;
			datapos = 0;
//			ddedata.packetnum = 0;
			for (int i = 0; i < ddedata.packetnum; i++)
			{
err=300;
				char packettype = ddedata.pBuffer[datapos];

//#define AAAPacket				1		// 认证数据包标志
//#define TCPPacket				2		// TCP数据包标志
//#define UDPPacket				3		// UDP数据包标志
//#define AllIPPacket			4		
//#define IdcHttpPacket			7		// Http下载抽样包标志
//#define ProxyTCP				8
//#define ProxyUDP				9
//#define UnipTCP				10
//#define UnipUDP				11
//#define CDMATCP				12
//#define CDMAUDP				13
//#define GPRSTCP				14
//#define GPRSUDP				15
//#define CdmaHttpPacket		16
//#define GprsHttpPacket		17
//#define CdmaUnipTCP			18
//#define CdmaUnipUDP			19
//#define GprsUnipTCP			20
//#define GprsUnipUDP			21
//#define HTTP200OK				16
				
				if ((packettype!=AAAPacket) &&
					(packettype!=AllIPPacket) &&
					(packettype!=UnipTCP) &&
					(packettype!=UnipUDP) &&
					(packettype<CdmaUnipTCP))
				{
err=301;
					ASSERT(ddedata.bufsize-datapos >= NEWHEADERLEN);
					memcpy(&dataheader, ddedata.pBuffer+datapos, NEWHEADERLEN);
					ASSERT((ddedata.bufsize-datapos-NEWHEADERLEN)>=dataheader.m_datalen);
					if ((NEWHEADERLEN+dataheader.m_datalen)>ddedata.bufsize-datapos)
					{
// 						WriteLog(m_logfilename, "发现错误的数据包!!");
						break;
					}
					if (packettype==HTTP200OK)
					{
						m_pSubPro->ProcessPacket(dataheader, Packet, ddedata.posaddr);
						m_packetnum++;
						datapos += NEWHEADERLEN+dataheader.m_datalen;
						continue;
					}
					if (dataheader.m_datalen==0)
					{
						m_packetnum++;
						datapos += NEWHEADERLEN;
						continue;
					}
					ASSERT(dataheader.m_datalen < MAXLEN);
					memcpy(Packet, ddedata.pBuffer+datapos+NEWHEADERLEN, dataheader.m_datalen);

					int ilen = NEWHEADERLEN;
					memset(Packet+dataheader.m_datalen, 0, 16);
					
// 					DWORD TmpIpid;
// 					TmpIpid = v_crc32Calc.GetCRC(Packet,dataheader.m_datalen);
//printf("ipid:%d,bhdatelen %d\r\n",TmpIpid,dataheader.datalen);
					datapos += NEWHEADERLEN+dataheader.m_datalen;
					char key[64];
					memset(key, 0, sizeof(key));
err=302;
					m_pSubPro->m_PacketFrom = dataheader.m_type;
					switch(dataheader.m_type)
					{
					case CDMATCP:
					case GPRSTCP:
						dataheader.m_type = TCPPacket;
						break;
					case CDMAUDP:
					case GPRSUDP:
						dataheader.m_type = UDPPacket;
						break;
					}
					SocketSession* s = this->LocateSession(dataheader, key);
					
//					InsertAllSocketMap(key,TmpIpid);

// 					LPAllSocketAAAINFO tmp_LPAllSocketAAAINFO;
// 					if (v_AllSocketMap.Lookup(key,tmp_LPAllSocketAAAINFO))
// 					{
// 						tmp_LPAllSocketAAAINFO->t_lasttime = time(0);
// 						if (tmp_LPAllSocketAAAINFO->ipidarr.GetSize()<100)
// 						{
// 							tmp_LPAllSocketAAAINFO->ipidarr.Add(TmpIpid);
// 						}
// 					}
// 					else
// 					{
// 						tmp_LPAllSocketAAAINFO = new AllSocketAAAINFO;
// 						tmp_LPAllSocketAAAINFO->t_lasttime = time(0);
// 						tmp_LPAllSocketAAAINFO->ipidarr.Add(TmpIpid);
// 						v_AllSocketMap.SetAt(key,tmp_LPAllSocketAAAINFO);
// 					}
					if (s)
					{
err=303;
// 						if (s->ipidarr.GetSize()<100)
// 						{
// 							s->ipidarr.Add(dataheader.ipid);
// 						}
						int re = s->InsertPacket(dataheader, Packet);
						switch(re)
						{
						case 1:
						case 0:
						case -1:
err=304;
							break;
						case 7:
						case 8:
err=305;
							if (s->CloseSession(0))
								this->m_SocketMap.RemoveKey(key);
							break;
						case -3:	//
						case -2:
						case -5:
						case -4:
err=306;
							if (s->CloseSession(0))
								this->m_SocketMap.RemoveKey(key);
							break;
						default:
							break;
						}
#ifdef		_DEBUG
						if (m_SocketMap.GetCount() == 0)
						{
							printf("%s SocketMap Count = 0\n", m_logfilename);
						}
#endif
					}
					else
					{
err=310;

						m_pSubPro->ProcessPacket(dataheader, Packet, ddedata.posaddr);
					}
				}
				else// if (packettype == AAAPacket)
				{
err=4;
					break;
//					if (m_ProcessAAA)
//					{
//						// 处理认证
//						memcpy(Packet,ddedata.pBuffer+datapos,AuthHeadLen);
//						ProcessAAAPacket((AAAPACKET*)Packet, now);
//					}
//					datapos=datapos+AuthHeadLen;
				}
				m_packetnum++;
			}
//			if (m_loopnum%100==0)
//				printf("Process packet%u\t loop%u(%u)\n", m_packetnum, m_loopnum, bufcount);
err=5;
			if (abs(dataheader.m_tm-cleartm)>=60)
			{
// 				printf("in Clear Socket Map!!!\n");
err=501;
				isClear++;
				cleartm = dataheader.m_tm;
			}
			if (isClear>0/* && (wait>0 || 0==(m_loopnum&31))*/)
			{
				int bcount = m_SocketMap.GetCount();
#ifdef		ADD_RUNSTATE_INFO
				DWORD begtm = GetTickCount();
				if (bDumpRunState)	// 清除运行轨迹
				{
					v_httprunstate.ClearState();
				}
#endif

//				ClearSocketMap(dataheader.tm);
err=502;
				if (m_SocketMap.ClearSocketMap(dataheader.m_tm, wait))
				{
					isClear--;
#ifndef			_DEBUG
					if((bcount > 0) && (m_SocketMap.GetCount() == 0))
					{
						printf("%s SocketMap Count = 0\n", m_logfilename);
					}
#endif
				}
err=503;
#ifdef		ADD_RUNSTATE_INFO
				DWORD endtm = GetTickCount();
				if ((endtm - begtm) > 5000)
				{
					WriteLog(m_logfilename, "清除哈希表(%d --> %d)用了(%d秒)",
						bcount, m_SocketMap.GetCount(), (endtm - begtm)/1000);
					if (bDumpRunState)
					{
						// Dump http 运行轨迹
						DumpRunState(1);
					}
				}
#endif
			}
			else if (wait>0)
			{
				::Sleep(min(wait, 200));
			}
		}
		catch(...)
		{
			WriteLog(m_logfilename, "RunForOneSubPro() Error=%d!!", err);
		}
	}
	return;
}

void DceMission::ClearSocketMap(time_t nowtm)
{
	SocketSession *s;
	POSITION pos;
	try
	{
// 		if (!this->m_tempdir.IsEmpty())
// 		{
// 			// 检查并创建临时路径
// 			if (_access(this->m_tempdir, 0)!=0)
// 				CreateDir(this->m_tempdir);
// 		}
		pos= m_SocketMap.GetStartPosition();
		CString strkey;
		while (pos != NULL)
		{
			m_SocketMap.GetNextAssoc(pos, strkey, s);
			if(nowtm >= s->m_closetm)
			{
				if (s->CloseSession(nowtm))
					m_SocketMap.RemoveKey(strkey);
			}
		}
	}
	catch(...)
	{
		WriteLog(m_logfilename, "ClearSocketMap() Error!!");
	}
	return;
}

// DDEFILEINFO* InitDDEList(LPCTSTR strpro, int probak)
// {
// 	ASSERT(v_ddethreadnum > 0);
// 	DDEFILEINFO*	pDDeFile = new DDEFILEINFO[v_ddethreadnum];
// 	DDELIST *tp = &v_ddelist;
// 	int j = 0;
// 	ASSERT(strpro != NULL);
// 	
// 	while(tp!=NULL)
// 	{
// 		DDeInfo *tmpDDeinfo = new DDeInfo;
// 		tmpDDeinfo->ddeip = tp->ip;
// 		tmpDDeinfo->pDdeFile = pDDeFile+j;
// 		tmpDDeinfo->spyaddr = tp->spyaddr;
// 		tmpDDeinfo->filetype = strpro;
// 		tmpDDeinfo->ddethnum = tp->threadnum;
// 		for (int i = 0; i < tp->threadnum; i++)
// 		{
// 			pDDeFile[j].filetm = 0xFFFFFFFF;
// 			pDDeFile[j].InsertFlg = 0;
// 			pDDeFile[j].ddethreadno = i;
// 			pDDeFile[j].pbuffer = NULL;//new char[FILEBUFFERLEN];
// 			j++;
// 		}
// 		// 创建DDE文件读取线程
// 		HANDLE H = CreateThread(NULL, 0, DDeFileReadPorc, tmpDDeinfo, 0, NULL);
// 		CloseHandle(H);
// 		tp = tp->pNext;
// 	}
// 	return pDDeFile;
// 	return NULL;
// }

int ReadDDeData(DDEFILEINFO* pDDEInfo, DATABUFFER& databuf, LPCTSTR pro)
{
	if (databuf.pBuffer!=NULL)
	{
		delete[] (char*)(databuf.pBuffer-sizeof(DATABUFFER));
		databuf.pBuffer = NULL;
	}
	databuf.bufsize = 0;
	databuf.packetnum = 0;
	databuf.posaddr = NULL;
	DWORD minftime = 0xFFFFFF00;
	int posarr[16];
	int poscount = 0;
	// 第一遍循环只找到一个最小的文件时间
	for (int i = 0; i < v_ddethreadnum; i++)
	{
		ASSERT(strcmp(pDDEInfo[i].m_ddefiletype, pro) == 0);
		if ((minftime < pDDEInfo[i].m_filetm) ||
			(pDDEInfo[i].m_InsertFlg == 0))
		{
			continue;
		}
		else if (minftime > pDDEInfo[i].m_filetm)
		{
			minftime = pDDEInfo[i].m_filetm;
			posarr[15] = i;
			poscount = 1;
		}
		else if (minftime == pDDEInfo[i].m_filetm)
		{
			if (poscount < 16)
			{
				posarr[15-poscount] = i;
				poscount++;
			}
		}
	}
	if (minftime==0xFFFFFF00)
	{
		// 如果没有找到就等待并返回
//		::Sleep(200);
		return 200; // 文件没有准备好
	}

	for (int j = 16-poscount; j < 16; j++)
	{
		ASSERT(posarr[j]>=0 && posarr[j]<v_ddethreadnum);
		DDEFILEINFO* tp = pDDEInfo+posarr[j];
		ASSERT(strcmp(tp->m_ddefiletype, pro) == 0);
		if(tp->m_queue.PopDataBuf(databuf))
		{
			return 0;
		}
		else if (tp->m_InsertFlg == 0x0F0F)
		{
			tp->m_queue.PopDataBuf2(databuf);
			InterlockedExchange((long*)&tp->m_filetm, 0xFFFFFFFF);
			InterlockedExchange((long*)&tp->m_InsertFlg, 0);
			if (databuf.packetnum>0)
				return 0;	// 有数据读出
		}
	}
	if (databuf.packetnum==0)
		return 20;	// 没有数据读出
	else
		return 0;	// 有数据读出
}

