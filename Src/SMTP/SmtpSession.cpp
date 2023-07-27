
#include "stdafx.h"
#include "smtp.h"

#include <comdef.h>
#import "..\\..\\lib\\msado15.dll" rename ("EOF","FileEOF") no_namespace 
#import "..\\..\\lib\\cdoex.dll" rename("Folder","CDOFolder") no_namespace

#include "SmtpSession.h"

SmtpSession::SmtpSession(DATEHEADER& packh, int aT)
:SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 30;		// һ���Ӻ����һ��CloseSession()����
	m_CSeqOffset = 0;
	m_CAckOffset = 0;
	m_FLAG = NOEML;
	m_UserSeq = 0;
	m_PassSeq = 0;
	m_sortflag = 0;
	m_nseq = 0;
	m_mailflag = FALSE;
	if(packh.m_sequence > MaxSeq)
	{
		m_CSeqOffset = SeqOffset;
	}
	if(packh.m_ack > MaxSeq)
	{
		m_CAckOffset = SeqOffset;
	}
	m_buffer = NULL;
	m_datlen = 0;
}

SmtpSession::~SmtpSession()
{
	Clear();
}

void SmtpSession::Clear()
{
	if (!m_indexfname.IsEmpty())
	{
		DeleteFile(m_indexfname);
		m_indexfname.Empty();
	}

	if (!m_realfname.IsEmpty())
	{
		DeleteFile(m_realfname);
		m_realfname.Empty();
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
	m_attfiles.RemoveAll();
	if (m_buffer != NULL)
	{
		delete[] m_buffer;
		m_buffer = NULL;
	}
	m_datlen = 0;

	m_nseq=0;
	m_fileseq=0;
	m_begfiletm = 0;
	m_FLAG = NOEML;
	m_Language = "";
}

BOOL SmtpSession::CloseSession(time_t nowtm)
{
	if(m_sortflag != 0)
	{
		m_Packetlst.sort();
		m_sortflag=0;
	}

	if (m_FLAG == NOEML)
	{
		// û���ʼ�ͷ ������ʼ�ͷ
		SMTPNextSeqSession();
	}
	if (-1 > SmtpLstToEml(g_SmtpFileBuffer, FILEBUFFERSIZE))
	{
		m_FLAG = NOEML;		// �����ʼ����ʼ���ԭʧ��
	}

	BOOL isDropSess = FALSE;
	if (nowtm != 0)
	{
		isDropSess = (nowtm - m_lastpacktm)>=300;	// 300sû���µİ������ٻỰ
		m_closetm += 60;		// ����Ự������ô60s���������
	}


	int loop=0;
	DWORD seq = 0xFFFFFFFF;
	while(1)
	{
		if (m_nseq == seq)
		{
			if (++loop >= 10)
			{
				loop=0;
				if (m_Packetlst.size()>0)
				{
					m_Packetlst.erase(m_Packetlst.begin());
					WriteLog(SMTPPRO, "loop=10 nsq==seq");
				}
				else
				{
					return TRUE;
				}
			}
		}
		else
		{
			seq = m_nseq;
			loop = 0;
		}
		if (m_FLAG == EMLOVER)	// һ���ʼ���ԭ���
		{
			DumpEmlFile();
		}
		else if((m_FLAG == EMLPROCING) && (!isDropSess))
		{
			return FALSE;
		}
		
		if ((m_Packetlst.size() == 0)
			&& ((m_FLAG != EMLPROCING) || (isDropSess)))
		{
			return TRUE;
		}

		Clear();	// ����м����
		if (!SMTPNextSeqSession())
		{
			ASSERT(m_Packetlst.size()==0);
			m_FLAG = NOEML;
			return TRUE;
		}
		else
		{
			int re = SmtpLstToEml(g_SmtpFileBuffer, FILEBUFFERSIZE);
			if (re < -1)
			{
				m_FLAG = NOEML;		// �����ʼ����ʼ���ԭʧ��
			}
			if ((m_FLAG != NOEML) && (m_FLAG != EMLOVER) && (!isDropSess))
			{
				break;
			}
		}
	}
	return FALSE;
}

int SmtpSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	int re=0;
	char * pbuffer=0;
	try
	{
		m_lastpacktm = packh.m_tm;
		if(memcmp(packh.m_sip,this->m_csip,4)==0)
		{

			GetSmtpPass(packh, pPacket);
			//MAIL RCPT DATA RSET NOOP QUIT AUTH
			TcpPacket	Tcpp(packh, pPacket, this->m_CSeqOffset, this->m_CAckOffset) ;
			m_Packetlst.push_back(Tcpp);
			Tcpp.buf=0;
			m_sortflag++;
		}
		else
		{
			// ���������صİ�������
			return 0;
		}
		//���ļ�ģ��
		if ((m_sortflag > 30) && (m_Packetlst.size() > 60))
		{
//			ASSERT(m_sip.Right(5) != "02396");
			// ÿ30�������򲢴���һ��
			this->m_Packetlst.sort();
			m_sortflag=0;
			if (m_FLAG == NOEML)
			{
				// û���ʼ�ͷ ������ʼ�ͷ
				SMTPNextSeqSession();
			}
			while(m_FLAG == EMLPROCING)
			{
				// ���ʼ�ͷ ��ԭ���ʼ�
				re = SmtpLstToEml(g_SmtpFileBuffer, FILEBUFFERSIZE);
				if (m_FLAG == EMLOVER)
				{
					// һ���ʼ���ԭ��� ����ʼ�
					DumpEmlFile();
				}
				else if (re < -1)
				{
					// һ���ʼ���ԭʧ�� �������ʼ�
					m_FLAG = NOEML;
				}
				else
					break;

				// �廷������
				Clear();
				// ������һ���ʼ�ͷ
				SMTPNextSeqSession();
			}
		}
	}
	catch (...)
	{
		re=-3;
		WriteLog(SMTPPRO, "InsertPacket() Error!!");
	}
	return re;
}


