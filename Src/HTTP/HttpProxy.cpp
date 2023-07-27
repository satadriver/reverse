// HttpProxy.cpp: implementation of the HttpProxy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "http.h"
#include "HttpProxy.h"
#include "../imsg/TaoBaoDecode.h"
// ClassHashHttpImsgProxy HttpImsgProxySocketMap;

Search SerMsn("MSN Messenger ");
Search LiveMsger("Live Messenger ");

//*************************************************
//	函数名称:		IsImsgProxy
//	描述:			判断一个Http会话是否是IMSG代理
//	输入参数:		pPacketInfo Http头信息
//					pPacket Http承载的数据
//					len		Http承载的数据长度
//	输出参数:		
//	返回值:			是Http代理就返回TRUE, 否则返回FALSE
//	其它:			
//*************************************************
BOOL IsImsgProxy(HttpCommInfo* pPacketInfo, const char* pPacket, int len)
{
	// 判断MSN

	// 情况二
	// POST /gateway/gateway.dll?SessionID=191308999.20056 HTTP/1.1
	// Accept: */*
	// Content-Type: text/xml; charset=utf-8
	// Content-Length: 94
	// User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; MSN Messenger 7.0.0813)
	// Host: 207.46.3.13
	// Connection: Keep-Alive
	// Cache-Control: no-cache
	// \r\n\r\n
	// SYN 5 2005-05-26T22:49:25.0170000-07:00 2005-05-26T22:49:25.0970000-07:00
	// GCF 6 Shields.xml

	if ((pPacketInfo->m_Content_Type=="application/x-msn-messenger")
		|| (SerMsn.find(pPacketInfo->m_useragent, pPacketInfo->m_useragent.GetLength())>0)
		|| (LiveMsger.find(pPacketInfo->m_useragent, pPacketInfo->m_useragent.GetLength())>0))
	{
		// 是MSN代理
		return TRUE;
	}

	// 判断YMSG
	if (((len > 4) && (memcmp(pPacket, "YMSG", 4) == 0))
		|| ((len > 8) && (memcmp(pPacket+4, "YMSG", 4) == 0)))
	{
		// 是YMSG代理
		return TRUE;
	}
	return FALSE;
}

HttpImsgProxy::HttpImsgProxy()
:SocketSession()
{
	
}

BOOL HttpImsgProxy::CloseSession(time_t nowtm)
{
	try
	{
#ifdef	ADD_RUNSTATE_INFO
		AutoDestory AutoDebug(&v_httprunstate, 3, -3);
#endif
		LPCTSTR pro;
		if (memcmp(m_imsgtype, "msn", 3)==0)
		{
			m_OpenCaseFlg = v_opencasemsn;
			m_OpenIndxFlg = v_openflagmsn;
			m_OpenWarnFlg = v_openwarnmsn;
			pro = _T("imsg_msn");
		}
		else if (memcmp(m_imsgtype, "ymsg", 4) == 0)
		{
			m_OpenCaseFlg = v_opencaseyahoo;
			m_OpenIndxFlg = v_openflagyahoo;
			m_OpenWarnFlg = v_openwarnyahoo;
			pro = _T("imsg_ymsg");
		}
		else
		{
			return TRUE;
		}

		// 写实体文件
		if (!m_msgtext.IsEmpty())
		{
			WriteImsgFile();
		}
		m_msgtext.Empty();
		if (!m_realfname.IsEmpty() && GetFileLength(m_realfname) > 0)
		{
			CString text;
			if (!m_userid.IsEmpty())
			{
				text.Format("user= %s\r\n", m_userid);
			}

			// 匹配案件和预警
			DWORD state = 0x10000;
			SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), state);

			// 写对照文件
			m_indexfname = m_realfname + ".indexfile.txt";
			WriteIndexFile(pro, pro, text, m_dceprocotol->m_httpMissObj);

		}
	}
	catch(...)
	{
		WriteLog(HTTPPRO, "ImsgProxyCloseSession() Error!!");
	}
	return TRUE;	// 一个会话只保持5分钟
}

