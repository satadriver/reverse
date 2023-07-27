//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ�һ���Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	Pop3Session
//	�汾:		1.0
//	��������:	2007-7-27
//	����:		��ǿ
//	��Ҫ����:	POP3Э��Ļ�ԭ
//	ģ������:
//	��Ҫ����:	
//**************************************************************** 
#include "stdafx.h"
#include "pop3.h"
#include "Pop3Session.h"

Search	g_SearchOctets("octets\r\n");

Pop3Session::Pop3Session(DATEHEADER& packh, int aT, BOOL isServer)
:SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 30;
	m_lastpacktm = packh.m_tm;
	m_CSeqOffset=0	;
	m_CAckOffset=0	;
	m_UserSeq=0;
	m_PassSeq=0;
	m_FLAG = NOEML;
	m_sortflag = 0;
	m_mailflag=FALSE;
	m_emlfilecount = 0;
	m_cntMail = 0;
	m_buffer = NULL;
	m_datlen = 0;

	if (isServer)
	{
		// ����IP����
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

Pop3Session::~Pop3Session()
{
	Clear();
	if (m_buffer != NULL)
	{
		delete[] m_buffer;
		m_buffer = NULL;
	}
}

void Pop3Session::Clear()
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
		// �����õķ�ʽ���������е��ַ���
		// �����൱��ֱ�Ӳ��������е��ַ���
		// ע��������κ�һ�������ĸı䶼��Ӱ�쵽����һ������
		if (s.GetLength()>2)
		{
			if (s.GetAt(s.GetLength()-2)=='*')
				s.ReleaseBuffer(s.GetLength()-2);
			DeleteFile(s);
		}
	}
	m_attfiles.SetSize(0, 8);
	m_emlfield.Empty();
	m_CaseArr.RemoveAll();		// ���������Ϣ
	m_WarnArr.RemoveAll();		// ���Ԥ����Ϣ
	m_Language = "";
	m_FLAG = NOEML;
	m_datlen = 0;
}

BOOL Pop3Session::Pop3NextSeqSession()
{
	TcpPacketListIt reslast, resitl, restmpitl;
	restmpitl = m_PacketList.begin();
	reslast = m_PacketList.end();
	while(restmpitl!=reslast)
	{
		resitl=restmpitl;
		restmpitl++;
		TcpPacket *pp = &*resitl;
		if (memicmp(pp->buf, "+OK", 3) == 0)
		{
			int pos = SearchCRLF.find(pp->buf, pp->Slen);
			if (pos > 2)
			{
				m_begfiletm = pp->tm;
				m_nseq = pp->Ssequence + pos + 2;
				m_fileseq = pp->Ssequence + pos + 2;
				m_endseq = GetemlEndSeq(pp->buf, pp->Ssequence);
				m_mailflag = FALSE;
				m_FLAG = EMLPROCING;
				return TRUE;
			}
		}
		m_PacketList.erase(resitl);
	}
	return FALSE;
}