BOOL SmtpSession::SMTPNextSeqSession()
{
	TcpPacketListIt resstart,reslast,resitl,restmpitl;
	resstart = m_Packetlst.begin();
	reslast = m_Packetlst.end();
	restmpitl = resstart;
	while(restmpitl != reslast)
	{
		resitl = restmpitl;
		restmpitl++;
#ifdef		_DEBUG
		TcpPacket *pp = &*resitl;
#endif
		if ((resitl->Slen >= 6) && (memicmp(resitl->buf, "RSET\r\n", 6) == 0))
		{
			m_begfiletm = resitl->tm;
			m_nseq = resitl->Ssequence + 6;
			m_fileseq = resitl->Ssequence + 6;
			m_mailflag = FALSE;
			m_FLAG = EMLPROCING;
			return TRUE;
		}
		else if ((resitl->Slen > 16)
			&& (memicmp(resitl->buf, "MAIL FROM:", 10) == 0)
			&& (SearchCRLF.find(resitl->buf, resitl->Slen) > 10))
		{
			m_begfiletm = resitl->tm;
			m_nseq = resitl->Ssequence;
			m_fileseq = resitl->Ssequence;
			m_mailflag = FALSE;
			m_FLAG = EMLPROCING;
			return TRUE;
		}
		m_Packetlst.erase(resitl);
	}
	return FALSE;
}

BOOL SmtpSession::CreateFilename()
{
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString TmpPath = g_smtpMissObj->GetTmpPath();

	m_realfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_smtp.eml",
		TmpPath, now.wYear, now.wMonth, now.wDay,
		now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_smtpFileNum);
	return TRUE;
}

