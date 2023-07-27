
#include "stdafx.h"
#include "http.h"
#include "TeSpread.h"
#include "HttpSession.h"
#include "HttpProxy.h"
#include "PublicFun.h"
#include "PostStruct.h"
#include "PostSplit.h"

#include "./../Getmoniterweb.h"
#include "./../IpQueryLib.h"
#include "DealHttpGet.h"

//#define		DUMPALLHTTPDOWN
extern CCrc32	g_crc32Calc;
extern moniterweb g_moniterweb;
// extern BOOL IsPopoPacket(HttpCommInfo& Request, HttpSession& httpsession);
extern CString SplitEmlName(CString emlname,CString username,BOOL isfrom);
extern IpQueryLib	k_IpQueryTable;	// IP区域表

void HttpSession::WriteHttpUrlFile(BOOL bCase)
{
int error=0;
	try
	{
		if(FALSE == bCase)
		{
			if (v_openflaghttpurl)
			{
				static DWORD staticttCheck = time(0);
				SYSTEMTIME now;
				GetLocalTime(&now);
				CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
				if (m_indexfname.IsEmpty())
				{
					m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u_httpurl.indexfile.txt",
						tmppath, now.wYear, now.wMonth, now.wDay,
						now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum);
				}
error=1;
				CString text = GetHttpInfo(m_Requestline, TRUE);
error=101;
				if(FALSE == text.IsEmpty())
				{
					unsigned __int64 ttKey;
					DWORD *pD = (DWORD*)&ttKey;
					memcpy(pD, m_csip, 4);
					DWORD dwURL = g_crc32Calc.GetCRC(text, text.GetLength());
error=102;
					memcpy(pD+1, &dwURL, 4);
					// CheckHash
					if(FALSE == g_IpHostMap.Lookup(ttKey, dwURL))
					{
error=103;
						dwURL = time(0);
						g_IpHostMap.SetAt(ttKey, dwURL);
error=104;
					//去除url 请注释
					WriteIndexFile("httpurl", "httpurl", text, m_dceprocotol->m_httpMissObj); 
					}
error=2;
					// ClearHash
					if(abs(time(0) -staticttCheck) > 10*60)
					{
						POSITION pos = g_IpHostMap.GetStartPosition();
						while(pos != NULL)
						{
							g_IpHostMap.GetNextAssoc(pos, ttKey, staticttCheck);
							if(abs(time(0) -staticttCheck) > 10*60)
							{
								g_IpHostMap.RemoveKey(ttKey);
							}
						}
						if(g_IpHostMap.GetCount() > 10000)
							staticttCheck = time(0)-5*60;
						else
							staticttCheck = time(0);
					}
				}
			}

		}
		else 
		{
error=3;
			static DWORD staticttCheck = time(0);
			SYSTEMTIME now;
			GetLocalTime(&now);
			CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
			if (m_indexfname.IsEmpty())
			{
				m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u_httpurl.indexfile.txt",
					tmppath, now.wYear, now.wMonth, now.wDay,
					now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum);
			}
error=4;
			CString text = GetHttpInfo(m_Requestline, TRUE);
	//	 	text.Format("url= %s\r\nreferer= %s\r\ncookie= %s\r\nuser-agent= %s\r\n", m_Requestline.m_URL, 
	//	 		m_Requestline.m_Referer, m_Requestline.m_Cookie, m_Requestline.m_useragent);
			//去除url 请注释
			WriteIndexFile("httpurl", "httpurl", text, m_dceprocotol->m_httpMissObj); 
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "WriteHttpUrlFile Error!! error=%d",error);

	}

}

//写手机MOMOIndex文件
void HttpSession::WriteMobileFetionFile(CString txt)
{
	static DWORD staticttCheck = time(0);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
	if (m_indexfname.IsEmpty())
	{
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u.imsg_fetion.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum);
	}

	WriteIndexFile("imsg_fetion", "imsg_fetion", txt, m_dceprocotol->m_httpMissObj);
}

//写手机MOMOIndex文件
void HttpSession::WriteMobileMomoFile(CString txt)
{
	static DWORD staticttCheck = time(0);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
	if (m_indexfname.IsEmpty())
	{
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u_%s.momo.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum,"1");
	}

	WriteIndexFile("momo", "momo", txt, m_dceprocotol->m_httpMissObj);
}

//写PCAnywhere Index文件
void HttpSession::WritePcanyWhereFile(CString txt)
{
	static DWORD staticttCheck = time(0);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
	if (m_indexfname.IsEmpty())
	{
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u.rc_pcanywhere.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum);
	}

	WriteIndexFile("rc_pcanywhere", "rc_pcanywhere", txt, m_dceprocotol->m_httpMissObj);
}

