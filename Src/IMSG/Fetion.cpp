//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	VP
//	版本:		1.0
//	创建日期:	2008-02-29
//	作者:		lp
//	主要功能:	fetion聊天的还原
//	模块描述:	
//	主要函数:	
//
//**************************************************************** 

#include "stdafx.h"
#include "imsg.h"
#include "Fetion.h"
//#include "publicfun.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern void ConverHttp2Html(char *tmp_content, int tmp_content_len);
extern BOOL ConvertUtf8toGbk(LPCTSTR cchString, int lenString, CString &strGbk);
extern BOOL NextValue(char **p);
extern BOOL NextLine(char **p);


FetionSession::FetionSession(DATEHEADER& packh, int aT)
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

FetionSession::~FetionSession()
{
	m_MsgBuflen = 0;
	if (m_pMsgBuf != NULL)
	{
		delete[] m_pMsgBuf;
	}

	g_ImsgSion--;
}


//PC版Fetion
BOOL isFetion(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_dport==8080)
	{
		if (packh.m_datalen < 13)
		{
			return FALSE;
		}

		if (memcmp(pPacket+3,"\x20\x66\x65\x74\x69\x6f\x6e\x2e\x63",9)==0)
		{
			return TRUE;
		}
		
		if (memcmp(pPacket,"\x4d\x20\x66\x65\x74\x69\x6f\x6e\x2e\x63\x6f\x6d\x2e",13)==0)
		{
			return TRUE;
		}
	}
	else if (packh.m_sport==8080)
	{
		if (memcmp(pPacket,"\x4d\x20",2) == 0)
		{
			return TRUE;
		}
		if (memcmp(pPacket,"BN ", 3) == 0)
		{
			// 确定发给服务器的消息实际联系人用
			return TRUE;
		}
		else
		{
			if (packh.m_datalen < 22)
			{
				return FALSE;
			}

			if (memcmp(pPacket,"\x53\x49\x50\x2d\x43\x2f\x32\x2e\x30\x20\x32\x30\x30\x20\x4f\x4b\x0d\x0a\x49\x3a\x20\x34",22)!=0)
			{
				return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
	}

/*	char * p = (char*)pPacket;
	char * t,* n;
	CString from,to,Value;
	while(*p)
	{
		char *tmp;
		t=p;
		if (!NextValue(&p))
		{
			if(!NextLine(&p))
				break;
			continue;
		}
		n=p;
		tmp=p;
		if(!NextLine(&p))
			break;
		_strlwr(t);

		if(strcmp(t,"f")==0)
		{
			from=n;
			from.TrimRight();
			from.MakeLower();
			
		}
		else if(strcmp(t,"t")==0)
		{
			to=n;
			to.TrimRight();
			to.MakeLower();
		}
		else if(strcmp(t,"l")==0)
		{
			Value=n;
			Value.TrimRight();
			Value.MakeLower();
			int count = atoi(Value);
			if (count > 0)
			{
				return true;
			}
			else
			{
				return FALSE;
			}
		}

	}*/
	return TRUE;
}

BOOL FetionSession::CloseSession(time_t nowtm)
{
	try
	{
		if (m_begfiletm == 0 )
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
		if (m_realfname.IsEmpty())
		{
			// 没有实体文件的是不合法数据
			return TRUE;
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
		m_OpenCaseFlg = v_opencasefetion;
		m_OpenWarnFlg = v_openwarnfetion;
		m_OpenIndxFlg = v_openflagfetion;
		DWORD state = 0;
		SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
	
		// 匹配语种
//		CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

		// 输出活动历史
		if (v_statImsgOther)
		{
			HistoryRecord hisRec;
			hisRec.SetRecord(IDX_FORWTYPE,	"1");
			hisRec.SetRecord(IDX_ID,		m_userid);
 			hisRec.SetRecord(IDX_USER,		m_userid);

			WriteHisData("imsg", "imsg_fetion", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
		}

		// 写对照文件
		if (m_realfname.IsEmpty())
		{
			m_indexfname = CreateFilename(m_sip, m_dip, "imsg_fetion.txt.indexfile.txt");
		}
		else
		{
			m_indexfname = m_realfname + ".indexfile.txt";
		}
		CString str;
		str.Format("user= %s\r\nfrom= %s\r\nto= %s\r\n", m_userid, m_from, m_to);
		WriteIndexFile("imsg_fetion", "imsg_fetion", str, g_pImsgMissObj);
		m_CaseArr.RemoveAll();
		m_WarnArr.RemoveAll();
		m_begfiletm = 0;
		m_closetm += 180;
		return FALSE;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "VPIMSession::CloseSession() Error!!");
		return TRUE;
	}
}

int FetionSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	try
	{
#define FRINED 1
#define COMEFRIEND 2
#define TOFRIEND 3

		int type ;
		if (packh.m_dport==8080)
		{
			if (packh.m_datalen < 13)
			{
				return FALSE;
			}
			if (memcmp(pPacket,"\x4d\x20\x66\x65\x74\x69\x6f\x6e\x2e\x63\x6f\x6d\x2e",13)!=0)
			{
				// 非 M fetion.com.
				return FALSE;
			}

			type = TOFRIEND;
		}
		else if (packh.m_sport==8080)
		{
			if (memcmp(pPacket,"\x4d\x20",2) == 0)
			{
				type = COMEFRIEND;
			}
			else if(memcmp(pPacket, "BN ", 3) == 0)
			{
				// <member uri="sip:691213543@fetion.com.cn;p=5545"/>
				char *pfirst = strstr(pPacket, "member uri=\"sip:");
				if(pfirst != NULL)
				{
					pfirst += 16;	//strlen("member uri=\"sip:");
					char *pend = strstr(pfirst, ";");
					if(pend  != NULL && pend -pfirst > 0)
					{
						m_fxcontentid  = CString(pfirst, pend-pfirst);
					}
				}
				return FALSE;
			}
			else
			{
				if (packh.m_datalen < 22)
				{
					return FALSE;
				}

				if (memcmp(pPacket,"SIP-C/", 6) != 0 
					|| memcmp(pPacket+7,".0 200 OK",9)!=0)
				{
					// 2010SP2前版本:SIP-C/2.0 200 OK
					// 2010SP2:SIP-C/4.0 200 OK 2010SP2
					return FALSE;
				}
				type = FRINED;
			}
		}
		else
		{
			return FALSE;
		}
// 		WORD plen = *((WORD*)pPacket);
// 		if ((plen <= 12) || (plen > packh.m_datalen))
// 		{
// 			return 0;
// 		}
		const char *tp = pPacket;
		g_Msglen = 0;



		char * p = (char*)pPacket;
		char * t,* n;
		CString from,to,Value;

		if (type == COMEFRIEND && packh.m_datalen > 15)
		{
			//拆出收件人 M 683891250 SIP-C/2.0
			char tmpchar[MAX_PATH];
			memcpy(tmpchar,pPacket,15);
			to.Format("%s",tmpchar);
			int ifind = to.Find(" ");
			if (ifind>0)
			{
				to.Format("%s",to.Mid(ifind+1));
			}
			ifind = to.Find(" ");
			if (ifind > 0)
			{
				to.Format("%s",to.Left(ifind));
			}
		}

		int msglen = 0;
		while(*p)
		{
			char *tmp;
			t=p;
			if (!NextValue(&p))
			{
				if(!NextLine(&p))
					break;
// 				if(memcmp(p, "\r\n", 2) == 0)
// 				{	// 双回车,结束头解析
// 					// 发现聊天(与机器人)中有 \r\n, 此时机器人的聊天内容解析不出来
// 					// 未发现与人的聊天中有\r\n的情况, \r\n会被替换为\n发出
// 					t = p+2;
// 					p = (char*)pPacket+packh.m_datalen;
// 					break;
// 				}
				continue;
			}
			n=p;
			tmp=p;
			if(!NextLine(&p))
				break;
// 			if(memcmp(p, "\r\n", 2) == 0)
// 			{
// 				t = p+2;
// 				p = (char*)pPacket+packh.m_datalen;
// 				break;
// 			}
			_strlwr(t);

			if(strcmp(t,"f")==0)
			{
				from=n;
				from.TrimRight();
				from.MakeLower();
				int ifind = from.Find(";");
				if (ifind>0)
				{
					from.Format("%s",from.Left(ifind));
				}
				if (type == TOFRIEND)
				{
					if (m_userid.IsEmpty())
					{
						m_userid = from;
					}
				}
				
			}
			else if(strcmp(t,"t")==0)
			{
				to=n;
				to.TrimRight();
				to.MakeLower();
				int ifind = to.Find(";");
				if (ifind>0)
				{
					to.Format("%s",to.Left(ifind));
				}
			}
			else if(strcmp(t,"l")==0)
			{
				if (m_userid.IsEmpty())
				{
					m_userid = to;
				}

				Value=n;
				Value.TrimRight();
				Value.MakeLower();
				msglen = atoi(Value);
				p=p+2;
			}
		}
		
		if(p-t == msglen)
		{
			CString convstr;
			if (type == TOFRIEND)
			{
				ConvertUtf8toGbk(t, msglen, convstr);
				if (!convstr.IsEmpty())
				{
					if(to.IsEmpty())
						to = m_fxcontentid;	// 由BN 命令获得的to

					int len = sprintf(g_pMsgBuffer,
					"%s → %s:\r\n%s\r\n\r\n",
					from, to, convstr);

					g_Msglen += len;
				}
			}
			else if (type == COMEFRIEND)
			{
				ConvertUtf8toGbk(t, msglen, convstr);
				if (!convstr.IsEmpty())
				{
					int len = sprintf(g_pMsgBuffer,
					"%s ← %s:\r\n%s\r\n\r\n",
					to, from, convstr);

					g_Msglen += len;
				}
			}
			else
			{
				FormatFriendList(t, msglen, convstr);
				if (!convstr.IsEmpty())
				{
					int len = sprintf(g_pMsgBuffer,
					"好友列表:\r\n%s\r\n\r\n",
					convstr);

					g_Msglen += len;
				}
			}
		}



		if (g_Msglen > 0)
		{

			if (m_begfiletm == 0)
			{
				m_begfiletm = packh.m_tm;
			}

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
		WriteLog(IMSGPRO, "fetionSession::InsertPacket() Error!!");
		return -3;
	}
	return 0;
}


// 写实体文件
void FetionSession::WriteRealFile(char* pData, int datalen)
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_fetion.txt");
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
BOOL FetionSession::FormatFriendList(LPCTSTR cchString, int lenString, CString &strGbk)
{
	char * p = (char*)cchString;
	char * t,* n,*m;
	t = strstr(p,"</contact>");

	while (t != NULL)
	{
		n=p;
		*t = 0;
		t++;
		p=t;
		
		m=strstr(n,"uri=\"");
		if(m != NULL)
		{
			n=m+5;
			m=strstr(n,";");
			if (m!=NULL)
			{
				*m=0;
				m++;
				strGbk.Format("%suserid %s ",(CString)strGbk,n);
			}
		}
		if (m != NULL)
		{
			m=strstr(m,"mobile-no=\"");
		}
		if(m != NULL)
		{
			n=m+11;
			m=strstr(n," ");
			if (m!=NULL)
			{
				*m = 0;
				m++;
				strGbk.Format("%sphone %s ",(CString)strGbk,n);
			}
		}
		strGbk.Format("%s\r\n",(CString)strGbk);
		t = strstr(p,"</contact>");
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//手机Fetion

BOOL isMobileFethion(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_dport==8000)
	{

		if (memcmp(pPacket+6,"\x66\x65\x74\x69\x6f\x6e\x2e\x63\x6f\x6d\x2e\x63\x6e",13)==0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

MebileFetionSession::MebileFetionSession(DATEHEADER& packh, int aT)
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

MebileFetionSession::~MebileFetionSession()
{
	m_MsgBuflen = 0;
	if (m_pMsgBuf != NULL)
	{
		delete[] m_pMsgBuf;
	}

	g_ImsgSion--;
}

BOOL MebileFetionSession::CloseSession(time_t nowtm)
{
	try
	{
		if (m_begfiletm == 0 )
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
		if (m_realfname.IsEmpty())
		{
			// 没有实体文件的是不合法数据
			return TRUE;
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
		m_OpenCaseFlg = v_opencasefetion;
		m_OpenWarnFlg = v_openwarnfetion;
		m_OpenIndxFlg = v_openflagfetion;
		DWORD state = 0;
		SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
	
		// 匹配语种
//		CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

		// 输出活动历史
		if (v_statImsgOther)
		{
			HistoryRecord hisRec;
			hisRec.SetRecord(IDX_FORWTYPE,	"1");
			hisRec.SetRecord(IDX_ID,		m_userid);
 			hisRec.SetRecord(IDX_USER,		m_userid);

			WriteHisData("imsg", "imsg_fetion", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
		}

		// 写对照文件
		if (m_realfname.IsEmpty())
		{
			m_indexfname = CreateFilename(m_sip, m_dip, "imsg_fetion.txt.indexfile.txt");
		}
		else
		{
			m_indexfname = m_realfname + ".indexfile.txt";
		}
		CString str;
		str.Format("user= %s\r\nfrom= %s\r\nto= %s\r\n", m_userid, m_from, m_to);
		WriteIndexFile("imsg_fetion", "imsg_fetion", str, g_pImsgMissObj);
		m_CaseArr.RemoveAll();
		m_WarnArr.RemoveAll();
		m_begfiletm = 0;
		m_closetm += 180;
		return FALSE;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "VPIMSession::CloseSession() Error!!");
		return TRUE;
	}
}

int MebileFetionSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	try
	{
#define FRINED 1
#define COMEFRIEND 2
#define TOFRIEND 3

		if (memcmp(pPacket+6,"\x66\x65\x74\x69\x6f\x6e\x2e\x63\x6f\x6d\x2e\x63\x6e",13)!=0)
		{
			return FALSE;
		}

		const char *tp = pPacket;
		g_Msglen = 0;

		CString sipnum,dipnum;
		CString msg;
		int poslen;

		sipnum = CString(pPacket+37);
		dipnum = CString(pPacket+48);
		if(dipnum.Find(";p=")!=-1)
		{
			poslen = dipnum.GetLength();

			CString tmpnum;
			tmpnum = dipnum.Left(dipnum.Find(";p="));
			dipnum = tmpnum;
		}

		msg = CString(pPacket+48+poslen+19);
		if (msg.GetLength() > 0)
		{

			if (m_begfiletm == 0)
			{
				m_begfiletm = packh.m_tm;
			}

			CString txt;
			txt.Format("%s → %s:\r\n%s\r\n\r\n",sipnum,dipnum,msg);

			// 写文件
			WriteRealFile((char*)(LPCSTR)txt, txt.GetLength());
		}
 	}
	catch(...)
	{
		WriteLog(IMSGPRO, "fetionSession::InsertPacket() Error!!");
		return -3;
	}
	return 0;
}


// 写实体文件
void MebileFetionSession::WriteRealFile(char* pData, int datalen)
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_fetion.txt");
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

// BOOL MobileFethion_process(DATEHEADER& packh,const char *pPacket)
// {
// 	CString sipnum,dipnum;
// 	CString msg;
// 
// 	char* Begbuf;
// 	char* Endbuf;
// 
// 
// 	
// 	if(sipnum.GetLength()>0 && dipnum.GetLength()>0)
// 	{
// 		Begbuf=strstr(pPacket,"SaveHistory");
// 		if (Begbuf!=NULL)
// 		{
// 			msg = CString(Begbuf+2);
// 
// 			//写对照文件
// 			CString txt;
// 			txt.Format("%s → %s:\r\n%s\r\n\r\n",sipnum,dipnum,msg);
// 
// 			FetionSession fs;
// 			fs.WriteRealFile((LPCSTR)txt,txt.GetLength());
// 		}
// 	}
// }
