// NogSession.cpp: implementation of the CNogSession class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "http.h"
#include "HttpSession.h"
#include "NogSession.h"
#include "PublicFun.h"
// #include "PublicFun.h"
#include "FreeGateDec.h"

CMapStringToOb	g_MapFgServer;
time_t			g_chkFgServer = 0;
//////////////////////////////////////////////////////////////////////////
//
//自由之门
//自由之门
// void sub_xorcode_40EBC7(BYTE* arg_0, int arg_4, BYTE* arg_8);
// void sub_makekey_40EB56(BYTE* arg_0, int arg_4, BYTE* arg_8);
const BYTE xor_fg_fixKey[] = 
{
	0x49, 0x75, 0x46, 0xFB, 0x14, 0x70, 0x6A, 0x65, 
	0xFC, 0x90, 0x0A, 0xA0, 0x86, 0x75, 0x04, 0x6A
};
unsigned char g_swapTable[256] = {0};
void InitSwapTable()
{
	if(g_swapTable[0] == 0)
	{
		char swapTmp[256];
		memset(swapTmp, -1, 256);
		for(unsigned char idx='!'; idx<='~'; idx++)
			swapTmp[idx] = idx;
		swapTmp['-'] = '.';
		swapTmp['.'] = 'L';
		swapTmp['/'] = '/';
		memcpy(swapTmp+'0', "-rsUVW4HIJ", 10);
		memcpy(swapTmp+'A', "GdefgOPQRST5hijk789_mnopql", 26);
		memcpy(swapTmp+'a', "vwxMN6yzABCDXY0Z123abctuEF", 26);
		swapTmp['_'] = 'K';

		memset(g_swapTable, -1, 256);
		for (idx = 0; idx < 255 ; idx++)
		{
			if(swapTmp[idx] != -1)
			{
				ASSERT(g_swapTable[swapTmp[idx]] == 0xFF);
				g_swapTable[swapTmp[idx]] = idx;
			}
		}
	}
}