//写kuaidi Index文件
void HttpSession::WriteKuaidiFile(CString txt)
{
	static DWORD staticttCheck = time(0);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
	if (m_indexfname.IsEmpty())
	{
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u.kuaidi.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum);
	}

	WriteIndexFile("kuaidi", "kuaidi", txt, m_dceprocotol->m_httpMissObj);
}

//写ssh Index文件
void HttpSession::WriteSshFile(CString txt)
{
	static DWORD staticttCheck = time(0);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
	if (m_indexfname.IsEmpty())
	{
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u.rc_ssh.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum);
	}

	WriteIndexFile("rc_ssh", "rc_ssh", txt, m_dceprocotol->m_httpMissObj);
}

//搜索引擎关键字协议
void HttpSession::WriteSearchFile(BOOL bCase)
{
	static DWORD staticttCheck = time(0);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
	if (m_indexfname.IsEmpty())
	{
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u_%s.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum,m_Requestline.m_SearchType);
	}

	CString text ;
	text.Format("searchtype= %s\r\nsearchkeytype=%s\r\nsearchkey= %s\r\n",m_Requestline.m_SearchType,m_Requestline.m_SearchKeyType,m_Requestline.m_SearchKey);
	WriteIndexFile(m_Requestline.m_SearchType, m_Requestline.m_SearchType, text, m_dceprocotol->m_httpMissObj);
}
void HttpSession::WriteSkypeIndex()
{
		CString text="";		

	// 写活动历史
	if( v_stat && v_statOther)
	{
		HistoryRecord hisRec;
		CString tmpstrhis;
		hisRec.SetRecord(IDX_ID,		m_id);
		hisRec.SetRecord(IDX_USER,		m_id);
		WriteHisData("skype", "skype", &hisRec, m_dceprocotol->m_httpMissObj, HTTP_PROTOCOL);//, HISTORY_DATA);
	}
	if (!m_id.IsEmpty())
	{
		text.Format("user= %s\r\n", m_id);
	}
	DWORD state = 1;
	SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), state,"skype");


	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
	m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u_%s.indexfile.txt",
		tmppath, now.wYear, now.wMonth, now.wDay,
		now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum, "skype");

	WriteIndexFile("skype", "skype", text, m_dceprocotol->m_httpMissObj);
}

void HttpSession::WriteTaoBaoIndex()
{
	CString text;		

	m_id = ConverHttp2Html(m_id);
	ConvertUtf8toGbk(m_id,m_id.GetLength(),m_id);

	// 写活动历史
	if( v_stat && v_statOther)
	{
		HistoryRecord hisRec;
		CString tmpstrhis;
		hisRec.SetRecord(IDX_ID,		m_id);
		hisRec.SetRecord(IDX_USER,		m_id);
		WriteHisData("imsg_taobao", "imsg_taobao", &hisRec, m_dceprocotol->m_httpMissObj, HTTP_PROTOCOL);//, HISTORY_DATA);
	}
	if (!m_id.IsEmpty())
	{
		
		text.Format("user= %s\r\n", m_id);
		DWORD state = 1;
		SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), state,"imsg_taobao");
	}


	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
	m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u_%s.indexfile.txt",
		tmppath, now.wYear, now.wMonth, now.wDay,
		now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum, "skype");

	WriteIndexFile("imsg_taobao", "imsg_taobao", text, m_dceprocotol->m_httpMissObj);
}

