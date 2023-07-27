/***************************************************************
Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
	文件名: 	FtpSession
	版本:		1.0
	创建日期:	2007-7-10 
	作者:		荣强
	主要功能:	FTP协议还原类
***************************************************************/ 
#include "stdafx.h"
#include "ftp.h"
#include "FtpSession.h"

CString ParentDir(CString pathname);

FtpSession::FtpSession(DATEHEADER& packh, int aT, BOOL bServer)
: SocketSession(packh, aT)
{
	m_ftpflag = FALSE;
	m_retrnum = 0;
	m_stornum = 0;	
	m_CSeqOffset	=0;
	m_CAckOffset	=0;
	m_lastpacktm = packh.m_tm;
	m_closetm = packh.m_tm + 180;
	
	if (bServer)
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
// 	if(packh.m_sequence > MaxSeq)
// 	{
// 		m_CSeqOffset = SeqOffset;
// 	}
// 	if(packh.m_ack > MaxSeq)
// 	{
// 		m_CAckOffset = SeqOffset;
// 	}
	g_FtpSion++;
}

FtpSession::~FtpSession()
{
	g_FtpSion--;
}

int FtpSession::InsertPacket(DATEHEADER& packh, const char* pPacket)
{
	m_lastpacktm = packh.m_tm;
	char tmppack[12];
	memset(tmppack+8, 0, 4);
	memcpy(tmppack, pPacket, 8);
	strupr(tmppack);
	if(memcmp(packh.m_sip, m_csip, 4)==0)
	{
		// 客户端的包
// 		if(m_Ssequence > ((packh.sequence - m_CSeqOffset) + packh.datalen))
// 		{
// 			return 0;
// 		}

		//ftpflg CWD CDUP STOR PASV PORT PWD
		if(!m_ftpflag)
		{
			if (memicmp(tmppack, "CWD",  3)==0 ||
				memicmp(tmppack, "CDUP", 4)==0 ||
				memicmp(tmppack, "STOR", 4)==0 ||
				memicmp(tmppack, "PASV", 4)==0 ||
				memicmp(tmppack, "PORT", 4)==0 ||
				memicmp(tmppack, "PWD",  3)==0 )
			{
				// 该会话是FTP会话
				m_ftpflag = TRUE;

				ASSERT(m_userid != "");
			}
		}
		if(	memicmp(tmppack, "CWD ", 4)==0 ||
			memicmp(tmppack, "CDUP", 4)==0 ||
			memicmp(tmppack, "PWD",  3)==0 )
		{
			if(m_ftpflag)
			{
				TcpPacket	Tcpp(packh, pPacket, m_CSeqOffset, m_CAckOffset);
				m_ClientPacketlst.push_back(Tcpp);
				Tcpp.buf=0;
			}
			return 0;				
		}

		if (memicmp(tmppack, "STOR", 4)==0 ||
			memicmp(tmppack, "RETR", 4)==0 )
		{
			if(m_ftpflag)
			{
				TcpPacket	Tcpp(packh, pPacket, m_CSeqOffset, m_CAckOffset) ;
				m_ClientPacketlst.push_back(Tcpp);
				Tcpp.buf=0;
			}
			return 0;
		}

		if(memicmp(tmppack, "USER", 4)==0)
		{
			// 用户名
			m_userid = pPacket+4;
			m_userid.TrimLeft();
			m_userid.TrimRight();
			return 0;
		}
		if(	memicmp(tmppack, "PASS", 4)==0 )
		{
			// 密码
			m_PASS=pPacket+4;
			m_PASS.TrimLeft();
			m_PASS.TrimRight();
			return 0;
		}
	}
	else
	{
		// 服务器端的包
		// PWD        257<space>"<directory-name>"<space><commentary>
		// CDUP 200 250
		// CWD  250 200
		if(	memcmp(tmppack, "257", 3)==0 ||
			memcmp(tmppack, "250", 3)==0 ||
			memcmp(tmppack, "200", 3)==0 )
		{
			if(m_ftpflag)
			{
				TcpPacket	Tcpp(packh, pPacket, m_CAckOffset, m_CSeqOffset);
				m_ServerPacketlst.push_back(Tcpp);
				Tcpp.buf=0;
				return 0;
			}
		}
	}
	return 0;
}

