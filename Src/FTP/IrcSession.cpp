// IrcSession.cpp: implementation of the CIrcSession class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ftp.h"
#include "IrcSession.h"
#include "../CheckHard/DogBM.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIrcSession::CIrcSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_servernseq = 0;
	m_clientnseq = 0;
	m_sortflg = 0;

	m_lastpacktm = packh.m_tm;
	m_closetm = packh.m_tm + 180;
}

CIrcSession::~CIrcSession()
{

}

int CIrcSession::InsertPacket( DATEHEADER& packh, const char *pPacket )
{
	m_lastpacktm = packh.m_tm;
	if(memcmp(packh.m_sip, m_csip, 4)==0 && packh.m_datalen > 5)
	{
		if (memcmp(pPacket, "NICK ",  5) ==0 )
		{
			m_userid = pPacket+5;
			m_userid.TrimLeft();
			m_userid.TrimRight();
			return 0;
		}
		else if(memicmp(pPacket, "PRIVMSG nickserv :identify", 26) == 0)
		{
			m_ircpass = pPacket+26;
			m_ircpass.TrimLeft();
			m_ircpass.TrimRight();
			return 0;
		}
		else
		{
			// 客户端的包
			m_sortflg++;
			DTcpPacket	Tcpp(packh, pPacket);
			m_PacketList.push_back(Tcpp);
			Tcpp.buf = 0;
		}
	}
	else
	{
		RbSearch srhPRIVMSG(" PRIVMSG ");
		if(pPacket[0]== 0x3a && srhPRIVMSG.find(pPacket, packh.m_datalen) > 1)
		{
			m_sortflg++;
			DTcpPacket	Tcpp(packh, pPacket);
			m_PacketList.push_back(Tcpp);
			Tcpp.buf = 0;
		}
	}
	
	int re;
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

BOOL CIrcSession::CloseSession( time_t nowtm )
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
		if (!m_textfilename.IsEmpty())
		{
			// 匹配案件、预警、屏蔽信息
			m_realfname = m_textfilename;
			m_textfilename.Empty();
			DWORD d_state = 0;
			SearchCaseOrWarn(this, NULL, 0, d_state);

			// 写对照文件并输出
			CString txt;
			if(FALSE == m_userid.IsEmpty())
				txt.Format("user= %s\r\n", m_userid);
			if(FALSE == m_ircpass.IsEmpty())
				txt.Format("%spass= %s\r\n", CString(txt), m_ircpass);
			m_indexfname = m_realfname + ".indexfile.txt";
			WriteIndexFile("irc", "irc", txt, g_pFtpMissObj);

			// 清除案件和预警信息
			m_CaseArr.RemoveAll();
			m_WarnArr.RemoveAll();
			m_begfiletm = 0;

			m_closetm += 180;		// 下一个3分钟再次调用

		}
		else
		{
			m_closetm += 60;		// 下一个1分钟再次调用
		}
		return (abs(nowtm - m_lastpacktm) >= 300);

	}
	catch (...)
	{
		WriteLog(FTPPRO, "CloseSession() Error!!");
		return TRUE;
	}
}

int CIrcSession::LstToFile( char* buf, int bufsize )
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
				if(memcmp(tmpitl->sip, m_cdip, 4)==0)
				{
					// service --> client
					m_servernseq = tmpitl->Ssequence;
					m_clientnseq = tmpitl->Sack;
				}
				else
				{
					m_servernseq = tmpitl->Sack;
					m_clientnseq = tmpitl->Ssequence;
				}
			}
		}

		while(tmpitl != last)
		{
			itl = tmpitl;
			DTcpPacket* pp = &*itl;
			tmpitl++;
			if (memcmp(pp->sip, m_csip, 4)==0)
			{
				// Client --> Server
				if((INT)(pp->Ssequence - m_clientnseq) >= 0)
				{
					m_clientnseq = pp->Nextsequence;
					//WriteTelnetPlayPacket(*pp);
					memcpy(buf + datalen, pp->buf, pp->Slen);
					datalen += pp->Slen;
				}				
				m_PacketList.erase(itl);
			}
			else
			{
				// Server --> Client
				if((INT)(pp->Ssequence - m_servernseq) >= 0)
				{
					m_servernseq = pp->Nextsequence;
					//WriteTelnetPlayPacket(*pp);
					memcpy(buf + datalen, pp->buf, pp->Slen);
					datalen += pp->Slen;
				}				
				m_PacketList.erase(itl);

			}

			
			if (datalen > (bufsize - 10000))
			{
				break;		// buf快满了
			}
		}
		
		if (datalen == 0)
		{
			if ( m_PacketList.size() > 200 )
			{
				return -3;
			}
			return  re;
		}

		//存文件模块
		//m_fileseq = m_servernseq;
		if (0 > WriteIrcTextfile(buf, datalen))
		{
			return -2;
		}
	}
	catch (...)
	{
		WriteLog(FTPPRO, "Irc LstTofile() Error!!");
		re = -3;
	}
	return re;
}

int CIrcSession::WriteIrcTextfile( char* pData, int datlen )
{
	if(m_textfilename.IsEmpty())
	{
		SYSTEMTIME ftm;
		GetLocalTime(&ftm);
		CString tmpdir = g_pFtpMissObj->GetTmpPath();
		m_textfilename.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u.irc.txt",
			tmpdir, ftm.wYear, ftm.wMonth, ftm.wDay, ftm.wHour,
			ftm.wMinute, ftm.wSecond, m_sip, m_dip, ++g_TelnetFileNum, rand());
	}
	
	FILE* fs = fopen(m_textfilename, "ab");
	if(fs != NULL)
	{
		fwrite(pData, 1, datlen, fs);
		fclose(fs);
		fs = NULL;
		return 1;
	}
	else
	{
		return -3;
	}
}