DWORD isFreeGate(const char* pPacket, DATEHEADER& packh, unsigned char rKey[16],HttpProtocol* httpprotocl)
{
	DWORD dwDeal = 0;
	try{

	if(packh.m_datalen == 102 && 0 == memcmp(pPacket, U89_CLIENTHELLO, 11))
	{
		unsigned char *psessid = (unsigned char *)(pPacket+0x2c); // clienthello中sessionid的位置
		unsigned char cKey[20];
		unsigned char cc1,cc2;
		// 交换session某位, 675也有此过程, 若未操作, 
		// 当交换位位于前8(产生key用)或0x10-0x14(IP加密后结果)间时675定位失败.
		int session1c = psessid[0x1c];		
		int session1b = psessid[0x1b];
		session1c = session1c % 0x18;
		psessid[session1c] = session1b;	// 交换
		//
		for (int ih = 0; ih < 8 ; ih++)
		{
			cc1 = psessid[ih]>>4;
			cc2 = psessid[ih]&0xF;
			if(cc1 >= 0 && cc1 <= 9)
				cKey[ih*2] = cc1+'0';
			else
				cKey[ih*2] = cc1+0x57;
			cKey[ih*2] ^= xor_fg_fixKey[ih*2];
			
			if(cc2 >= 0 && cc2 <= 9)
				cKey[ih*2+1] = cc2+'0';
			else
				cKey[ih*2+1] = cc2+0x57;
			cKey[ih*2+1] ^= xor_fg_fixKey[ih*2+1];
		}

		BYTE xorstring[300];
		BYTE *ppipp = NULL;
		if(FALSE == dwDeal)
		{
			sub_makekey_40EB56(cKey, 16, xorstring);
			DWORD encodeip = *(DWORD*)(psessid+0x10);	

			// sessionid中第16-20为服务器IP变换
			sub_xorcode_40EBC7((unsigned char*)&encodeip, 4, xorstring);
			ppipp = (unsigned char*)&encodeip;
			if(memcmp(packh.m_dip,ppipp,4)==0)
			{
				// find fg675 client hello
				memcpy(rKey, cKey, 16);
				dwDeal = FG_675HELLO;
			}
		}

		if(FALSE == dwDeal)
		{
			sub_makekey_40EB56(cKey, 16, xorstring);
			char tmpDecode[0x20];		// 由临时变量中解
			memcpy(tmpDecode, psessid, 0x20);

			// 676解密内容为 第8字节开始的0x13位, 其中0x10后4字节为IP.
			sub_xorcode_40EBC7((unsigned char*)(tmpDecode+8), 0x13, xorstring);
			ppipp = (unsigned char*)(tmpDecode+0x10);
			if(memcmp(packh.m_dip,ppipp,4)==0)
			{
				// find fg676 client hello
				memcpy(rKey, cKey, 16);
				dwDeal = FG_676HELLO;
			}
		}

		CString dIP;
		CFgServer *pServer = NULL;
		dIP.Format("%u.%u.%u.%u", packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3] );
		if(g_MapFgServer.Lookup(dIP, (CObject*&)pServer))
		{
			pServer->m_lastUse = time(0);
			if(FALSE == dwDeal)
			{
				char tmpDecode[0x20];		// 由临时变量中解
				for (int iK = 0; iK < pServer->m_cntKey && FALSE == dwDeal; iK++)
				{
					memcpy(cKey, pServer->m_Key+iK*16, 16);
					sub_makekey_40EB56(cKey, 16, xorstring);		// 取一个Key

					memcpy(tmpDecode, psessid, 0x20);
					sub_xorcode_40EBC7((unsigned char*)(tmpDecode+0x10), 4, xorstring);
					ppipp = (unsigned char*)(tmpDecode+0x10);
					if(memcmp(packh.m_dip,ppipp,4)==0)
					{
						// find 675 通讯
						memcpy(rKey, cKey, 16);
						dwDeal = FG_675COMMUNICATION;
					}
				}
			}

			if(FALSE == dwDeal)
			{
				char tmpDecode[0x20];		// 由临时变量中解
				for (int iK = 0; iK < pServer->m_cntKey && FALSE == dwDeal; iK++)
				{
					memcpy(cKey, pServer->m_Key+iK*16, 16);
					sub_makekey_40EB56(cKey, 16, xorstring);		// 取一个Key

					memcpy(tmpDecode, psessid, 0x20);
					sub_xorcode_40EBC7((unsigned char*)(tmpDecode+8), 0x13, xorstring);
					ppipp = (unsigned char*)(tmpDecode+0x10);
					if(memcmp(packh.m_dip,ppipp,4)==0)
					{
						// find 676 通讯
						memcpy(rKey, cKey, 16);
						dwDeal = FG_676COMMUNICATION;
					}
				}
			}
		}

		if(dwDeal == FG_675HELLO || dwDeal == FG_676HELLO)
		{
//			g_freegatePacketCount++;
			// 匹配案件和预警
			HttpSession tmpsession(packh, g_httpFrom);
			tmpsession.m_OpenCaseFlg = v_opencasewujie;
			tmpsession.m_OpenWarnFlg = v_openwarnwujie;
			tmpsession.m_OpenIndxFlg = v_openflagwujie;
			tmpsession.m_spyaddr = g_DDEPOS;
			DWORD state = 0;
			SearchCaseOrWarn(&tmpsession, NULL, 0, state);

			// 写对照文件
			tmpsession.m_Requestline.m_Browser = TE_NEWFREEGATE;
			tmpsession.m_dceprocotol=httpprotocl;
//			tmpsession.m_Requestline.m_BrowserVer = 0x808;
			tmpsession.WriteTeUrlIndexFile();
			//WriteTeUrlIndexFile(packh, g_httpFrom, tmpsession, ver);
			return TRUE;
		}
	}
	else
	{
		// 检查获得 17 03 00 查找服务器过程中的回应包.
		if(memcmp(pPacket, "\x17\x03\x00", 3) == 0)
			dwDeal = 5;
	}

	}catch (...) {
		WriteLog(HTTPPRO, "isFreegate() Error!!");
	}

	return dwDeal;
}

void CheckFreegateServer()
{
	time_t now = time(0);
	if(abs(now-g_chkFgServer) > 5*60)
	{
		time(&g_chkFgServer);

		POSITION pos;
		pos = g_MapFgServer.GetStartPosition();
		CString key;
		CFgServer *pServer = NULL;
		while (pos)
		{
			g_MapFgServer.GetNextAssoc(pos, key, (CObject*&)pServer);
			if(pServer->CheckIsOutTime(now))	// 5分钟未确认, 删
			{
				delete pServer;	
				g_MapFgServer.RemoveKey(key);
			}
		}
		if(g_MapFgServer.GetCount() > 1000)
			WriteLog(HTTPPRO, "当前有确定Fg服务器, 哈希 %d项", g_MapFgServer.GetCount());
	}
}

CXorBuffer::CXorBuffer()
{
	c_havelen = 0;
	c_oneBuf = NULL;//, 0, 0xffff);

	s_havelen = 0;
	s_oneBuf = NULL;//, 0, 0xffff);

	c_ack = s_seq = 0;
}

