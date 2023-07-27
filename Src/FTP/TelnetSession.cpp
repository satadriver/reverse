// TelnetSession.cpp: implementation of the TelnetSession class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ftp.h"
#include "TelnetSession.h"
#include "winsock2.h"
//#include "..\\QQ_Crypt.h"

BOOL WriteIndexFIle(TelnetSession * s);

TelnetSession::TelnetSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_playspytm = packh.m_tm;
 	m_closetm = packh.m_tm + 180;
	m_Ssequence = 0;
	m_servernseq = 0;
	m_clientnseq = 0;
	m_fileseq = 0;
	m_validatetelnet = FALSE;
	m_validatetelnetCTR = FALSE;
	m_numcmd = 0;
	m_numctr = 0;
	m_sortflg = 0;

	if (packh.m_dport != 23)
	{
		// 交换会话方向
		ASSERT(packh.m_sport == 23);
		CString tstr = m_sip;
		m_sip = m_dip;
		m_dip = tstr;
		
		tstr = m_dmac;
		m_dmac = m_smac;
		m_smac = tstr;

		m_AuthInfo.SwapAuthInfo();

		memcpy(m_csip, packh.m_dip, 4);
		memcpy(m_cdip, packh.m_sip, 4);
	}

	m_playbuffer = NULL;
	m_playlen = 0;

	g_TelnetSion++;
}

TelnetSession::~TelnetSession()
{
	if (m_playbuffer != NULL)
	{
		delete[] m_playbuffer;
		m_playbuffer = NULL;
	}
	m_playlen = 0;

	if ((!m_textfilename.IsEmpty()) && (_access(m_textfilename, 0) == 0))
	{
		DeleteFile(m_textfilename);
	}
	if ((!m_playfilename.IsEmpty()) && (_access(m_playfilename, 0) == 0))
	{
		DeleteFile(m_playfilename);
	}

	g_TelnetSion--;
}

