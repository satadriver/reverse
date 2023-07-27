// ImapSession.cpp: implementation of the ImapSession class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "pop3.h"
#include "ImapSession.h"
#include "../Algorithm.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define _DEBUG_IMAP
extern CString SplitEmlName(CString emlname, CString username, BOOL isfrom);

ImapSession::ImapSession(DATEHEADER& packh, int aT, BOOL isServer)
:SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 30;
	m_lastpacktm = packh.m_tm;
	m_CSeqOffset=0	;
	m_CAckOffset=0	;
	m_FLAG = NOEML;
	m_sortflag = 0;
	m_mailflag=FALSE;
	m_emlfilecount = 0;
	m_cntMail = 0;
	m_buffer = NULL;
	m_datlen = 0;

	if (isServer)
	{
		// 交换IP方向
		memcpy(m_cdip,packh.m_sip,4);
		memcpy(m_csip,packh.m_dip,4);
		m_AuthInfo.SwapAuthInfo();
		CString s;
		s = m_dmac;
		m_dmac = m_smac;
		m_smac = s;
		s = m_sip;
		m_sip = m_dip;
		m_dip = s;

		if(packh.m_sequence > MaxSeq)
		{
			m_CAckOffset = SeqOffset;
		}
		if(packh.m_ack > MaxSeq)
		{
			m_CSeqOffset = SeqOffset;
		}
	}
	else
	{
		if(packh.m_sequence > MaxSeq)
		{
			m_CSeqOffset = SeqOffset;
		}
		if(packh.m_ack > MaxSeq)
		{
			m_CAckOffset = SeqOffset;
		}
	}
}

ImapSession::~ImapSession()
{

}

int ImapSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	int re = 0;
	char * pbuffer=0;
	try
	{
		m_lastpacktm = packh.m_tm;
		if(memcmp(packh.m_sip, this->m_csip, 4)==0)
		{
			// tags LOGIN "user" "pass"
			CString tags, user, pass;
			int ioff = 0;
			int itmp = 0;
			char *pPart = strchr(pPacket, ' ');
			if(pPart != NULL)
			{
				ioff = pPart-pPacket+1;
				tags = CString(pPacket, ioff);
				tags.TrimLeft(" \"");
				tags.TrimRight(" \"\r\n");
			}
			if(pPart != NULL && memcmp(pPacket+ioff, "LOGIN", 5) == 0)
			{
				ioff += 5;
				pPart = strstr(pPacket+ioff, "\" ");
				if(pPart != NULL)
				{
					itmp = pPart-(pPacket+ioff);
					user = CString(pPacket+ioff, itmp);
					ioff += itmp;
					user.TrimLeft(" \"");
					user.TrimRight(" \"\r\n");
				}
			}
			if(pPart != NULL && pPacket[ioff] == '\"')
			{
				pass = CString(pPacket+ioff);
				pass.TrimLeft(" \"");
				pass.TrimRight(" \"\r\n");
			}
			return 0; 
		}
		else
		{
			TcpPacket	Tcpp(packh, pPacket, this->m_CAckOffset, this->m_CSeqOffset) ;
			m_PacketList.push_back(Tcpp);
			Tcpp.buf=0;
			m_sortflag++;
			if((m_sortflag > 10) || (m_PacketList.size() > 60))
			{
				m_PacketList.sort();
				m_sortflag=0;
				if (m_FLAG == NOEML)
				{
					// 没有邮件头 则查找邮件头
					FindImapEmlHead();
				}
				while(m_FLAG == EMLPROCING)
				{
					// 有邮件头 则还原该邮件
					re = Pop3LstToEml(g_Pop3FILEBUFFER, FILEBUFFERSIZE);
					if (m_FLAG == EMLOVER)
					{
						// 一封邮件还原完毕 输出邮件
						DumpEmlFile();
					}
					else if (re < -1)
					{
// 						if( strncmp(g_Pop3FILEBUFFER, "+OK", 3)==0 && 
// 							(-1 != g_SearchOctets.find(g_Pop3FILEBUFFER)) &&
// 							GetFileLength(m_realfname) < 180)
// 						{
// 							int limitPacket = 60;
// 							if(m_cntMail > 0)
// 							{
// 								if(m_cntMail-m_emlfilecount > 20)
// 									limitPacket = (m_cntMail-m_emlfilecount)*3;
// 							}
// 							if(m_PacketList.size() < limitPacket)
// 							{
// 								re = -1;
// 								break;
// 							}
// 						}
						// 一封邮件还原失败 跳过该邮件
						m_FLAG = NOEML;
					}
					else
						break;

					Clear();
					// 查找下一封邮件头
					FindImapEmlHead();
				}
			}
		}
	}
	catch (...)
	{
		re=-3;
		WriteLog(POP3PRO, "InertPacket() Error!!");
	}
	return re;
}