void HttpSession::WriteTeUrlIndexFile()
{
	int error=0;
	try
	{
		LPCTSTR pro = "";
		LPCTSTR ver = "";
		CString URL;
		CString Cookie;
		CString Referer;
		CString text;		

		// 特浏类型
		switch(m_Requestline.m_Browser)
		{
		case	TE_WUJIE:					// 无界
			pro = "ultrabrowse";
			ver="u13.1-3版";
			break;
		case	TE_FREEGATE:				// 自由之门
			pro = "freegateurl";
			if (m_Requestline.m_BrowserVer <= 0x602)
			{
				if (!m_Requestline.m_URL.IsEmpty())
				{
					text.Format("url= %s\r\n", m_Requestline.m_URL);
				}
				if(!m_Requestline.m_Referer.IsEmpty())
				{
					text.Format("%s%s\r\n",(CString)text,m_Requestline.m_Referer);
				}
			}
			break;
		case	TE_NEWFREEGATE:				// 自由之门
			pro = "freegateurl";
			ver = "6.7及以上版本";
			if (m_Requestline.m_Referer.IsEmpty())
			{
				text.Format("url= %s\r\n", m_Requestline.m_URL);
			}
			else
			{
				text.Format("url= %s\r\nreferer= %s\r\n",
					m_Requestline.m_URL, m_Requestline.m_Referer);
			}
			break;
		case	TE_FREEGATE686p:
			pro = "freegateurl";
			ver = "6.86专业版";
			break;
		case	TE_FREEGATE689b3:
			pro = "freegateurl";
			ver = "自由门6.98-7.15或逍遥游1.3-1.8或动网通3.1-3.2版";
			break;

		case TE_FREEGATE707: 
			pro = "freegateurl";
			ver = "自由门7.07版";
			break;
		case TE_FREEGATE708:
			pro = "freegateurl";
			ver = "自由门7.08版";
			break;
		case TE_FREEGATE740:
			pro = "freegateurl";
			ver = "自由门7.40版";
			break;
		case TE_FREEGATE742:
			pro = "freegateurl";
			ver = "自由门7.42版";
			break;
		case	TE_DYNAPASS:				// 动网通
			pro = "dynapassurl";
			if (m_Requestline.m_Referer.IsEmpty())
			{
				text.Format("url= %s\r\n", m_Requestline.m_URL);
			}
			else
			{
				text.Format("url= %s\r\nreferer= %s\r\n",
					m_Requestline.m_URL, m_Requestline.m_Referer);
			}
			ver = "2.0";
			break;
		case	TE_FIREPHONENIX:			// 火凤凰
			pro = "firephoenix";
			ver = "3.1";

			break;
		case TE_OPENVPN:
			pro="openvpn";
			ver="2.2";						//无意义
			break;
		case TE_OPENSSH:
			pro="openssh";
			ver="4.3";						//无意义
			break;
		default:
			return;
		}
error=1;		

		if(CString(ver).GetLength() == 0)
		{
			switch(m_Requestline.m_BrowserVer)
			{
			case 0x100:
				ver = "1.0";
				break;
			case 0x600:
				ver = "6.0";
				break;
			case 0x602:
				ver = "6.2";
				break;
			case 0x603:
				ver = "6.3";
				break;
			case 0x608:
				ver = "6.8";
				break;
			case 0x609:
				ver = "6.9";
				break;
			case 0x0800:
				ver = "8.0";
			case 0x801:
				ver = "8.1";
				break;
			case 0x802:
				ver = "8.2";
				break;
			case 0x805:
				ver = "8.5";
				break;
			case 0x806:
				ver = "8.6";
				break;
			case 0x807:
				ver = "8.7";
				break;
			case 0x808:
				ver = "8.7以上版本";
				break;			
			case 0x1301:
				ver = "13.1";
				break;
			}
		}
error=3;
		// 写活动历史
		if( v_stat && v_statOther)
		{
			HistoryRecord hisRec;
			CString tmpstrhis;

	// 		hisRec.SetRecord(IDX_FORWTYPE,	"1");
	//  		hisRec.SetRecord(IDX_ID,		m_userid);
	//  		hisRec.SetRecord(IDX_PASS,		m_PASS);
	// 		if(m_stornum > 0)
	// 			hisRec.SetRecord(IDX_STATE, "上传");
	// 		else 
	// 			hisRec.SetRecord(IDX_STATE, "下载");
	//		hisRec.GetHisRecord(tmpstrhis);
	//		WriteArrToData(HTTP_PROTOCOL, HISTORY_DATA, tmpstrhis, NULL);
error=4;
			WriteHisData(pro, pro, &hisRec, m_dceprocotol->m_httpMissObj, HTTP_PROTOCOL);//, HISTORY_DATA);
		}

		text.Format("%sver= %s\r\n", (CString)text, ver);

		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u_%s.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum, pro);
error=5;
		WriteIndexFile(pro, pro, text, m_dceprocotol->m_httpMissObj);
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "WriteTeUrlIndexFile Error!! error=%d",error);
	}
}

BOOL HttpSession::SetIdcDomain()
{
	CString strISP;
	if (!m_Requestline.m_bproxy && !m_ProxyType)
	{
		if (m_dip.Mid(16).CompareNoCase("00080")!=0)
		{
			return FALSE;
		}
		if (k_IpQueryTable.GetLocalWebSite(m_dip.Left(15)))
		{
			CString str;	
			CString domain=SplitTopDomain(m_Requestline.m_Host);
			str.Format("%s\1%s",domain,m_dip.Left(15));
			v_IdcServerStat[HTTP_PROTOCOL].AddServer(str);
		}
	}
	return TRUE;
}