int SmtpSession::SmtpLstToEml(char* buf, int bufsize)
{
	ASSERT(bufsize > 1024*24);
	TcpPacketListIt start, last, itl, tmpitl;
	int i=0;
	int re=0;
	FILE *fs=0;
	int filebufferlen=0;
	try
	{
		start = m_Packetlst.begin();
		last = m_Packetlst.end();
		tmpitl = start;	
		time_t begtm = 0;
		while(tmpitl != last)
		{
			itl = tmpitl;
			TcpPacket *pp = &*tmpitl;
			tmpitl++;
			if (pp->Nextsequence <= m_nseq)
			{
				// �ظ��İ�����Ҫ�İ�
				m_Packetlst.erase(itl);		
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
				if ((pp->Ssequence == m_nseq)
					&& ((strcmp(pp->buf, "QUIT\r\n") == 0)
						|| (pp->Slen >= 6 && strncmp(pp->buf, "RSET\r\n", 6) == 0)))
				{
					re = 8;		//over
					break;
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
				m_Packetlst.erase(itl);
				if(memcmp(buf + filebufferlen - 5, "\r\n.\r\n", 5) == 0)
				{
					re=8;	//over
					break;
				}
				if(filebufferlen > bufsize - 10000)
				{
					// ��ʱ������buf������
					break;
				}
			}
		}

		//����ʼ� 
		if ((re == 8) && ((m_datlen + filebufferlen) < 220)
			&& (m_realfname.IsEmpty()))
		{
			//�ļ�̫С�ӵ�
			return -4;
		}
		if ((filebufferlen == 0) && (re == -1))
		{
			int packcount = m_Packetlst.size();
			if ((abs(begtm - m_lastpacktm) <= 60) && (packcount < 500))
			{
				return 0;
			}
			WriteLog(SMTPPRO, "packetCount=%d, sip=%s", packcount, m_sip);
			return -3;
		}
		if(filebufferlen == 0)
		{
			return re;
		}

		// ���滹ԭ����ʱ����
		m_fileseq = m_nseq;
		return SaveEmlTmpFile(re, buf, filebufferlen);
	}
	catch (...)
	{
		WriteLog(SMTPPRO, "SmtpLstToEml() Error!!");
		re=-3;
	}
	return re;
}

int SmtpSession::SaveEmlTmpFile(int flg, char* pData, int datlen)
{
	char* chkbuf;
	int chklen = 0;
	if ((m_datlen + datlen) <= SMTP_SIONBUFSIZE)
	{
		if (m_buffer == NULL)
		{
			m_buffer = new char[SMTP_SIONBUFSIZE];
		}
		memcpy(m_buffer+m_datlen, pData, datlen);
		m_datlen += datlen;
		datlen = 0;
		chkbuf = m_buffer;
		chklen = m_datlen;
	}
	else if (m_datlen > 0)
	{
		ASSERT(m_buffer != NULL);
		memcpy(m_buffer+m_datlen, pData, SMTP_SIONBUFSIZE-m_datlen);
		chkbuf = m_buffer;
		chklen = SMTP_SIONBUFSIZE;
	}
	else
	{
		chkbuf = pData;
		chklen = datlen;
	}

	if(!m_mailflag)
	{
		//����֮ǰ����Ƿ����ʼ� //from m_to 
		m_mailflag = CheckIfTrueMail(chkbuf, chklen);
		if (!m_mailflag && ((flg == 8) || (chklen >= SMTP_SIONBUFSIZE)))
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
			if(flen <= 0 || flen >= SMTP_MAXMAILSIZE)	
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

void SmtpSession::GetSmtpPass(DATEHEADER& packh, const char* pPacket)
{
	//	CMailCoder m_FLAG;
	char	Src[MAXLEN];
	char	Result[MAXLEN];
	try
	{
		if (packh.m_datalen>17 && 
			memcmp("AUTH PLAIN ", pPacket,11)==0 && 
			memcmp("\r\n", pPacket+packh.m_datalen-2, 2)==0)
		{
			CString userpass;
			userpass=CString(pPacket+11,packh.m_datalen-13);
			memset(Src,'\0',sizeof(Src));
			memset(Result,'\0',sizeof(Result));
			memcpy(Src,userpass,userpass.GetLength());
			
			//			m_FLAG = new CMailCoder;
			CMailCoder::base64_decode(Src,packh.m_datalen -2,Result);
			if (Result[0]==0 && Result[1]!=0)
			{
				// <NULL>user<NULL>pass<NULL>
				m_userid = Result + 1;
				m_PASS = Result + (m_userid.GetLength() + 2);
			}
			else
			{
				userpass.Format("%s",Result);
				int pos=0;
				pos = userpass.Find(' ');
				if(pos <= 0)
				{
					return;
				}
				m_userid = userpass.Left(pos);
				m_PASS = userpass.Right(userpass.GetLength() - pos - 1);
			}
			//			delete m_FLAG;
			m_UserSeq = 0xffffffff;
			return;
		}
		if ((packh.m_datalen == 12) && (strncmp(pPacket, "AUTH LOGIN\r\n", 12) == 0))
		{
			m_UserSeq = packh.m_sequence + 12;
			return;
		}
		//AUTH PLAIN AGNvb2xzdGFyMTQAeHRmbmhh\r\n �µ������ʽ  user pass
		if ((packh.m_datalen > 0) && (packh.m_sequence == m_UserSeq))
		{
			m_PassSeq = packh.m_sequence + packh.m_datalen;
			if (packh.m_datalen <= 2)
			{
				return;
			}
			memset(Src, '\0', sizeof(Src));
			memset(Result, '\0', sizeof(Result));
			memcpy(Src, pPacket, packh.m_datalen - 2);
			CMailCoder::base64_decode(Src, packh.m_datalen - 2, Result);
			m_userid.Format("%s", Result);
			return;
		}
		if ((packh.m_datalen > 0) && (packh.m_sequence == m_PassSeq))
		{
			memset(Src, '\0', sizeof(Src));
			memset(Result, '\0', sizeof(Result));
			if (packh.m_datalen > 2)
			{
				memcpy(Src, pPacket, packh.m_datalen - 2);
			}
			else
			{
				return;
			}
			CMailCoder::base64_decode(Src, packh.m_datalen - 2, Result);
			m_PASS.Format("%s", Result);
			return;
		}
	}
	catch(...)
	{
		WriteLog(SMTPPRO, "GetSmtpPass() Error!!");
	}
}