BOOL ImapSession::CloseSession(time_t nowtm)
{
	if(m_sortflag!=0)
	{
		m_PacketList.sort();
		m_sortflag = 0;
	}
	if (m_FLAG == NOEML)
	{
		// 没有邮件头 则查找邮件头
		FindImapEmlHead();
	}
	if (-1 > Pop3LstToEml(g_Pop3FILEBUFFER, FILEBUFFERSIZE))
	{
		m_FLAG = NOEML;
	}
	BOOL isDropSess = FALSE;
	if (nowtm != 0)
	{
		isDropSess = (nowtm - m_lastpacktm)>=300;
		m_closetm += 60;
	}

	while(1)
	{
		if (m_FLAG == EMLOVER)	// 一封邮件还原完毕
		{
			DumpEmlFile();
		}
		else if((m_FLAG == EMLPROCING) && (!isDropSess))
		{
			return FALSE;
		}

//		m_ServerLine.Clear();
		Clear();
		if (!FindImapEmlHead())
		{
			m_FLAG = NOEML;
			if (m_emlfilecount==0 && !m_userid.IsEmpty() && !m_PASS.IsEmpty())
			{
				m_OpenIndxFlg = v_openflagpop3;
				m_OpenCaseFlg = v_opencasepop3;
				m_OpenWarnFlg = v_openwarnpop3;

// 				CString text;	// 需要匹配案件和预警的串
// 				text.Format("____%s\r\n____%s", 
// 					, m_PASS);
				// 匹配案件和预警
				DWORD d_state = 1;
				SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 1);
				SearchCaseOrWarn(this, m_PASS, m_PASS.GetLength(), d_state,"pop3");

				// 处理IDC数据
				if(v_stat && !m_userid.IsEmpty() && m_emlfilecount>0)
				{
					CString strISP;
					if (v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
					{
						CString str;
						str.Format("%s\1%s\1pop3\1邮件服务\1%s\1POP3\1%s\1\1",
							m_dip.Left(15), strISP, m_dip.Right(5), TYPESTATE_DECIDED);
						v_IdcServerStat[POP3_PROTOCOL].AddServer(str);
					}
				}				HistoryRecord hisRec;
				CString from = SplitEmlName(m_to, m_userid, FALSE);

				if (v_statMail &&  
					(!from.IsEmpty() || !m_userid.IsEmpty())) //pop3无收件人and user为空不写表
				{
					char spytm[40];
					GetTimeString(m_lastpacktm, "%Y-%m-%d %H:%M:%S", spytm, 40);

					if (m_userid.IsEmpty())
					{
						m_userid.Format("%s", from);
					}
					else if (from.IsEmpty())
					{
						from = m_userid;
					}

					HistoryRecord hisRec;
					hisRec.SetRecord(IDX_BEGTM,		spytm);
					hisRec.SetRecord(IDX_ENDTM,		spytm);
					hisRec.SetRecord(IDX_FORWTYPE,	"1");
					hisRec.SetRecord(IDX_ID,		from);
					hisRec.SetRecord(IDX_USER,		from);

					from = SplitEmlName(m_from, "", TRUE);
					hisRec.SetRecord(IDX_CONTACT,	from);
					hisRec.SetRecord(IDX_PASS,		m_PASS);
					hisRec.SetRecord(IDX_STATE,		"收信");
					if (m_ProxyType == 1)
					{
						hisRec.SetRecord(IDX_IFAGENT,		"2");
					}

					WriteHisData("imap", "imap", &hisRec, g_pop3MissObj, POP3_PROTOCOL);//, HISTORY_DATA);
					
				}

				// 写对照文件并输出(只有Sleuth和案件才输出)
				CString text;
				text.Empty();
				if (!m_userid.IsEmpty())
				{
					text.Format("user= %s\r\n", m_userid);
				}
				if (!m_PASS.IsEmpty())
				{
					text.Format("%spass= %s\r\n", (CString)text, m_PASS);
				}
				text.Format("%snofile= yes\r\n", (CString)text);

				SYSTEMTIME now;
				GetLocalTime(&now);
				CString TmpPath = g_pop3MissObj->GetTmpPath();
				m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_imap.eml.indexfile.txt",
					TmpPath, now.wYear, now.wMonth, now.wDay,
					now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_Pop3FileNum);

				WriteIndexFile("imap", "imap", text, g_pop3MissObj);
			}
			return TRUE;
		}
		else
		{
			int re = Pop3LstToEml(g_Pop3FILEBUFFER, FILEBUFFERSIZE);
			if (re < -1)
			{
				m_FLAG = NOEML;
			}
			if ((m_FLAG != NOEML) && (m_FLAG != EMLOVER) && (!isDropSess))
			{
				break;
			}
		}
	}
	return FALSE;
}

