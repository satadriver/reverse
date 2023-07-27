//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	Soqq
//	版本:		1.0
//	创建日期:	2007-7-12
//	作者:		荣强
//	主要功能:	Soqq即时消息还原
//	模块描述:	
//	主要函数:	SoqqSession::FormatSoqqPacket
//**************************************************************** 
#include "stdafx.h"
#include "imsg.h"
#include "Soqq.h"


SoqqSession::SoqqSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	g_ImsgSion++;
	m_direction = 0;
	m_closetm = packh.m_tm + 180;
}

SoqqSession::~SoqqSession()
{
	g_ImsgSion--;
}

int SoqqSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	try
	{
		m_lastpacktm = packh.m_tm;
		if (packh.m_type==ProxyTCP)
		{
			m_ProxyType = 1;
			packh.m_type = TCPPacket;
		}

		if (!IsSoQQPacket(packh, pPacket))
		{
			return 0;		// 这个包不是需要的包
		}

		// 简单查找重复的包
		if (SearchSeq(m_Seq, packh.m_sequence))
		{
			return 0;		// 找到重复的包就返回
		}


		FormatSoqqPacket(packh, pPacket);
		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "SoqqSession::InsertPacket() Error!!");
		return -3;
	}
}

//*************************************************
//	函数名称:		SoqqSession::FormatSoqqPacket
//	描述:			还原单个包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			
//	其它:			
//**************************************************
void SoqqSession::FormatSoqqPacket(DATEHEADER& packh, const char* pPacket)
{
	try
	{
		CString text;
		DWORD uid=0;
		DWORD len = 0;
		int direction = 1;
		switch (pPacket[1])
		{
		case 1:		// id与邮箱转换
			uid = *((DWORD*)(pPacket+8));
			if(uid != 0)
			{
				return;
			}
			
			len = Swap4Byte(*((DWORD*)(pPacket+12)));
			if (len < 100)
			{
				if ((len+12) < packh.m_datalen)
				{
					m_SoqqEml = CString(pPacket+16, len);
				}
			}
			else
			{
				// Server --> Client
				direction = (memcmp(packh.m_sip, m_csip, 4)==0)? 2 : 1;
				m_userid.Format("%u", len);
			}
			if (!m_userid.IsEmpty() && !m_SoqqEml.IsEmpty())
			{
				text.Format("通过邮箱[%s]获取搜QID: %s", m_SoqqEml, m_userid);
				m_closetm = 10;
				OutputFile(packh.m_tm, text, direction);
			}
			else
			{
				return;
			}
			break;

		case 2:		// 用户上线
			if ((*((DWORD*)(pPacket+8)) == 0) && (*((DWORD*)(pPacket+12)) != 0))
			{
				// Server --> Client
				direction = (memcmp(packh.m_sip, m_csip, 4)==0)? 2 : 1;

				if (*((WORD*)(pPacket+6)) > 0x11)
				{
					// 上线成功
					uid = *((DWORD*)(pPacket + 12));
					m_userid.Format("%u", Swap4Byte(uid));
					text = _T("上线");
					m_closetm = 30;
					OutputFile(packh.m_tm, text, direction);
				}
				else
				{
					return;
				}
			}
			else
			{
				return;
			}
			break;
		case 3:		// 下线
			if ((*((DWORD*)(pPacket+8)) != 0) && (*((DWORD*)(pPacket+12)) == 0))
			{
				// Client --> Server
				direction = (memcmp(packh.m_sip, m_csip, 4)==0)? 1 : 2;

				uid = *((DWORD*)(pPacket+8));
				m_userid.Format("%u", Swap4Byte(uid));
				text = _T("下线");
				m_closetm = 20;
				OutputFile(packh.m_tm, text, direction);
			}
			else
			{
				return;
			}
			break;
		case 4:		// 心跳(确定客户端)
			return;
			break;
		case 6:		// 聊天
			if (*((DWORD*)(pPacket+8))!=0 && *((DWORD*)(pPacket+12))!=0)
			{
				// Client --> Server
				direction = (memcmp(packh.m_sip, m_csip, 4)==0)? 1 : 2;

				char strbuf[20];
				if (m_direction==1)
				{
					if (m_userid.IsEmpty())
					{
						uid = Swap4Byte(*((DWORD*)(pPacket+8)));
						m_userid.Format("%u", uid);
					}
					uid = Swap4Byte(*((DWORD*)(pPacket+12)));
					sprintf(strbuf, "%u", uid);
					
					if(m_Colloquist.IsEmpty())
					{
						m_Colloquist = strbuf;
					}
					else if (m_Colloquist.Find(strbuf)==-1)
					{
						m_Colloquist.Format("%s, %s", (CString)m_Colloquist, uid);
					}
					else
					{
						return;
					}
				}
				else
				{
					if (m_userid.IsEmpty())
					{
						uid = Swap4Byte(*((DWORD*)(pPacket+12)));
						m_userid.Format("%u", uid);
					}
					uid = Swap4Byte(*((DWORD*)(pPacket+8)));
					sprintf(strbuf, "%u", uid);
					if(m_Colloquist.IsEmpty())
					{
						m_Colloquist = strbuf;
					}
					else if (m_Colloquist.Find(strbuf)==-1)
					{
						m_Colloquist.Format("%s, %s", (CString)m_Colloquist, uid);
					}
					else
					{
						return;
					}
				}
				text = _T("通讯");
				OutputFile(packh.m_tm, text, direction);
			}
			else
			{
				return;
			}
			break;
		default:
			return;
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "FormatSoqqPacket() Error!!");
	}
}