void HttpImsgProxy::WriteImsgFile()
{
	if (m_realfname.IsEmpty())
	{
		CString pro;
		pro.Format("imsg_%s", m_imsgtype);

		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
		m_realfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++m_dceprocotol->m_httpFileNum, pro);
	}
	FILE* fp = fopen(m_realfname, "ab");
	if (fp)
	{
		fwrite((LPCTSTR)m_msgtext, 1, m_msgtext.GetLength(), fp);
		fclose(fp);
	}

}

//将 代理信息插入到hash
BOOL InsertImsgProxy(IMSGPROXY &imsgproxy, HttpCommInfo &PackInfo,
					 LPCTSTR spyaddr, HttpSession* pSession)
{
int error=0;
	ASSERT(pSession != NULL);
	try
	{
#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(55);
#endif
		BOOL caseflg;
		CString key;
		if(PackInfo.m_Content_Type=="application/x-msn-messenger" && imsgproxy.datalen>3)
		{
			imsgproxy.ImsgType = "msn";
			key.Format("%s_%s_%s", imsgproxy.ImsgType, imsgproxy.sip, imsgproxy.dip);
			caseflg = v_opencasemsn;
		}
		else if ((PackInfo.m_emlfield == "text/xml; charset=utf-8")
			&& ((SerMsn.find(PackInfo.m_useragent, PackInfo.m_useragent.GetLength())>0)
				|| (LiveMsger.find(PackInfo.m_useragent, PackInfo.m_useragent.GetLength())>0))
			&& (imsgproxy.datalen>4 && imsgproxy.pPacketData[3] == ' '))
		{
			// POST /gateway/gateway.dll?SessionID=191308999.20056 HTTP/1.1
			// Accept: */*
			// Content-Type: text/xml; charset=utf-8
			// Content-Length: 94
			// User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; MSN Messenger 7.0.0813)
			// Host: 207.46.3.13
			// Connection: Keep-Alive
			// Cache-Control: no-cache
			// \r\n\r\n
			// SYN 5 2005-05-26T22:49:25.0170000-07:00 2005-05-26T22:49:25.0970000-07:00
			// GCF 6 Shields.xml
			imsgproxy.ImsgType = "msn";
			key.Format("%s_%s_%s", imsgproxy.ImsgType, imsgproxy.sip, imsgproxy.dip);
			caseflg = v_opencasemsn;
		}
		else if (imsgproxy.datalen>4 && (memcmp(imsgproxy.pPacketData, "YMSG", 4) ==0))
		{
			imsgproxy.ImsgType = "ymsg";
			caseflg = v_opencaseyahoo;
		}
		else if (imsgproxy.datalen>8 && (memcmp(imsgproxy.pPacketData+4, "YMSG", 4) == 0))
		{
			imsgproxy.datalen-=4;
			imsgproxy.pPacketData+=4;
			imsgproxy.ImsgType = "ymsg";
			caseflg = v_opencaseyahoo;
		}
		else
		{
			return FALSE;
		}
error=2;
		// 查询代理Socket
		HttpImsgProxy *s = 0;
error=21;
		if (!key.IsEmpty() && (pSession->m_dceprocotol->m_httpMissObj->m_SocketMap.Lookup(key, (SocketSession*&)s)))
		{
			imsgproxy.userid = s->m_userid;
			imsgproxy.linkman = s->m_linkman;
			imsgproxy.bReaord = s->m_bRecord;
		}
		else
		{
			imsgproxy.bReaord = FALSE;
		}
error=3;
#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(56);
#endif
		imsgproxy.pmsgbuf = pSession->m_dceprocotol->m_CHUNKBUFFER;
		memset(imsgproxy.pmsgbuf, 0, 8);
		imsgproxy.msglen = 0;
//		if (CheckImsgProxy(imsgproxy))
error=4;
		if (CheckImsgProxy(&imsgproxy))
		{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(57);
#endif

			if (s != NULL)
			{
				if (s->m_userid.GetLength() < imsgproxy.userid.GetLength())
				{
					s->m_userid = imsgproxy.userid;
				}
				if (s->m_linkman.GetLength() < imsgproxy.linkman.GetLength())
				{
					s->m_linkman = imsgproxy.linkman;
				}
				if (imsgproxy.msglen > 0)
				{
					s->m_msgtext += imsgproxy.pmsgbuf;
				}
			}
			else
			{
				// 建立ImsgProxySession
				if (memcmp(imsgproxy.ImsgType, "ymsg", 4) == 0)
				{
					if (!imsgproxy.userid.IsEmpty())
						key.Format("ymsg_%s", imsgproxy.userid);
					else
						return TRUE;
error=6;
					if (pSession->m_dceprocotol->m_httpMissObj->m_SocketMap.Lookup(key, (SocketSession*&)s))
					{
//						s->m_msgtext += imsgproxy
						s->m_bRecord = imsgproxy.bReaord;
						return TRUE;
					}
				}
error=5;
				ASSERT(s==NULL);
				s = new HttpImsgProxy;
				s->m_dceprocotol=pSession->m_dceprocotol;

 				s->m_dataSrc = imsgproxy.m_dataSrc;
				s->m_AuthInfo.m_UserAccount1 = pSession->m_AuthInfo.m_UserAccount1;
				s->m_AuthInfo.m_Phone1 = pSession->m_AuthInfo.m_Phone1;
				s->m_AuthInfo.m_UserAccount2 = pSession->m_AuthInfo.m_UserAccount2;
				s->m_AuthInfo.m_Phone2 = pSession->m_AuthInfo.m_Phone2;
				s->m_ProxyType = imsgproxy.proxytype;
				s->m_sip = imsgproxy.sip;
				s->m_dip = imsgproxy.dip;
				memcpy(s->m_csip, imsgproxy.csip, 4);
				memcpy(s->m_cdip, imsgproxy.cdip, 4);
				s->m_begfiletm = imsgproxy.packettm;
				s->m_lastpacktm = imsgproxy.packettm;
				s->m_smac = imsgproxy.smac;
				s->m_dmac = imsgproxy.dmac;
#ifdef		_DEBUG
				s->m_closetm = imsgproxy.packettm+300;
#else
				s->m_closetm = imsgproxy.packettm+300;
#endif
				s->m_imsgtype = imsgproxy.ImsgType;
				s->m_spyaddr = spyaddr;
				s->m_userid = imsgproxy.userid;
				s->m_linkman = imsgproxy.linkman;
				s->m_msgtext = imsgproxy.pmsgbuf;
//				s->m_msgtext = imsgproxy.msgtext;
//				s->m_vLanID = imsgproxy.vlanid;
				s->m_bRecord = imsgproxy.bReaord;
				pSession->m_dceprocotol->m_httpMissObj->m_SocketMap.SetAt(key, (SocketSession*&)s);
			}
		}
#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(58);
#endif
		return TRUE;
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "InsertImsgProxy() Error!!=%d",error);
		return FALSE;
	}
}