BOOL Pop3Session::CloseSession(time_t nowtm)
{
	if(m_sortflag!=0)
	{
		m_PacketList.sort();
		m_sortflag = 0;
	}
	if (m_FLAG == NOEML)
	{
		// û���ʼ�ͷ ������ʼ�ͷ
		Pop3NextSeqSession();
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
		if (m_FLAG == EMLOVER)	// һ���ʼ���ԭ���
		{
			DumpEmlFile();
		}
		else if((m_FLAG == EMLPROCING) && (!isDropSess))
		{
			return FALSE;
		}

//		m_ServerLine.Clear();
		Clear();
		if (!Pop3NextSeqSession())
		{
			m_FLAG = NOEML;
			if (m_emlfilecount==0 && !m_userid.IsEmpty() && !m_PASS.IsEmpty())
			{
				m_OpenIndxFlg = v_openflagpop3;
				m_OpenCaseFlg = v_opencasepop3;
				m_OpenWarnFlg = v_openwarnpop3;

// 				CString text;	// ��Ҫƥ�䰸����Ԥ���Ĵ�
// 				text.Format("____%s\r\n____%s", 
// 					, m_PASS);
				// ƥ�䰸����Ԥ��
				DWORD d_state = 1;
				SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 1);
				SearchCaseOrWarn(this, m_PASS, m_PASS.GetLength(), d_state,"pop3");

				// ����IDC����
				if(v_stat && !m_userid.IsEmpty() && m_emlfilecount>0)
				{
					CString strISP;
					if (v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
					{
						CString str;
						str.Format("%s\1%s\1pop3\1�ʼ�����\1%s\1POP3\1%s\1\1",
							m_dip.Left(15), strISP, m_dip.Right(5), TYPESTATE_DECIDED);
						v_IdcServerStat[POP3_PROTOCOL].AddServer(str);
					}
				}
				// д���ʷ

				HistoryRecord hisRec;
				CString from = SplitEmlName(m_to, m_userid, FALSE);

				if (v_statMail &&  
					(!from.IsEmpty() || !m_userid.IsEmpty())) //pop3���ռ���and userΪ�ղ�д��
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
					hisRec.SetRecord(IDX_STATE,		"����");
					if (m_ProxyType == 1)
					{
						hisRec.SetRecord(IDX_IFAGENT,		"2");
					}

					WriteHisData("pop3", "pop3", &hisRec, g_pop3MissObj, POP3_PROTOCOL);//, HISTORY_DATA);
					
				}

				// д�����ļ������(ֻ��Sleuth�Ͱ��������)
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
				m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pop3.eml.indexfile.txt",
					TmpPath, now.wYear, now.wMonth, now.wDay,
					now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_Pop3FileNum);

				WriteIndexFile("pop3", "pop3", text, g_pop3MissObj);

				if (!m_from.IsEmpty() && !m_to.IsEmpty())
				{
					SYSTEMTIME now;
					GetLocalTime(&now);
					CString strtm;
					strtm.Format("%04d%02d%02d%02d%02d%02d", now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond);
					
					CString strfs;
					strfs.Format("%s\t%s\t%s\t%s\r\n",m_from, m_to, "POP3", strtm);
					CString strfp;
					strfp.Format("v:\\netspy\\statdata\\emlfrdshp\\%s-%d.txt",strtm, rand());
					FILE *fp = fopen(strfp,"w+b");
					if (fp)
					{
						fwrite(strfs,1,strfs.GetLength(),fp);
						fclose(fp);
					}
					
				}
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

//���� +OK 123456 octets\r\n ���� ������SEQ
UINT Pop3Session::GetemlEndSeq(const char *buf, UINT seq)
{
	UINT endseq = 0;
	CString strok = buf;
	int pos = 0;
	pos = strok.Find("\r\n", 0);
	strok.MakeLower();
	if(pos > 2)
	{
		strok=strok.Left(pos);
	}
	else
	{
		return 0;
	}

	if(strok.Find("octets") > 0)
	{
		strok.Replace("+ok","");
		strok.Replace("octets","");
		endseq=atol(strok);
	}
	if(endseq > 0)
	{
		endseq = endseq + seq + pos + 3;
	}
	return endseq;
}

int Pop3Session::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
#ifdef _DEBUG
	static cntPacket = 0;

	cntPacket ++;
#endif
	int re = 0;
	char * pbuffer=0;
	try
	{
		m_lastpacktm = packh.m_tm;
		if(memcmp(packh.m_sip, this->m_csip, 4)==0)
		{
			char tmppack[16];
			memset(tmppack+12, 0, 4);
			memcpy(tmppack, pPacket, 12);
			strupr(tmppack);
			
			if(memicmp(tmppack,"USER ",5)==0)	//�û���
			{
				m_userid = pPacket+5;
				m_userid.ReleaseBuffer(m_userid.GetLength()-2);
			}
			else if(memicmp(tmppack,"PASS ",5)==0)	//����
			{
				m_PASS = pPacket+5;
				m_PASS.ReleaseBuffer(m_PASS.GetLength()-2);
			}
			return 0; 
		}
		else
		{
			TcpPacket	Tcpp(packh, pPacket, this->m_CAckOffset, this->m_CSeqOffset) ;
			m_PacketList.push_back(Tcpp);
			Tcpp.buf=0;
			m_sortflag++;
			if((m_sortflag > 30) && (m_PacketList.size() > 60))
			{
				m_PacketList.sort();
				m_sortflag=0;
				if (m_FLAG == NOEML)
				{
					// û���ʼ�ͷ ������ʼ�ͷ
					Pop3NextSeqSession();
				}
				while(m_FLAG == EMLPROCING)
				{
					// ���ʼ�ͷ ��ԭ���ʼ�
					re = Pop3LstToEml(g_Pop3FILEBUFFER, FILEBUFFERSIZE);
					if (m_FLAG == EMLOVER)
					{
						// һ���ʼ���ԭ��� ����ʼ�
						DumpEmlFile();
					}
					else if (re < -1)
					{
						if( strncmp(g_Pop3FILEBUFFER, "+OK", 3)==0 && 
							(-1 != g_SearchOctets.find(g_Pop3FILEBUFFER)) &&
							GetFileLength(m_realfname) < 180)
						{
							int limitPacket = 60;
							if(m_cntMail > 0)
							{
								if(m_cntMail-m_emlfilecount > 20)
									limitPacket = (m_cntMail-m_emlfilecount)*3;
							}
							if(m_PacketList.size() < limitPacket)
							{
								re = -1;
								break;
							}
						}
						// һ���ʼ���ԭʧ�� �������ʼ�
						m_FLAG = NOEML;
					}
					else
						break;

					// �廷������
//					m_ServerLine.Clear();
					Clear();
					// ������һ���ʼ�ͷ
					Pop3NextSeqSession();
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

BOOL Pop3Session::CreateFilename()
{
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString TmpPath = g_pop3MissObj->GetTmpPath();

	m_realfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pop3.eml",
		TmpPath, now.wYear, now.wMonth, now.wDay,
		now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_Pop3FileNum);

	return TRUE;
}

int Pop3Session::Pop3LstToEml(char * buf, int bufsize)
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
				// �ظ��İ�����Ҫ�İ�
				m_PacketList.erase(itl);		
				continue;
			}
			else if (pp->Ssequence > m_nseq)
			{
				//���������˳�
				begtm = pp->tm;
				re = -1;
				break;
			}
			else
			{
				// ���������ݰ�
				if ((pp->Ssequence == m_nseq) && (pp->Slen >= 5))
				{
					if ((memcmp(pp->buf, "+OK", 3) == 0)
						&& (SearchCRLF.find(pp->buf, pp->Slen) >= 3))
					{
						re = 8;		// over
						break;
					}
					if ((memcmp(pp->buf, "+ERR", 4) == 0)
						&& (memcmp(pp->buf + pp->Slen - 2, "\r\n", 2) == 0))
					{
						m_PacketList.erase(itl);
						re=-3;//����
						break;
					}
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

				// �ж��ʼ��Ƿ����
				// �Ƿ񵽴���������к�
				if(m_endseq > 0)
				{
					if(m_nseq == m_endseq + 2)
					{
						re = 8;		//over
						break;
					}
				}

				// �Ƿ񵽴������־"\r\n.\r\n"
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
					// ��ʱ������buf������
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
				//���ݲ����� 
				WriteLog(POP3PRO, "packetCount=%d, sip=%s", m_PacketList.size(), m_sip);
				return -3;
			}
			else if(re != 8)
			{
				return re;
			}
		}

		// ���滹ԭ����ʱ����
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

int Pop3Session::SaveEmlTmpFile(int flg, char* pData, int datlen)
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
		//����֮ǰ����Ƿ����ʼ� //from m_to 
		m_mailflag = CheckIfTrueMail(chkbuf, chklen);
		if (!m_mailflag && ((flg == 8) || (chklen >= POP3_SIONBUFSIZE)))
		{
			// ���ݽ��������8K�������ʼ��ͷ�����ԭ
			return -3;
		}
	}

	if ((datlen > 0) || (flg == 8))
	{
		// ���ݽ��������ݴ���8K �ͱ����ļ�
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
				m_FLAG = EMLOVER;		// һ���ʼ���ԭ����
			}
		}
		else
		{
			return -3;
		}
	}
	return flg;
}
