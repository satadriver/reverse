#include "stdafx.h"
#include "ReadDDeFile.h"
#include "DCE.h"
#ifdef		_DEBUG
	DWORD	readpacketcount=0;
	DWORD	readbufcount=0;
#endif

long		m_ReadDdeFileNum=0;
long		m_TotalPacketNum=0;
long		m_TotalByteNum=0;
#ifdef		PACKETUNM
long		m_TotalQueueUnit1=0;
long		m_TotalQueueUnit2=0;
#endif

DDeDataQueue	v_HttpSpread;
// #define		SEARCHDDEFILECOUNT		32
// 
// extern UINT GetIdFormProtocol(LPCTSTR strprotocol);
// 
// typedef struct STRUCT_FILEARRAY
// {
// public:
// 	STRUCT_FILEARRAY()	{memset(this, 0, sizeof(lastfile)+4);}
// 	~STRUCT_FILEARRAY() { }
// 	int		filecount;
// 	DWORD	lastsertm;		// 上一次搜索文件的时间
// 	char	lastfile[16];
// 	CString filearr[SEARCHDDEFILECOUNT];
// 
// }FILEARRAY;

void FilAuthMap::SetAtEx(DWORD key, NEWAAAPACKET* pval)
{
	ASSERT_VALID(this);

	UINT nHash;
	CAssoc* pAssoc;
	if ((pAssoc = GetAssocAt(key, nHash)) == NULL)
	{
		// 新的单元
		ASSERT(m_pHashTable != NULL);
		//	InitHashTable(m_nHashTableSize);

		// it doesn't exist, add a new Association
		pAssoc = NewAssoc();
		pAssoc->nHashValue = nHash;
		pAssoc->key = key;
		pAssoc->pNext = m_pHashTable[nHash];
		
		memset(&pAssoc->value, 0, sizeof(pAssoc->value));
		pAssoc->value.m_starttm = pval->m_tm;
		memcpy(pAssoc->value.m_useraccount, pval->m_strUserName, MaxUserName);
		memcpy(pAssoc->value.m_usercelllac, pval->m_strUserCellLac, MaxUserName);
		memcpy(pAssoc->value.m_usercellci, pval->m_strUserCellCi, MaxUserName);
		memcpy(pAssoc->value.m_userimei, pval->m_strUserImei, MaxUserName);
		memcpy(pAssoc->value.m_phone, pval->m_strPhone, MaxTelNo);

		m_pHashTable[nHash] = pAssoc;
	}
	else
	{
		// 已有的单元 覆盖老数据
		memset(&pAssoc->value, 0, sizeof(pAssoc->value));
		pAssoc->value.m_starttm = pval->m_tm;
		memcpy(pAssoc->value.m_useraccount, pval->m_strUserName, MaxUserName);
		memcpy(pAssoc->value.m_phone, pval->m_strPhone, MaxTelNo);
	}
	return;
}

BOOL FilAuthMap::LookupEx(DWORD key, AuthHead*& pval)
{
	ASSERT_VALID(this);

	UINT nHash;
	CAssoc* pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	pval = &pAssoc->value;
	return TRUE;
}

void FilAuthMap::ClearMap()
{
	if (m_nCount > 0)
	{
		memset(m_pHashTable, 0, 4*m_nHashTableSize);

		m_nCount = 0;
		m_pFreeList = NULL;
		m_pBlocks->FreeDataChain();
		m_pBlocks = NULL;
	}
}


int DDEFILEINFO::Open(CString& bakfile)
{
	ASSERT(m_pf == NULL);
	ASSERT(!m_fname.IsEmpty());
	try
	{
#ifdef	_DEBUG
		printf("处理文件:%s\n", m_fname.Mid(m_fname.ReverseFind('\\')-2));
#endif
		// 清除认证哈希表
		m_FileAuthMap.ClearMap();

		// 备份文件
		if (!bakfile.IsEmpty())
		{
			CString path;
			path=bakfile.Left(bakfile.ReverseFind('\\'));
			if(_access(path,0)!=0)
				CreateDir(path);
			CopyFile(m_fname, bakfile, TRUE);
		}
		
		// 打开文件
		m_datalen = 0;
		m_pf = fopen(m_fname, "rb");
		if (m_pf == NULL)
		{
			WriteLog("readdde", "文件%s打开失败!!", m_fname);
			Delete();
			return 0;
		}
		DWORD flen = _filelength(m_pf->_file);
		if (flen > 0)
		{
			if (m_pbuffer==NULL)
				m_pbuffer = new char[2048];
			return 1;
		}
		else
			Delete();
	}
	catch(...)
	{
		WriteLog("readdde", "DDEFile Open() Error!!");
		Delete();
	}
	return 0;
}