void HttpSession::DumpHttpFile(CString attflst)
{
	//	httpmail发邮件(相当于smtp)
	//	httpmail收邮件(相当于pop3)
	//	http_webmail 
	//	http上传 
	//	http下载 
	//	freegateup 自由之门上传
	//	freegatedn 自由之门下载
	//	dynapassup 动网通上传
	//	dynapassdn 动网通下载
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(90);
#endif
	FILE *fp=0;
	CString indexfilename;
	CString PRO;
	CString filename;
	CString* emlfield=0;
	CStringArray* filearray=0;
// 	BOOL caseflag = FALSE;
// 	BOOL indexflag = FALSE;
	BOOL isWriteFile = TRUE;
	CString tmpCaseID;
	BOOL bCheckSPL = TRUE;
	CString taobaotxt;
	if(m_Requestline.m_Action==HTTP_POST)
	{
#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(91);
#endif
		//判断是否淘宝登录包，并把协议写成imsg_taobao

		if (m_Requestline.m_URL.Find("/member/login.jhtml") >=0 &&
			m_Requestline.m_Host.Find(".taobao.com") >= 0)
		{
//			if(FormatTaobaoLogin(m_realfname,taobaotxt,m_spyaddr))
//			{
//				PRO = "imsg_taobao";
//			}
		}
		if(m_Requestline.m_pro=="webmail")
		{
			PRO = "webmail";
			m_Requestline.m_pro.Empty();
		}


		emlfield=&(m_Requestline.m_emlfield);
		m_indexfname = m_realfname + ".indexfile.txt";

		CString strurl;	// 需要匹配案件和预警的串
		strurl = m_Requestline.m_URL + "\r\n\r\n" + m_Requestline.m_Referer +
			"\r\n\r\n" + m_Requestline.m_Cookie + "\r\n\r\n" + m_Requestline.m_Cookie2 +
			"\r\n\r\n" + m_Requestline.m_SetCookie2 + "\r\n\r\n" + attflst;
		strurl.Format("%s \r\n\r\n %s",(CString)strurl, emlfield);
		strurl.MakeLower();	// 要匹配线索的URL必须转换成小写字符

		if (!PRO.IsEmpty())
		{
			;
		}
/*		else if(m_Requestline.m_Content_Type == "multipart/form-data")
		{
			m_OpenCaseFlg = v_opencasehttpup;
			m_OpenIndxFlg = v_openflaghttpup;
			m_OpenWarnFlg = v_openwarnhttpup;
			if (m_attfiles.GetSize()>0)
			{
				PRO="webmail";		// 有附件的算Webmail

				CString strISP;
				if (!m_Requestline.m_bproxy && !m_ProxyType && v_stat && v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
				{
					BOOL isIDC = TRUE;
					CString strDomain = m_Requestline.m_Host;
					BOOL isDomain = !(strDomain.GetLength()<4);
					int pos = strDomain.ReverseFind(':');		// zzzzzz:yyyy
					int port2 = atoi(m_dip.Right(5));
					if (pos>0)
					{
						int port1 = atoi(strDomain.Mid(pos+1));
						if (port1 == port2)
							strDomain.ReleaseBuffer(pos);
						else
							isIDC = FALSE;
					}
					else if (port2!=80)
						isIDC = FALSE;
					
					// 如果最后的一个点后面的如果不是字母就不要host段的数据
					if (isIDC)
					{
						pos = strDomain.ReverseFind('.');
						if (pos <= 0)
							isDomain = FALSE;
						else
						{
							isDomain = FALSE;
							char *tp = m_Requestline.m_Host.GetBuffer(0);
							for (int i=pos+1; i<m_Requestline.m_Host.GetLength(); i++)
							{
								if (tp[i]>':')
								{
									isDomain = TRUE;
									break;
								}
							}
						}
						CString str;
						if (isDomain)
						{
							strDomain.MakeLower();
							if (memcmp((LPCTSTR)strDomain, "www.", 4)!=0)
							{
								if (strDomain.Find('.')<0)
								{
									str.Format("%03u.%03u.%03u.%03u\1%s\1webmail\1WEB服务\1%s\1%s\1%s\1%s\1", 
										m_cdip[0], m_cdip[1], m_cdip[2], m_cdip[3],	
										strISP, m_dip.Right(5), "WEB方式发邮件",
										 TYPESTATE_POSSIBLE, INTERACT_DYNAMIC);
								}
								else
								{
									str.Format("%03u.%03u.%03u.%03u\1%s\1webmail\1WEB服务\1%s\1%s\1%s\1%s\1%s", 
										m_cdip[0], m_cdip[1], m_cdip[2], m_cdip[3],
										strISP, m_dip.Right(5), "WEB方式发邮件", 
										TYPESTATE_POSSIBLE, INTERACT_DYNAMIC, strDomain);
								}
							}
							else
							{
								str.Format("%03u.%03u.%03u.%03u\1%s\1webmail\1WEB服务\1%s\1%s\1%s\1%s\1%s", 
									m_cdip[0], m_cdip[1], m_cdip[2], m_cdip[3],
									strISP, m_dip.Right(5), "WEB方式发邮件", 
									TYPESTATE_POSSIBLE, INTERACT_DYNAMIC, strDomain);
							}
						}
						else
						{
							str.Format("%03u.%03u.%03u.%03u\1%s\1webmail\1WEB服务\1%s\1%s\1%s\1%s\1", 
								m_cdip[0], m_cdip[1], m_cdip[2], m_cdip[3],	strISP, m_dip.Right(5),
								"WEB方式发邮件", TYPESTATE_POSSIBLE, INTERACT_DYNAMIC);
						}
						v_IdcServerStat[HTTP_PROTOCOL].AddServer(str);
					}
				}
			}
			else
			{
				PRO = "httpup";	
			}
		}*/
		else if(m_Requestline.m_Content_Type=="message/rfc822" || m_Requestline.m_Content_Type=="message/rfc821")
		{
			m_OpenCaseFlg = v_opencasesmtp;
			m_OpenIndxFlg = v_openflagsmtp;
			m_OpenWarnFlg = v_openwarnsmtp;
			PRO="smtp";
		}
		else 
		{
			if (m_Requestline.m_useragent == "IMW WinHTTP AS")
				return;
			if (m_Requestline.m_Browser==TE_FREEGATE)
			{
				PRO="freegateup";
				m_OpenCaseFlg = v_opencasefgup;
				m_OpenIndxFlg = v_openflagfgup;
				m_OpenWarnFlg = v_openwarnfgup;
			}
			else if (m_Requestline.m_Browser==TE_DYNAPASS)
			{
				PRO="dynapassup";
				m_OpenCaseFlg = v_opencasedynaup;
				m_OpenIndxFlg = v_openflagdynaup;
				m_OpenWarnFlg = v_openwarndynaup;
			}
			else 
			{
// 				if(CheckFreeGate63(m_sip.Left(15), m_dip.Left(15)))
// 				{
// 					m_Requestline.m_Browser = TE_FREEGATE;
// 					m_Requestline.m_BrowserVer = 0x603;
// 					m_OpenCaseFlg = v_opencasefgup;
// 					m_OpenIndxFlg = v_openflagfgup;
// 					m_OpenWarnFlg = v_openwarnfgup;
// 					PRO = "freegateup";
// 				}
// 				else
				{
					m_OpenCaseFlg = v_opencasehttpup;
					m_OpenIndxFlg = v_openflaghttpup;
					m_OpenWarnFlg = v_openwarnhttpup;
					PRO ="httpup";

				}
			}
		}

		// 匹配案件和预警
		DWORD state = 0x10000;		// 需要匹配ip+URL
		SearchCaseOrWarn(this, (char*)(LPCTSTR)strurl, strurl.GetLength(), state,PRO);

		if (PRO=="httpup")
		{
			// 拆解上传
			DATEHEADER packh;
			SplitHttpupToHis(m_Requestline.m_URL, m_Requestline.m_Content_Type, this, "httpup", packh,"", 0);
		}

		if (/*(m_CaseArr.GetSize() == 0) && */(!m_rubbishflag))
		{
			// 匹配URL的屏蔽信息
			m_rubbishflag = SearchBurrishByURL(strurl);
		}


		isWriteFile = (m_CaseArr.GetSize() > 0 || (!m_rubbishflag && m_OpenIndxFlg));
	}
	else if (m_Requestline.m_Action==HTTP_GET)
	{
#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(92);
#endif
	
		emlfield=&(m_Responseline.m_emlfield);

		m_indexfname = m_realfname+".indexfile.txt";

		CString text;	// 需要匹配案件和预警的串
		text = m_Requestline.m_URL + ' ' + m_Requestline.m_Referer + ' ' + m_Requestline.m_Cookie +
			' ' + m_Requestline.m_Cookie2 + ' ' + m_Requestline.m_SetCookie2 /*+ ' ' + emlfield*/;
		text.Format("%s____%s",(CString)text, emlfield);
		text.MakeLower();	// 要匹配线索的URL必须转换成小写字符

		// 匹配案件和预警
		DWORD state = 0x10000;		// 需要匹配ip+URL

		if (/*(m_CaseArr.GetSize() == 0) && */(!m_rubbishflag))
		{
			// 匹配URL的屏蔽信息
			m_rubbishflag = SearchBurrishByURL(text);
		}

		if(m_Responseline.m_Content_Type=="message/rfc822" || m_Responseline.m_Content_Type=="message/rfc821")
		{
			m_OpenCaseFlg = v_opencasepop3;
			m_OpenIndxFlg = v_openflagpop3;
			m_OpenWarnFlg = v_openwarnpop3;
 			PRO = "pop3";
			SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), state,PRO);

// 			isWriteFile = ((m_CaseArr.GetSize() > 0 &&caseflag) || (!m_rubbishflag&&indexflag));

//lp			isWriteFile = ((!CaseID.IsEmpty()&&caseflag) || (!rubbishflag&&indexflag));
		}

	
		else if (m_Requestline.m_Browser==TE_FREEGATE)
		{
			bCheckSPL=FALSE;
			m_OpenCaseFlg = v_opencasefgdn;
			m_OpenIndxFlg = v_openflagfgdn;
			m_OpenWarnFlg = v_openwarnfgdn;
			PRO="freegatedown";
			SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), state,PRO);

			// 写URL