BOOL ImapSession::FindImapEmlHead()
{
// FETCH (UID 存在, BODY[] 存在, {长度} 存在
// * 11 FETCH (UID 11 BODY[] {3458}

	Search srhFETCH("FETCH (UID");
	Search srhBODY("BODY[] {");

	TcpPacketListIt reslast, resitl, restmpitl;
	restmpitl = m_PacketList.begin();
	reslast = m_PacketList.end();
	while(restmpitl!=reslast)
	{
		resitl=restmpitl;
		restmpitl++;
		TcpPacket *pp = &*resitl;
		int ib = srhBODY.find(pp->buf, pp->Slen);
		if (srhFETCH.find(pp->buf, pp->Slen) > 0 && ib > 10)
		{
			int pos = SearchCRLF.find(pp->buf, pp->Slen);
			if (pos > 2 && pp->buf[pos-1]=='}')
			{
#ifdef _DEBUG_IMAP
				printf("find %s\r\n", CString(pp->buf, pos));
#endif
				CString strok = CString(pp->buf+ib+srhBODY.m, pos-ib);
				DWORD endpos = atol(strok);
				
				m_begfiletm = pp->tm;
				m_nseq = pp->Ssequence + pos + 2;
				m_fileseq = pp->Ssequence + pos + 2;
 				m_endseq = endpos + pp->Ssequence+pos+2;//GetemlEndSeq(pp->buf, ib, pp->Ssequence);
				m_mailflag = FALSE;
				m_FLAG = EMLPROCING;
				return TRUE;
			}
		}
		m_PacketList.erase(resitl);
	}
	return FALSE;
}

int ImapSession::Pop3LstToEml( char * buf, int bufsize )
{
	int re = 0;
	TcpPacketListIt last, itl, tmpitl;
	int i=0;
	FILE *fs=0;
	int filebufferlen=0;
	time_t begtm = 0;
	BOOL IsFirst = TRUE;

	try
	{
		tmpitl = m_PacketList.begin();
		last = m_PacketList.end();
		while(tmpitl != last)
		{
			itl = tmpitl;
			tmpitl++;
			TcpPacket *pp = &*itl;
			if (pp->Nextsequence <= m_nseq)
			{
				// 重复的包或不需要的包
				m_PacketList.erase(itl);		
				continue;
			}
			else if (pp->Ssequence > m_nseq)
			{
				//不连续就退出
				begtm = pp->tm;
				re = -1;
				break;
			}
			else
			{
				// 连续的数据包
				if ((pp->Ssequence == m_nseq) && (pp->Slen >= 5))
				{
					Search srhCompleted("FETCH completed");
					if (srhCompleted.find(pp->buf, pp->Slen) > 6
						&& (SearchCRLF.find(pp->buf, pp->Slen) >= 3))
					{
						re = 8;		// over
						break;
					}
// 					if ((memcmp(pp->buf, "+ERR", 4) == 0)
// 						&& (memcmp(pp->buf + pp->Slen - 2, "\r\n", 2) == 0))
// 					{
// 						m_PacketList.erase(itl);
// 						re=-3;//错误
// 						break;
// 					}
				}

				if(pp->Ssequence < m_fileseq)
				{
					memcpy(buf,
						pp->buf + m_fileseq - pp->Ssequence,
						pp->Slen - m_fileseq + pp->Ssequence);
				}
				else
				{
					memcpy(buf - m_fileseq + pp->Ssequence,
						pp->buf, pp->Slen);
				}
				if (m_nseq < pp->Ssequence + pp->Slen)
				{
					m_nseq = pp->Ssequence + pp->Slen;
					filebufferlen = m_nseq - m_fileseq;
				}
				buf[filebufferlen] = 0;
				m_PacketList.erase(itl);

				// 判断邮件是否结束
				// 是否到达结束的序列号
				if(m_endseq > 0)
				{
					if(m_nseq == m_endseq + 2)
					{
						re = 8;		//over
						break;
					}
				}

				// 是否到达结束标志"\r\n.\r\n"
				int lastpacketCRLF = FALSE;
				if ((filebufferlen > 2)
					&& (memcmp(buf + filebufferlen - 2, "\r\n", 2) == 0))
				{
					lastpacketCRLF=TRUE;
				}
				if ((filebufferlen >= 5) && (memcmp(buf + filebufferlen - 5, "\r\n.\r\n", 5) == 0)
					|| ((filebufferlen == 3) && (memcmp(buf, ".\r\n", 3) == 0) && (lastpacketCRLF)))
				{
					re = 8;		//over
					break;
				}

				if(filebufferlen > bufsize - 10000)
				{
					// 临时缓冲区buf快满了
					break;
				}
			}
		}

		if (filebufferlen == 0)
		{
			if (re == -1)
			{
				if ((abs(begtm-m_lastpacktm) <= 40) /*&& (m_PacketList.size() < 500)*/)
				{
					return -1;
				}
				//数据不连续 
				WriteLog(POP3PRO, "packetCount=%d, sip=%s", m_PacketList.size(), m_sip);
				return -3;
			}
			else if(re != 8)
			{
				return re;
			}
		}

		// 保存还原的临时数据
		m_fileseq = m_nseq;
		return SaveEmlTmpFile(re, buf, filebufferlen);

	}
	catch (...)
	{
		WriteLog(POP3PRO, "Pop3LstToEml() Error!!");
		re=-3;
	}
	return re;	
}