void DDEFILEINFO::Delete()
{
	if (m_pf != NULL)
	{
		int re = fclose(m_pf);
		if (re != 0)
		{
			::Sleep(200);
			re = fclose(m_pf);
			if (re != 0)
			{
				WriteLog("readdde", "关闭文件<%s>失败! ErrCode = %d", m_fname, re);
			}
		}
		m_pf = NULL;
	}
	
	if(!m_fname.IsEmpty() && (_access(m_fname,0)==0))
	{
#ifdef	_DEBUG
		printf("删除文件:%s\n", m_fname.Mid(m_fname.ReverseFind('\\')-2));
#endif
		BOOL isDelSucceed = FALSE;
		if(!DeleteFile(m_fname))
		{
			::Sleep(200);
			if (!DeleteFile(m_fname))
			{
				::Sleep(200);
				if (DeleteFile(m_fname))
				{
					isDelSucceed = TRUE;
				}
			}
			else
			{
				isDelSucceed =  TRUE;
			}
		}
		else
		{
			isDelSucceed = TRUE;
		}
			
		if (!isDelSucceed)
		{
			WriteLog("readdde", "文件%s删除失败!!", m_fname);
		}
		InterlockedIncrement(&m_ReadDdeFileNum);
	}
}

int DDEFILEINFO::Read(char *pfilebuf)
{
	int len = 0;
	try
	{
		if (m_pf == NULL)
		{
			Delete();
			return 0;
		}
		int readlen;
		if (m_datalen > 0)
		{
			memcpy(pfilebuf, m_pbuffer, m_datalen);
			pfilebuf += m_datalen;
			readlen = FILEBUFFERLEN - m_datalen;
			len = m_datalen;
			m_datalen = 0;
		}
		else
			readlen = FILEBUFFERLEN;

		int tmpreadlen = fread(pfilebuf, 1, readlen, m_pf);
		if ((tmpreadlen != readlen) || (feof(m_pf)))
		{
			Delete();
		}
		len += tmpreadlen;
	}
	catch(...)
	{
		WriteLog("readdde", "DDEFILEINFO::Read() Error!!");
		Delete();
		len = 0;
	}
	return len;
}

BOOL DDeDataQueue::PopDataBuf(DATABUFFER& dbuf)
{
//	memset(&dbuf, 0, sizeof(DATABUFFER));
	ASSERT(dbuf.packetnum == 0);
	if (unitcount>1)//(pfirst != plast)
	{
		ASSERT(pfirst != plast);
		memcpy(&dbuf, pfirst, sizeof(DATABUFFER));
		pfirst = pfirst->pNext;
		InterlockedDecrement(&unitcount);
#ifdef		PACKETUNM
		InterlockedIncrement(&m_TotalQueueUnit2);
#endif
		return TRUE;
	}
	else
		return FALSE;

//	ASSERT(readqueid==0 || readqueid==1);
//	if (pfirst[readqueid] != NULL)
//	{
//		memcpy(&dbuf, pfirst, sizeof(DATABUFFER));
//		pfirst[readqueid] = pfirst[readqueid]->pNext;
//	}
//	if (pfirst[readqueid] == NULL)
//	{
//		long addqueid = (readqueid==1)? 2 : 1;
//		if (pfirst[addqueid] != NULL)
//			InterlockedExchange(&readqueid, addqueid);
//	}
//	return dbuf.packetnum > 0;

//	if (quesize[readqueid]>0)
//	{
//		DATABUFFER *pReadQue = (readqueid==0)? que1+readpos[readqueid] : que2+readpos[readqueid];
//		dbuf.bufsize = pReadQue->bufsize;
//		dbuf.packetnum = pReadQue->packetnum;
//		dbuf.posaddr = pReadQue->posaddr;
//		dbuf.newftime = pReadQue->newftime;
//		memcpy(dbuf.pBuffer, pReadQue->pBuffer, pReadQue->bufsize);
//		readpos[readqueid]++;
//		quesize[readqueid]--;
//		return TRUE;
//	}
//	else
//		return FALSE;
}

BOOL DDeDataQueue::PopDataBuf2(DATABUFFER& dbuf)
{
	if (unitcount > 0)
	{
		ASSERT(pfirst == plast);
		ASSERT(pfirst != NULL);
		ASSERT(pfirst->pNext == NULL);
		ASSERT(dbuf.packetnum == 0);
		ASSERT(unitcount==1);
		memcpy(&dbuf, pfirst, sizeof(DATABUFFER));
		pfirst = plast = NULL;
	}
	else
	{
		pfirst = plast = NULL;
	}
	InterlockedExchange(&unitcount, 0);
#ifdef		PACKETUNM
	InterlockedIncrement(&m_TotalQueueUnit2);
#endif
	return TRUE;
	
}

// BOOL DDeDataQueue::PushDataBuf(DATABUFFER& dbuf)
// {
// 	ASSERT(dbuf.bufsize < MAX_BLOCK_SIZE-20);
// 	ASSERT(dbuf.packetnum>0);
// 	ASSERT(dbuf.pBuffer!=NULL);
// 	DATABUFFER *pbuf = (DATABUFFER*)new char [dbuf.bufsize + sizeof(DATABUFFER)];
// 	memcpy(pbuf, &dbuf, sizeof(DATABUFFER));
// 	pbuf->pBuffer = (char*)(pbuf+1);
// 	memcpy(pbuf->pBuffer, dbuf.pBuffer, dbuf.bufsize);
// 	if (pfirst==NULL)
// 	{
// 		ASSERT(plast == NULL);
// 		pfirst = plast = pbuf;
// 	}
// 	else
// 	{
// 		plast->pNext = pbuf;
// 		plast = pbuf;
// 	}
// 
// #ifdef		PACKETUNM
// 	InterlockedIncrement(&m_TotalQueueUnit1);
// #endif
// 	InterlockedIncrement(&unitcount);
// 	return TRUE;
// }