//			WriteFreeGateUrlFile("freegateurl", v_opencasefgurl, v_openflagfgurl);
			if (m_Responseline.m_filetype==1)
				return;
			ASSERT(m_Requestline.m_BrowserVer < 0x603);
			isWriteFile = ((m_CaseArr.GetSize() > 0 && m_OpenCaseFlg) || (!m_rubbishflag&& m_OpenIndxFlg));
			
//lp			isWriteFile = ((!CaseID.IsEmpty()&&caseflag) || (!rubbishflag&&indexflag));
		}
		else if (m_Requestline.m_Browser==TE_DYNAPASS)
		{
			bCheckSPL=FALSE;
			m_OpenCaseFlg = v_opencasedynadn;
			m_OpenIndxFlg = v_openflagdynadn;
			m_OpenWarnFlg = v_openwarndynadn;
// 			caseflag = v_opencasedynadn;
// 			indexflag = v_openflagdynadn;
 			PRO="dynapassdown";
			SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), state,PRO);

// 			WriteFreeGateUrlFile("dynapassurl", v_opencasedynaurl, v_openflagdynaurl);
// 			if (m_Responseline.m_filetype==1)
// 				return;
// 			isWriteFile = ((m_CaseArr.GetSize() > 0 &&caseflag) || (!m_rubbishflag&&indexflag));

