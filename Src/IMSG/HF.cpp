//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	HF
//	版本:		1.0
//	创建日期:	2007-7-11
//	作者:		荣强
//	主要功能:	浩方游戏的处理
//	模块描述:
//	主要函数:	IsHfPacket
//				FormatHfPacket
//**************************************************************** 
#include "stdafx.h"
#include "Imsg.h"
#include "hf.h"


HfSession::HfSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_acctype = 0xF;
	m_infonum = 0;
	m_closetm = packh.m_tm + 120;

	g_GameSion++;
}

HfSession::~HfSession()
{
	g_GameSion--;
}

int HfSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	try
	{
		if ((m_infonum == -1) || (m_acctype == -1))
		{
			return 0;	// 已经处理过了
		}

		ASSERT(ProxyTCP == (TCPPacket+6));
		ASSERT(ProxyUDP == (UDPPacket+6));

		if ((packh.m_type==ProxyTCP) || (packh.m_type == ProxyUDP))
		{
			m_ProxyType = 1;
			packh.m_type -= 6;
		}

		FormatHfPacket(packh, pPacket);

		if ((m_infonum == 0xF0) && (m_acctype != 0xF))
		{
			m_OpenIndxFlg = v_openflaghf;
			m_OpenCaseFlg = v_opencasehf;
			m_OpenWarnFlg = v_openwarnhf;
			CString txt;
			txt.Format("%s________%s", m_userid,m_password);
			// 匹配案件、预警和屏蔽信息
			DWORD state = 0;
			SearchCaseOrWarn(this, txt, txt.GetLength(), state,"imsg_hf");
			SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

			// 处理IDC数据源
			CString tmpisp;
			if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), tmpisp))
			{
				if (m_dataSrc == DATASRC_OTHER)
				{
					m_dataSrc = DATASRC_IDC;
					m_strISP = tmpisp;
				}
				if (v_statIdc)
				{
					CString ttstr;
					ttstr.Format("%s\1%s\1hf\1游戏\1%s\1浩方\1%s\1\1", 
						m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
					v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttstr);
				}
			}
			else if ((m_dataSrc == DATASRC_OTHER)
				&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
			{
				m_dataSrc = DATASRC_IDC;
			}

			// 输出活动历史
			if (v_statGame)
			{
				HistoryRecord hisRec;
 				hisRec.SetRecord(IDX_FORWTYPE,	"1");
 				hisRec.SetRecord(IDX_ID,		m_userid);
				hisRec.SetRecord(IDX_USER,		m_userid);

				hisRec.SetRecord(IDX_STATE, "上线");
				WriteHisData("imsg", "imsg_hf", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
			}

			// 输出文件
			LPCTSTR stracctype;
			switch(m_acctype)
			{
			case 0:
				stracctype = _T("acctype= 对战平台号\r\n");
				break;
			case 1:
				stracctype = _T("acctype= 浩号\r\n");
				break;
			case 2:
				stracctype = _T("acctype= 盛大通行证\r\n");
				break;
			default:
				stracctype = _T("");
				break;
			}
			txt.Format("user= %s\r\npass= %s\r\n%sstat= 上线\r\n",
				m_userid, m_password, stracctype);
			m_indexfname = CreateFilename(m_sip, m_dip, "imsg_hf.indexfile.txt");
			WriteIndexFile("imsg_hf", "imsg_hf", txt, g_pImsgMissObj);
			m_CaseArr.RemoveAll();
			m_WarnArr.RemoveAll();
			m_infonum = -1;
			m_acctype = -1;
		}
		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "HfSession::InsertPacket() Error!!");
		return -3;
	}
}

//*************************************************
//	函数名称:		HfSession::FormatHfPacket
//	描述:			分析浩方的数据包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			
//	其它:			
//**************************************************
void HfSession::FormatHfPacket(DATEHEADER& packh, const char* pPacket)
{
	int direction = 0;
	if(memcmp(pPacket, "\x28\x0\x0\x0", 4) == 0)
	{
		if(memcmp(pPacket+8, "\x03\x0\x0\x0", 4) == 0)
		{
			// Server --> Client
			m_infonum |= 0x10;
		}
		else if(memcmp(pPacket+8, "\x04\x0\x0\x0", 4) == 0)
		{
			// Client --> Server
			m_infonum |= 0x20;
		}
		else	// 登陆失败
		{
			m_infonum = 0;
			m_userid.Empty();
			m_password.Empty();
			m_acctype = 0xF;
		}
	}
	else if(memcmp(pPacket, "\xb0\x01\x0\x0", 4) == 0)
	{
		// 用户登陆包 Client --> Server
		if(packh.m_datalen < 128)
			return ;

		if(memcmp(packh.m_sip, m_csip, 4) == 0)
		{
			direction = 1;
		}
		else
		{
			direction = 2;
		}
		m_infonum |= 0x40;
		m_acctype = *(DWORD*)(pPacket+20);
		if(m_acctype == 1)
		{
			m_userid.Format("%u", *(DWORD*)(pPacket+40));
		}
		else
		{
			m_userid = CString(pPacket+40);
		}
		m_password = CString(pPacket+72);
	}
	else if((memcmp(pPacket+1, "\x04\x0\x0", 3) == 0)
	 || (memcmp(pPacket+1, "\x04\x0\x0", 3) == 0) 
	 || (memcmp(pPacket+1, "\x05\x0\x0", 3) == 0) )
	{
		// 登陆回应包 Server --> Client
		if(memcmp(pPacket+8, "\0\0\0\0", 4) == 0)
		{
			m_infonum |= 0x80;
			if(memcmp(packh.m_dip, m_csip, 4) == 0)
			{
				direction = 1;
			}
			else
			{
				direction = 2;
			}
		}
	}
	if (direction == 2)
	{
		// 交换会话方向
		CString tstr = m_sip;
		m_sip = m_dip;
		m_dip = tstr;

		DWORD ip;
		memcpy(&ip, m_csip, 4);
		memcpy(m_csip, m_cdip, 4);
		memcpy(m_cdip, &ip, 4);

		tstr = m_dmac;
		m_dmac = m_smac;
		m_smac = tstr;

		m_AuthInfo.SwapAuthInfo();
	}
}

/// 浩方对战平台 /////////////////////////////////////////////////////////
//
//登陆包为1201端口, 分为四步.
//
//开始
//
//1, 客户端发   28 00 00 00 开头的握手包.
//2, 服务器回应 28 00 00 00 开头的包, 与客户端包整体只有一字节不同.
//3, 客户端发 用户名/密码 登陆包. 以b0 01 00 00开头, 
//   偏移20位后的四字节表求帐号类型02为盛大通行证, 01为浩号, 00为对战平台号.
//   偏移40字节后为登陆用户名, 偏移72字节后为密码, 密码16位的串.
//   之后为平台版本等信息.
//4, 服务器返回成功或错误. 成功以68 04 00 00开头(V4.5.8以 98 04 00 00开头), 其偏移8位后4字节衡为00 00 00 00. 
//   失败以28 00 00 00开头, 偏移8位后的4字节表示失败原因, 已知, 12 00 00 00, 为密码错误, 07 00 00 00为服务器忙.
//
//结束.
// 服务器成功包, xx 04 00 开始, xx 类似为版本, 版本变化则此位变化.
//////////////////////////////////////////////////////////////////////////
