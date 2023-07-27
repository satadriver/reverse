// wuyiguagua.cpp: implementation of the wuyiguagua class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "imsg.h"
#include "wuyiguagua.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// WuyiguaguaSession::WuyiguaguaSession()
// {
// 
// }
// 
// WuyiguaguaSession::~WuyiguaguaSession()
// {
// 
// }
WuyiguaguaSession::WuyiguaguaSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_direction = 0;
	m_closetm = packh.m_tm + 180;
	m_bIdc = FALSE;
	m_MsgBuflen = 0;
	m_pMsgBuf = NULL;
	m_begfiletm = 0;
	
	g_ImsgSion++;
}

WuyiguaguaSession::~WuyiguaguaSession()
{
	m_MsgBuflen = 0;
	if (m_pMsgBuf != NULL)
	{
		delete[] m_pMsgBuf;
	}

	g_ImsgSion--;
}

BOOL WuyiguaguaSession::CloseSession(time_t nowtm)
{
	try
	{
		if (m_begfiletm == 0)
		{
			// 自从上次存文件以来一直没有包到达就销毁该会话
			return TRUE;
		}
		
		// 输出文件
		if (m_MsgBuflen > 0)
		{
			// 写实体文件
			WriteRealFile(g_pMsgBuffer, 0);
		}
		
		// 这里不处理IDCServer 和 活动历史
		// 处理数据源
		if(m_dataSrc == DATASRC_OTHER)
		{
			if ((v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP))
				|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
			{
				m_dataSrc = DATASRC_IDC;
			}
		}

		// 匹配案件和预警
// 		m_OpenCaseFlg = v_opencaseVp;
// 		m_OpenWarnFlg = v_openwarnVp;
		m_OpenIndxFlg = v_openflagimsg51;
		DWORD state = 0;
		SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
	
		// 匹配语种
		CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

		// 写对照文件
		if (m_realfname.IsEmpty())
		{
			m_CaseArr.RemoveAll();
			m_WarnArr.RemoveAll();
			m_begfiletm = 0;
			m_closetm += 180;
			return FALSE;
//			m_indexfname = CreateFilename(m_sip, m_dip, "imsg_51.txt.indexfile.txt");
		}
		else
		{
			m_indexfname = m_realfname + ".indexfile.txt";
		}
		CString str;
		if (!m_userid.IsEmpty())
		{
			str.Format("user= %s\r\n", m_userid);
		}

		WriteIndexFile("imsg_51", "imsg_51", str, g_pImsgMissObj);
		m_CaseArr.RemoveAll();
		m_WarnArr.RemoveAll();
		m_begfiletm = 0;
		m_closetm += 180;
		return FALSE;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "51IMSession::CloseSession() Error!!");
		return TRUE;
	}
}

int WuyiguaguaSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	try
	{
		if (m_begfiletm == 0)
		{
			m_begfiletm = packh.m_tm;
		}
		WORD plen = *((WORD*)pPacket);
		if(packh.m_datalen<50)
 		{
 			return 0;
 		}
// 		if ((plen <= 12) || (plen > packh.m_datalen))
// 		{
// 			return 0;
// 		}
		if (memcmp(pPacket,"\x90\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",12)==0 &&
			memcmp(pPacket+15,"\x00\x04",0)==0)
		{
			if(UserLogin(packh, pPacket))//上线
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		if (memcmp(pPacket,"\x00\x00\x00\x00\x00\x00\x0a",7)==0&&
			memcmp(pPacket+15,"\x00\x0b",0)==0)
		{
			;//接收数据，有很多符合此特征，但没有聊天内容的要根据串的内容进行排除
		}
		else
		{
			return FALSE;
		}
		const char *tp = pPacket;
		g_Msglen = 0;
		char* tmptp=NULL;
		char userid[200];
		memset(userid,0,200);
//		tmptp=strstr(tp+33,"\0");
//		if(tmptp!=NULL)
		{
			memcpy(userid,tp+28,20);
			const char *tmp = tp+36;
			char* begtp=strstr(tmp+8,"<a>");
			char* endtp=strstr(tp+46,"</a>");
			if (begtp == NULL ||
				endtp == NULL)
			{
				return FALSE;
			}
			int len=0;
			if (endtp-begtp + g_Msglen> MSGBUFLEN)
			{
				len=MSGBUFLEN-g_Msglen;
			}
			else
			{
				len=endtp-begtp-3;
			}
			if (len<=0)
			{
				return FALSE;
			}
			memcpy(g_pMsgBuffer+g_Msglen," ← ",2);
			g_Msglen+=2;
			memcpy(g_pMsgBuffer+g_Msglen,(char*)(LPCTSTR)userid,strlen(userid));
			g_Msglen+=strlen(userid);

			memcpy(g_pMsgBuffer+g_Msglen,":",1);
			g_Msglen+=1;
			memcpy(g_pMsgBuffer+g_Msglen,begtp+3,len);
			g_Msglen+=len;
			memcpy(g_pMsgBuffer+g_Msglen,"\r\n",2);
			g_Msglen+=2;
		}
// 		do
// 		{
// 			if (stricmp(tp+12, "ClientText") == 0)
// 			{
// 				g_Msglen += FormatVpIM(packh, pPacket);		// 聊天信息
// 			}
// 			else if (stricmp(tp+12, "SystemMessage") == 0)	// 系统消息 (用于区分客户端)
// 			{
// 				// Server --> Client
// 				if (m_direction == 0)	// 还没有确定方向
// 				{
// 					if (memcmp(packh.m_sip, m_csip, 4)==0)
// 					{
// 						m_direction = 2;
// 					}
// 					else
// 					{	
// 						m_direction = 1;
// 					}
// 				}
// 			}
// 			else if (stricmp(tp+12, "VerifyUser") == 0)
// 			{
// 				g_Msglen += UserLogin(packh, pPacket);	// 登录
// 			}
// 
// 			if (m_direction == 2)
// 			{
// 				// 交换会话方向
// 				CString tstr = m_sip;
// 				m_sip = m_dip;
// 				m_dip = tstr;
// 				tstr = m_dmac;
// 				m_dmac = m_smac;
// 				m_smac = tstr;
// 				DWORD ip;
// 				memcpy(&ip, m_csip, 4);
// 				memcpy(m_csip, m_cdip, 4);
// 				memcpy(m_cdip, &ip, 4);
// 				m_AuthInfo.SwapAuthInfo();
// 				m_direction = 1;
// 			}
// 
// 			WORD len = *((WORD*)(pPacket+plen));
// 			if (len<=12)
// 			{
// 				break;
// 			}
// 			tp = pPacket+plen;
// 			plen += len;
// 		}while(plen <= packh.m_datalen);

		if (g_Msglen > 0)
		{
			// 写实体文件
			memset(g_pMsgBuffer + g_Msglen, 0, 4);
			if (g_Msglen + m_MsgBuflen >= MSGBUFLEN)
			{
				// 写文件
				WriteRealFile(g_pMsgBuffer, g_Msglen);
			}
			else
			{
				if (m_pMsgBuf == NULL)
				{
					m_pMsgBuf = new char[MSGBUFLEN];
				}
				memcpy(m_pMsgBuf + m_MsgBuflen, g_pMsgBuffer, g_Msglen+1);
				m_MsgBuflen += g_Msglen;
			}
			g_Msglen = 0;
		}
 	}
	catch(...)
	{
		WriteLog(IMSGPRO, "51Session::InsertPacket() Error!!");
		return -3;
	}
	return 0;
}
// 0000   00 14 78 99 fc 48 00 03 0d 5f e3 d0 08 00 45 00  ..x..H..._....E.
// 0010   00 66 f5 22 00 00 80 11 ba 2d c0 a8 00 24 72 50  .f.".....-...$rP
// 0020   58 1a 12 12 1b 61 00 52 ac a5 90 01 00 00 00 00  X....a.R........
// 0030   00 00 00 00 00 00 53 9a 00 04 32 17 00 00 00 29  ......S...2....)
// 0040   61 70 61 70 65 6e 67 2c 65 31 30 61 64 63 33 39  apapeng,e10adc39
// 0050   34 39 62 61 35 39 61 62 62 65 35 36 65 30 35 37  49ba59abbe56e057
// 0060   66 32 30 66 38 38 33 65 00 00 00 00 00 00 00 00  f20f883e........
// 0070   00 00 66 00                                      ..f.