BOOL CXorBuffer::AddBuf(BOOL bClient, int total, unsigned char *packet, int len)
{
	DWORD needlen = 0;
	BOOL bOK = FALSE;
	if(bClient)
	{
		if(c_oneBuf == NULL)
		{
			c_oneBuf = new unsigned char[0xFFFF];
			memset(c_oneBuf, 0, 0xFFFF);
		}
		memcpy(c_oneBuf+c_havelen, packet, len);
		c_havelen += len;
		ASSERT(c_havelen < 0xFFFF);
		needlen = c_oneBuf[3]*256+c_oneBuf[4];
		if(c_havelen >= needlen)
		{
			bOK = TRUE;
		}
	}
	else
	{
		if(s_oneBuf == NULL)
		{
			s_oneBuf = new unsigned char[0xFFFF];
			memset(s_oneBuf, 0, 0xFFFF);
		}
		memcpy(s_oneBuf+s_havelen, packet, len);
		s_havelen += len;
		ASSERT(s_havelen < 0xFFFF);
		needlen = s_oneBuf[3]*256+s_oneBuf[4];
		if(s_havelen >= needlen)
		{
			bOK = TRUE;
		}
	}

	return bOK;
}

BOOL CXorBuffer::ResetBuf(BOOL bClient, int trimlen)
{
	BOOL bOK = FALSE;
	if(bClient)
	{
		ASSERT(c_oneBuf != NULL);
		ASSERT(c_havelen >= trimlen);
		c_havelen -= trimlen;
		if(c_havelen > 0)
		{
			memmove(c_oneBuf, c_oneBuf+trimlen, c_havelen);
			ASSERT(memcmp(c_oneBuf, "\x17\x03\x00", 3) == 0);
		}
	}
	else
	{
		ASSERT(s_oneBuf != NULL);
		ASSERT(s_havelen >= trimlen);
		s_havelen -= trimlen;
		if(s_havelen == 0)
		{
			s_seq = 0;
		}
		else
		{
			s_seq += trimlen;
			memmove(s_oneBuf, s_oneBuf+trimlen, s_havelen);
			ASSERT(memcmp(s_oneBuf, "\x17\x03\x00", 3) == 0);
		}
	}

	return bOK;
}

BOOL CXorBuffer::HaveBegin(BOOL bCheckClient)
{
	if(bCheckClient)
	{
		if(c_havelen > 0)
			return TRUE;
	}
	else
	{
		if(s_havelen > 0)
			return TRUE;
	}

	return FALSE;
}

BOOL CXorBuffer::CheckHaveOne(BOOL bCheckClient, unsigned char **pdata)
{
	BOOL rHaveOne = FALSE;
	DWORD xorlen = 0;
	if(bCheckClient)
	{
		if(c_havelen > 0 && c_oneBuf != NULL)
		{
			ASSERT(memcmp(c_oneBuf, "\x17\x03\x00", 3) == 0);
			xorlen = c_oneBuf[3]*256+c_oneBuf[4];
			if(c_havelen >= xorlen+5)
			{
				*pdata = c_oneBuf;
				rHaveOne = TRUE;
			}
		}
	}
	else
	{
		if(s_havelen > 0 && s_oneBuf != NULL)
		{
			ASSERT(memcmp(s_oneBuf, "\x17\x03\x00", 3) == 0);
			xorlen = s_oneBuf[3]*256+s_oneBuf[4];
			if(s_havelen >= xorlen+5)
			{
				*pdata = s_oneBuf;
				rHaveOne = TRUE;
			}
		}
	}

	return rHaveOne;
}