//*************************************************
//	函数名称:		OutputFile
//	描述:			输出文件
//	输入参数:		optm 时间
//					optxt 是动作文本(即对照文件中的状态)
//					direction 是会话的方向
//	输出参数:		
//	返回值:			
//	其它:			
//**************************************************
void SoqqSession::OutputFile(time_t optm, LPCTSTR optxt, int direction)
{
	m_begfiletm = optm;
	m_endfiletm = 0;
	if (direction == 2)
	{
		// 交换会话方向
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

	// 处理IDC数据源
	CString str;
	if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), str))
	{
		if (m_dataSrc == DATASRC_OTHER)
		{
			m_dataSrc = DATASRC_IDC;
			m_strISP = str;
		}
		if (v_statIdc)
		{
			CString ttt;
			ttt.Format("%s\1%s\1soqq\1即时消息\1%s\1SOQ\1%s\1\1",
				m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttt);
		}
	}
	else if ((m_dataSrc == DATASRC_OTHER) &&
		(v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
	{
		m_dataSrc = DATASRC_IDC;
	}
		 
	// 输出活动历史
	if (v_statImsgOther)
	{
		HistoryRecord hisRec;
 		hisRec.SetRecord(IDX_FORWTYPE,	"1");
 		hisRec.SetRecord(IDX_ID,		m_userid);
		hisRec.SetRecord(IDX_USER,		m_userid);

		hisRec.SetRecord(IDX_CONTACT,	m_Colloquist);
		if (!m_SoqqEml.IsEmpty())
		{
			hisRec.SetRecord(IDX_USER,		m_SoqqEml);
			hisRec.SetRecord(IDX_ID,		m_SoqqEml);
			hisRec.SetRecord(IDX_STATE,		"获取ID");
		}
		else
		{
			hisRec.SetRecord(IDX_USER,		m_userid);
			hisRec.SetRecord(IDX_STATE,		optxt);
		}
		WriteHisData("imsg", "imsg_soqq", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
	}

	// 匹配案件、预警和屏蔽信息
	m_OpenWarnFlg = v_openwarnSoQ;
	m_OpenCaseFlg = v_opencaseSoQ;
	m_OpenIndxFlg = v_openflagSoQ;
	str.Format("%s\r\n____%s\r\n____%s\r\n", m_userid,m_SoqqEml, m_Colloquist);
	DWORD dwstate = 0;
	SearchCaseOrWarn(this, str, str.GetLength(), dwstate);
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

	// 输出文件
	str.Format("user= %s\r\n", m_userid);
	if (!m_Colloquist.IsEmpty())
	{
		str.Format("%scontentid= %s\r\n", (CString)str, m_Colloquist);
		m_Colloquist.Empty();
	}
	if (!m_SoqqEml.IsEmpty())
	{
		str.Format("%semilid= %s\r\n", (CString)str, m_SoqqEml);
		m_SoqqEml.Empty();
	}
	str.Format("%sstat= %s\r\n", (CString)str, optxt);
	m_indexfname = CreateFilename(m_sip, m_dip, "imsg_soqq.indexfile.txt");
	ASSERT(!m_indexfname.IsEmpty());
	WriteIndexFile("imsg_soqq", "imsg_soqq", str, g_pImsgMissObj);
	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
	return;
}



/************************* Soqq **************************/
/**********************************************************
TCP 端口不定 上下线包以12 xx 00 01开始, 初次登陆有将邮箱号转换为SoQ号的步骤.
	xx取值, 01登记(邮箱转换成ID) 02登陆, 03离线, 其它未知.

coolstar1980@sohu.com 登陆SoQ<2.2.2.105 --> 服务器>
0000   00 e0 4c 55 a0 1b 00 e0 4c 00 27 90 08 00 45 00  ..LU....L.'...E.
0010   00 7d 2a cd 40 00 80 06 f7 01 02 02 02 69 3d 87  .}*.@........i=.
0020   96 ba 05 7c 0d ad 86 27 d2 22 8c a6 a9 ee 50 18  ...|...'."....P.
0030   44 10 e1 d1 00 00
			 12 01 00 01 00 00<00 55>00 00  D............U..	0055代表片断长度
0040   00 00 00 00 00<15 63 6f 6f 6c 73 74 61 7 31 39  ......coolstar19
0050   38 30 40 73 6f 68 75 2e 63 6f 6d>0c bf 24 f5 e4  80@sohu.com..$..
0060   c5 12 69 8c 6b b0 15 a8 b7 10 63 83 56 70 ac 50  ..i.k.....c.Vp.P
0070   f9 fa 15 b4 ca 23 87 e3 b1 e7 80 de 20 f2 aa 08  .....#...... ...
0080   2e 79 e0 1a 5d 98 66 6d 6e 5a d3                 .y..].fmnZ.

00 5f 19 6d 为 coolstar1980@sohu.com 对应的SoQ号: 6232429, 服务器返回
0000   00 e0 4c 00 27 90 00 e0 4c 55 a0 1b 08 00 45 00  ..L.'...LU....E.
0010   00 49 35 e9 40 00 33 06 39 1a 3d 87 96 ba 02 02  .I5.@.3.9.=.....
0020   02 69 0d ad 05 7c 8c a6 a9 ee 86 27 d2 77 50 18  .i...|.....'.wP.
0030   16 d0 bf 64 00 00
			 12 01 00 01 00 00 00 21 00 00  ...d.........!..
0040   00 00<00 5f 19 6d>00 cc 18 cb fe 01 4e b5 13 45  ..._.m......N..E
0050   62 6c 15 23 74 5a cd                             bl.#tZ.

以SoQ号6232429登陆, <2.2.2.105 --> 服务器>
0000   00 e0 4c 55 a0 1b 00 e0 4c 00 27 90 08 00 45 00  ..LU....L.'...E.
0010   00 60 2a d3 40 00 80 06 f7 10 02 02 02 69 3d 87  .`*.@........i=.
0020   96 c2 05 7d 0d ae 83 7e bb 6a 8b 70 f9 bd 50 18  ...}...~.j.p..P.
0030   44 10 4f 38 00 00
			 12 02 00 01 00 00 00 38<00 5f  D.O8.........8._
0040   19 6d>00 00 00 00 97 a0 30 50 25 b0 0d fa 98 06  .m......0P%.....
0050   53 00 c1 dc 3a f5 14 0b 28 42 68 36 9b 7f 19 45  S...:...(Bh6...E
0060   a4 84 64 f9 69 f6 0a 96 cf 88 dd a8 d8 55        ..d.i........U

登陆成功, 服务器返回
0000   00 e0 4c 00 27 90 00 e0 4c 55 a0 1b 08 00 45 00  ..L.'...LU....E.
0010   00 81 ac bc 40 00 33 06 c2 06 3d 87 96 c2 02 02  ....@.3...=.....
0020   02 69 0d ae 05 7d 8b 70 f9 bd 83 7e bb a2 50 18  .i...}.p...~..P.
0030   16 d0 02 06 00 00
			 12 02 00 01 00 00 00 59 00 00  .............Y..
0040   00 00<00 5f 19 6d>00 db 21 e8 b6 91 03 2b d0 cb  ..._.m..!....+..
0050   29 69 73 e3 3f a5 2b b6 3a 8c 64 6e ea 8d 43 6e  )is.?.+.:.dn..Cn
0060   cf 46 17 58 d8 54 cf 11 ae 7e a7 a3 ff fc 3b 02  .F.X.T...~....;.
0070   27 f7 51 a3 4e 69 01 e5 6d 13 29 96 fd 45 3e 4f  '.Q.Ni..m.)..E>O
0080   19 ca bc 94 01 0d b5 52 53 ab 99 e3 e3 bf 1e     .......RS......

后续上传包(怀疑与用户好友相关)
0000   00 e0 4c 55 a0 1b 00 e0 4c 00 27 90 08 00 45 00  ..LU....L.'...E.
0010   00 38 2a d6 40 00 80 06 f7 35 02 02 02 69 3d 87  .8*.@....5...i=.
0020   96 c2 05 7d 0d ae 83 7e bb a2 8b 70 fa 16 50 18  ...}...~...p..P.
0030   43 b7 8f 9c 00 00
			 12 04 00 01 00 00 00 10 00 5f  C.............._
0040   19 6d 00 00 00 00                                .m....

0000   00 e0 4c 55 a0 1b 00 e0 4c 00 27 90 08 00 45 00  ..LU....L.'...E.
0010   01 48 2a d7 40 00 80 06 f6 24 02 02 02 69 3d 87  .H*.@....$...i=.
0020   96 c2 05 7d 0d ae 83 7e bb b2 8b 70 fa 16 50 18  ...}...~...p..P.
0030   43 b7 34 50 00 00
			 12 05 00 01 00 00 00 20<00 5f  C.4P......... ._
0040   19 6d>00 00 00 00 b9 22 57 1f d7 a5 ee 86 0d b1  .m....."W.......
0050   ac ab 7b 40 7b 30 12 05 00 01 00 00 00 20 00 5f  ..{@{0....... ._
0060   19 6d 00 00 00 00 90 eb a9 3f a4 eb d5 a4 90 f3  .m.......?......
0070   36 65 df 1b 5a 16 12 05 00 01 00 00 00 20 00 5f  6e..Z........ ._
0080   19 6d 00 00 00 00 2b b7 ec 9f 0f ac 59 da be 0f  .m....+.....Y...
0090   5b 07 d7 c8 c0 ff 12 05 00 01 00 00 00 20 00 5f  [............ ._
00a0   19 6d 00 00 00 00 74 1f c8 7d bd 8b 20 73 36 85  .m....t..}.. s6.
00b0   ea 6a a7 f2 04 96 12 05 00 01 00 00 00 20 00 5f  .j........... ._
00c0   19 6d 00 00 00 00 2e 0e 7b 96 73 de 72 7a 77 ff  .m......{.s.rzw.
00d0   5f 63 81 cd f9 1f 12 05 00 01 00 00 00 20 00 5f  _c........... ._
00e0   19 6d 00 00 00 00 c4 01 88 c3 ad 74 63 d7 c5 f1  .m.........tc...
00f0   e2 e1 0f a2 09 6a 12 05 00 01 00 00 00 20 00 5f  .....j....... ._
0100   19 6d 00 00 00 00 be 2e 55 9b 5e 78 e9 6a 20 25  .m......U.^x.j %
0110   54 a6 ae 73 ec 2e 12 05 00 01 00 00 00 20 00 5f  T..s......... ._
0120   19 6d 00 00 00 00 c1 89 ea f1 12 bb dd d7 21 68  .m............!h
0130   47 b7 94 c0 90 70 12 05 00 01 00 00 00 20 00 5f  G....p....... ._
0140   19 6d 00 00 00 00 11 e0 fa 55 ea 3f 69 74 ba 8b  .m.......U.?it..
0150   7b 53 39 20 3a 91                                {S9 :.

后续返回包
0000   00 e0 4c 00 27 90 00 e0 4c 55 a0 1b 08 00 45 00  ..L.'...LU....E.
0010   00 39 ac be 40 00 33 06 c2 4c 3d 87 96 c2 02 02  .9..@.3..L=.....
0020   02 69 0d ae 05 7d 8b 70 fa 16 83 7e bb b2 50 18  .i...}.p...~..P.
0030   16 d0 bc 72 00 00
			 12 04 00 00 00 00 00 11 00 00  ...r............
0040   00 00 00 5f 19 6d 00                             ..._.m.

0000   00 e0 4c 00 27 90 00 e0 4c 55 a0 1b 08 00 45 00  ..L.'...LU....E.
0010   03 59 ac c0 40 00 33 06 bf 2a 3d 87 96 c2 02 02  .Y..@.3..*=.....
0020   02 69 0d ae 05 7d 8b 70 fa 27 83 7e bc d2 50 18  .i...}.p.'.~..P.
0030   19 20 1a 8c 00 00
			 12 05 00 01 00 00 01 41 00 00  . ...........A..
0040   00 00 00 5f 19 6d 00 fa 5b 87 0e c5 d6 6a 91 cb  ..._.m..[....j..
0050   0a b7 a1 d6 db 3b d8 4f cd 65 95 54 79 53 8c 03  .....;.O.e.TyS..
0060   0d 6b ad 41 1e 98 35 86 17 66 e3 b9 b0 a2 ae 9f  .k.A..5..f......
0070   8d 10 39 55 95 56 69 f2 ee d2 60 0f 79 fc 08 8b  ..9U.Vi...`.y...
0080   a9 da 41 f7 f1 c1 e3 c5 da ce a7 5f 2d cd 07 a3  ..A........_-...
0090   02 7e f2 f0 eb 3e 15 1b 7b 60 df 86 6d 41 34 27  .~...>..{`..mA4'
00a0   dc b7 46 ae de 72 b6 eb 9f c9 e7 34 24 62 a6 9a  ..F..r.....4$b..
00b0   8b 43 f2 e3 e5 4f b4 f5 b6 7e 2c db 5c 05 52 0b  .C...O...~,.\.R.
00c0   e8 23 f3 b7 90 f7 c2 bc f1 df af 4f 6f c6 a9 9e  .#.........Oo...
00d0   31 dc fa 33 b6 8c 40 39 46 3f 3a 94 ca 06 3e 93  1..3..@9F?:...>.
00e0   a5 e9 88 62 18 c0 1d 12 06 e8 39 8c e7 54 ee 56  ...b......9..T.V
00f0   2b 0c de df ac dd 96 44 ad 85 71 0a 85 a8 a3 de  +......D..q.....
0100   d6 d4 97 48 f1 e1 a9 ad be 55 4f f3 df 81 3a 2a  ...H.....UO...:*
0110   c0 11 56 e1 76 b3 d2 52 29 ce 5d 94 fd b3 eb f6  ..V.v..R).].....
0120   23 e5 b6 ae 3f 15 9a 13 9e 07 b8 e6 18 bf 75 48  #...?.........uH
0130   e4 af 26 58 7e f7 ac 2d 7e a5 53 3e 13 2e 81 75  ..&X~..-~.S>...u
0140   c4 9a b7 77 1d 33 be d3 3c d4 a4 94 c6 c7 54 6d  ...w.3..<.....Tm
0150   92 58 e9 b5 b7 dd 69 1b 3a 70 ad d7 4e 73 d7 99  .X....i.:p..Ns..
0160   66 92 24 ae be d3 83 83 58 84 39 5e 0c f8 80 47  f.$.....X.9^...G
0170   93 ff c4 8f 90 26 2b 12 05 00 01 00 00 00 29 00  .....&+.......).
0180   00 00 00 00 5f 19 6d 00 92 76 71 ee 2b 67 d3 49  ...._.m..vq.+g.I
0190   a2 4b 5a c3 e5 32 4c 60 ab 26 40 97 e0 0d ed 98  .KZ..2L`.&@.....
01a0   12 05 01 01 00 00 00 21 00 00 00 00 00 5f 19 6d  .......!....._.m
01b0   00 83 78 68 10 8c f9 17 71 3f 8f 66 af 9f 54 f3  ..xh....q?.f..T.
01c0   a7 12 05 01 01 00 00 00 21 00 00 00 00 00 5f 19  ........!....._.
01d0   6d 00 65 1f 0f 84 07 1b 73 8c 45 aa a1 cc 22 51  m.e.....s.E..."Q
01e0   72 33 12 05 01 01 00 00 00 69 00 00 00 00 00 5f  r3.......i....._
01f0   19 6d 00 f3 5d 53 ba 13 46 53 46 4f f6 fb 88 2c  .m..]S..FSFO...,
0200   cc 95 2d cc 2b e1 6c c1 2d fb cd 6c cc 7c 78 b0  ..-.+.l.-..l.|x.
0210   a4 88 17 85 62 76 83 c4 63 26 03 fd 98 5e 34 8d  ....bv..c&...^4.
0220   ab e1 f6 65 18 99 82 16 27 15 ac 92 2a 79 eb 95  ...e....'...*y..
0230   f4 f7 1f 8c 60 5b 8c 7e 70 1b 19 e8 7b a4 5d 5c  ....`[.~p...{.]\
0240   e7 5e be b3 00 33 81 7a dc a5 47 12 05 01 01 00  .^...3.z..G.....
0250   00 00 69 00 00 00 00 00 5f 19 6d 00 cb 59 78 d9  ..i....._.m..Yx.
0260   3b ab 68 11 95 d0 90 62 f8 fc 9a 48 f9 17 1b 7d  ;.h....b...H...}
0270   ca 9e f7 5f 6b 3b 39 69 c4 2b 72 c5 94 2c 56 31  ..._k;9i.+r..,V1
0280   b9 5c cd 34 e8 2f 19 6e 57 2e 4e da 0d d4 b7 91  .\.4./.nW.N.....
0290   81 de 53 d7 00 77 f3 d6 f9 50 45 bf 56 be 22 5e  ..S..w...PE.V."^
02a0   d5 bb 29 ea f9 a8 1e 12 0d f2 e3 59 ef fd c9 3a  ..)........Y...:
02b0   ad f2 65 cf 12 05 01 01 00 00 00 29 00 00 00 00  ..e........)....
02c0   00 5f 19 6d 00 f2 70 fa b5 e7 a4 88 9e db 92 d9  ._.m..p.........
02d0   35 80 bf 5f f6 b5 6c 01 d3 83 11 fe 6b 12 05 01  5.._..l.....k...
02e0   01 00 00 00 69 00 00 00 00 00 5f 19 6d 00 b0 c5  ....i....._.m...
02f0   71 d0 47 8a 58 84 9e 9f ef ef 31 16 b9 87 f3 41  q.G.X.....1....A
0300   33 62 24 05 d4 9b fa f3 d5 df d1 87 98 fd 77 37  3b$...........w7
0310   b1 83 dc c5 f9 58 b4 33 80 e4 98 ed ef d2 1c fb  .....X.3........
0320   10 94 ba 8d cb f7 da 07 95 2c f7 0d e2 5e f5 5d  .........,...^.]
0330   1d 76 ed 2e e1 16 56 7e a3 2a 75 a7 78 ad e3 ab  .v....V~.*u.x...
0340   7d f2 6d a0 0f e6 12 05 01 01 00 00 00 21 00 00  }.m..........!..
0350   00 00 00 5f 19 6d 01 00 8e e1 bb 6d 6b a4 c8 ec  ..._.m.....mk...
0360   ed b2 0e 9f 0f b7 0a                             .......

6232409离线
0000   00 e0 4c 55 a0 1b 00 e0 4c 00 27 90 08 00 45 00  ..LU....L.'...E.
0010   00 48 32 30 40 00 80 06 ef cb 02 02 02 69 3d 87  .H20@........i=.
0020   96 c2 05 9e 01 bb 89 86 d7 56 2e 06 a4 1a 50 18  .........V....P.
0030   41 d7 2c ca 00 00
			 12 03 00 01 00 00 00 20<00 5f  A.,.......... ._
0040   19 59>00 00 00 00 92 41 ca fa 28 8a 6c 7c 0e 4c  .Y.....A..(.l|.L
0050   0e e5 6c 82 86 2d                                ..l..-

服务器返回包
0000   00 e0 4c 00 27 90 00 0f 3d a0 8a c0 08 00 45 00  ..L.'...=.....E.
0010   00 49 32 7e 40 00 33 06 7f d6 3d 87 96 c2 c0 a8  .I2~@.3...=.....
0020   00 69 0d ae 04 5a 52 b7 5d b1 e2 43 60 3f 50 18  .i...ZR.]..C`?P.
0030   19 20 24 ca 00 00 
                         12 03 00 01 00 00 00 21 00 00  . $..........!..
0040   00 00 00 5f 19 6d 00 ed 73 82 6a e6 3e 57 72 aa  ..._.m..s.j.>Wr.
0050   a8 bf a7 89 d7 df f3                             .......

**********************************************************/