BOOL DDeDataQueue::PushDataBuf(DATABUFFER* pBuffer)
{
	ASSERT(pBuffer != NULL);
	ASSERT(pBuffer->packetnum>0);
	ASSERT(pBuffer->pBuffer!=NULL);
	ASSERT(pBuffer->pNext == NULL);
	if (pfirst==NULL)
	{
		ASSERT(plast == NULL);
		pfirst = plast = pBuffer;
	}
	else
	{
		plast->pNext = pBuffer;
		plast = pBuffer;
	}

#ifdef		PACKETUNM
	InterlockedIncrement(&m_TotalQueueUnit1);
#endif
	InterlockedIncrement(&unitcount);
	return TRUE;
}


DWORD CTimeToLTime(const char* stime/*, DWORD& ltime*/)
{
	DWORD ltime;
	int year, month, day;
	int hour, min, sec;
	int num = sscanf(stime, "%04u%02u%02u%02u%02u%02u",
		&year, &month, &day, &hour, &min, &sec);
	if (num == 6)
	{
		if ((year < 2000) || (year > 2038) || (month < 1) || (month > 12) ||
			(day < 1) || (day > 31) ||  (hour > 23) || (min > 59) ||  (sec > 59))
		{
			ltime = 0xFFFFFFFF;
		}
		else
		{
			long tt = CTime(year, month, day, hour, min, sec).GetTime();
			if (tt <= 0)/* || (tt > time(0) + 60))*/
				ltime = 0xFFFFFFFF;
			else
				ltime = (DWORD)tt;
		}
	}
	else
		ltime = 0xFFFFFFFF;
	return ltime;
}