int WuyiguaguaSession::UserLogin(DATEHEADER& packh, const char* pPacket)
{
	char user[200];
	memset(user,0,200);
// 	if (stricmp(pPacket + 12+ 64, "UserID") != 0)
// 	{
// 		return 0;
// 	}
	const char* tpend;
	tpend=strstr(pPacket+22,",");
	if (tpend!=NULL)
	{
		memcpy(user,pPacket+22,tpend-(pPacket+22));
	}
	m_userid.Format("%s",user);

	if (m_userid.IsEmpty())
	{
		m_begfiletm = 0;
		return 0;
	}
//	if ((m_infonum == 0xF0) && (m_acctype != 0xF))
	{
		m_OpenIndxFlg = v_openflaghf;
		m_OpenCaseFlg = v_opencasehf;
		m_OpenWarnFlg = v_openwarnhf;
		CString txt;
		txt.Format("%s", m_userid);
		// 匹配案件、预警和屏蔽信息
		DWORD state = 0;
		SearchCaseOrWarn(this, txt, txt.GetLength(), state);
		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

		// 处理IDC数据源
		CString tmpisp;
// 		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), tmpisp))
// 		{
// 			if (m_dataSrc == DATASRC_OTHER)
// 			{
// 				m_dataSrc = DATASRC_IDC;
// 				m_strISP = tmpisp;
// 			}
// 			if (v_statIdc)
// 			{
// 				CString ttstr;
// 				ttstr.Format("%s\1%s\1hf\1\1%s\1浩方\1%s\1\1", 
// 					m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
// 				v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttstr);
// 			}
// 		}
// 		else 
// 		if ((m_dataSrc == DATASRC_OTHER)
// 			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
// 		{
// 			m_dataSrc = DATASRC_IDC;
// 		}
// 
		// 输出活动历史
		if (v_statGame)
		{
			HistoryRecord hisRec;
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		m_userid);
			hisRec.SetRecord(IDX_USER,		m_userid);

			hisRec.SetRecord(IDX_STATE, "上线");
			WriteHisData("imsg", "imsg_51", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
		}


		txt.Format("user= %s\r\nstat= 上线\r\n",
			m_userid);
		m_indexfname = CreateFilename(m_sip, m_dip, "imsg_51.indexfile.txt");
		WriteIndexFile("imsg_51", "imsg_51", txt, g_pImsgMissObj);
		m_CaseArr.RemoveAll();
		m_WarnArr.RemoveAll();
		m_begfiletm = 0;
	}
	return 1;
}



// 写实体文件
void WuyiguaguaSession::WriteRealFile(char* pData, int datalen)
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_51.txt");
		ASSERT(!m_realfname.IsEmpty());
	}
	FILE* fp = fopen(m_realfname, "ab");
	if (fp == NULL)
	{
		return;
	}
	if (m_MsgBuflen > 0)
	{
		fwrite(m_pMsgBuf, m_MsgBuflen, 1, fp);
		m_MsgBuflen = 0;
		delete[] m_pMsgBuf;
		m_pMsgBuf = NULL;
	}
	if (datalen > 0)
	{
		fwrite(pData, datalen, 1, fp);
	}
	fclose(fp);
}

