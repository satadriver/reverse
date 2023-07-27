//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	Bf
//	版本:		1.0
//	创建日期:	2007-7-11
//	作者:		荣强
//	主要功能:	处理边锋协议
//	模块描述:	
//	主要函数:	IsBfPacket
//				BfSession::InsertPacket
//**************************************************************** 

#include "stdafx.h"
#include "imsg.h"
#include "bf.h"


BfSession::BfSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	
	g_GameSion++;
}

BfSession::~BfSession()
{
	g_GameSion--;
}

//*************************************************
//	函数名称:		BfSession::InsertPacket
//	描述:			处理一个会话的一个包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			
//	其它:			
//**************************************************
int BfSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	try
	{
		if (!IsBfPacket(packh, pPacket))
		{
			// 不是边锋的包
			return 0;
		}

		if ((packh.m_type == ProxyTCP) || (packh.m_type == ProxyUDP))
		{
			m_ProxyType = 1;
		}

		// 处理数据包
		CString struid;			// userid
		CString strcompid;		// compid
		CString strNik;			// nik
		BOOL isPass = FALSE;	// 
		static LPCTSTR minpwd = "                    ";
		static LPCTSTR maxpwd = "\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e\x7e";
		int offset = pPacket[14]+14+1+1;
		const char *pwd=pPacket+offset;
		if ((memcmp(pwd, minpwd, 20) > 0)
			&& (memcmp(pwd, maxpwd, 20) < 0))
		{
			offset += 20;
			if (pPacket[offset]==0)
			{
				// Client --> Server
				offset += 16+6;
				if ((pPacket[offset]+offset+1) == packh.m_datalen)
				{
					if (pPacket[offset]>0)
					{
						int computeridlen = min(pPacket[offset], 24);
						if ((memcmp(pPacket+offset+1, minpwd, computeridlen) > 0)
							&& (memcmp(pPacket+offset+1, maxpwd, computeridlen) < 0))
						{
							strcompid = pPacket+offset+1;
						}
					}
				}
				else
				{
					// 非法包
					return 0;
				}
			}
			else
			{
				// Server --> Client
				int d = offset+(BYTE)pPacket[offset]+1+16+5;
				if (d+(BYTE)pPacket[d]+1 == packh.m_datalen)
				{
					if (pPacket[d]>0)
					{
						int computeridlen = min(pPacket[d], 24);
						if ((memcmp(pPacket+d+1, minpwd, computeridlen) > 0)
							&& (memcmp(pPacket+d+1, maxpwd, computeridlen) < 0))
						{
							strcompid = pPacket+d+1;
						}
					}
				}
				else
				{
					// 非法包
					return 0;
				}
				strNik = CString(pPacket+offset+1, (BYTE)pPacket[offset]);
				isPass = TRUE;	// 登陆成功
				if (memcmp(packh.m_sip, m_csip, 4) == 0)
				{
					// 交换IP方向
					CString tstr = m_sip;
					m_sip = m_dip;
					m_dip = tstr;
					tstr = m_dmac;
					m_dmac = m_smac;
					m_smac = tstr;
					DWORD ip;
					memcpy(&ip, m_csip, 4);
					memcpy(m_csip, m_cdip, 4);
					memcpy(m_cdip, &ip, 4);
					m_AuthInfo.SwapAuthInfo();
				}
			}
			struid = CString(pPacket+15, pPacket[14]);
		}

		if (isPass)
		{
			if (!strNik.IsEmpty())
			{
				m_userid.Format("%s [昵称:%s]", struid, strNik);
			}
			else
			{
				m_userid = struid;
			}

			m_begfiletm = packh.m_tm;
			m_OpenCaseFlg = v_opencasebf;
			m_OpenWarnFlg = v_openwarnbf;
			m_OpenIndxFlg = v_openflagbf;

			// 匹配案件、预警和屏蔽信息
			DWORD state = 0;
			SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
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
					ttstr.Format("%s\1%s\1bf\1游戏%s\1边锋\1%s\1\1",
						m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
					v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttstr);
				}
			}
			else if ((m_dataSrc == DATASRC_OTHER)
				&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), tmpisp)))
			{
				m_dataSrc = DATASRC_IDC;
				m_strISP = tmpisp;
			}

			// 输出活动历史
 			if (v_statGame)
			{
				HistoryRecord hisRec;
 				hisRec.SetRecord(IDX_FORWTYPE,	"1");
 				hisRec.SetRecord(IDX_ID,		m_userid);
				 hisRec.SetRecord(IDX_USER,		m_userid);

				hisRec.SetRecord(IDX_STATE, "上线");
				WriteHisData("imsg_bf", "imsg_bf", &hisRec, g_pImsgMissObj,	IMSG_PROTOCOL);//, HISTORY_DATA);
			}

			// 写对照文件
			if ((m_CaseArr.GetSize() > 0) || (m_WarnArr.GetSize() > 0)
				|| (!m_rubbishflag && m_OpenIndxFlg))
			{
				SYSTEMTIME now;
				GetLocalTime(&now);
				LPCTSTR subpro;
				CString tmppath = g_pImsgMissObj->GetTmpPath();
				ASSERT(!tmppath.IsEmpty());
				m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_1_%u_imsg_bf.indexfile.txt",
					tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
					now.wMinute, now.wSecond, m_sip, m_dip, rand(), subpro);
				CString text;
				text.Format("user= %s\r\nstat= 上线\r\n", m_userid);
				WriteIndexFile("imsg_bf", "imsg_bf", text, g_pImsgMissObj);
				m_CaseArr.RemoveAll();
				m_WarnArr.RemoveAll();
			}
		} // end if(isPass)
		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "BfSession::InsertPacket() Error!!");
		return -3;	
	}
}


