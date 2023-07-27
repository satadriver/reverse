//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	Vnc
//	版本:		1.0
//	创建日期:	2007-7-28
//	作者:		荣强
//	主要功能:	VNC协议还原
//	模块描述:
//	主要函数:	
//**************************************************************** 
#include "stdafx.h"
#include "ftp.h"
#include "terminal.h"

// Search	Searchrdpdr("rdpdr");
// Search	Searchrdpsnd("rdpsnd");
// Search	Searchcliprdr("cliprdr");

BOOL IsTerminalPacket(DATEHEADER& packh, const char* pPacket)
{
	// RFB xxx.xxx\n	xxx为三位数字
	if (packh.m_datalen > 0x20
		&& (memcmp(pPacket, "\x03\x00", 2)==0 )
		/*&& (pPacket[5]==0xe0 )*/
		&& memcmp(pPacket+6,"\x00\x00\x00\x00",4)==0) //连接包
	{
		 
		if(memcmp(pPacket+11,"Cookie: ",8)==0)
		{
			return TRUE;
		}		
	
	}
	else if(packh.m_datalen == 8
		&& (memcmp(pPacket, "\x03\x00", 2)==0 )
		/*&& (pPacket[5]==0xe0 )*/
		&& memcmp(pPacket+4,"\x02\xf0\x80\x28",4)==0)
	{
			return TRUE;
	}
	return FALSE;
}

TerminalSession::TerminalSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 60;
	m_lastpacktm = packh.m_tm;
	m_sippackcount = 0;
	m_dippackcount = 0;
	m_direction = 0;
	m_bOutput = FALSE;
	g_TerminalSion++;
	
}

TerminalSession::~TerminalSession()
{
	g_TerminalSion--;
}

int TerminalSession::InsertPacket(DATEHEADER& packh, const char* pPacket)
{
	try
	{

		if ( packh.m_datalen>0x20 && memcmp(pPacket, "\x03\x00", 2)==0 &&
		  memcmp(pPacket+11,"Cookie: ",8)==0) //连接包
		{
			 
				m_userid=CString(pPacket+0x1c,packh.m_datalen-0x1c-2);
		 		
		}


		m_sippackcount = m_dippackcount = 0;
//		m_OpenCaseFlg = v_opencasevnc;
		m_OpenIndxFlg = v_openflagterminal;
//		m_OpenWarnFlg = v_openwarnvnc;

		// 匹配案件和预警
		DWORD d_state = 0;
		SearchCaseOrWarn(this, NULL, 0, d_state);

		// 处理IDC
		CString isp;
		if(v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), isp))
		{
			if (m_dataSrc == DATASRC_OTHER)
			{
				m_dataSrc = DATASRC_IDC;
				m_strISP = isp;
			}
			// 输出IDCServer
			// SIP\1SUBPRO\11PRO\1PORT\1ClassType\1DOMAIN 
			CString str;
			str.Format("%s\1%s\1rc_Terminal\1Terminal服务\1%s\1远程控制\1%s\1\1", 
				m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[FTP_PROTOCOL].AddServer(str);
		}
		else if ((m_dataSrc == DATASRC_OTHER)
			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), isp)))
		{
			m_dataSrc = DATASRC_IDC;
			m_strISP = isp;
		}

		// 写活动历史(该协议不输出活动历史)

		// 写对照文件
		CString tmpdir = g_pFtpMissObj->GetTmpPath();
		SYSTEMTIME now;
		GetLocalTime(&now);
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u_rc_terminal.indexfile.txt",
			tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, m_sip, m_dip, ++g_VncFileNum, rand());


		CString str;
		//应该写user，为适应查询的情况，user userid都写上
		str.Format("user=%s\r\nuserid=%s\r\nstat= 登录\r\n",m_userid,m_userid);
		WriteIndexFile("rc_terminal", "rc_terminal", str, g_pFtpMissObj);
	}
	catch(...)
	{
		WriteLog(FTPPRO, "InsertTerminalPacket() Error!!");
	}
	return 1;
}

BOOL TerminalSession::CloseSession(time_t nowtm)
{
	m_closetm += 60;
	return abs(nowtm-m_lastpacktm>=180);
}