//void InsertMsnProxy(IMSGPROXY &imsgproxy , CString caseid,int limittype,CString strObjid  ,CString LimitAddr,int AllWatchFlg,CString UserAccount,CString Phone,CString spyaddr)
//{
//	return ;
//	CString strKey;
//	CString strInfo;
//	HttpImsgProxy *s=0;
//	
//	try
//	{
//		ASSERT(FALSE);
////		strInfo=Process_MsnProxy(imsgproxy);
//	
//		if( strInfo!="")
//		{
//			strKey.Format("msn_%s_%s",(imsgproxy.Direction==1)?imsgproxy.sip:imsgproxy.dip,
//				(imsgproxy.Direction==1)?imsgproxy.dip:imsgproxy.sip);
//			if(HttpImsgProxySocketMap.Lookup(strKey,s)>0)
//			{
//				if(s>0)
//				{
//					ImsgPacketInfo yahooinfo;
//					yahooinfo.starttm =imsgproxy.packettm;
//					yahooinfo.strInfo=strInfo;
//					if(s->ImsgInfoLst.size() ==0)
//						s->starttm=imsgproxy.packettm;
//					s->ImsgInfoLst.push_back(yahooinfo);
//					if(s->CaseID=="" && caseid !="")
//					{
//						s->CaseID=caseid;
//						s->LimitType=limittype;
//						s->ObjectID=strObjid;
//						s->AllWatchFlg=AllWatchFlg;
//						s->LimitAddr=LimitAddr;		
//					}
//				}
//			}
//			else if(imsgproxy.userid!="" )
//			{
//				s=new HttpImsgProxy;
//				s->strKey=strKey;
//				ImsgPacketInfo yahooinfo;
//				yahooinfo.starttm =imsgproxy.packettm;
//				yahooinfo.strInfo=strInfo;
//				if(s->ImsgInfoLst.size() ==0)
//					s->starttm=imsgproxy.packettm;
//				s->ImsgInfoLst.push_back(yahooinfo);
//				if(s->CaseID=="" && caseid !="")
//				{
//					s->CaseID=caseid;
//					s->LimitType=limittype;
//					s->ObjectID=strObjid;
//					s->AllWatchFlg=AllWatchFlg;
//					s->LimitAddr=LimitAddr;		
//				}		
//				s->imsgid=imsgproxy.userid;
//				s->starttm=imsgproxy.packettm;
//				s->sip=(imsgproxy.Direction==1)?imsgproxy.sip:imsgproxy.dip;
//				s->dip=(imsgproxy.Direction==1)?imsgproxy.dip:imsgproxy.sip;
//				s->strPRO="httpmsn"		;		//httpmsn httpymsg httpicqaol
//				s->imsgtype="imsg_msn"		;		//msn ymsg icqaol	
//				s->UserAccount=UserAccount	;
//				s->Phone=Phone				;
//				s->spyaddr=spyaddr;
//				HttpImsgProxySocketMap.SetAt(strKey,s);
//			}
//		}
//	}
//	catch (...)
//	{
//		WriteLog(strHttp, "InsertMsnProxy() Error!!");
//	}
//}
//
//void InsertYahooProxy(IMSGPROXY &imsgproxy , CString caseid,int limittype,CString strObjid  ,CString LimitAddr,int AllWatchFlg,CString UserAccount,CString Phone,CString spyaddr)
//{
//	return ;
//	CString strKey;
//	CString strInfo;
//	HttpImsgProxy *s=0;
//
//	try
//	{
//		ASSERT(FALSE);
//		
////		strInfo=Process_YahooProxy(imsgproxy);
//
//		if(imsgproxy.userid!="" && strInfo!="")
//		{
//			if(imsgproxy.userid=="服务器")
//				return;
//			strKey.Format("yahoo_%s",imsgproxy.userid);
//			if(HttpImsgProxySocketMap.Lookup(strKey,s)>0)
//			{
//				if(s>0)
//				{
//					ImsgPacketInfo yahooinfo;
//					yahooinfo.starttm =imsgproxy.packettm;
//					yahooinfo.strInfo=strInfo;
//					if(s->ImsgInfoLst.size() ==0)
//						s->starttm=imsgproxy.packettm;
//					s->ImsgInfoLst.push_back(yahooinfo);
//					if(s->CaseID=="" && caseid !="")
//					{
//						s->CaseID=caseid;
//						s->LimitType=limittype;
//						s->ObjectID=strObjid;
//						s->AllWatchFlg=AllWatchFlg;
//						s->LimitAddr=LimitAddr;		
//					}
//				}
//			}
//			else
//			{
//				s=new HttpImsgProxy;
//				s->strKey=strKey;
//				ImsgPacketInfo yahooinfo;
//				yahooinfo.starttm =imsgproxy.packettm;
//				yahooinfo.strInfo=strInfo;
//				if(s->ImsgInfoLst.size() ==0)
//					s->starttm=imsgproxy.packettm;
//				s->ImsgInfoLst.push_back(yahooinfo);
//				if(s->CaseID=="" && caseid !="")
//				{
//					s->CaseID=caseid;
//					s->LimitType=limittype;
//					s->ObjectID=strObjid;
//					s->AllWatchFlg=AllWatchFlg;
//					s->LimitAddr=LimitAddr;		
//				}		
//				s->spyaddr=spyaddr;
//				s->imsgid=imsgproxy.userid;
//				s->starttm=imsgproxy.packettm;
//				s->sip=(imsgproxy.Direction==1)?imsgproxy.sip:imsgproxy.dip;
//				s->dip=(imsgproxy.Direction==1)?imsgproxy.dip:imsgproxy.sip;
//				s->strPRO="httpymsg"		;		//httpmsn httpymsg httpicqaol
//				s->imsgtype="imsg_ymsg"		;		//msn ymsg icqaol	
//				s->UserAccount=UserAccount	;
//				s->Phone=Phone				;
//				HttpImsgProxySocketMap.SetAt(strKey,s);
//			}
//		}
//	}
//	catch (...)
//	{
//		WriteLog(strHttp, "InsertYahooProxy() Error!!");
//	}
//}
BOOL FormatTaobaoLogin(CString filename,CString &taobaotxt,LPCTSTR spyaddr)
{
	CString str;
	char* ptxt;
	char buf[10*1024];
	int tlen=GetFileLength(filename);

	FILE * fs=0;
	if(tlen>0)
	{
		fs=fopen(filename,"rb");
		if(fs>0	)
		{
			if (tlen>=sizeof(buf)-10)
				ptxt = str.GetBufferSetLength(tlen+10);
			else
				ptxt = buf;
			memset(ptxt,0,tlen+10);
			fread(ptxt,1,tlen,fs);
			fclose(fs);
		}
	}


	CString username,pass;
	CString content;
	content.Format("%s",ptxt);
	int ifind = content.Find("TPL_username=");
	if (ifind < 0)
	{
		return false;
	}
	content.Format("%s",content.Mid(ifind+13));
	ifind = content.Find("\xA");
	if (ifind < 0)
	{
		return false;
	}
	username.Format("%s",content.Left(ifind));
	ifind = content.Find("TPL_password=");
	if (ifind < 0)
	{
		return false;
	}
	content.Format("%s",content.Mid(ifind+13));
	ifind = content.Find("\xA");
	if (ifind < 0)
	{
		return false;
	}
	pass.Format("%s",content.Left(ifind));

	CString tmpstr;
	CStringArray tmparr;

	if (pass.Find("3DES") >= 0)
	{
		int hh = pass.ReverseFind('_');
		char despass[50];
		memset(despass,0,50);
//		CString despass;
		sprintf(despass, "%s" ,pass.Mid(hh+1));
//		int len =despass.GetLength();
		int len =strlen(despass);
	WriteLog(HTTPPRO,"ptxt:%s",ptxt);

	WriteLog(HTTPPRO,"despass pass:%s",despass);
		
		char	Result[MAXLEN],outbuf[MAXLEN];
		memset(Result,0,MAXLEN);
		memset(outbuf,0,MAXLEN);

		hexstrtoint10str((char*)/*(LPCTSTR)*/despass, Result);
	WriteLog(HTTPPRO,"hexstrtoint10str despass Result:%s",Result);

		Dec_TaoBao_Pass((unsigned char*)Result,(unsigned char*)outbuf,strlen(Result));
	WriteLog(HTTPPRO,"despass Result:%s",Result);

	if (strlen(outbuf) < 1)
	{
		return FALSE;
	}

		taobaotxt.Format("user= %s\r\npass= %s\r\nstat= 获得TAOBAO密码\r\n",username,outbuf);
		tmpstr.Format("%s\1%s",username,outbuf);
	}
	else
	{
		if (pass.GetLength()<=0)
		{
			return FALSE;
		}
		taobaotxt.Format("user= %s\r\npass= %s\r\nstat= 获得TAOBAO密码\r\n",username,pass);
		tmpstr.Format("%s\1%s",username,pass);
		
	}

	tmparr.Add(tmpstr);
	WriteArrToData(HTTP_PROTOCOL, TAOBAOPASS_DATA, "", &tmparr);
	return FALSE;//淘宝也不写实体，写到httpup中
}
//单包淘宝登录处理，但没有发现有单包淘宝
BOOL FormatTaobaoLogin(DATEHEADER& packh,CString url,char* pSplitxt,LPCTSTR spyaddr/*CString url,char* pSplitxt,SocketSession* pSion*/)
{
	if (url.Find("/member/login.jhtml") < 0)
	{
		return FALSE;
	}
 	TaoBaoFriend taobaofriend(packh, g_httpFrom);
	WriteLog(HTTPPRO,"FormatTaobaoLogin");

	CString username,pass;
	CString content;
	content.Format("%s",pSplitxt);
	int ifind = content.Find("TPL_username=");
	if (ifind < 0)
	{
		return false;
	}
	content.Format("%s",content.Mid(ifind+13));
	ifind = content.Find("\xA");
	if (ifind < 0)
	{
		return false;
	}
	username.Format("%s",content.Left(ifind));
	ifind = content.Find("TPL_password=");
	if (ifind < 0)
	{
		return false;
	}
	content.Format("%s",content.Mid(ifind+13));
	ifind = content.Find("\xA");
	if (ifind < 0)
	{
		return false;
	}
	pass.Format("%s",content.Left(ifind));

/*	char* username;
	char* p = strstr(pSplitxt, "TPL_username");
	*(p+12)=0;
	p=p+12;
	username = strstr(p,"\1");
	*username=0;
	userid.Format("%s",p);
	p = username+1;

	p = strstr(p,"TPL_password=");
	*(p+13)=0;
	p=p+13;
	username = strstr(p,"\1");
	*username=0;
	pass.Format("%s",p);
*/	
	if (pass.Find("3DES") > 0)
	{
		int hh = pass.ReverseFind('_');
		CString despass;
		despass = pass.Mid(hh+1);
		int len =despass.GetLength();
		
		char	Result[MAXLEN],outbuf[MAXLEN];
		memset(Result,0,MAXLEN);
		memset(outbuf,0,MAXLEN);

		hexstrtoint10str((char*)(LPCTSTR)despass, Result);
		Dec_TaoBao_Pass((unsigned char*)Result,(unsigned char*)outbuf,strlen(Result));
		if (strlen(outbuf) < 1)
		{
			return TRUE;
		}
// 		taobaofriend.m_idxtext.Format("user= %s\r\npass= %s\r\n",username,Result);
// 		taobaofriend.m_spyaddr = spyaddr;
		CString tmpstr;
		CStringArray tmparr;
		tmpstr.Format("%s\1%s",username,outbuf);
		
//		WriteLog(HTTP_PROTOCOL,"增加淘宝pass：%s",tmpstr);
		tmparr.Add(tmpstr);
		WriteArrToData(HTTP_PROTOCOL, TAOBAOPASS_DATA, "", &tmparr);

	}
//	taobaofriend.CloseSession();
	return TRUE;
}
BOOL testtaobaopass()