BOOL TelnetSession::CloseSession(time_t nowtm)
{
	try
	{
		if (m_sortflg != 0)
		{
			m_PacketList.sort();
			m_sortflg = 0;
			LstToFile(g_FtpFILEBUFFER, FILEBUFFERSIZE);
		}

		m_OpenIndxFlg = v_openflagtelnet;
		m_OpenCaseFlg = v_opencasetelnet;
		m_OpenWarnFlg = v_openwarntelnet;

		
 		// 过滤参数
		if (!m_textfilename.IsEmpty()
			&& m_validatetelnet
			&& (GetFileLength(m_textfilename) > 100))
		{
			// 匹配案件、预警、屏蔽信息
			m_realfname = m_textfilename;
			m_textfilename.Empty();
			DWORD d_state = 0;
			SearchCaseOrWarn(this, NULL, 0, d_state);

			// 处理IDC数据
			CString strISP;
			if (v_stat && v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
			{
				CString str;
				str.Format("%s\1%s\1telnet\1TELNET服务\1%s\1TELNET\1%s\1\1", 
					m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[TELNET_PROTOCOL].AddServer(str);
			}

			// 写活动历史(该协议不输出活动历史)

			// 写对照文件并输出
			m_indexfname = m_realfname + ".indexfile.txt";
			WriteIndexFile("telnet", "telnet", "", g_pFtpMissObj);

			// 清除案件和预警信息
			m_CaseArr.RemoveAll();
			m_WarnArr.RemoveAll();
			m_begfiletm = 0;

			m_closetm += 180;		// 下一个3分钟再次调用

		}
		else if (!m_validatetelnet && ((m_lastpacktm - m_playspytm)>300))
		{
			// 5分钟内没有分析出是Telnet的包就结束会话
			return TRUE;
		}
		else
		{
			m_closetm += 60;		// 下一个1分钟再次调用
		}
 		
		BOOL isDrop = (abs(nowtm - m_lastpacktm) >= 300);
		if (((m_lastpacktm - m_playspytm) > 30*60) || isDrop)
		{
 			// 5分钟没有新的数据包就输出播放文件
			// 或连续30分钟就输出一个播放文件
			if (m_playlen > 0)
			{
				WriteTelnetPlayFile(NULL, NULL, 0);
			}
			if(m_validatetelnet && (GetFileLength(m_playfilename) > 400))
			{
				if (m_PacketList.size()>0)
				{
					DTcpPacketListIt last, itl, tmpitl;
					tmpitl = m_PacketList.begin();
					m_endfiletm = tmpitl->tm;
				}
				else
				{
					m_endfiletm = m_lastpacktm;
				}
				m_realfname = m_playfilename;
				m_playfilename.Empty();
				// 匹配案件、预警、屏蔽信息
				DWORD d_state = 0;
				SearchCaseOrWarn(this, NULL, 0, d_state);

				// 处理IDC数据
				CString strISP;
				if (v_stat && v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
				{
					CString str;
					str.Format("%s\1%s\1telnet\1TELNET服务\1%s\1TELNET\1%s\1\1", 
						m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
					v_IdcServerStat[TELNET_PROTOCOL].AddServer(str);
				}

				// 写活动历史(该协议不输出活动历史)

				// 写对照文件并输出
				m_indexfname = m_realfname + ".indexfile.txt";
				m_begfiletm = m_playspytm;
				WriteIndexFile("telnet", "telnet", "", g_pFtpMissObj);

				// 清除案件和预警信息
				m_CaseArr.RemoveAll();
				m_WarnArr.RemoveAll();
				m_playspytm = m_endfiletm;
				m_begfiletm = 0;
				m_endfiletm = 0;
			}
		}
		return isDrop;
	}
	catch (...)
	{
		WriteLog(FTPPRO, "CloseSession() Error!!");
		return TRUE;
	}
}

int TelnetSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	int re=0;
	m_lastpacktm = packh.m_tm;
	DTcpPacket	Tcpp(packh, pPacket);
	m_PacketList.push_back(Tcpp);
	m_sortflg++;
	Tcpp.buf = 0;
	
	//如果包数达到20 就写盘
	if (m_sortflg > 20)
	{
		m_sortflg = 0;
		m_PacketList.sort();
		re = LstToFile(g_FtpFILEBUFFER, FILEBUFFERSIZE);
		if((re == -1) && (m_PacketList.size() > 200))
		{
			re=-3;
		}
	}
	return re;
}

int TelnetSession::LstToFile(char *buf, int bufsize)
{
	ASSERT(bufsize > 512*1024);
	int re=0;
	int datalen = 0;
	try
	{
		DTcpPacketListIt last, itl, tmpitl;
		tmpitl = m_PacketList.begin();
		last = m_PacketList.end();
		if (last != tmpitl)
		{
			if (m_begfiletm == 0)
			{
				m_begfiletm = tmpitl->tm;	// 设置文件的截获时间
			}
			if ((m_servernseq == 0) && (m_clientnseq == 0))
			{
				if(tmpitl->sport == 23)
				{
					m_servernseq = tmpitl->Ssequence;
					m_clientnseq = tmpitl->Sack;
					m_Ssequence  = tmpitl->Ssequence;
					m_fileseq    = tmpitl->Ssequence;
				}
				else
				{
					m_servernseq = tmpitl->Sack;
					m_clientnseq = tmpitl->Ssequence;
					m_Ssequence  = tmpitl->Sack;
					m_fileseq    = tmpitl->Sack;
				}
			}
		}

		while(tmpitl != last)
		{
			itl = tmpitl;
			DTcpPacket* pp = &*itl;
			tmpitl++;
			if (pp->dport == 23)
			{
				// Client --> Server
				if((INT)(pp->Ssequence - m_clientnseq) >= 0)
				{
					m_clientnseq = pp->Nextsequence;
					WriteTelnetPlayPacket(*pp);
				}				
				m_PacketList.erase(itl);
				continue;
			}

			// Server --> Client
			if ((int)(pp->Nextsequence - m_servernseq) <= 0)
			{
				m_PacketList.erase(itl);
				// 重复的包
				continue;
			}

			if ((int)(pp->Ssequence - m_servernseq) > 0)
			{
				//不连续就退出
				re = -1;
				break;
			}

			// 连续的包
			WriteTelnetPlayPacket(*pp);		// 保存play文件
			if (pp->Ssequence == m_servernseq)
			{
				memcpy(buf + datalen, pp->buf, pp->Slen);
				datalen += pp->Slen;
			}
			else
			{
				int pos = m_servernseq - pp->Ssequence;
				ASSERT(pos < pp->Slen);
				memcpy(buf + datalen, pp->buf + pos, pp->Slen - pos);
				datalen += pp->Slen - pos;
			}
			m_servernseq = pp->Nextsequence;
			m_PacketList.erase(itl);
			
			if (datalen > (bufsize - 10000))
			{
				break;		// buf快满了
			}
		}
		
		if (datalen == 0)
		{
			if ((re == -1) && (m_PacketList.size() > 200))
			{
				return -3;
			}
			return  re;
		}

		//存文件模块
		m_fileseq = m_servernseq;
		if (0 > WriteTelnetTextfile(buf, datalen))
		{
			return -2;
		}
	}
	catch (...)
	{
		WriteLog(FTPPRO, "LstTofile() Error!!");
		re = -3;
	}
	return re;
}

int TelnetSession::WriteTelnetTextfile(char* pData, int datlen)
{
	// 检查数据是否是telnet
	if (!m_validatetelnet && !m_validatetelnetCTR)
	{
		int tmpcmdnum=0;
		tmpcmdnum = parsetelnetcommand((UCHAR *)pData, datlen, TRUE);
		if(tmpcmdnum > 6)
		{
			m_validatetelnet = TRUE;
			m_validatetelnetCTR = TRUE;
		}
		else
		{
			tmpcmdnum=parsetelnetctr((UCHAR *)pData, datlen, TRUE);
			if(tmpcmdnum > 50)
			{
				m_validatetelnet = TRUE;
				m_validatetelnetCTR = TRUE;	
			}
		}
	}

	if(m_textfilename.IsEmpty())
	{
		SYSTEMTIME ftm;
		GetLocalTime(&ftm);
		CString tmpdir = g_pFtpMissObj->GetTmpPath();
		m_textfilename.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u.telnet.txt",
			tmpdir, ftm.wYear, ftm.wMonth, ftm.wDay, ftm.wHour,
			ftm.wMinute, ftm.wSecond, m_sip, m_dip, ++g_TelnetFileNum, rand());
	}
	
	FILE* fs = fopen(m_textfilename, "ab");
	if(fs != NULL)
	{
		// modify by zzh 2004-11-09 for 去掉telnet中控制符
		TranslateTelnetBuf(pData, datlen, fs);
		fclose(fs);
		fs = NULL;
		return 1;
	}
	else
	{
		return -3;
	}
}

void TranslateTelnetBuf(LPCTSTR srcbuf, int srclen, FILE* pf)
{
	ASSERT(pf != NULL);
	ASSERT(srcbuf != NULL);
	ASSERT(srclen > 0);
	
	char codebuf[20];
	memset(codebuf, 0, 20);
	int codeptr = 0;
	
	int iPreLine = -1;
	int state = 1;
	int dstPos = 0;
	int iPreBuf = 0;
	BOOL bFinish = FALSE;
	for (int i = 0; i < srclen; i++)
	{
		switch(srcbuf[i])
		{
		case '\r':
		case '\n':
			if (state != 2)
			{
				state = 2;
				memcpy(g_PROCESSBUFFER+dstPos, "\r\n", 2);
				dstPos+=2;
			}
			break;
		case 27:
			if (srcbuf[i+1] == '[')
			{
				state = 3;
				codeptr = 0;
				codebuf[codeptr]=0;
				i++;
			}
			else
			{
				g_PROCESSBUFFER[dstPos++] = srcbuf[i];
			}
			break;
		default:
			if (state == 3)
			{
				if (srcbuf[i] > 64)
				{
					codebuf[codeptr]=0;
					switch(srcbuf[i])
					{
					case 'H':
						{
							char *buffer = codebuf;
							int PosX, PosY;
							PosX = PosY = 0;
							while(*buffer && *buffer != ';')
								PosY = PosY*10 + *buffer++ - '0';
							
							if(*buffer)
								buffer++;
							
							while(*buffer && *buffer != ';')
								PosX = PosX*10 + *buffer++ - '0';
							
							PosX = (PosX)?--PosX:0;
							PosY = (PosY)?--PosY:0;

							if(PosY != iPreLine)
							{
								memcpy(g_PROCESSBUFFER+dstPos, "\r\n", 2);
								dstPos += 2;
							}
							if((PosY > iPreLine) && (PosX > 0))
							{
								memset(g_PROCESSBUFFER+dstPos, ' ', PosX);
								dstPos += PosX;
							}
							iPreLine = PosY;
						}
						break;
					case 'B':
						{
							memcpy(g_PROCESSBUFFER+dstPos, "\r\n\r\n", 4);
							dstPos += 4;
						}
						break;
					case 'C':
						{
							int cnt = abs(atoi(codebuf));
							cnt = min(cnt, 80);
							memset(g_PROCESSBUFFER + dstPos, ' ', cnt);
							dstPos += cnt;
						}
						break;
					default:
						break;
					}
					state = 1;
				}
				else
				{
					codebuf[codeptr++] = srcbuf[i];
					if (codeptr >= 18)
					{
						memcpy(g_PROCESSBUFFER+dstPos, codebuf, codeptr);
						dstPos += codeptr;
						state = 1;
					}
					else
						continue;
				}
			}
			else
			{
				if (srcbuf[i] != 0)
				{
					g_PROCESSBUFFER[dstPos++] = srcbuf[i];
				}
				state = 1;
			}
			break;
		}
		if (dstPos > PROCESSBUFFERLEN-8192)
		{
			fwrite(g_PROCESSBUFFER, 1, dstPos, pf);
			dstPos = 0;
		}
	}
	if (dstPos > 0)
	{
		fwrite(g_PROCESSBUFFER, 1, dstPos, pf);
	}
}

int TelnetSession::WriteTelnetPlayFile(OLDDATEHEADER* pPackh, char* pData, int datlen)
{
	ASSERT((m_playlen + datlen) > 0);
	if (m_playfilename.IsEmpty())
	{
		SYSTEMTIME ftm;
		GetLocalTime(&ftm);
		CString tmpdir = g_pFtpMissObj->GetTmpPath();
		m_playfilename.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u.teln",
			tmpdir, ftm.wYear, ftm.wMonth, ftm.wDay, ftm.wHour,
			ftm.wMinute, ftm.wSecond, m_sip, m_dip, ++g_TelnetFileNum, rand());
	}
	FILE* pf = fopen(m_playfilename, "ab");
	if (pf)
	{
		
		if (m_playlen > 0)
		{
			fwrite(m_playbuffer, 1, m_playlen, pf);
			m_playlen = 0;
		}
		if (pPackh != NULL)
		{
			fwrite(pPackh, 1, sizeof(OLDDATEHEADER), pf);
		}
		if (datlen > 0)
		{
			fwrite(pData, 1, datlen, pf);
		}
		fclose(pf);
		pf = NULL;
		return 0;
	}
	else
	{
		return -2;
	}
}

int TelnetSession::WriteTelnetPlayPacket(DTcpPacket& packet)
{
//VMPBEGIN


	// 加密数据
	UCHAR OUTBUF[2000];
	OLDDATEHEADER DataHeadert;
//	UCHAR KEY[]="\x2\x9\xa\xf\x8\x4\xe\x7\x9\x0\x6\x3\x1\xc\xd\xb";
//	UCHAR KEY[]="\x22\x19\x4A\xFD\x87\x40\x4E\x87\xF9\x6A\xA6\xD3\xC1\x0A\xDD\xCB";		// New Key
	INT outbuflen = 0;
	DataHeadert.m_type = 2;
	DataHeadert.m_tm = packet.tm;
	DataHeadert.m_tm1 = packet.tm1;
	memcpy(DataHeadert.m_sip, packet.sip, 4);
	memcpy(DataHeadert.m_dip, packet.dip, 4);

	DataHeadert.m_sport=packet.sport;
	DataHeadert.m_dport=packet.dport;
	DataHeadert.m_ipid=0;
	DataHeadert.m_vlanid=0;
	DataHeadert.m_sequence=packet.Ssequence;
	DataHeadert.m_ack=packet.Sack;
	DataHeadert.m_control=packet.control;
	DataHeadert.m_datalen = packet.Slen;
	char* key="\x22\x19\x4A\xFD\x87\x40\x4E\x87\xF9\x6A\xA6\xD3\xC1\x0A\xDD\xCB";
	qq_encrypt((UCHAR *)packet.buf, DataHeadert.m_datalen, (UCHAR*)key, OUTBUF, &outbuflen);
	DataHeadert.m_datalen = outbuflen;
//	memcpy(DataHeadert.mac, "\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1", 12);
	memcpy(DataHeadert.m_mac, "\x2\x2\x2\x2\x2\x2\x2\x2\x2\x2\x2\x2", 12);

	if ((outbuflen + sizeof(OLDDATEHEADER) + m_playlen) >= TLENET_SIONBUFSIZE)
	{
		// 写文件
		if (0 > WriteTelnetPlayFile(&DataHeadert, (char*)OUTBUF, outbuflen))
		{
			return -2;
		}
	}
	else
	{
		if (m_playbuffer == NULL)
		{
			m_playbuffer = new char[TLENET_SIONBUFSIZE];
		}
		memcpy(m_playbuffer + m_playlen, &DataHeadert, sizeof(OLDDATEHEADER));
		m_playlen += sizeof(OLDDATEHEADER);
		memcpy(m_playbuffer + m_playlen, OUTBUF, outbuflen);
		m_playlen += outbuflen;
	}
//VMPEND
 	return 0;
}

// BOOL WriteIndexPlayFile(TelnetSession* s)
// {
// 	FILE *fp=0;
// 	
// 	s->playindexfile=s->playfilename+".indexfile.txt";
// 	
// 	fp=fopen(s->playindexfile,"wb");
// 	if(!fp)
// 		return FALSE;	
// 
// 	CString tmppos;
// 	AAAInfo pAaaInfo;
// 	if(GetProbeAAAInfo(s->sip,s->dip,s->smac,s->dmac,&pAaaInfo,s->ipidarr,
// 		tmppos))
// 	{
// 
// 	}
// 	else
// 		tmppos.Format("%s",s->spyaddr);
// 
// 	char spytm[40];
// 	GetTimeString(s->starttm, "%Y-%m-%d %H:%M:%S", spytm, sizeof(spytm));
// 	char endtm[40];
// 	GetTimeString(s->lastptm, "%Y-%m-%d %H:%M:%S", endtm, sizeof(endtm));
// 	CString tmpsip, tmpdip;
// 	tmpsip=s->sip.Left(15);
// 	BOOL isReal;
// 	int ltype;
// 	if (s->LimitType > 200)
// 	{
// 		AddRealIP(tmpsip, s->CaseID, s->LimitAddr, s->LimitType);
// 		isReal = FALSE;
// 		ltype = s->LimitType - 200;
// 	}
// 	else if (s->LimitType > 100)
// 	{
// 		isReal = TRUE;
// 		ltype = s->LimitType - 100;
// 	}	
// 	else
// 	{
// 		isReal = FALSE;
// 		ltype = s->LimitType;
// 	}
// 	tmpsip.Replace(".","_");
// 	tmpdip=s->dip.Left(15);
// 	tmpdip.Replace(".","_");
// 	
// 	WriteIndexfileField(fp, "[1]\r\npro= telnet\r\nspyaddr", tmppos);
// 	WriteIndexfileField(fp, "tm",spytm);
// 	WriteIndexfileField(fp, "etm",endtm);
// 	WriteIndexfileField(fp, "sip",tmpsip);
// 	WriteIndexfileField(fp, "sport",s->sip.Right(5));
// 	WriteIndexfileField(fp, "dip",tmpdip);
// 	WriteIndexfileField(fp, "dport",s->dip.Right(5));
// 	WriteIndexfileField(fp, "smac", s->smac);
// 	WriteIndexfileField(fp, "dmac", s->dmac);
// 	if (!s->CaseID.IsEmpty())
// 	{
// 		WriteIndexfileField(fp, "caseid",s->CaseID);
// 		WriteIndexfileField(fp, "objid",s->ObjectID);
// 		WriteIndexfileField(fp, "limitaddr",s->LimitAddr);
// 		WriteIndexfileField(fp, "limittype",ltype);
// 	}
// 	WriteIndexfileField(fp, "account",s->UserAccount1);
// 	WriteIndexfileField(fp, "phone",s->Phone1);
// 	WriteIndexfileField(fp, "2account",s->UserAccount2);
// 	WriteIndexfileField(fp, "2phone",s->Phone2);
// 	WriteIndexfileField(fp, "filename",	GetShortName(s->playfilename));
// 	if (isReal)
// 		WriteIndexfileField(fp,"扩展", "TRUE");
// 	fclose(fp);	
// 
// 	CString strISP;
// 	if (v_stat && v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)s->cdip)), strISP))
// 	{
// 		CString str;
// 		str.Format("%s\1%s\1telnet\1TELNET服务\1%s\1TELNET\1%s\1\1", 
// 			s->dip.Left(15), strISP, s->dip.Right(5), TYPESTATE_DECIDED);
// 		v_IdcServerStat[TELNET_PROTOCOL].AddServer(str);
// // 		str.Format("%s_%05u_telnet", s->dip.Left(15), atoi(s->dip.Right(5)));
// // 		v_IdcPortStat[TELNET_PROTOCOL].AddServer(str);
// 	}
// 	return TRUE;
// }

/*
	telnet 命令的两种格式
		0xff opt1 opt2
		oxff 0xfa opt2 ... 0xff 0xf0
		opt1 0xf0 0xf1 ... 0xfe
		opt2 0 1 3 0x18 0x1f 0x21 0x23 0x27
*/
int parsetelnetcommand(UCHAR *databuf,int len, BOOL server)
{
	int numcommand=0;
	if(databuf[0]!=0xff)
	{
		return 0;
	}
	
	if(len==3)
	{
		if(databuf[1] >0xf0 && databuf[1]<0xff 
			&& (databuf[2]== 0   || databuf[2]== 1   
			|| databuf[2]== 3    || databuf[2]== 0x18    
			|| databuf[2]== 0x1f   || databuf[2]== 0x21   
			|| databuf[2]== 0x23   || databuf[2]== 0x27  ))
		{
			return 1;
		}
		else
			return 0;
	}
	int numchar=0;
	BOOL telnetSB=0;
	for (numchar=0;numchar<len;numchar++)
	{
		
		if(databuf[numchar]==0xff && 
			databuf[numchar+1] >=0xf0 && databuf[numchar+1]<0xff )
		{
			switch(databuf[numchar+1])
			{
			case 250:
				if(databuf[numchar+2]== 0   || databuf[numchar+2]== 1   
					|| databuf[numchar+2]== 3    || databuf[numchar+2]== 0x18    
					|| databuf[numchar+2]== 0x1f   || databuf[numchar+2]== 0x21 
					|| databuf[numchar+2]== 0x23   || databuf[numchar+2]== 0x27 )
				{
					numchar+=2;
					telnetSB=TRUE;
				}
				break;
			case 240:
				if(telnetSB)
				{
					numchar++;
					telnetSB=0;
					numcommand++;
				}
				break;
			case 241:
			case 242:
			case 243:
			case 244:
			case 245:
			case 246:
			case 247:
			case 248:
			case 249:
			case 251:
			case 252:
			case 253:
			case 254:
				if(databuf[numchar+2]== 0   || databuf[numchar+2]== 1   
					|| databuf[numchar+2]== 3    || databuf[numchar+2]== 0x18    
					|| databuf[numchar+2]== 0x1f   || databuf[numchar+2]== 0x21   
					|| databuf[numchar+2]== 0x23   || databuf[numchar+2]== 0x27 )
				{
					numchar+=2;
					telnetSB=0;
					numcommand++;					
				}
				break;
			default:
				break;
			}
			if(numchar>33)
				break;
		}
	}
	return numcommand;
}

int parsetelnetctr(UCHAR *databuf,int len, BOOL server)
{
	int numcommand=0;
	int numchar=0;
	for (numchar=0;numchar<len;numchar++)
	{
		if(databuf[numchar]==0x1b && 	databuf[numchar+1] ==0x5b )
		{
				numchar+=2;
				numcommand++;	
				continue;
		}
	}
	return numcommand;
}