//lp			isWriteFile = ((!CaseID.IsEmpty()&&caseflag) || (!rubbishflag&&indexflag));
		}
		else
		{
			bCheckSPL=FALSE;
			m_OpenCaseFlg = v_opencasehttpdown;
			m_OpenIndxFlg = v_openflaghttpdown;
			m_OpenWarnFlg = v_openwarnhttpdown;
// 			caseflag = v_opencasehttpdown;
// 			indexflag = v_openflaghttpdown;
			PRO="httpdown";
			SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), state,PRO);

			if (m_Responseline.m_filetype==2)
			{
				// 匹配IDC网站类型关键字
				// 首先匹配IDC服务器IP
				CString strISP;
				if (!m_Requestline.m_bproxy && !m_ProxyType && v_stat && v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
				{
					// 匹配IDC网站类型关键字
					BOOL isIDC = TRUE;
					CString strDomain = m_Requestline.m_Host;
					BOOL isDomain = !(strDomain.GetLength()<4);
					int pos = strDomain.ReverseFind(':');		// zzzzzz:yyyy
					int port2 = atoi(m_dip.Right(5));
					if (pos>0)
					{
						int port1 = atoi(strDomain.Mid(pos+1));
						if (port1 == port2)
							strDomain.ReleaseBuffer(pos);
						else
							isIDC = FALSE;
					}
					else if (port2!=80)
						isIDC = FALSE;

					if (isIDC)
					{
						CString type = SearchIDCHttpKey(m_realfname);
						if (!type.IsEmpty())
						{
							// 写IDCServer表
							CString str;
							LPCTSTR strInteract;
							if(m_Requestline.m_URL.Find('?') > 0)
								strInteract = INTERACT_DYNAMIC;
							else
								strInteract = INTERACT_STATIC;
							if (isDomain)
							{
								strDomain.MakeLower();
								if (memcmp((LPCTSTR)strDomain, "www.", 4)!=0)
								{
									if (strDomain.Find('.')<0)
									{
										str.Format("%03u.%03u.%03u.%03u\1%s\1get\1WEB服务\1%s\1%s\1%s\1%s\1", 
											m_cdip[0], m_cdip[1], m_cdip[2], m_cdip[3],	
											strISP, m_dip.Right(5),
											type, TYPESTATE_POSSIBLE, strInteract);
									}
									else
									{
										str.Format("%03u.%03u.%03u.%03u\1%s\1get\1WEB服务\1%s\1%s\1%s\1%s\1%s", 
											m_cdip[0], m_cdip[1], m_cdip[2], m_cdip[3],
											strISP, m_dip.Right(5),
											type, TYPESTATE_POSSIBLE, strInteract, strDomain);
									}
								}
								else
								{
									str.Format("%03u.%03u.%03u.%03u\1%s\1get\1WEB服务\1%s\1%s\1%s\1%s\1%s", 
										m_cdip[0], m_cdip[1], m_cdip[2], m_cdip[3],
										strISP, m_dip.Right(5),
										type, TYPESTATE_POSSIBLE, strInteract, strDomain);
								}
							}
							else
							{
								str.Format("%03u.%03u.%03u.%03u\1%s\1get\1WEB服务\1%s\1%s\1%s\1%s\1", 
									m_cdip[0], m_cdip[1], m_cdip[2], m_cdip[3],	strISP, m_dip.Right(5),
									type, TYPESTATE_POSSIBLE, strInteract);
							}
							v_IdcServerStat[HTTP_PROTOCOL].AddServer(str);
						}
					}
				}

				if ((m_CaseArr.GetSize() <= 0) && (m_ProxyType != 15))
				{
					// 首先匹配回溯线索中的IP+URL
					isWriteFile = SearchIndexByIPURL(m_sip.Left(15), m_dip.Left(15), text, text.GetLength());
					if(!isWriteFile)
					{
						// 不是案件 且 不是回溯的IP+URL 就匹配预设关键字
						isWriteFile = SearchHttpFilter(m_realfname,
							m_sip,m_dip,m_Requestline.m_URL,m_Requestline.m_Host,"httpdown",NULL);
					}
				}
			}
			else 
			{
				if (m_Responseline.m_filetype == 1)
				{
					// 图片和脚本
					isWriteFile = ((m_CaseArr.GetSize() > 0) && (v_opencasehttpimage));
				}
				else
				{
					// 其他类型的文件
					isWriteFile = (m_CaseArr.GetSize() > 0);
				}
			}
		}
	}
	else
	{
		return;
	}