/********************** 边锋示例包 ***********************/
/**********************************************************
登陆1		userid: rongforbf  password: 111222   (登陆成功)
Client-->Server(port:4310)

0030                     01 00 00 00 01 00 00 00 02 00  C...............
0040   01 00 00 00 09 72 6f 6e 67 66 6f 72 62 66 14 5b  .....rongforbf.[
0050   5b 5b 51 51 51 3b 3b 60 34 34 40 33 32 6a 3e 4a  [[QQQ;;`44@32j>J
0060   3a 5c 38 00 00 00 0e 00 00 00 01 00 00 00 00 00  :\8.............
0070   1f 00 00 00 00 00 00 00 00 14 20 20 20 20 20 20  ..........      
0080   20 20 4e 4e 31 57 54 34 43 31 39 4e 54 48          NN1WT4C19NTH

Server-->Client

0030                     01 00 00 00 01 00 00 00 02 00  C.j.............
0040   01 00 00 00 09 72 6f 6e 67 66 6f 72 62 66 14 5b  .....rongforbf.[
0050   5b 5b 51 51 51 3b 3b 60 34 34 40 33 32 6a 3e 4a  [[QQQ;;`44@32j>J
0060   3a 5c 38 04 72 6f 6e 67 94 00 0e 00 00 00 01 00  :\8.rong........
0070   00 00 00 00 1f 00 00 00 00 00 00 00 00 14 20 20  ..............  
0080   20 20 20 20 20 20 4e 4e 31 57 54 34 43 31 39 4e        NN1WT4C19N
0090   54 48                                            TH

登陆2		userid: rongforbf  password: 123456
Client-->Server

0030                     01 00 00 00 01 00 00 00 02 00  C..U............
0040   01 00 00 00 09 72 6f 6e 67 66 6f 72 62 66 14 46  .....rongforbf.F
0050   4c 46 5d 4f 67 4b 4b 54 63 5b 59 39 76 3f 6d 62  LF]OgKKTc[Y9v?mb
0060   5a 39 3e 00 00 00 0e 00 00 00 01 00 00 00 00 00  Z9>.............
0070   1f 00 00 00 00 00 00 00 00 14 20 20 20 20 20 20  ..........      
0080   20 20 4e 4e 31 57 54 34 43 31 39 4e 54 48          NN1WT4C19NTH

Server-->Client

0030                     01 00 00 00 01 00 00 00 02 00  C..U............
0040   01 00 08 00 09 72 6f 6e 67 66 6f 72 62 66 14 46  .....rongforbf.F
0050   4c 46 5d 4f 67 4b 4b 54 63 5b 59 39 76 3f 6d 62  LF]OgKKTc[Y9v?mb
0060   5a 39 3e 00 00 00 0e 00 00 00 01 00 00 00 00 00  Z9>.............
0070   1f 00 00 00 00 00 00 00 00 14 20 20 20 20 20 20  ..........      
0080   20 20 4e 4e 31 57 54 34 43 31 39 4e 54 48          NN1WT4C19NTH

登陆3		userid: abcdefg   password:  111222
Client--Servier
0000   00 0f 3d a0 8a c0 00 90 f5 31 08 5f 08 00 45 00  ..=......1._..E.
0010   00 7e d1 ff 40 00 80 06 53 bd c0 a8 00 a6 3d b6  .~..@...S.....=.
0020   d5 b8 09 3c 10 d6 3a 4a f3 3f 75 59 af ef 50 18  ...<..:J.?uY..P.
0030   43 e0 72 b9 00 00 01 00 00 00 01 00 00 00 02 00  C.r.............
0040   01 00 00 00 07 61 62 63 64 65 66 67 14 78 78 78  .....abcdefg.xxx
0050   32 32 32 5c 5c 33 37 37 61 66 66 3d 41 6b 5b 7a  222\\377aff=Ak[z
0060   3b 00 00 00 0e 00 00 00 01 00 00 00 00 00 25 00  ;.............%.
0070   00 00 00 00 00 00 00 14 20 20 20 20 20 20 20 20  ........        
0080   4e 4e 31 57 54 34 43 31 39 4e 54 48              NN1WT4C19NTH

Server--Client
0000   00 0f 3d a0 8a c0 00 90 f5 31 08 5f 08 00 45 00  ..=......1._..E.
0010   00 7e d1 ff 40 00 80 06 53 bd c0 a8 00 a6 3d b6  .~..@...S.....=.
0020   d5 b8 09 3c 10 d6 3a 4a f3 3f 75 59 af ef 50 18  ...<..:J.?uY..P.
0030   43 e0 72 b9 00 00 01 00 00 00 01 00 00 00 02 00  C.r.............
0040   01 00 00 00 07 61 62 63 64 65 66 67 14 78 78 78  .....abcdefg.xxx
0050   32 32 32 5c 5c 33 37 37 61 66 66 3d 41 6b 5b 7a  222\\377aff=Ak[z
0060   3b 00 00 00 0e 00 00 00 01 00 00 00 00 00 25 00  ;.............%.
0070   00 00 00 00 00 00 00 14 20 20 20 20 20 20 20 20  ........        
0080   4e 4e 31 57 54 34 43 31 39 4e 54 48              NN1WT4C19NTH

登陆时前8个字节固定01 00 00 00 01 00 00 00
第15个字节是用户名的长度(大于0小于24)后面是用户名
之后是密码长度(固定20B)
登陆成功后密码后跟随昵称否则返回原登陆的包

0030                     23 00 cc 00 45 3b a2 f3 aa 8c  B4.K..#...E;....
0040   f6 63 26 3e 2c 8c 93 87 91 f5 4f a0 52 cc e3 6b  .c&>,.....O.R..k
0050   e9 80 b0 6c 9f 96 6c 52 28                       ...l..lR(


0030                     23 00 cc 00 87 6e dd 66 12 40  B.U...#....n.f.@
0040   12 72 e6 eb 01 be be 96 b8 0a 13 99 7e 82 cf 9d  .r..........~...
0050   9c aa 99 aa aa 80 e8 5a 83                       .......Z.
**************************************************************/



