BOOL FtpSession::CloseSession(time_t nowtm)
{
//	BOOL ret = (abs(nowtm-this->lastpacktm)>300);
	TcpPacketListIt startit, endit,tmpit,itl;
	TcpPacketListIt Sstartit, Sendit,Stmpit,Sitl;
	
	CString filesfield;
	
	m_ClientPacketlst.sort();
	m_ServerPacketlst.sort();
	startit	= m_ClientPacketlst.begin();
	endit	= m_ClientPacketlst.end();
	itl=startit;
	Sstartit	= m_ServerPacketlst.begin();
	Sendit		= m_ServerPacketlst.end();
	Sitl=Sstartit;

	m_CurDir.Empty();
// 	if (itl!=endit)
// 	{
// 		m_begfiletm = itl->tm;	// 设置文件的截获时间
// 	}
	while (itl!=endit)
	{
		if (m_begfiletm == 0)
		{
			m_begfiletm = itl->tm;
		}
		BOOL exitflag=FALSE;
		BOOL serverokflag=FALSE;
		tmpit=itl;
		itl++;
		//查找服务器的相应代码 257 200 250 服务器的ack==客户端的seq 
		while(Sitl!=Sendit)
		{
			Stmpit=Sitl;
			if(itl->Ssequence ==Sitl->Sack)
			{
				serverokflag=TRUE;
				break;
			}
			if(itl->Ssequence < Sitl->Sack)
			{
				serverokflag=FALSE;
				break;
			}
			Sitl++;		
			m_ServerPacketlst.erase(Stmpit);
		}

		//解析当前目录
		if (memicmp(tmpit->buf, "CWD ", 4)==0 )
		{
			if(serverokflag && itl->Ssequence ==Stmpit->Sack 
				&& (memcmp(Stmpit->buf, "200", 3)==0 ||
				memcmp(Stmpit->buf, "250", 3)==0 ))
			{
				//进入目录
				m_CurDir=this->ProcessCMD(tmpit->buf);
				
			}
		}	
		
		if (memicmp(tmpit->buf, "CDUP", 4)==0 )
		{
			if(serverokflag &&  itl->Ssequence ==Stmpit->Sack	
				&& (memcmp(Stmpit->buf, "200", 3)==0 ||
				memcmp(Stmpit->buf, "250", 3)==0 ))
			{
				//父目录
				m_CurDir = ParentDir(m_CurDir);
			}
		}
		
		if (memicmp(tmpit->buf, "PWD", 3)==0 )
		{
			if(serverokflag &&  itl->Ssequence ==Stmpit->Sack
				&& memcmp(Stmpit->buf,"257",3)==0 )
			{
				m_CurDir = Process257(Stmpit->buf);	
			}
		}

		if (memicmp(tmpit->buf, "RETR", 4)==0 )	// 下载
		{
			AddFtpFile(m_retrnum, filesfield, tmpit->buf, m_lastdnFile);
		}
		if (memicmp(tmpit->buf, "STOR", 4)==0 )	// 上传
		{
			AddFtpFile(m_stornum, filesfield, tmpit->buf, m_lastupFile);
		}
		
		m_ClientPacketlst.erase(tmpit);
		
	}
	
	if (((m_retrnum || m_stornum ) && (!filesfield.IsEmpty()))
		&& (abs(nowtm - m_begfiletm) >= 180) && (m_begfiletm!=0))
	{
		m_OpenIndxFlg = v_openflagftp;
		m_OpenCaseFlg = v_opencaseftp;
		m_OpenWarnFlg = v_openwarnftp;

		CString text;	// 需要匹配案件和预警的串
		text.Format("____%s\r\n____%s\r\n____%s", 
			m_userid, m_PASS, filesfield);
		// 匹配案件和预警
		DWORD d_state = 0;
		SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), d_state);
		text.Empty();

		// 处理IDC数据
		CString strISP;
		if (v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), m_strISP))
		{
			CString str;
			if (this->m_stornum > 0)
			{
				// SIP\1SUBPRO\11PRO\1PORT\1ClassType\1DOMAIN 
				str.Format("%s\1%s\1ftpup\1FTP服务\1%s\1FTP\1%s\1\1", 
					m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[FTP_PROTOCOL].AddServer(str, m_stornum);
			}
			if (this->m_retrnum > 0)
			{
				str.Format("%s\1%s\1ftpdown\1FTP服务\1%s\1FTP\1%s\1\1", 
					m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[FTP_PROTOCOL].AddServer(str, m_retrnum);
			}
		}

		// 写活动历史
		if( v_stat && v_statOther && 
			(m_stornum > 0 || (m_retrnum > 0 && m_userid.CompareNoCase("anonymous") != 0)))
		{
			HistoryRecord hisRec;
			CString tmpstrhis;

 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		m_userid);
 			hisRec.SetRecord(IDX_USER,		m_userid);
 			hisRec.SetRecord(IDX_PASS,		m_PASS);
			if(m_stornum > 0)
				hisRec.SetRecord(IDX_STATE, "上传");
			else 
				hisRec.SetRecord(IDX_STATE, "下载");
			WriteHisData("ftp", "ftp", &hisRec, g_pFtpMissObj, FTP_PROTOCOL);//, HISTORY_DATA);
		}
		// 写对照文件并输出
		if (!m_userid.IsEmpty())
		{
			text.Format("user= %s\r\n", m_userid);
		}
		if (!m_PASS.IsEmpty())
		{
			text.Format("%spass= %s\r\n", (CString)text, m_PASS);
		}
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmpdir = g_pFtpMissObj->GetTmpPath();
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_ftp.indexfile.txt",
			tmpdir, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_FtpFileNum);

		WriteIndexFile("ftp", "ftp", text+filesfield, g_pFtpMissObj);
		m_stornum=0;
		m_retrnum = 0;
		m_begfiletm = 0;
		m_CaseArr.RemoveAll();
		m_WarnArr.RemoveAll();
	}

	// 清理服务器返回的包的队列
	Stmpit		= m_ServerPacketlst.begin();
	Sendit		= m_ServerPacketlst.end();
	Sitl=Stmpit;
	time_t oldtm = m_closetm - 90;
	while(Sitl!=Sendit)
	{
		Stmpit=Sitl;
		Sitl++;		
		if(itl->tm < oldtm)
		{
			m_ServerPacketlst.erase(Stmpit);
		}
		else
		{
			break;
		}
	}

	if (abs(nowtm - m_lastpacktm) >= 300)
	{
		return TRUE;
	}
	else
	{
		m_closetm += 60;
		return FALSE;
	}
}