//#ifdef		DUMPALLHTTPDOWN
	isWriteFile = TRUE;
//#endif
	if(PRO == "httpdown")
	{
//		Deal360MID(m_Requestline, *this);

		
		if(IsPopoLogin(m_Requestline, *this))
		{
			;
		}
		else if(IsWebQQLogin(m_Requestline, *this))
		{
			;
		}
	

	}
	else if (PRO == "httpup")
	{
		if(m_Requestline.m_Content_Type.CompareNoCase("application/octet-stream") == 0 )
			isWriteFile = FALSE;	/// 上传且二进制流不输出
		if(m_Requestline.m_Content_Type.IsEmpty() )
			m_Requestline.m_bproxy = FALSE;
	}
	if (isWriteFile)
	{// 写对照文件

#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(93);
#endif
		int hlen = WriteField(m_dceprocotol->m_HttpFileBuffer,		"代理",			m_Requestline.m_bproxy? "TRUE" : "");
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"url",			m_Requestline.m_URL);

//处理监管网站

		hlen += g_moniterweb.GetMoniterweb(m_dceprocotol->m_HttpFileBuffer+hlen,m_Requestline.m_URL);

		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"referer",		m_Requestline.m_Referer);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"cookie",		m_Requestline.m_Cookie);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"cookie_clone",	m_Requestline.m_Cookie_Clone);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"cookie2",		m_Requestline.m_Cookie2);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"setcookie2",	m_Requestline.m_SetCookie2);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"user-agent",	m_Requestline.m_useragent);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"accept-language",	m_Requestline.m_Language);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"accept-charset",	m_Requestline.m_Charset);

		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"from",	m_Requestline.m_from);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"user",	m_Requestline.m_from);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"to",	m_Requestline.m_to);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"cc",	m_Requestline.m_cc);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"bcc",	m_Requestline.m_bcc);
		hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"subject",	m_Requestline.m_subject);
		if (m_Requestline.m_bRbmm)
		{
			hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen, "rmb",	"true");
		}

		if(m_Requestline.m_iswap)
		{
			hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen, "wap",	"true");
		}