//**********************************************************
//	检查一个数据包的合法性，返回数据包的长度(包括包头信息长度)
//	返回-1表示数据包有错误,
//	返回0 表示缓冲区中的剩余数据不是一个完整的数据包
//**********************************************************
int CheckPacket(char *pPacket, int buflen, time_t& packtime, FilAuthMap& authmap, DATABUFFER& databuf)
{
// #define AAAPacket			1		// 认证数据包标志
// #define TCPPacket			2		// TCP数据包标志
// #define AllIPPacket			3		// UDP数据包标志
// #define UnipPacket			4		
// #define IdcHttpPacket		7		// Http下载抽样包标志
// #define ProxyTCP				8
// #define ProxyUDP				9
// #define UnipTCP				10
// #define UnipUDP				11
// #define CDMATCP				12
// #define CDMAUDP				13
// #define GPRSTCP				14
// #define GPRSUDP				15
// #define HTTP200OK			16
	
//#define CdmaUnipTCP			18
//#define CdmaUnipUDP			19
//#define GprsUnipTCP			20
//#define GprsUnipUDP			21
	if (buflen <= 0)
		return 0;
	if (*pPacket == AllIPPacket) // || (*pPacket == 5 AllIP) || (*pPacket == 6 allIP-IDS))
	{
		if (buflen >= sizeof(sIpTrace))
		{
			packtime = ((sIpTrace*)pPacket)->etm;
			return sizeof(sIpTrace);
		}
		return 0;
	}
	// AAA = 1, TCP = 2,7(HttpDown),8(代理) UDP = 3,9(代理)
	if ((*pPacket < AAAPacket) || (*pPacket > GprsUnipUDP)/* || (*pPacket == 4)*/)
	{
		// 包类型非法
		return -1;
	}
	
	if (*pPacket == AAAPacket)
	{
		// AAA 数据包
		if ((buflen < AuthHeadLen) || (databuf.bufsize < AuthHeadLen))
		{
			return 0;
		}
		NEWAAAPACKET* tp = (NEWAAAPACKET*)(pPacket);
		if (tp->m_bWriteFile != 1)
		{
			if (tp->m_RecType == 1)	
			{
				// 上线认证数据入本地哈希表
				authmap.SetAtEx(*((DWORD*)tp->m_ClientIp), tp);
			}
		}
		else
		{
			packtime = tp->m_tm;

			// 该认证数据写文件
			memcpy(databuf.pBuffer, pPacket, AuthHeadLen);
			databuf.pBuffer += AuthHeadLen;
			databuf.bufsize -= AuthHeadLen;
			databuf.packetnum++;
		}
		return AuthHeadLen;
	}
	else
	{
		if ((buflen < OLDHEADERLEN) || (databuf.bufsize < NEWHEADERLEN))
		{
			return 0;
		}
		// TCP/UDP 数据包
		OLDDATEHEADER* tp = (OLDDATEHEADER*)(pPacket);
		if(tp->m_tm<=0x47791200 || tp->m_tm1<0)	// 时间不能小于2008-1-1 00:00:00
		{
			return -2;		// 包时间非法
		}
		if (tp->m_datalen > 1514)
		{
			// 包长度超长
			return -3;		// 包长度非法
		}
		
		packtime = tp->m_tm;
		if (tp->m_type == UnipTCP || tp->m_type == UnipUDP ||
			tp->m_type >= CdmaUnipTCP || tp->m_type == HTTP200OK)
		{
			AuthHead* psipauth = NULL;
			AuthHead* pdipauth = NULL;
			DATEHEADER* phead = (DATEHEADER*)databuf.pBuffer;
			phead->m_type = tp->m_type;
			if (authmap.LookupEx(*(DWORD*)(tp->m_sip), psipauth))
			{
				memcpy(&phead->m_siptm, psipauth, sizeof(AuthHead));
			}
			else
			{
				memset(&phead->m_siptm, 0, sizeof(AuthHead));
// #ifdef		_DEBUG
// 				phead->m_siptm = time(0);
// 				sprintf(phead->m_sipUserName, "sip_Account_%03u.%03u.%03u.%03u", 
// 					tp->m_sip[0], tp->m_sip[1], tp->m_sip[2], tp->m_sip[3]);
// 				sprintf(phead->m_sipPhone, "sip_Phone_%03u.%03u.%03u.%03u", 
// 					tp->m_sip[0], tp->m_sip[1], tp->m_sip[2], tp->m_sip[3]);
// #endif
			}
			if (authmap.LookupEx(*(DWORD*)(tp->m_dip), pdipauth))
			{
				memcpy(&phead->m_diptm, pdipauth, sizeof(AuthHead));
			}
			else
			{
				memset(&phead->m_diptm, 0, sizeof(AuthHead));
// #ifdef		_DEBUG
// 				phead->m_diptm = time(0);
// 				sprintf(phead->m_dipUserName, "dip_Account_%03u.%03u.%03u.%03u", 
// 					tp->m_dip[0], tp->m_dip[1], tp->m_dip[2], tp->m_dip[3]);
// 				sprintf(phead->m_dipPhone, "dip_Phone_%03u.%03u.%03u.%03u", 
// 					tp->m_dip[0], tp->m_dip[1], tp->m_dip[2], tp->m_dip[3]);
// #endif
			}
			memcpy(&phead->m_type2, pPacket, OLDHEADERLEN);
			databuf.pBuffer += NEWHEADERLEN;
			databuf.bufsize -= NEWHEADERLEN;
			databuf.packetnum++;
			return OLDHEADERLEN;
		}
		if ((buflen < tp->m_datalen + OLDHEADERLEN) 
			|| (databuf.bufsize < tp->m_datalen + NEWHEADERLEN))
		{
			return 0;
		}
		AuthHead* psipauth = NULL;
		AuthHead* pdipauth = NULL;
		DATEHEADER* phead = (DATEHEADER*)databuf.pBuffer;
		phead->m_type = tp->m_type;
		if (authmap.LookupEx(*(DWORD*)(tp->m_sip), psipauth))
		{
			memcpy(&phead->m_siptm, psipauth, sizeof(AuthHead));
		}
		else
		{
			memset(&phead->m_siptm, 0, sizeof(AuthHead));
// #ifdef		_DEBUG
// 			phead->m_siptm = time(0);
// 			sprintf(phead->m_sipUserName, "sip_Account_%03u.%03u.%03u.%03u", 
// 				tp->m_sip[0], tp->m_sip[1], tp->m_sip[2], tp->m_sip[3]);
// 			sprintf(phead->m_sipPhone, "sip_Phone_%03u.%03u.%03u.%03u", 
// 				tp->m_sip[0], tp->m_sip[1], tp->m_sip[2], tp->m_sip[3]);
// #endif
		}
		if (authmap.LookupEx(*(DWORD*)(tp->m_dip), pdipauth))
		{
			memcpy(&phead->m_diptm, pdipauth, sizeof(AuthHead));
		}
		else
		{
			memset(&phead->m_diptm, 0, sizeof(AuthHead));
// #ifdef		_DEBUG
// 			phead->m_diptm = time(0);
// 			sprintf(phead->m_dipUserName, "dip_Account_%03u.%03u.%03u.%03u", 
// 				tp->m_dip[0], tp->m_dip[1], tp->m_dip[2], tp->m_dip[3]);
// 			sprintf(phead->m_dipPhone, "dip_Phone_%03u.%03u.%03u.%03u", 
// 				tp->m_dip[0], tp->m_dip[1], tp->m_dip[2], tp->m_dip[3]);
// #endif
		}
		memcpy(&phead->m_type2, pPacket, tp->m_datalen + OLDHEADERLEN);
		int dlen = tp->m_datalen + NEWHEADERLEN;
		databuf.packetnum++;
		databuf.pBuffer += dlen;
		databuf.bufsize -= dlen;
		return tp->m_datalen + OLDHEADERLEN;
	}
}