CString ParentDir(CString pathname)
{
	int pos=0;
	pos=pathname.ReverseFind('/');
	if(pos>0)
		return pathname.Left(pos);
	else
		return "/";
}

CString FtpSession::ProcessCMD(CString cmd)
{
	cmd = cmd.Mid(4);
	cmd.TrimLeft();
	cmd.TrimRight();
	if(cmd == ".")
	{
		return m_CurDir;
	}
	else if(cmd == "..")
	{
		return ParentDir(m_CurDir);
	}
	else if(cmd.GetAt(0) == '/')
	{
		return cmd;
	}
	else
	{
		cmd=m_CurDir + CString("/") + cmd;
		cmd.Replace("//", "/");
		return cmd;
	}
}

CString  FtpSession::Process257(CString cmd)
{
	int pos=0;
	pos=cmd.Find("\"");
	if(pos>0)
	{
		cmd=cmd.Mid(pos+1);
		pos=cmd.ReverseFind('"');//双引号
		if (pos>0)
		{
			cmd=cmd.Left(pos);
			cmd.Replace("\"\"","\"");
			return cmd;
		}
		else
			return m_CurDir;
	}
	else
	{
		return m_CurDir;
	}
}

void FtpSession::AddFtpFile(int &num , CString &field ,const char * buf, CString &lastFile)
{
	CString line;
	CString file;
	if (strlen(buf) >= 8)
	{
		file = buf;
		file = file.Mid(4);
		file.TrimLeft();
		file.TrimRight();
		if(file.GetAt(0) != '/')
		{
			file = m_CurDir + CString("/") + file;
		}
		file.Replace("//","/");
		/// file 去%, 转gb2312
		ConverHttp2Html(file);
		ConvertUtf8toGbk(file, file.GetLength(), file);
		
		if(0 != lastFile.Compare(file))
		{
			if (memicmp(buf, "RETR", 4)==0 )
			{
				line.Format("下载%d= %s\r\n", ++num, file);
				field += line;
			}
			else if (memicmp(buf, "STOR", 4)==0 )
			{
				line.Format("上传%d= %s\r\n", ++num, file);
				field += line;
			}
			lastFile = file;
		}
	}
}