int ImapSession::SaveEmlTmpFile( int flg, char* pData, int datlen )
{
	char* chkbuf;
	int chklen = 0;
	if (datlen > 0)
	{
		if ((m_datlen + datlen) <= POP3_SIONBUFSIZE)
		{
			if (m_buffer == NULL)
			{
				m_buffer = new char[POP3_SIONBUFSIZE];
			}
			memcpy(m_buffer+m_datlen, pData, datlen);
			m_datlen += datlen;
			datlen = 0;
			chkbuf = m_buffer;
			chklen = m_datlen;
		}
		else if ((m_datlen > 0) && (!m_mailflag))
		{
			ASSERT(m_buffer != NULL);
			memcpy(m_buffer+m_datlen, pData, POP3_SIONBUFSIZE-m_datlen);
			chkbuf = m_buffer;
			chklen = POP3_SIONBUFSIZE;
		}
		else
		{
			chkbuf = pData;
			chklen = datlen;
		}
	}
	else
	{
		chkbuf = m_buffer;
		chklen = m_datlen;
	}

	if(!m_mailflag)
	{
		//存盘之前检查是否是邮件 //from m_to 
		m_mailflag = CheckIfTrueMail(chkbuf, chklen);
		if (!m_mailflag && ((flg == 8) || (chklen >= POP3_SIONBUFSIZE)))
		{
			// 数据结束或大于8K还不是邮件就放弃还原
			return -3;
		}
	}

	if ((datlen > 0) || (flg == 8))
	{
		// 数据结束或数据大于8K 就保存文件
		if(m_realfname.IsEmpty())
		{
			if(!CreateFilename())
			{
				return -3;
			}
		}

		if (flg == 8)
		{
			if (datlen > 0)
			{
				if (memcmp(pData + datlen - 5, "\r\n.\r\n", 5) == 0)
				{
					datlen -= 5;
				}
			}
			else
			{
				if (memcmp(m_buffer + m_datlen - 5, "\r\n.\r\n", 5) == 0)
				{
					m_datlen -= 5;
				}

			}
		}

		FILE* fs = fopen(m_realfname, "ab");
		if(fs != NULL)
		{
			fwrite(m_buffer, m_datlen, 1, fs);
			m_datlen = 0;
			if (datlen > 0)
			{
				fwrite(pData, datlen, 1, fs);
			}
			fclose(fs);
			int flen = GetFileLength(m_realfname);
			if(flen <= 200 || flen >= POP3_MAXMAILSIZE)	
			{
				return -3;
			}
			if((flg == 8) && (m_mailflag))
			{
				m_FLAG = EMLOVER;		// 一封邮件还原结束
			}
		}
		else
		{
			return -3;
		}
	}
	return flg;
}

BOOL ImapSession::CreateFilename()
{
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString TmpPath = g_pop3MissObj->GetTmpPath();

	m_realfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_imap.eml",
		TmpPath, now.wYear, now.wMonth, now.wDay,
		now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_Pop3FileNum);

	return TRUE;
}

void ImapSession::Clear()
{
	m_nseq=0;
	m_fileseq=0;
	m_endseq = 0;
	if (!m_realfname.IsEmpty())
	{
		DeleteFile(m_realfname);
		m_realfname.Empty();
	}
	if (!m_indexfname.IsEmpty())
	{
		DeleteFile(m_indexfname);
		m_indexfname.Empty();
	}
	for (int i=0; i<m_attfiles.GetSize(); i++)
	{
		CString& s = m_attfiles.ElementAt(i);	
		// 以引用的方式操作数组中的字符串
		// 这样相当于直接操作数组中的字符串
		// 注意对其中任何一个变量的改变都会影响到另外一个变量
		if (s.GetLength()>2)
		{
			if (s.GetAt(s.GetLength()-2)=='*')
				s.ReleaseBuffer(s.GetLength()-2);
			DeleteFile(s);
		}
	}
	m_attfiles.SetSize(0, 8);
	m_emlfield.Empty();
	m_CaseArr.RemoveAll();		// 清除案件信息
	m_WarnArr.RemoveAll();		// 清除预警信息
	m_Language = "";
	m_FLAG = NOEML;
	m_datlen = 0;	
}