int DDEFILEINFO::PushQueue(LPCTSTR spyaddr, char *pfilebuf)
{
	// 如果当前队列中的单元数小于10那么块的大小为16K
	// 如果当前队列中的单元数小于30那么块的大小为32K
	// 如果当前队列中的单元数大于等于于30那么块的大小为60K
int error=0;
	try
	{
		UINT BlockSize;
		if(m_queue.unitcount<10)
		{
			BlockSize = 16*1024;
		}
		else if (m_queue.unitcount<30)
		{
			BlockSize = 32*1024;
		}
		else
		{
			BlockSize = 60*1024;
		}
error=2;
		time_t packettime = 0;
		int bufpos = 0;
		DATABUFFER tmpbuf;
		tmpbuf.pBuffer = new char[BlockSize];
		DATABUFFER* pbuf = (DATABUFFER*)tmpbuf.pBuffer;

		tmpbuf.pBuffer += sizeof(DATABUFFER);
		tmpbuf.bufsize = BlockSize - sizeof(DATABUFFER);
		tmpbuf.packetnum = 0;

		pbuf->pBuffer = tmpbuf.pBuffer;
		pbuf->posaddr = spyaddr;
		pbuf->packetnum = 0;
		pbuf->bufsize = 0;
		pbuf->pNext = 0;

error=3;
		// 读文件
		int filebuflen = Read(pfilebuf);

		// 检查每一个包
		while(v_DceRunFlg)
		{
			BOOL isADD = FALSE;
error=4;
			int len = CheckPacket(pfilebuf+bufpos, filebuflen, packettime, m_FileAuthMap, tmpbuf);
			if (len>0)
			{
				// 一个有效的数据包
				InterlockedIncrement(&m_TotalPacketNum);
				InterlockedExchangeAdd(&m_TotalByteNum, len);
				filebuflen-=len;
				bufpos+=len;
			}
			else if (len==0)
			{
				// 缓冲区中的数据长度不够
				isADD = TRUE;
			}
			else
			{
error=5;
				if (_access("e:\\netspy\\flg\\skiperrpct",0)!=0)
				{
					// 错误的数据包
					WriteLog("readdde", "在文件<%s>中发现错误的数据包!! ErrCode=%d", m_fname, len);
					int pos = m_fname.ReverseFind('.')+1;
					if (pos>0 && pos<m_fname.GetLength())
					{
						CString baktmpfile;
						baktmpfile.Format("%s\\tmp\\%d.%s",
							v_WorkPath,
							GetTickCount()%5, m_fname.Mid(pos));
						if (CopyFile(m_fname, baktmpfile, FALSE))
						{
							WriteLog("readdde", "备份文件%s成功", baktmpfile);
						}
						else
						{
							FILE* pf = fopen(baktmpfile, "wb");
							if (pf)
							{
								fwrite(pfilebuf, 1, bufpos+filebuflen, pf);
								fclose(pf);
							}
						}
					}
				}
error=6;
				Delete();
				isADD = TRUE;
			}
			if (isADD)
			{
				BOOL isContinue = TRUE;
				pbuf->packetnum = tmpbuf.packetnum;
				pbuf->bufsize = BlockSize-tmpbuf.bufsize;
error=7;
				if (pbuf->packetnum>0)
				{
					// 发送数据到队列
					m_queue.PushDataBuf(pbuf);
				}
				else
				{
error=8;
					delete[] pbuf;
					isContinue = FALSE;
				}
error=9;
				if ((len==0) && (m_pf!=NULL) && (filebuflen<2048))
				{
					BakData(pfilebuf + bufpos, filebuflen);
					break;
				}
				else if ((len <= 0) && (!isContinue))
				{
					break;
				}
				
				// 初始化新的buf
				if(m_queue.unitcount<10)
				{
					BlockSize = 16*1024;
				}
				else if (m_queue.unitcount<30)
				{
					BlockSize = 32*1024;
				}
				else
				{
					BlockSize = 60*1024;
				}
error=10;
				tmpbuf.pBuffer = new char[BlockSize];
				pbuf = (DATABUFFER*)tmpbuf.pBuffer;
				tmpbuf.pBuffer += sizeof(DATABUFFER);
				tmpbuf.bufsize = BlockSize - sizeof(DATABUFFER);
				tmpbuf.packetnum = 0;

				pbuf->pBuffer = tmpbuf.pBuffer;
				pbuf->posaddr = spyaddr;
				pbuf->packetnum = 0;
				pbuf->bufsize = 0;
				pbuf->pNext = 0;
			}
		}
		return (m_pf==NULL)? -1 : 1;
	}
	catch (...)
	{
		WriteLog("readdde", "PushQueue() Error!! %d",error);
		return -1;
	}
}