// 		if(!m_Requestline.m_WeiBoName.IsEmpty())
// 		{
// 			hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen, "weiboname",	m_Requestline.m_WeiBoName);
// 		}


		if(m_Requestline.m_Action==HTTP_POST)	// "POST"
		{
			hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"content-type",	m_Requestline.m_Content_Type);
		}
		else
		{
			hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"content-type",	m_Responseline.m_Content_Type);
		}
		if (m_Requestline.m_Browser==TE_FREEGATE)
		{
			if (m_Requestline.m_BrowserVer==0x600)
			{
				hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"ver",	"6.0");
			}
			else if (m_Requestline.m_BrowserVer==0x601)
			{
				hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"ver",	"6.1");
			}
			else if (m_Requestline.m_BrowserVer==0x602)
			{
				hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"ver",	"6.2");
			}
			else if (m_Requestline.m_BrowserVer==0x603)
			{
				hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"ver",	"6.3");
			}

		}
		else if (m_Requestline.m_Browser==TE_DYNAPASS)
		{
			if (m_Requestline.m_BrowserVer==0x200)
			{
				hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen,	"ver",	"2.0");
			}
		}

		memset(m_dceprocotol->m_HttpFileBuffer+hlen, 0, 8);
		CString text = CString(m_dceprocotol->m_HttpFileBuffer, hlen);
		if (PRO == "imsg_taobao")
		{
			text = taobaotxt;
		}
		if (PRO == "httpup")
		{
			if((m_CaseArr.GetSize()==0) && (PRO == "httpup"))
			{
				CString useragent = m_Requestline.m_useragent;
				useragent.MakeLower();
				// 非案件 非浏览器发出POST丢弃
				if (-1 == (g_ExplorerType1.find(useragent, useragent.GetLength()))
					&& (-1 == (g_ExplorerType2.find(useragent, useragent.GetLength()))))
				{
//					return;
				}
			}
		}
		if(!m_Requestline.m_WeiBoName.IsEmpty())
		{
			PRO=m_Requestline.m_WeiBoName;
//			hlen += WriteField(m_dceprocotol->m_HttpFileBuffer+hlen, "weiboname",	m_Requestline.m_WeiBoName);
		}

/*		if( v_stat && v_statOther &&
			(PRO.CompareNoCase("dynapassdown")==0||
			PRO.CompareNoCase("freegatedown")==0||
			PRO.CompareNoCase("dynapassup")==0||
			PRO.CompareNoCase("freegateup")==0||
			PRO.CompareNoCase("freegateurl")==0||
			PRO.CompareNoCase("ultrabrowse")==0))
		{
			HistoryRecord hisRec;
			CString tmpstrhis;
			WriteHisData(PRO, PRO, &hisRec, g_httpMissObj, HTTP_PROTOCOL);//, HISTORY_DATA);
		}*/

		if (PRO.CompareNoCase("webmail")==0)
		{
			m_OpenIndxFlg = v_openflagwebmail;

			HistoryRecord hisRec;
			CString tmpstrhis;
			CString tmpusername;
			tmpusername=SplitEmlName(m_Requestline.m_from,tmpusername,TRUE);
			if (!tmpusername.IsEmpty())
			{
				hisRec.SetRecord(IDX_USER,tmpusername);
				hisRec.SetRecord(IDX_ID,tmpusername);
				tmpusername=SplitEmlName(m_Requestline.m_to,tmpusername,TRUE);
				hisRec.SetRecord(IDX_CONTACT,tmpusername);

				WriteHisData(PRO, PRO, &hisRec, m_dceprocotol->m_httpMissObj, HTTP_PROTOCOL);//, HISTORY_DATA);
			}

		}
#ifdef		_DEBUG
		if (m_Requestline.m_Action == HTTP_POST)
		{
			printf("dump file Content_length=%d\n", m_Requestline.m_Content_length);
		}
		else
		{
			printf("dump file Content_length=%d\n", m_Responseline.m_Content_length);
		}
#endif
		WriteIndexFile(PRO, PRO, text, m_dceprocotol->m_httpMissObj);


#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(97);
#endif

	}
	return;
}

void HttpSocketMap::ClearHttpSocketMap(time_t now)
{
	HttpInfo tmpinfo;
	SOCKETKEY tmpkey;
	POSITION pos = GetStartPosition();
	while(pos)
	{
		GetNextAssoc(pos, tmpkey, tmpinfo);
		if (abs(now - tmpinfo.firsttime)>=60)	// 删除1分钟前的数据
			RemoveKey(tmpkey);
	}
}

void HttpSocketMap::AddHttpRequest(SOCKETKEY& key, CString reqinfo)
{
	HttpInfo tmpinfo1, tmpinfo;
	tmpinfo.firsttime = time(0);
	tmpinfo.info = reqinfo;
	if (!Lookup(key, tmpinfo1))
	{
		// 新的Socket
		SetAt(key, tmpinfo);
	}
}

void HttpSocketMap::AddHttpResponse(DATEHEADER& packh)
{
	SOCKETKEY key;
	memcpy(&key.dip, packh.m_sip, 4);
	memcpy(&key.sip, packh.m_dip, 4);
	key.dport = packh.m_sport;
	key.sport = packh.m_dport;
	HttpInfo tmpinfo;
	if (this->Lookup(key, tmpinfo))
	{
		if (!tmpinfo.info.IsEmpty())
		{
			// 增加统计数据到队列
#ifdef		_DEBUG
			printf("ADD IDCWEB %s\n", tmpinfo.info);
#endif
			v_IdcServerStat[HTTP_PROTOCOL].AddServer(tmpinfo.info);
		}
		// 删除哈希单元
		RemoveKey(key);	
	}
}