BOOL CXorBuffer::SetSeqAck( BOOL bClient, DWORD seq, DWORD ack )
{
	if(bClient)
		c_ack = ack;
	else
	{
		if(s_seq == 0)
			s_seq = seq;
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
//
CNogSession::CNogSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	InitSwapTable();	// 初始化全局变换表

	m_ver = 0;
	m_bClientOK = TRUE;
	m_bServerOK = TRUE;

	m_cnseq = 0;
	m_snseq = 0;

	m_PrevGET = 0;
	m_Prev200OK = 0;

	memset(m_Key, 0, 16);

	// 无包多长时间关闭会话(1分钟)
	m_closetm = packh.m_tm + 60;
	m_spyaddr = g_DDEPOS;

	// 处理IDC数据源
	if(m_dataSrc == DATASRC_OTHER)
	{
		// 匹配IDC数据源
		if ((v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP))
			|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
		{
			m_dataSrc = DATASRC_IDC;
		}
	}

}


CNogSession::~CNogSession()
{
	// 清
	ASSERT(m_mapGet200OK.GetCount() == 0);

	// 其它操作
}

BOOL CNogSession::DealPacketList(BOOL bClient, BOOL bLast)
{
	DWORD xorlen = 0;
	DWORD datalen = 0;
	unsigned char *pdata = NULL;
	unsigned char *pdatahdr = NULL;

	DWORD nextseq;
	TcpPacketListIt start, last, itl, tmpitl;
	if(bClient)
	{
		nextseq = m_cnseq;
		m_cpacketlist.sort();
		start = m_cpacketlist.begin();
		last = m_cpacketlist.end();
	}
	else
	{
		nextseq = m_snseq;
		m_spacketlist.sort();
		start = m_spacketlist.begin();
		last = m_spacketlist.end();
	}
	tmpitl = start;	
	time_t begtm = 0;
	while(tmpitl != last)
	{
		itl = tmpitl;
		TcpPacket *pp = &*tmpitl;
		tmpitl++;


		if (pp->Nextsequence <= nextseq)
		{
			// 重复的包或不需要的包
			m_xorBuffer.SetSeqAck(bClient, pp->Ssequence, pp->Sack);
			if(bClient)
				m_cpacketlist.erase(itl);		
			else
				m_spacketlist.erase(itl);
			continue;
		}
		else if (nextseq > 0 && pp->Ssequence > nextseq)
		{
			//不连续就退出
 			begtm = pp->tm;
			break;
		}
		else
		{
			// 连续的数据包
			if(nextseq > 0)
			{
// 				ASSERT(pp->Ssequence == nextseq);
				if(pp->Ssequence != nextseq)
				{
					break;
				}
			}
			// 解
			pdata = (unsigned char*)(pp->buf);
			datalen = pp->Slen;
			
			if(bClient)
			{
				m_cnseq = pp->Ssequence + pp->Slen;

				nextseq = m_cnseq;
			}
			else
			{
				m_snseq = pp->Ssequence + pp->Slen;

				nextseq = m_snseq;
			}

			while(datalen > 5)
			{
				if( memcmp(pdata, "\x14\x3\x0", 3) == 0 
				 || memcmp(pdata, "\x16\x3\x0", 3) == 0)
				{
					xorlen = pdata[3]*256+pdata[4];
					pdata += (5+xorlen);
					datalen -= (5+xorlen);
				}
				else
				{
					break;
				}
			}
			if(datalen > 0)
			{
				BOOL bReady = FALSE;
				if(memcmp(pdata, "\x17\x3\x0", 3) == 0)
				{
					// begin
					xorlen = pdata[3]*256+pdata[4];
					m_xorBuffer.SetSeqAck(bClient, pp->Ssequence, pp->Sack);
					bReady = m_xorBuffer.AddBuf(bClient, xorlen, pdata, datalen);
				}
				else
				{
					if(m_xorBuffer.HaveBegin(bClient))
					{
						m_xorBuffer.SetSeqAck(bClient, pp->Ssequence, pp->Sack);
						bReady = m_xorBuffer.AddBuf(bClient, 0, pdata, datalen);
					}
					else
					{
						// 非17 03 00 起始且前无17 03 00起
						ASSERT(FALSE);
						break;
					}
				}

				// 满, 解
				if(bReady)
				{
					// 多包, 有剩余部分
					unsigned char *ptmpdata = NULL;
					while(m_xorBuffer.CheckHaveOne(bClient, &ptmpdata))
					{
						ASSERT(memcmp(ptmpdata, "\x17\x03\x00", 3) == 0);
						if(memcmp(ptmpdata, "\x17\x03\x00", 3) == 0)
						{
							xorlen = ptmpdata[3]*256+ptmpdata[4];
							DecodePacket(bClient, ptmpdata, 
								m_xorBuffer.c_ack, m_xorBuffer.s_seq);
							m_xorBuffer.ResetBuf(bClient, xorlen+5);
						}
					}
				}
			}
		}

		if(bClient)
			m_cpacketlist.erase(itl);
		else
			m_spacketlist.erase(itl);
	}

	return TRUE;
}

int CNogSession::InsertPacket( DATEHEADER& dataheader,const char *pPacket )
{
#ifdef _DEBUG
// 	if(!(dataheader.m_dport == 1643 || dataheader.m_sport == 1643))
// 		return 0;
#endif
	DWORD errNog = 0;
	try{

	BOOL bDeal = FALSE;
	BOOL bClient = FALSE;
	if(m_bClientOK && memcmp(dataheader.m_sip, m_csip, 4) == 0)
	{
//		g_freegatePacketCount ++;
		// client --> server
		bClient = TRUE;
		TcpPacket	Tcpp(dataheader, pPacket, 0, 0) ;
		m_cpacketlist.push_back(Tcpp);
		Tcpp.buf = 0;
		if(m_cpacketlist.size() > 20 )
			bDeal = TRUE;
	}
	else if(m_bServerOK)
	{
//		g_freegatePacketCount ++;
		bClient = FALSE;
		TcpPacket	Tcpp(dataheader, pPacket, 0, 0) ;
		m_spacketlist.push_back(Tcpp);
		Tcpp.buf = 0;
		if(m_spacketlist.size() > 20 && m_PrevGET > 0)
			bDeal = TRUE;
	}
	
	errNog = 1;
	if(bDeal)
	{
		errNog = 2;
		DealPacketList(bClient, FALSE);
	}

	}catch (...){
		WriteLog(HTTPPRO, "NogSession InsertPacket Error!! = %d", errNog);
	}



	return 0;
}


BOOL CNogSession::CloseSession( time_t nowtm )
{
// 	WriteLog(HTTPPRO, "By Pass CloseSession");

	DealPacketList(TRUE, TRUE);
	DealPacketList(FALSE, TRUE);

	//处理哈希中剩余未完成的请求
	CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
	CString strSEQ;
	COneGet200OK *pOb = NULL;
	POSITION pos = m_mapGet200OK.GetStartPosition();
	while(pos != NULL)
	{
		m_mapGet200OK.GetNextAssoc(pos, strSEQ, (CObject*&)pOb);
		pOb->Submit(TRUE, TRUE);
		DealOneGet(pOb);

		delete pOb;
	}
	m_mapGet200OK.RemoveAll();
	
	return TRUE;
}

BOOL CNogSession::DecodePacket(BOOL bClient, unsigned char *pdata, DWORD ClientAck, DWORD ServerSeq)
{
	try{

	Search	Search20HTTP(" HTTP");//TROJDATAID);
	Search	SearchDT("/dt/");//TROJDATAID);

	DWORD xorlen = pdata[3]*256+pdata[4];

	if(bClient)
		sub_xorcode_40EBC7((unsigned char*)(pdata+5), xorlen, m_cxorstring);
	else
		sub_xorcode_40EBC7((unsigned char*)(pdata+5), xorlen, m_sxorstring);

	BOOL bReset = FALSE;
	CString tmppath =  m_dceprocotol->m_httpMissObj->GetTmpPath();
	if(bClient)
	{
		int dtfind = SearchDT.find((char*)(pdata+5), xorlen);
		int dtfindend = Search20HTTP.find((char*)(pdata+5), xorlen);
		if(dtfindend > dtfind && dtfind != -1)
		{
			ASSERT(dtfindend-dtfind < 250);
			if(dtfindend-dtfind < 250)
			{
				// 解 Url
				CString tmp;
				char *dttmp = tmp.GetBufferSetLength(256);
				memset(dttmp, 0, 250);
				memcpy(dttmp, pdata+5+dtfind, dtfindend-dtfind);
				int idx = 4;
				while(dttmp[idx] != 0)
				{
					if(dttmp[idx] == '/' && strchr(dttmp+idx+1, '/') == NULL)
						break;

					ASSERT(g_swapTable[dttmp[idx]] != 0xFF);
					dttmp[idx] = g_swapTable[dttmp[idx]];
					idx++;
				}
				memcpy(pdata+5+dtfind, dttmp, dtfindend-dtfind);
			}
		}
		// 解 Referer

		CString strSEQ;
		COneGet200OK *pOneGet = NULL;
		if(m_PrevGET != 0)
		{
			strSEQ.Format("0x%08X", m_PrevGET);
			if(m_mapGet200OK.Lookup(strSEQ, (CObject*&)pOneGet))
			{
				if(m_PrevGET != ClientAck)
				{
					if(pOneGet->Submit(TRUE, FALSE))
					{
#ifdef _DEBUG
						printf("A提交一个会话, 序号:%s\n", strSEQ);
#endif
						// 处理
						DealOneGet(pOneGet);

						// 完成, 清
						delete pOneGet;
						m_mapGet200OK.RemoveKey(strSEQ);
					}
					pOneGet = NULL;			// 新建
				}
			}
		}

		// GET POST CONNECT ...
		// WriteLog(HTTPPRO, "SEQ:0x%08X, %u %s", ClientAck, xorlen, CString((char*)pdata+5, 5));
		if(memcmp(pdata+5, "POST", 4) == 0)
		{
			bReset = TRUE;
			m_PrevGET = ClientAck;
		}
		else if(memcmp(pdata+5, "GET", 3) == 0)
		{
			bReset = TRUE;
			m_PrevGET = ClientAck;
		}

		strSEQ.Format("0x%08X", m_PrevGET);
		if(FALSE ==m_mapGet200OK.Lookup(strSEQ, (CObject*&)pOneGet))
		{
			pOneGet = new COneGet200OK;
		}
		pOneGet->PutData(TRUE, pdata+5, xorlen, bReset);
		m_mapGet200OK.SetAt(strSEQ, pOneGet);
	}
	else
	{

		CString strSEQ;
		COneGet200OK *pOneGet = NULL;
		// WriteLog(HTTPPRO, "ACK:0x%08X, %u %s", ServerSeq, xorlen, CString((char*)pdata+5, 5));
		if(memcmp(pdata+5, "HTTP", 4) == 0)
		{
			if(m_Prev200OK != 0)
			{
				strSEQ.Format("0x%08X", m_Prev200OK);
				if(m_mapGet200OK.Lookup(strSEQ, (CObject*&)pOneGet))
				{
					if(m_Prev200OK != ServerSeq)
					{
						if(pOneGet->Submit(FALSE, TRUE))
						{
#ifdef _DEBUG
							printf("B提交一个会话, 序号:%s\n", strSEQ);
#endif
							// 确认, 如果get与200都已提交, 处理并完成
							DealOneGet(pOneGet);

							// 清处理完的会话
							delete pOneGet;
							m_mapGet200OK.RemoveKey(strSEQ);
						}

						pOneGet = NULL;			// 新建
					}
				}
			}

			bReset = TRUE;
			m_Prev200OK = ServerSeq;
		}

		strSEQ.Format("0x%08X", m_Prev200OK);
		if(FALSE ==m_mapGet200OK.Lookup(strSEQ, (CObject*&)pOneGet))
		{
			pOneGet = new COneGet200OK;
		}
		pOneGet->PutData(FALSE, pdata+5, xorlen, bReset);
		m_mapGet200OK.SetAt(strSEQ, pOneGet);
	}

	}catch (...) {
		WriteLog(HTTPPRO, "DecodePacket Error!!");
	}
	return TRUE;
}

BOOL CNogSession::DealOneGet(COneGet200OK *pOneGet)
{
	DWORD errDeal = 0;
	try{

	CString tmpdir =  m_dceprocotol->m_httpMissObj->GetTmpPath();

#ifdef _DEBUG
	CString strOut;
	strOut.Format("%s%d.req", tmpdir, g_httpFileNum);
	WriteTmpData(strOut, pOneGet->pGetData, pOneGet->haveGetlen);
	strOut.Format("%s%d.res", tmpdir, g_httpFileNum);
	WriteTmpData(strOut, pOneGet->p200OKData, pOneGet->have200Len);
#endif // _DEBUG

// 	ASSERT(pOneGet->pGetData != NULL);
	if(pOneGet->haveGetlen < 5)
	{
		return FALSE;
	}

	errDeal = 1;
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath =  m_dceprocotol->m_httpMissObj->GetTmpPath();
	HttpCommInfo oneHttpHdr;
	BOOL bWriteFile = FALSE;
	BOOL bWriteFreegate = TRUE;

	CString PRO, Suffix, text;
	if(memcmp(pOneGet->pGetData, "GET ", 4) == 0)
	{
		errDeal = 10;
		if(FormatHttpReqHeader(oneHttpHdr,
			(char*)pOneGet->pGetData, pOneGet->haveGetlen, FALSE, m_cdip, atoi(m_dip.Right(5))))
		{
			printf("paOK");
		}
		else
		{
			// WriteLog(HTTPPRO, "paFails");
			return FALSE;
		}
		CString fname;
		if( oneHttpHdr.m_Content_Type=="application/octet-stream" ||
			oneHttpHdr.m_Content_Type=="" )
		{
			fname=GetUnixShortName(oneHttpHdr.m_URL);
			if(fname != "")
			{
				//检查后缀
				oneHttpHdr.m_filetype=CheckSuffix(fname, Suffix);
			}
		}
		if(Suffix.IsEmpty())
		{
			oneHttpHdr.m_filetype=CheckSuffix(oneHttpHdr.m_Content_Type, Suffix);
		}


		errDeal = 11;
		//////////////////////////////////////////////////////////////////////////
		// ParseUrl() //处理HttpUrl
		CString strurl = oneHttpHdr.m_URL + ' ' + oneHttpHdr.m_Referer;
		strurl.MakeLower();
		DWORD state = 0x10000;		// 需要匹配ip+URL
		SearchCaseOrWarn(this, (char*)(LPCTSTR)strurl, strurl.GetLength(), state);
		if (/*(m_CaseArr.GetSize() == 0) && */(!m_rubbishflag))
		{
			// 匹配URL的屏蔽信息
			m_rubbishflag = SearchBurrishByURL(strurl);
		}
		if(m_CaseArr.GetSize() > 0)
		{
			// 是案件
			if (oneHttpHdr.m_filetype != 1)	// 不是图片和脚本
			{
				// 写HttpUrl文件
				if (m_indexfname.IsEmpty())
				{
					m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u_httpurl.indexfile.txt",
						tmppath, now.wYear, now.wMonth, now.wDay,
						now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_httpFileNum);
				}
				if (oneHttpHdr.m_Referer.IsEmpty())
				{
					text.Format("url= %s\r\n", oneHttpHdr.m_URL);
				}
				else
				{
					text.Format("url= %s\r\nreferer= %s\r\n",
						oneHttpHdr.m_URL, oneHttpHdr.m_Referer);
				}
				WriteIndexFile("freegateurl", "freegateurl", text,  m_dceprocotol->m_httpMissObj);
			}
		}

		// 清除案件和预警信息
		m_CaseArr.RemoveAll();
		m_WarnArr.RemoveAll();
		//////////////////////////////////////////////////////////////////////////

		errDeal = 12;
		if(pOneGet->p200OKData != NULL)
		{
			char *pData = (char*)pOneGet->p200OKData;	// 200回应
			DWORD datalen = pOneGet->have200Len;

			int headlen = -1;		// HTTP头的长度
			BOOL unixhead = FALSE;
			headlen = Search4ByteFind(pData, datalen);//0d 0a 0d 0a
			if (headlen < 0)
			{
				headlen = Search2ByteFind(pData, datalen);// 0a 0a
				unixhead=TRUE;
			}

			ASSERT(datalen > 0);
			if(headlen > 0)
			{
		errDeal = 13;
				//////////////////////////////////////////////////////////////////////////
				CString emlfield=oneHttpHdr.m_emlfield;
				text = oneHttpHdr.m_URL + ' ' + oneHttpHdr.m_Referer + ' ' + oneHttpHdr.m_Cookie +
					' ' + oneHttpHdr.m_Cookie2 + ' ' + oneHttpHdr.m_SetCookie2 /*+ ' ' + emlfield*/;
				text.Format("%s____%s",(CString)text, emlfield);
				text.MakeLower();	// 要匹配线索的URL必须转换成小写字符

				// 匹配案件和预警
				DWORD state = 0x10000;		// 需要匹配ip+URL
				SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), state);
				if (/*(m_CaseArr.GetSize() == 0) && */(!m_rubbishflag))
				{
					// 匹配URL的屏蔽信息
					m_rubbishflag = SearchBurrishByURL(text);
				}
				//////////////////////////////////////////////////////////////////////////

				PRO = "freegatedown";
				CFile file;
				m_realfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s%s",
					tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
					m_sip, m_dip, ++g_httpFileNum, "freegatedown.", Suffix);

				if (!file.Open(m_realfname, CFile::modeCreate | CFile::modeReadWrite))
				{
					return FALSE;
				}

				file.SeekToBegin();
				file.Write(pData+headlen, datalen-headlen);
				file.Close();

				if (memcmp(pData+headlen,"\x47\x49\x46",3) == 0)
				{
					bWriteFreegate = FALSE;
				}

				//
				if(memcmp(pData+headlen, "\x1f\x8b\x08", 3) == 0)
				{
					// gzip
					oneHttpHdr.m_ifcompressed = TRUE;
					DealCompressedFile(m_realfname, &oneHttpHdr);
				}

// 				CString attflst;
// 				DealWebMail(this, oneHttpHdr, attflst);
			}
		}
		else
		{
			PRO = "freegateurl";

		}
		errDeal = 14;
		// 处理案件信息
		if (oneHttpHdr.m_filetype == 2)
		{
			if ((m_CaseArr.GetSize() <= 0) && (m_ProxyType != 15))
			{
				// 首先匹配回溯线索中的IP+URL
				bWriteFile = SearchIndexByIPURL(m_sip.Left(15), m_dip.Left(15), text, text.GetLength());
				if(!bWriteFile)
				{
					// 不是案件 且 不是回溯的IP+URL 就匹配预设关键字
					bWriteFile = SearchHttpFilter(m_realfname,
						m_sip,m_dip,oneHttpHdr.m_URL,oneHttpHdr.m_Host,"httpdown",NULL);
				}
			}
		}