int SearchOneDir(LPCTSTR path, DDEFILEINFO& farr, CString* pArr, int& filecount, BOOL isCatch, DWORD& filestate)
{
	// 首先删除dde的积压标志文件
	HANDLE hfindfile;
 	WIN32_FIND_DATA finddata;
	char searchpath[200];
	sprintf(searchpath, "%s20*.%s", path, farr.m_ddefiletype);
	DWORD begin = GetTickCount();
	hfindfile = FindFirstFile(searchpath, &finddata);
	DWORD now = GetTickCount();
	int count = 0;
	if (hfindfile != INVALID_HANDLE_VALUE)
	{
		filestate &= 0x7FFFFFFF;
		try
		{
			do
			{
				if (memcmp(finddata.cFileName, farr.m_lastfile, 16) >= 0)
				{
// 					if (isCatch)
// 						pArr[filecount++].Format("%s*", finddata.cFileName);
// 					else
						pArr[filecount++] = finddata.cFileName;
					if(++count>=40)
						break;
				}
				else
				{
					sprintf(searchpath, "%s%s", path, finddata.cFileName);
					if(!DeleteFile(searchpath))
						WriteLog("readdde", "历史文件%s删除失败!!", searchpath);
				}
			}while(FindNextFile(hfindfile, &finddata));
		}
		catch (...)
		{
			WriteLog("readdde", "SearchDDEFile() Error!!");
		}
		FindClose(hfindfile);
	}
	else if (now - begin > 15000)
	{
		WriteLog("dce","搜索目录%s花费了大于15秒时间，并且搜索失败",searchpath);
		filestate |= 0x80000000;
		return 0;
	}
	else
	{
		if (_access(path,0) == 0)
		{
			filestate &= 0x7FFFFFFF;
		}
	}

	if (strcmp(farr.m_ddefiletype,"http") == 0)
	{
		CString tmpstr;
		tmpstr.Format("%sjiya.flg",path);
		if (_access(tmpstr,0) == 0)
		{
			WriteLog("dce","发现积压并搜到数据%s,count:%d",tmpstr,count);
		}
	}

// 	if (!isCatch)
// 	{
// 		filestate &= 0x7FFFFFFF;
// 		if (filecount<=4)
// 		{
// 			DWORD mark = (~(1<<farr.proid)) & filestate;
// 			if (mark != filestate)
// 			{
// 				filestate = mark;
// 			}
// 			if (filestate == 0)
// 			{
// 				sprintf(searchpath,  "%sjiya.flg", path);
// 				if (_access(searchpath, 0) == 0)
// 				{
// 					if (!DeleteFile(searchpath))
// 						WriteLog("dce", "标志文件%s删除失败!", searchpath);
// 					else
// 						WriteLog("dce", "标志文件%s删除成功!", searchpath);
// 				}
// 			}
// 		}
// 		else if (count > 4)
// 		{
// 			DWORD mark = (1<<farr.proid) | filestate;
// 			if (mark != filestate)
// 			{
// 				filestate = mark;
// 			}
// 		}
// 	}
	return count;

}

int compstr(const void* s1, const void* s2)
{
	// 正序
	return strcmp(*((LPCTSTR*)s1), *((LPCTSTR*)s2));
}

//BOOL SearchDDEFile(LPCTSTR path, LPCTSTR strpro, DDEFILEINFO* pFileArr, int thnum)
//{
//	char strpath[200];
//	DWORD now = GetTickCount();
//	for (int i = 0; i < thnum; i++)
//	{
//		if ((pFileArr[i].filecount > 0)/* || (abs(now-pFileArr[i].lastsertm)<5000)*/)
//			continue;
//		pFileArr[i].filecount = 0;
//		sprintf(strpath, "%s\\%d\\20*.%s", path, i+1, strpro);
//		if (SearchOneDir(strpath, pFileArr[i])>0)
//		{
//#ifdef	_DEBUG
//			printf("搜索到%d个文件在目录%d\n", pFileArr[i].filecount, i+1);
//#endif
//			// 到排序文件名
//			qsort(pFileArr[i].filearr, pFileArr[i].filecount, sizeof(CString), compstr);
//		}
//		pFileArr[i].lastsertm = now;
//	}
//	return TRUE;
//}