{
// 		CString taobaotxt;
// taobaotxt="TPL_username=婧妤姐姐\r\nCtrlVersion=1,0,0,7\r\nsupport=000001\r\ntid=XOR_1_000000000000000000000000000000_6358442736090F050E760A7B\r\nTPL_password=3DES_2_000000000000000000000000000000_658673CB84C971B7380DD1468F553A18\r\nSubmit=登 录\r\nactionForStable=enable_post_user_action\r\naction=Authenticator\r\nTPL_redirect_url=http://upload.taobao.com/auction/publish/publish.htm?auction_type=b&auth_alert=true\r\nevent_submit_do_login=anything\r\nabtest=\r\npstrong=\r\nfrom=\r\nyparam=\r\ndone=";
// 	DATEHEADER packh;
// 	FormatTaobaoLogin(packh,"url",(char*)(LPCTSTR)taobaotxt,"gg"/*CString url,char* pSplitxt,SocketSession* pSion*/);

//		FormatTaobaoLogin("v:\\aaa.txt",taobaotxt,"aa");
	{
		CString pass="3DES_2_000000000000000000000000000000_3055BF45BCA0676247F2CA58525E86FF";
		int hh = pass.ReverseFind('_');
		CString despass;
		despass = pass.Mid(hh+1);
		int len =despass.GetLength();
		
		char	Result[MAXLEN],outbuf[MAXLEN];
		memset(Result,0,MAXLEN);
		memset(outbuf,0,MAXLEN);
		hexstrtoint10str((char*)(LPCTSTR)despass, Result);
		Dec_TaoBao_Pass((unsigned char*)Result,(unsigned char*)outbuf,strlen(Result));
// 				int hh = pass.ReverseFind('_');
// 		char despass[50];
// 		memset(despass,0,50);
// 		sprintf(despass, "%s" ,pass.Mid(hh+1));
// 		int len =strlen(despass);
// 		
// 		char	Result[MAXLEN];
// 		memset(Result,0,MAXLEN);
// 		hexstrtoint10str((char*)/*(LPCTSTR)*/despass, Result);
// 		Dec_TaoBao_Pass((unsigned char*)Result,len);
	}
	{
		CString pass="uid=4ii9M N2bIU=\xAnum=1toids=cntaobaozpliudan.v=5.70.04W";
		CString content;
		content.Format("%s",pass);
		int ifind = content.Find("uid=");
		if (ifind < 0)
		{
			return false;
		}
		content.Format("%s",content.Mid(ifind+4));
		ifind = content.Find('\xA');
		if (ifind < 0)
		{
			return false;
		}
		CString uid = content.Left(ifind);
		int len=uid.GetLength();
		char	char_Result[MAXLEN];
		memset(char_Result,0,MAXLEN);
		CMailCoder::base64_decode((char*)(LPCTSTR)uid,uid.GetLength()-1,char_Result);
		Dec_TaoBao_Uid((unsigned char*)char_Result ,len/*strlen(char_Result)*/);

	}

	return TRUE;
}
//获得好友列表
//uid=ObySTwA68MKGIZXQRkUqVg==&num=6&toids=cntaobaohao_zhen_shi.cntaobaolantianfeixing.cntaobaorita825.cntaobaoviviyun.cntaobaozbIMW.cntaobao.......&v=5.50.02W
BOOL FormatTaobaoFrient(DATEHEADER& packh,CString url,char* pSplitxt,LPCTSTR spyaddr,CString host,HttpSession* pSion/*CString url,char* pSplitxt,SocketSession* pSion*/)
{
		return FALSE;

	if (url.Find("/memberinfo/getuserflags") < 0 ||
		host.Find("web.im.alisoft.com") < 0)
	{
		return FALSE;
	}
 	TaoBaoFriend taobaofriend(packh, g_httpFrom);
	taobaofriend.m_dceprocotol=pSion->m_dceprocotol;
	taobaofriend.m_begfiletm = packh.m_tm;
// 	taobaofriend.m_sip = pSion->m_sip;
// 	taobaofriend.m_dip = pSion->m_dip;
// 	taobaofriend.m_smac = pSion->m_smac;
// 	taobaofriend.m_dmac = pSion->m_dmac;
//*****************************************
	WriteLog(HTTPPRO,"FormatTaobaoFrient:%s",pSplitxt);

	CString content;
	content.Format("%s",pSplitxt);
	if (content.Find("v=5.60")>0)//此版本暂不支持
	{
		return FALSE;
	}
	int ifind = content.Find("uid=");
	if (ifind < 0)
	{
		return false;
	}
	content.Format("%s",content.Mid(ifind+4));
	ifind = content.Find('\xA');
	if (ifind < 0)
	{
		return false;
	}
	CString uid = content.Left(ifind);
	char	char_Result[MAXLEN];
	memset(char_Result,0,MAXLEN);
	CMailCoder::base64_decode((char*)(LPCTSTR)uid,uid.GetLength()-1,char_Result);
	Dec_TaoBao_Uid((unsigned char*)char_Result ,strlen(char_Result));
	
	CString Result;
	Result.Format("%s",char_Result);
	Result.Replace("\2","");
	Result.Replace("\3","");
	Result.Replace("\4","");
	Result.Replace("\5","");
	Result.Replace("\6","");
	Result.Replace("\7","");
	Result.Replace("\8","");
	Result.Replace("\9","");

	CString frindlist;
	taobaofriend.m_msgtext.Format("用户%s获得好友列表:",Result);	
	
	ifind = content.Find("num");
	if (ifind < 0)
	{
		return FALSE;
	}
	content.Format("%s",content.Mid(ifind+4));
	ifind = content.Find('\xA');
	if (ifind < 0)
	{
		return false;
	}
	CString strnum = content.Left(ifind);
	int num = atoi(strnum);
	if (num <1)
	{
		return false;
	}
	ifind = content.Find("toids=");
	if (ifind < 0)
	{
		return FALSE;
	}

	content.Format("%s",content.Mid(ifind+6));
	for (int hh = 0; hh < num; hh++)
	{
		ifind = content.Find('\1');
		if(ifind < 0)
		{
			break;
		}
		taobaofriend.m_msgtext.Format("%s %s",(CString)taobaofriend.m_msgtext,content.Left(ifind));
		content.Format("%s",content.Mid(ifind+1));
	}
	taobaofriend.m_spyaddr = spyaddr;
	taobaofriend.m_idxtext.Format("user= %s\r\nstat= 获得好友列表\r\n",Result);
	WriteLog(HTTPPRO,"aobaofriend.m_idxtext:%s",taobaofriend.m_idxtext);

//********************************************
/*	char* p = strstr(pSplitxt, "uid=");
	*(p+3) = 0;
	p=p+4;
	pSplitxt = p;
	p = strstr(p, "num");
	*p=0;
	p=p+1;
	CString uid;
	uid.Format("%s",pSplitxt);
	
	char	Result[MAXLEN];
	memset(Result,0,MAXLEN);
	CMailCoder::base64_decode((char*)(LPCTSTR)uid,uid.GetLength()-1,Result);
	Dec_TaoBao_Uid((unsigned char*)Result ,strlen(Result));

	CString frindlist;
	taobaofriend.m_msgtext.Format("用户%s获得好友列表:",Result);	

	*p=0;
	*(p+2)=0;
	p=p+3;
	char* numstr = strstr(p,"toids");
	*numstr=0;
	int num = atoi(p);
	p = numstr+6;
	char* tmp;
	for (int hh = 0; hh < num; hh++)
	{
		tmp = strstr(p,"\x01");
		*tmp = 0;
		taobaofriend.m_msgtext.Format("%s %s",(CString)taobaofriend.m_msgtext,p);
		p=tmp++;
	}*/
	taobaofriend.CloseSession();
	return TRUE;
}