// 		else
// 		{
// 			if (oneHttpHdr.m_filetype == 1)
// 			{
// 				// 图片和脚本
// 				bWriteFile = ((m_CaseArr.GetSize() > 0) && (v_opencasehttpimage));
// 			}
// 			else
// 			{
// 				// 其他类型的文件
// 				bWriteFile = (m_CaseArr.GetSize() > 0);
// 			}
// 		}
		errDeal = 15;
		if (bWriteFreegate)
		{
			if(oneHttpHdr.m_filetype == 1)	// image
				bWriteFile = ((m_CaseArr.GetSize() > 0) && (v_opencasehttpimage));
			else
				bWriteFile = TRUE;
		}
		else
		{
			bWriteFile = FALSE;
		}
	}
	else
	{
		errDeal = 20;
		ASSERT(memcmp(pOneGet->pGetData, "POST", 4) == 0);
		char *pData = (char*)pOneGet->pGetData;		// POST头
		DWORD datalen = pOneGet->haveGetlen;

		int headlen = -1;		// HTTP头的长度
		BOOL unixhead = FALSE;
		headlen = Search4ByteFind(pData, datalen);//0d 0a 0d 0a
		if (headlen < 0)
		{
			headlen = Search2ByteFind(pData, datalen);// 0a 0a
			unixhead=TRUE;
		}


		ASSERT(datalen > 0);
		if(headlen > 0)
		{
			if(FormatHttpReqHeader(oneHttpHdr,
				(char*)pOneGet->pGetData, pOneGet->haveGetlen, FALSE, m_cdip, atoi(m_dip.Right(5))))
			{
				printf("paOK");
			}
			else
			{
				//WriteLog(HTTPPRO, "paFails");
				return FALSE;
			}
			// ParseUrl() //处理HttpUrl

			PRO = "freegateup";
			int len = datalen-headlen;
			if(len > 0)
			{
			errDeal = 21;
				CFile file;
				m_realfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.txt",
					tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
					m_sip, m_dip, ++g_httpFileNum, PRO);
				if (!file.Open(m_realfname, CFile::modeCreate | CFile::modeReadWrite))
				{
					return FALSE;
				}
				if(oneHttpHdr.m_Content_Type.Find("urlencoded")>0)
				{
					memcpy(m_dceprocotol->m_HttpFileBuffer, pData+headlen, len);
					memset(m_dceprocotol->m_HttpFileBuffer+len, 0, 8);
					ConverHttp2Html(m_dceprocotol->m_HttpFileBuffer, len);
					len = strlen(m_dceprocotol->m_HttpFileBuffer);

					file.SeekToBegin();
					file.Write(m_dceprocotol->m_HttpFileBuffer, len);
					file.Close();
				}
				else
				{
					file.SeekToBegin();
					file.Write(pData+headlen, datalen-headlen);
					file.Close();
				}

				// 处理Webmail

				// 处理案件信息

				bWriteFile = TRUE;
			}
		}
	}

	errDeal = 30;
	if(FALSE == bWriteFile)
	{
		if(_access(m_realfname, 0) == 0)
		{
			DeleteFile(m_realfname);
			m_realfname.Empty();
		}
	}
	else
	{
		int hlen = WriteField(m_dceprocotol->m_HttpFileBuffer,		"url",			oneHttpHdr.m_URL);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"referer",		oneHttpHdr.m_Referer);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"cookie",		oneHttpHdr.m_Cookie);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"cookie2",		oneHttpHdr.m_Cookie2);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"setcookie2",	oneHttpHdr.m_SetCookie2);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"user-agent",	oneHttpHdr.m_useragent);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"accept-language",	oneHttpHdr.m_Language);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"accept-charset",	oneHttpHdr.m_Charset);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"content-type",		oneHttpHdr.m_Content_Type);

		CString text = CString(m_dceprocotol->m_HttpFileBuffer, hlen);