//读文件线程
DWORD WINAPI DDeFileReadPorc(LPVOID p)
{
	int error=0;
	DWORD profilestate = 0;
	DWORD lastasstm = 0;
	DWORD failcount = 0;
	DDeInfo* pDDeInfo = (DDeInfo*)p;
	DDEFILEINFO* pDDeFile = pDDeInfo->pDdeFile;
	char *pFileDataBuffer = new char[FILEBUFFERLEN];
	CString SerachPath;
//	CString CatchPath;
	CString SearchFile[40];
// 	CStringArray  CatchFile;
// 	CatchFile.SetSize(0, 32);
// 	CatchPath.Format("rd /s /q x:\\xxx\\tmp\\%s", pDDeInfo->ddeip);
// 	system(CatchPath);
// 	CatchPath.Format("x:\\xxx\\tmp\\%s\\", pDDeInfo->ddeip);
	
	if (_access(v_WorkPath+"\\flg\\localdde.flg", 0) == 0)
	{
		SerachPath.Format("%s:\\netspy\\%s\\1\\", g_ddepath,pDDeInfo->ddeip);
	}
	else
	{
		if (strcmp(v_localip, pDDeInfo->ddeip) != 0)
			SerachPath.Format("\\\\%s\\%s$\\netspy\\%s\\1\\", pDDeInfo->ddeip, g_ddepath,v_localip);
		else
			SerachPath.Format("%s:\\netspy\\%s\\1\\", g_ddepath,v_localip);
	}

pDDeInfo->spyaddr=v_ddelist.spyaddr;
	WriteLog("dce", "DDEPOS: %s Path: %s", pDDeInfo->spyaddr, SerachPath);

	srand(time(0) + GetTickCount());
	while(v_DceRunFlg)
	{
		try
		{
			int		filecount=0;
			pDDeFile = pDDeInfo->pDdeFile;
			BOOL Wait = TRUE;
			BOOL bJiya = FALSE;
			while(pDDeFile)
			{
				DWORD now = GetTickCount();
				if (((now-pDDeFile->m_lastsertm)>=5000) && (pDDeFile->m_filecount<=0))
// 					||(((now-pDDeFile->lastsertm)>=30000) && (pDDeFile->InsertFlg!=0xA0A0)))
				{
					pDDeFile->m_filecount = 0;	
					pDDeFile->m_lastsertm = now;
					int fcount = 0;
					if ((0==(profilestate&0x80000000)) || ((now-lastasstm)>=60000))
					{
						DWORD tmpfstat = profilestate;
						lastasstm = GetTickCount();
error=1;
						SearchOneDir(SerachPath, *pDDeFile, SearchFile, fcount, FALSE, profilestate);		// 搜索DDE的Z盘
		

						if (profilestate & 0x80000000)
						{
							if (0==(failcount&15))
								WriteLog("dce", "DDE<%s> Z盘无法访问!!", pDDeInfo->ddeip);
							if (failcount<5)	// 最初的5次是每间隔20s访问一次, 以后就是每1分钟访问一次
								lastasstm = GetTickCount()-40000;	
							failcount++;
						}
						else
						{
							if (tmpfstat & 0x80000000)
							{
								WriteLog("dce", "DDE<%s> Z盘恢复访问!! 连续失败次数%u", pDDeInfo->ddeip, failcount);
								
							}
								failcount = 0;
						}
					}
					if (fcount>0)
					{
#ifdef	_DEBUG
						printf("搜索到%d个文件在目录\n", fcount);
#endif
						// 到排序文件名(正序)
						qsort(SearchFile, fcount, sizeof(CString), compstr);
						pDDeFile->m_filecount = fcount = min(fcount, SEARCHDDEFILECOUNT);
						for (int i=fcount-1; i>=0; i--)
						{
							pDDeFile->m_filearr[fcount-1-i] = SearchFile[i];
						}
					}
				}
				filecount += pDDeFile->m_filecount;
error=3;
				while (pDDeFile->m_filecount>0)
				{
					if (pDDeFile->m_InsertFlg == 0x0F0F)
					{
						if ((pDDeFile->m_queue.unitcount==0) ||
							(pDDeFile->m_filetm > 0x80000000))
						{
							InterlockedExchange((LPLONG)&pDDeFile->m_InsertFlg, 0);
						}
error=34;
						if ((now - pDDeFile->m_queuestate) >= 40000)
						{
							pDDeFile->m_queuestate = now;
							char strftm[40];
							memset(strftm, 0, sizeof(strftm));
							if (pDDeFile->m_filetm >= 0x80000000)
								sprintf(strftm, "%08X", pDDeFile->m_filetm);
							else
								GetTimeString((time_t)pDDeFile->m_filetm, "%Y%m%d%H%M%S", strftm, 30);
							WriteLog("readdde", "DDE%s Queue%s Size: %d CurrentTime: %s",
								pDDeInfo->ddeip, pDDeFile->m_ddefiletype, pDDeFile->m_queue.unitcount, strftm);
						}
					}
					else if (pDDeFile->m_InsertFlg == 0)
					{
						// 开始读一个新文件
						ASSERT(pDDeFile->m_fname.IsEmpty());
						CString bakfile;
						CString& tmpfile = pDDeFile->m_filearr[pDDeFile->m_filecount-1];
error=35;
// 						if (tmpfile.GetAt(tmpfile.GetLength()-1) == '*')
// 						{
// 							tmpfile.ReleaseBuffer(tmpfile.GetLength()-1);
// 							pDDeFile->fname.Format("%s%s", CatchPath, tmpfile);
// 						}
// 						else
						{
							pDDeFile->m_fname.Format("%s%s", SerachPath, tmpfile);
						}
						if (v_bddefilebak[pDDeFile->m_proid]!=0)
							bakfile.Format("e:\\netspy\\ddebak\\%s\\1\\%s", pDDeInfo->ddeip, 
							pDDeFile->m_filearr[pDDeFile->m_filecount-1]);
						DWORD filetime = CTimeToLTime(pDDeFile->m_filearr[pDDeFile->m_filecount-1]);
error=36;

						if (filetime<0x80000000)
						{
							if (pDDeFile->m_filearr[pDDeFile->m_filecount-1].GetAt(15) == '1')
								filetime -= 4;	// 头文件
							pDDeFile->Open(bakfile);
error=38;

							pDDeFile->m_queuestate=now;
							InterlockedExchange((long*)&pDDeFile->m_filetm, filetime);
							InterlockedExchange((LPLONG)&pDDeFile->m_InsertFlg, 0xA0A0);
							Wait = FALSE;
						}
						else
						{
error=37;

							DeleteFile(pDDeFile->m_fname);
							pDDeFile->m_filearr[pDDeFile->m_filecount].Empty();
							pDDeFile->m_filecount--;
							continue;
						}
					}
					else if (pDDeFile->m_InsertFlg == 0xA0A0)
					{
						// 填充队列
//						while (v_DceRunFlg && !pDDeFile->queue.IsFull())
//						if (!pDDeFile->queue.IsFull())
//						{
						Wait = FALSE;
						pDDeFile->m_queuestate=now;
error=39;
						if (pDDeFile->PushQueue(pDDeInfo->spyaddr, pFileDataBuffer)<0)
						{
							// 该文件已经读取完成 并已经全部放入队列中了
							pDDeFile->m_FileAuthMap.ClearMap();
							pDDeFile->m_datalen = 0;
							pDDeFile->m_filecount--;
							pDDeFile->m_filearr[pDDeFile->m_filecount].Empty();
							InterlockedExchange((LPLONG)&pDDeFile->m_InsertFlg, 0x0F0F);
							pDDeFile->m_fname.Empty();
						}
//						}
					}
					break;
				}
				pDDeFile=pDDeFile->m_pNext;
			}
error=4;
			if (filecount < 3 )
			{
				if (_access(SerachPath+"jiya.flg", 0) == 0)
				{
					if (!DeleteFile(SerachPath+"jiya.flg"))
						WriteLog("dce", "标志文件%s删除失败!", SerachPath+"jiya.flg");
					else
						WriteLog("dce", "标志文件%s删除成功!", SerachPath+"jiya.flg");
				}
			}
error=5;
			if (Wait)
			{
// 				CString tmpcatch;
// 				int size = CatchFile.GetSize();
// 				if (size>0)
// 				{
					// 缓存一个DDE文件到本地的Catch目录: "x:\\xxx\\tmp\\ddeip\\"
// 					CreateDir(CatchPath);
// 					CString& tmpfile = CatchFile.ElementAt(--size);
// 					CString srcfile, dstfile;
// 					srcfile.Format("%s%s", SerachPath, tmpfile);
// 					dstfile.Format("%s%s", CatchPath, tmpfile);
// 					MoveFile(srcfile, dstfile);
// 					CatchFile.SetSize(size);
// 				}
// 				else
					::Sleep(20);
			}
		}
		catch(...)
		{
			WriteLog("readdde", "DDeFileReadPorc() Loop Error!! %d",error);
		}
	}
	delete[] pFileDataBuffer;
	return 0;
}

