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
#include "Vnc.h"

BOOL IsVNCPacket(DATEHEADER& packh, const char* pPacket)
{
	// RFB xxx.xxx\n	xxx为三位数字
	if ((packh.m_datalen==12) && (pPacket[11] == '\n') &&
		(memicmp(pPacket, "RFB ", 4)==0) && (pPacket[7] == '.'))
	{
		int ver1 = atoi(pPacket+4);
		int ver2 = atoi(pPacket+8);
		if (ver1>0 && ver1<50 && ver2>=0 && ver2<200)
			return TRUE;
	}
	return FALSE;
}

VncSession::VncSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 60;
	m_lastpacktm = packh.m_tm;
	memset(m_sippack, 0, sizeof(m_sippack));
	memset(m_dippack, 0, sizeof(m_dippack));
	m_sippackcount = 0;
	m_dippackcount = 0;
	m_direction = 0;
	m_bOutput = FALSE;
	g_VncSion++;
}

VncSession::~VncSession()
{
	g_VncSion--;
}

int VncSession::InsertPacket(DATEHEADER& packh, const char* pPacket)
{
	try
	{
		if (m_bOutput)
		{
			// 该会话已经处理过了
			return 0;
		}
		if (IsVNCPacket(packh, pPacket))
		{
			if (packh.m_type == ProxyTCP)
			{
				m_ProxyType = 1;
			}
			if (memcmp(packh.m_sip, m_csip, 4) == 0)
			{
				for (int i=0; i<m_sippackcount; i++)
				{
					if (m_sippack[i].m_seq == packh.m_sequence)
						return 1;	// 重发包
				}
				for (int j=0; j<m_dippackcount; j++)
				{
					if (m_dippack[j].m_seq == packh.m_ack)
					{
						m_bOutput = TRUE;
						m_direction = 2;
						break;
					}
					else if (m_dippack[j].m_seq+12 == packh.m_ack)
					{
						m_bOutput = TRUE;
						m_direction = 1;
						break;
					}
				}
				if (!m_bOutput)
				{
					m_sippack[m_sippackcount].m_seq = packh.m_sequence;
					m_sippack[m_sippackcount].m_ack = packh.m_ack;
					m_sippackcount++;
				}
			}
			else // if (memcmp(packh.sip, csip, 4) == 0)
			{
				ASSERT(memcmp(packh.m_dip, m_csip, 4) == 0);
				for (int i=0; i<m_dippackcount; i++)
				{
					if (m_dippack[i].m_seq == packh.m_sequence)
						return 1;	// 重发包
				}
				for (int j=0; j<m_sippackcount; j++)
				{
					if (m_sippack[j].m_seq == packh.m_ack)
					{
						m_bOutput = TRUE;
						m_direction = 1;
						break;
					}
					else if (m_sippack[j].m_seq+12 == packh.m_ack)
					{
						m_bOutput = TRUE;
						m_direction = 2;
						break;
					}
				}
				if (!m_bOutput)
				{
					m_dippack[m_dippackcount].m_seq = packh.m_sequence;
					m_dippack[m_dippackcount].m_ack = packh.m_ack;
					m_dippackcount++;
				}
			} // end if (memcmp(packh.sip, csip, 4) == 0)

			if (m_bOutput)
			{
				if (m_direction == 2)
				{
					// 交换方向
					CString tstr = m_sip;
					m_sip = m_dip;
					m_dip = tstr;
					
					tstr = m_dmac;
					m_dmac = m_smac;
					m_smac = tstr;

					m_AuthInfo.SwapAuthInfo();

					BYTE ip[4];
					memcpy(ip, m_csip, 4);
					memcpy(m_csip, m_cdip, 4);
					memcpy(m_cdip, ip, 4);
				}
				m_sippackcount = m_dippackcount = 0;
				m_OpenCaseFlg = v_opencasevnc;
				m_OpenIndxFlg = v_openflagvnc;
				m_OpenWarnFlg = v_openwarnvnc;

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
					str.Format("%s\1%s\1rc_vnc\1VNC服务\1%s\1远程控制\1%s\1\1", 
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
				m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u_rc_vnc.indexfile.txt",
					tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour,
					now.wMinute, now.wSecond, m_sip, m_dip, ++g_VncFileNum, rand());
				WriteIndexFile("rc_vnc", "rc_vnc", "stat= 登录\r\n", g_pFtpMissObj);
			} // end if(m_bOutput)
		} // end if(IsVNCPacket(packh, pPacket))
	}
	catch(...)
	{
		WriteLog(FTPPRO, "InsertVncPacket() Error!!");
	}
	return 1;
}

BOOL VncSession::CloseSession(time_t nowtm)
{
	m_closetm += 60;
	return abs(nowtm-m_lastpacktm>=180);
}