// 		ASSERT(FALSE == m_realfname.IsEmpty());
		if(m_realfname.IsEmpty())
		{
			m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.indexfile.txt",
				tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
				m_sip, m_dip, ++g_httpFileNum, PRO);
		}
		else
		{
			m_indexfname = m_realfname+".indexfile.txt";
		}
		WriteIndexFile(PRO, PRO, text,  m_dceprocotol->m_httpMissObj);
	}

	}catch (...) {
		WriteLog(HTTPPRO, "DealOneGet Error!! = %d", errDeal);
	}
	return TRUE;
}

BOOL CNogSession::WriteTmpData( CString szFilename, unsigned char *pData, int datalen )
{
	if(datalen > 0 && pData != NULL)
	{
		CFile fpTmp;
		fpTmp.Open(szFilename, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
		if(fpTmp.m_hFile != CFile::hFileNull)
		{
			fpTmp.SeekToEnd();
			fpTmp.Write(pData, datalen);
			fpTmp.Close();
			return TRUE;
		}
		WriteLog(HTTPPRO, "%s fails", szFilename);
	}
	return FALSE;
}

void CNogSession::SetOriKey( int ver, unsigned char key[16], const unsigned char *pFirst)
{
	memcpy(m_Key, key, 16);
	sub_makekey_40EB56(m_Key, 16, m_cxorstring);
	sub_makekey_40EB56(m_Key, 16, m_sxorstring);
	unsigned char tmpFirst[0x20];
	memcpy(tmpFirst, pFirst+0x2c, 0x20);
	if(ver == FG_675COMMUNICATION)
	{
		m_ver = 0x675;
		sub_xorcode_40EBC7((unsigned char*)(tmpFirst+0x10), 4, m_cxorstring);
	}
	else
	{
		m_ver = 0x676;
		sub_xorcode_40EBC7((unsigned char*)(tmpFirst+8), 0x13, m_cxorstring);
	}


	return ;
}