TaoBaoFriend::TaoBaoFriend(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 180;
	m_begfiletm = 0;
	
}

TaoBaoFriend::~TaoBaoFriend()
{

}
BOOL TaoBaoFriend::CloseSession()
{
	try
	{
		LPCTSTR pro;
		pro = _T("imsg_taobao");

		// 写实体文件
		if (!m_msgtext.IsEmpty())
		{
			WriteImsgFile();
		}
		m_msgtext.Empty();
		m_OpenCaseFlg = v_opencasetaobao;
		m_OpenWarnFlg = v_openwarntaobao;
		m_OpenIndxFlg = v_openflagtaobao;

		if (!m_realfname.IsEmpty() && GetFileLength(m_realfname) > 0)
		{
			CString checktext;
			checktext.Format("%s____________%s",m_msgtext,m_idxtext);
			// 匹配案件和预警
			DWORD state = 0x10000;
			SearchCaseOrWarn(this, m_idxtext, m_idxtext.GetLength(), state);

			// 写对照文件
			m_indexfname = m_realfname + ".indexfile.txt";
			WriteIndexFile(pro, pro, m_idxtext, m_dceprocotol->m_httpMissObj);

		}
	}
	catch(...)
	{
		WriteLog(HTTPPRO, "TaoBaoFriendCloseSession() Error!!");
	}
	return TRUE;	// 一个会话只保持5分钟
}
void TaoBaoFriend::WriteImsgFile()
{
	if (m_realfname.IsEmpty())
	{
		CString pro;
		pro.Format("imsg_taobao");

		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
		m_realfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_httpFileNum, pro);
	}
	FILE* fp = fopen(m_realfname, "ab");
	if (fp)
	{
		fwrite((LPCTSTR)m_msgtext, 1, m_msgtext.GetLength(), fp);
		fclose(fp);
	}

}