int printThreadFile(BOOL &bJiya)
{
	bJiya = FALSE;
	DDELIST	*pDDeip = &v_ddelist;
	int filecount=0;
	CString str;
	DWORD begtm = GetTickCount();
	while (pDDeip != NULL)
	{
		for (int i = 1; i <= pDDeip->threadnum; i++)
		{
			DWORD nowtm = GetTickCount();
			if ((nowtm-begtm) > 60000)
			{
				CreateFlgFile();
				begtm = nowtm;
			}
			int  threadfilecount = 0;
			char ddepath[300];
			int len = 0;
//			if (v_programtype == SYSTEM_SLEUTH)
//				len = sprintf(ddepath, "v:\\netspy\\127.0.0.1\\%d\\200*.*", i);
			if (_access(v_WorkPath+"\\flg\\localdde.flg", 0) == 0)
			{
				len = sprintf(ddepath, "%s:\\netspy\\%s\\%d\\20*.*", g_ddepath,pDDeip->ip, i);
			}
			else
			{
				if (stricmp(pDDeip->ip, v_localip) == 0)
					len = sprintf(ddepath, "%s:\\netspy\\%s\\%d\\20*.*", g_ddepath,v_localip, i);
				else
					len = sprintf(ddepath, "\\\\%s\\%s$\\netspy\\%s\\%d\\20*.*", pDDeip->ip, g_ddepath,v_localip, i);
			}
			// 搜索并打开文件
			str = ddepath;
			HANDLE hfindfile;
			WIN32_FIND_DATA finddata;
			hfindfile = FindFirstFile(ddepath, &finddata);
			CString JiyaFlg;
			if (hfindfile != INVALID_HANDLE_VALUE)
			{
				ddepath[len-6] = 0;
				str = ddepath;
				JiyaFlg.Format("%sJiya.flg", str);
				try
				{
					do
					{
						filecount++;
						threadfilecount++;
						str.Format("%s\r\n%s", (CString)str, finddata.cFileName);
					}while(FindNextFile(hfindfile, &finddata));
				}
				catch (...)
				{
					WriteLog("dce", "printThreadFile() Error!!");
				}
				FindClose(hfindfile);
				if (threadfilecount > 16)
				{
					bJiya = TRUE;
					WriteLog("dce", "Error!! 发现积压文件 %s", str);
				}
				else if (0==(_access(JiyaFlg, 0)))
				{
					bJiya = TRUE;
				}
			}
		}
		pDDeip = pDDeip->pNext;
	}
	return filecount;
}

//////////////////////////////////////////////////////////////////////////
void AddHttpSpread(DATABUFFER &oneBuf)
{
// 	if(v_HttpSpread.unitcount < 100)
// 		v_HttpSpread.PushDataBuf(oneBuf);
#ifdef _DEBUG
// 	else
// 	{
// 		printf("v_httpSpread Too More, >=100 \n");
// 	}
#endif
}

BOOL GetHttpSpread(DATABUFFER &oneBuf)
{
	return v_HttpSpread.PopDataBuf(oneBuf);
}