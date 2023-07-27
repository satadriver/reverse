
#include "stdafx.h"
#include "http.h"
#include "PostStruct.h"
#include "PostSplit.h"
#include "httpsession.h"
#include "PublicFun.h"
#include "httpproxy.h"
#include "./../Getmoniterweb.h"
#include "NetHotTable.h"

// BOOL ParserGetUrl(HttpCommInfo& Resquest, DATEHEADER& packh, const char* pPacket);
// BOOL ParserPostUrl(HttpCommInfo& Resquest, int headpos, DATEHEADER& packh, const char* pPacket);
// BOOL ParserHttpOk(HttpCommInfo& Response,int headpos, DATEHEADER& packh, const char* pPacket);
BOOL UrlWriteStat(HttpCommInfo& Resquest, DATEHEADER& packh,CString sphone,CString suseracount);
NetHotTable splitnethot;
extern moniterweb g_moniterweb;
CMap<CString,LPCSTR,LPQQMapInfo,LPQQMapInfo&> QQInfo;

void DealCompressedFile(CString &filename, HttpCommInfo * phttpinfo)
{
	CString strDFile;
	CString strCmd;
	if(phttpinfo->m_ifcompressed)
	{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(260);
#endif
		strDFile=filename+".gz";
		int i=rename(filename,strDFile);
		if(i==0)
		{
			strCmd.Format("%s\\bin\\gunzip.exe \"%s\"",	v_RootPath, strDFile);
//			system(strCmd);
			PROCESS_INFORMATION pInfo;
			STARTUPINFO         sInfo;
			sInfo.cb              = sizeof(STARTUPINFO);
			sInfo.lpReserved      = NULL;
			sInfo.lpReserved2     = NULL;
			sInfo.cbReserved2     = 0;
			sInfo.lpDesktop       = NULL;
			sInfo.lpTitle         = NULL;
			sInfo.dwFlags         = STARTF_USESHOWWINDOW;
			sInfo.dwX             = 0;
			sInfo.dwY             = 0;
			sInfo.dwFillAttribute = 0;
			sInfo.wShowWindow     = SW_HIDE;
			if (!CreateProcess(NULL, (char*)(LPCTSTR)strCmd, 
				NULL, NULL, FALSE, 0, NULL,	NULL, &sInfo, &pInfo))
			{
				WriteLog(HTTPPRO, "Error!! Run gunzip.exe fail!");
				filename=strDFile;
				return;
			}
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(261);
#endif
			if (WAIT_OBJECT_0 != WaitForSingleObject(pInfo.hProcess, 20000))
			{
				WriteLog(HTTPPRO, "Error!! Run gunzip.exe(%d)%s TimeOut! Kill it",
					pInfo.dwProcessId, strCmd);
				TerminateProcess(pInfo.hProcess, 0);
			}
			CloseHandle(pInfo.hProcess);
			CloseHandle(pInfo.hThread);
			
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(262);
#endif
			if (_access(strDFile,0)==0)	//unzip false
			{
				filename=strDFile;
			}
		}
	}
}

//******************************************************************************
//	将文件传输格式(POST)HTTP转化为ASCII格式
//******************************************************************************
void ConverHttp2Html(char *tmp_content, int tmp_content_len)
{
	char *tp1=NULL, *tp2=tmp_content, *p=tmp_content;
	char cStrTmp[4];
	cStrTmp[2] = 0;
	for(int i=0; i<tmp_content_len; i++, tp2++)
	{
		switch (*tp2)
		{
		case '+':
			*tp2 = ' ';
			break;/*
		case '&':
			*tp2 = '\n';
			break;*/
		case '%'://将%AB形式的十六进制码转换为二进制码
			if(*(tp2 + 1)=='u') //即"%u" unicode 编码形式
			{
// 				*tp2 = ' ';
// 				*(tp2 + 1) = '#';
// 				tp2+=2;
 				break;
			}

			cStrTmp[0] = *(tp2 + 1);
			cStrTmp[1] = *(tp2 + 2);
			
			//出错处理
			if(cStrTmp[0] == 0 || cStrTmp[1] == 0)
				break;
			if( tmp_content_len - i - 3 < 0 )
				break;
			
			if (cStrTmp[0]>='A' && cStrTmp[0]<='F')
				cStrTmp[0]=cStrTmp[0]-'A'+10;
			else if (cStrTmp[0]>='a' && cStrTmp[0]<='a')
				cStrTmp[0]=cStrTmp[0]-'a'+10;
			else
				cStrTmp[0]-='0';

			if (cStrTmp[1]>='A' && cStrTmp[1]<='F')
				cStrTmp[1]=cStrTmp[1]-'A'+10;
			else if (cStrTmp[1]>='a' && cStrTmp[1]<='a')
				cStrTmp[1]=cStrTmp[1]-'a'+10;
			else
				cStrTmp[1]-='0';

			*(tp2+2) = cStrTmp[0]*16 + cStrTmp[1];
			if (tp1!=NULL)
			{
				int len = tp2-tp1;
				memmove(p, tp1, len);
				p+=len;
			}
			else
			{
				p=tp2;
			}
			tp1 = tp2+2;
			memset(p, 0, 2);
			tp2+=2;
			i+=2;
			//出错处理

//			memcpy(tmp_content+1, tmp_content+3, tmp_content_len - i - 3);
			//移动后最后两位变为零
//			*(tmp_content + tmp_content_len - i - 2 ) = 0;
//			*(tmp_content + tmp_content_len - i -1  ) = 0;
		}
	}
	if(tp1!=NULL)
	{
		int len = tp2-tp1;
		memmove(p, tp1, len+1);
//		memset(p+len, 0, 2);
	}
}

// int DealOnePacketGet(DATEHEADER& packh, const char* Packet)
// {
// //Get 主要 命令
// //GET /url?sa=T&start=2&url=http%3A//soyolet.wx-e.com/ziliaoshoucang/diannaojishu/unicode02.htm HTTP/1.1
// //Accept: */*
// //Referer: http://www.google.com/search?hl=zh-CN&ie=UTF-8&oe=UTF-8&q=%E5%AD%97%E7%AC%A6%E9%9B%86+%E5%87%BD%E6%95%B0+C%2B%2B&lr=
// //Accept-Language: zh-cn
// //Accept-Encoding: gzip, deflate
// //User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)
// //Host: www.google.com
// //Connection: Keep-Alive
// //Cookie: PREF=ID=2487ee559e414dfc:TM=1071286718:LM=1071450637:TB=2:S=MrUoElGcJwI6kZAu
// 	BOOL unixhead=FALSE;
// 	BOOL sessionflag=false;
// 	int i=Search4ByteFind(Packet, packh.datalen);//0d 0a 0d 0a
// 	if (i<0)
// 	{
// 		i=Search2ByteFind(Packet, packh.datalen);// 0a 0a
// 		unixhead=TRUE;
// 	}
// 	if(i>0 && i<=packh.datalen)
// 	{
// 		HttpCommInfo Resquest;
// 		memcpy(FILEBUFFER, Packet, packh.datalen);
// 		memset(FILEBUFFER+packh.datalen, 0, 8);
// 		if(!FormatHttpReqHeader(Resquest, FILEBUFFER, i, unixhead, packh.dip, packh.dport, &packh))
// 			return TRUE;
// 		Resquest.dataSrc = GetDataSrc(packh, v_httpFrom, Resquest.strISP);
// 		Resquest.Stm = packh.tm;
// 		ParserGetUrl(Resquest, packh, Packet);
// 		return TRUE;
// 	}
// 	return 0;
// }
// //处理Post
// //by wq add 	Post增加 url过滤 关键字过滤
// int DealOnePacketPost(DATEHEADER& packh, const char* Packet)
// {
// 
// //POST /in.jsp HTTP/1.1
// //Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, application/msword, application/x-shockwave-flash, */*
// //Referer: http://sms.163.com/usererror.php?paths=%2Fservice%2Fsendmsg_pop.php%3Fbd%3D%26send%3D1%26phone%3D13864301553%26message%3D%D7%A3%C4%E3%CA%A5%B5%AE%BF%EC%C0%D6%A3%A1%26send_type%3D
// //Accept-Language: zh-cn
// //Content-Type: application/x-www-form-urlencoded
// //Accept-Encoding: gzip, deflate
// //User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)
// //Host: reg4.163.com
// //Content-Length: 295
// //Connection: Keep-Alive
// //Cache-Control: no-cache
// //Cookie: nn_V=846750514; NETEASE_SSN=13806434228; URSJESSIONID=afNW-Pcym8yc
// //
// //url=http%3A%2F%2Fsms.163.com%2Fservice%2Fredirect.php%3Foldpaths%3D%252Fservice%252Fsendmsg_pop.php%253Fbd%253D%2526send%253D1%2526phone%253D13864301553%2526message%253D%25D7%25A3%25C4%25E3%25CA%25A5%25B5%25AE%25BF%25EC%25C0%25D6%25A3%25A1%2526send_type%253D&username=13806434228&password=386660
// 
// 	BOOL unixhead=FALSE;
// 	BOOL sessionflag=false;
// 	BOOL re=TRUE;
// 	int i=Search4ByteFind(Packet, packh.datalen);//0d 0a 0d 0a
// 	if (i<0)
// 	{
// 		i=Search2ByteFind(Packet, packh.datalen);// 0a 0a
// 		unixhead=TRUE;
// 	}
// 	if (i >0 && i<packh.datalen )
// 	{
// 		HttpCommInfo Resquest;
// //		char ServerIp[20];
// //		sprintf(ServerIp, "%d.%d.%d.%d", packh.dip[0], packh.dip[1], packh.dip[2], packh.dip[3]);
// 		memcpy(FILEBUFFER, Packet, packh.datalen);
// 		memset(FILEBUFFER+packh.datalen, 0, 8);
// 		if(!FormatHttpReqHeader(Resquest, FILEBUFFER, i, unixhead, packh.dip, packh.dport,&packh))
// 			return 1;
// 		Resquest.dataSrc = GetDataSrc(packh, v_httpFrom, Resquest.strISP);
// 
// 		if(Resquest.useragent == "IMW WinHTTP AS")
// 			return 1;
// 		if ( (Resquest.Content_length+i) <=packh.datalen)
// 		{
// 			return ParserPostUrl(Resquest, i, packh, Packet);
// 		}
// 	}
// 	return 0;
// }

// int DealOnePacketHttpOk(DATEHEADER& packh, const char* Packet)
// {
// #ifdef	ADD_RUNSTATE_INFO
// 	AutoDestory AutoDebug(&v_httprunstate, 300, -300);
// #endif
// 	BOOL unixhead=FALSE;
// 	BOOL sessionflag=false;
// 	BOOL re=TRUE;
// 	int i=Search4ByteFind(Packet, packh.datalen);//0d 0a 0d 0a
// 	if (i<0)
// 	{
// 		i=Search2ByteFind(Packet, packh.datalen);// 0a 0a
// 		unixhead=TRUE;
// 	}
// 	if (i >0 && i<packh.datalen )
// 	{
// 		HttpCommInfo Response;
// 		Response.Content_length=packh.datalen-i;
// //		char ServerIp[20];
// //		sprintf(ServerIp, "%d.%d.%d.%d", packh.sip[0], packh.sip[1], packh.sip[2], packh.sip[3]);
// 		memcpy(FILEBUFFER, Packet, packh.datalen);
// 		memset(FILEBUFFER+packh.datalen, 0, 8);
// 		if(!FormatHttpReqHeader(Response,FILEBUFFER,i,unixhead,packh.sip, packh.sport))
// 			return 1;
// 		Response.dataSrc = GetDataSrc(packh, v_httpFrom, Response.strISP);
// 		if ( (Response.Content_length+i) <=packh.datalen && Response.Content_length>8)
// 			return ParserHttpOk(Response, i, packh, Packet);
// 	}
// 	return 0;
// }

//************************************************************************
//	当 dport!=80 要检查 AAA 固定IP IP_URL 如果是保存session
//************************************************************************
// BOOL ISSavePost(DATEHEADER& packh, const char* Packet)
// {
// 	if (!v_opencasehttpup)
// 		return FALSE;
// 	// 对照认证信息
// 	AAAInfo *pAAA;
// 	AAAInfo aaa;
// 	char sip[40];
// 	char dip[40];
// 	BOOL ret=TRUE;
// 	int	isIndexIpUrl=1;
// 	try
// 	{
// 		CaseInfo caseinfo;
// 		BOOL isCase = FALSE;
// 		sprintf(sip, "%03d.%03d.%03d.%03d", packh.sip[0], packh.sip[1], packh.sip[2], packh.sip[3]);
// 		sprintf(dip, "%03d.%03d.%03d.%03d", packh.dip[0], packh.dip[1], packh.dip[2], packh.dip[3]);
// 		CString smac, dmac;
// 		FormatMac(packh.mac, smac, dmac);
// 		if (NULL != (pAAA = GetAAAInfo(*((DWORD*)packh.sip), *((DWORD*)packh.dip), aaa, packh.tm)))
// 		{
// 			if (!pAAA->CaseID.IsEmpty())
// 				isCase = TRUE;
// 		}
// 		else if (packh.vlanid!=0)
// 		{
// 			// 没有匹配上帐号就把vlan加入倒帐号字段中
// 			aaa.UserAccount1.Format("VLanID_%d", packh.vlanid);
// 			// 匹配Vlan的线索
// 			if (SearchCaseByVlan(packh.vlanid, caseinfo))
// 			{
// 				isCase = TRUE;
// 				aaa.CaseID		= caseinfo.CaseID;
// 				aaa.LimitAddr	= caseinfo.LimitAddr;
// 				aaa.LimitType	= caseinfo.LimitType;
// 				aaa.ObjectID	= caseinfo.ObjectID;
// 			}
// 			
// 		}
// 		
// 		if (!isCase)
// 		{
// 			int len = 0;
// 			if (SearchCaseByIP(sip, dip, caseinfo))
// 				isCase = TRUE;
// 			else if (SearchCaseByMAC(smac, dmac, caseinfo))
// 				isCase = TRUE;
// 			else
// 			{
// 				memcpy(FILEBUFFER, Packet, packh.datalen+8);
// 				ConverHttp2Html(FILEBUFFER, packh.datalen);
// 				len = strlen(FILEBUFFER);
// 				// 对照IP+URL
// 				if (3==SearchCaseByIPURL(sip, dip, FILEBUFFER, len, caseinfo))
// 					isCase = TRUE;
// 			}
// 			if (isCase)
// 			{
// 				aaa.CaseID		= caseinfo.CaseID;
// 				aaa.LimitAddr	= caseinfo.LimitAddr;
// 				aaa.LimitType	= caseinfo.LimitType;
// 				aaa.ObjectID	= caseinfo.ObjectID;
// 			}
// 			else
// 			{
// 				if (3==SearchIndexByIPURL(sip, dip, FILEBUFFER, len))
// 					isIndexIpUrl = 50;
// 				else
// 					return ret;
// 			}
// 		}
// 
// 		ASSERT(isCase==TRUE || isIndexIpUrl==50);
// 		HttpSession *s= new HttpSession(packh, v_httpFrom, aaa, isIndexIpUrl);
// 		
// 		if(s>0)
// 		{
// 			pthisMissObj->m_SocketMap.SetAt(s->sip+"_"+s->dip, (SocketSession*&)s);
// 			s->InsertPacket(packh, Packet);
// 		}
// 	}
// 	catch(...)
// 	{
// 		WriteLog(strHttp, "ISSavePost() Error!!");
// 		return FALSE;
// 	}
// 	return ret;
// }

//下一行指针	//并把CR LF置0
BOOL NextLine(char **p)
{
	ASSERT(p!=NULL);
	ASSERT(*p!=NULL);
	char* tp = strchr(*p, '\n');
	if (tp != NULL)
	{
		if (*(tp-1) == '\r')
			*(tp-1) = 0;
		*tp = 0;
		*p = tp+1;
		return TRUE;
	}
	else
		return FALSE;
	return TRUE;
}

//Value第二段指针	去掉 ; \0x20 CR LF -->0 0;
BOOL NextTocken(char **p)
{
	
	while (**p) 
	{
		if (*(*p)==';' ||*(*p)==' ' ||*(*p)==',')
		{
			*(*p)=0;
			(*p)++;
			return TRUE;
		}
		else if(*(*p)==CR || *(*p)==LF )
			return FALSE;
		else
			(*p)++;
	}
	return FALSE;
}

//取一行的值  ':' \0x20 --->0 0; 如果fasle 说明http头结束
BOOL NextValue(char **p)
{
	while (**p)
	{
		if (*(*p)==':')
		{
			
			*(*p)=0;
			(*p)++;
			
			if(*(*p)==' ')
			{
				
				*(*p)=0;
				(*p)++;
			}
			return TRUE;
		}
		else if( *(*p)==CR || *(*p)==LF )
			return FALSE;
		else
			(*p)++;
	}
	return FALSE;

}

/************************************************************************/
/*         格式化HttpHeader                                             */
/************************************************************************/
BOOL FormatHttpReqHeader(HttpCommInfo& header ,char *buff,int len,
			BOOL unixhead, unsigned char cServerIp[4], WORD port, DATEHEADER* packh)
{
	CString strHeader;
	char * pPacket, * p, *n,*t;//p一般为行指针或临时指针 t为http Field名指针  n为 Field名的值指针
	CString uri;
	CString Value;
	BOOL singleline=FALSE;

	HttpCommInfo *preq;

	preq=&header;
	preq->m_IDCInfo.Empty();
//	preq->spyaddr=DDEPOS;
	BOOL isIDC = TRUE;	// 是否是IDC的数据
	pPacket=buff;
	p=buff;
	int err = 0;
	try
	{
///////////处理第一行////////////////////////////////////////////////
	if (memcmp(pPacket,"GET ",4)==0)
	{
		preq->m_Action=HTTP_GET;
		if(memcmp(pPacket+4,"http://",7)==0)
		{
err=1;
			p+=4;
			t=p;
			if(!NextTocken(&p))
				return FALSE;
			if(!NextLine(&p))
				return FALSE;
			uri=t;
			preq->m_bproxy=TRUE;
			isIDC = FALSE;	// http代理不按IDC处理
		}
		else
		{
err=2;
			p+=4;
			t=p;
			if(!NextTocken(&p))
				return FALSE;
			if(!NextLine(&p))
				return FALSE;
			preq->m_bproxy = FALSE;
			uri=t;
		}
		if (uri.GetLength()>1)
		{
err=3;
			if (uri.GetAt(uri.GetLength()-1)=='/')
	//		if (uri.Right(1).CompareNoCase("/")==0)
				header.m_issave=TRUE;
			else
			{
				int pos = uri.ReverseFind('.');
				if ((pos>0) && (pos+7 >= uri.GetLength()))
				{
					if (uri.Find("htm", pos+1)>=0)
						header.m_issave = TRUE;
				}
				header.m_issave=FALSE;
			}
		}
	}
	else if(memcmp(pPacket,"POST ",5)==0 || memcmp(pPacket,"PUT ",4)==0)
	{
		preq->m_Action=HTTP_POST;
		if(memcmp(pPacket+5,"http://",7)==0)
		{
err=4;
			p+=5;
			t=p;
			if(!NextTocken(&p))
				return FALSE;
			if(!NextLine(&p))
				return FALSE;
			uri=t;
			preq->m_bproxy=TRUE;
			isIDC = FALSE;	// http代理不按IDC处理
		}
		else
		{
err=5;
			p+=5;
			t=p;
			if(!NextTocken(&p))
				return FALSE;
			if(!NextLine(&p))
				return FALSE;
			preq->m_bproxy = FALSE;
			uri=t;
		}
	}
	else if(memcmp(pPacket,"HTTP/1.",7) == 0)
	{
		int ts = memcmp(pPacket+8, " 200", 4);
		if (ts == 0)
		{
err=6;
			preq->m_Content_length = Maxfilelen-1;
			preq->m_Action=HTTP_200OK;
			isIDC = FALSE;
			if(!NextLine(&p))
				return FALSE;
		}
		else if (ts < 0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	//////处理后面的行////////////////////////////////////////////////////////////////////
	while(*p)
	{
err=7;
		char *tmp;
		t=p;
		if (!NextValue(&p))
			break;
		n=p;
		tmp=p;
		if(!NextLine(&p))
			break;
		if ((v_programtype == SYSTEM_SLEUTH)
			&& (strcmp(t, "UsEr-AgEnT") == 0))
		{
			preq->m_bRbmm = TRUE;
		}
		_strlwr(t);
err=71;
		if(strcmp(t,"content-length")==0)
		{
			preq->m_Content_length=atoi(n);
			if (preq->m_Content_length > Maxfilelen*2)
			{
				return -2;
			}
		}
		else if(strcmp(t,"transfer-encoding")==0)
		{
			Value=n;
			Value.TrimRight();
			Value.MakeLower();
			if(Value.Find("chunked")>=0)
			{
				preq->m_ifchunked=TRUE;
				preq->m_Content_length = 0x10000000;
			}
		}
		else if(strcmp(t,"content-encoding")==0)
			//|| strcmp(t, "accept-encoding") == 0) freegate 所有都有该字段, 值固定gzip,deflate
		{
			Value=n;
			Value.TrimRight();
			if(Value.Find("gzip" )>=0)
				preq->m_ifcompressed =TRUE;
		}
		else if(strcmp(t,"content-type")==0)
		{
			preq->m_emlfield = n;
			NextTocken(&tmp);
			_strlwr(n);
			preq->m_Content_Type=n;
/************************************************************************
//常用Content_Type 
图片动画
//image/jpeg							.jpg
//image/pjpeg							.jpg
//image/gif								.gif
//image/tiff							.tif
//image/png								.png
//application/x-shockwave-flash,		.swf
脚本
//application/x-javascript				.js.txt
//text/css								.css.txt
//////////////////////////////////////////////////////////////////////////
//text/plain							.txt
//text/html								.htm
//text/xml								.xml
//message/rfc822						.eml
//message/rfc821						.eml

//application/msword					.doc
//application/pdf"						.pdf
//压缩
//application/zip						.zip
//application/x-gzip					.gz
//application/x-tar						.tar

//上传
multipart/form-data						.txt	boundary=
application/x-www-form-urlencoded		.txt
application/www-form-urlencoded			.txt
//流
//application/octet-stream								
************************************************************************/
			if(strcmp(n,"multipart/form-data")==0)
			{
				preq->m_strBoundary=tmp;
				int pos1=preq->m_strBoundary.Find("boundary=");
				if (pos1>=0)
					preq->m_strBoundary=preq->m_strBoundary.Mid(pos1+strlen("boundary="));
			}
		}
		else if(strcmp(t, "user-agent") == 0)
			preq->m_useragent=n;
		else if(strcmp(t,"host")==0)
		{
			preq->m_Host=n;
 		}
		else if(strcmp(t,"referer")==0)
			preq->m_Referer=ConverHttp2Html(n);
		else if (strcmp(t,"cookie")==0)
		{
			preq->m_Cookie_Clone=n;
			preq->m_Cookie= ConverHttp2Html(n);
		}
		else if (strcmp(t,"cookie2")==0)
			preq->m_Cookie2=ConverHttp2Html(n);
		else if (strcmp(t,"set-cookie2")==0)
			preq->m_SetCookie2=ConverHttp2Html(n);
		else if (strcmp(t, "accept-language") == 0)
		{
			preq->m_Language = n;
		}
		else if (strcmp(t, "accept-charset") == 0)
		{
			preq->m_Charset = n;
		}
		else if (strcmp(t, "accept") == 0)
		{
			preq->m_Accept = n;
		}
		else if (strcmp(t, "mail-upload-name") == 0)
		{
			preq->m_emailAttName = n;
		}
		else if (strcmp(t, "mail-upload-size") == 0)
		{
			preq->m_emailAttSize = n;
		}
		else if (strcmp(t, "mail-upload-offset") == 0)
		{
			preq->m_emailAttOffset = n;
		}
		else if (strcmp(t, "mail-upload-length") == 0)
		{
			preq->m_emailAttLegnth = n;
		}
	}


	//判断是否wap 
	if(preq->m_Accept.Find("vnd.wap.wml")>0 && preq->m_Host.Find("wap")>=0) //wap
	{
		preq->m_iswap=TRUE;	 
	}

	if (p)
	{
	}

err=8;
	if (preq->m_Action==HTTP_200OK)
		return TRUE;
	if (preq->m_Action==HTTP_POST && preq->m_useragent=="IMW WinHTTP AS")
		return FALSE;	// httpLib 的包
	ASSERT(preq->m_Action==HTTP_GET || preq->m_Action==HTTP_POST);
	if (preq->m_Host.GetLength()>=7 && preq->m_Host.GetLength()<=21)
	{
err=9;
		int l = preq->m_Host.GetLength()-1;
		LPCTSTR phost = preq->m_Host;
		preq->m_biphost = true;
		while (l>=0)
		{
			if (phost[l] > ':')
			{
				preq->m_biphost = false;
				break;
			}
			l--;
		}
	}
	// 统计IDC服务器访问量
	// 判断服务器IP是否是IDC服务器
	// 符合IDCip段 不是HTTP代理, 有host段且不是IP的 才走IDC逻辑
	CString strISP;
	if (isIDC && v_stat && v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)cServerIp)), strISP))
	{
#ifdef			_DEBUG
		if (port!=80 && preq->m_Host.IsEmpty())
			int t = port;
#endif
err=10;
		CString strDomain = preq->m_Host;
		isIDC = !(strDomain.GetLength()<4);
		int pos = strDomain.ReverseFind(':');		// zzzzzz:yyyy
		if (pos>0)
		{
			// 非标准端口的WEB 在GET 或 POST中的host段都带有目标网站的端口号
			WORD dstport = atoi(strDomain.Mid(pos+1));
			if (dstport != port)		// TCP的目标端口号和目标网站的端口号不相等就丢弃
				isIDC = FALSE;
			else
				strDomain.ReleaseBuffer(pos);
		}
		else if (port != 80)
		{
			// 是socket代理 或是其它的浏览工具
			isIDC = FALSE;
		}
		// 如果最后的一个点后面的如果不是字母就不要host段的数据
// 		if (isIDC)
// 		{
// 			pos = strDomain.ReverseFind('.');
// 			if (pos <= 0)
// 				isIDC = FALSE;
// 			else
// 			{
// 				isIDC = FALSE;
// 				char *tp = preq->Host.GetBuffer(0);
// 				for (int i=pos+1; i<preq->Host.GetLength(); i++)
// 				{
// 					if (tp[i]>':')
// 					{
// 						isIDC = TRUE;
// 						break;
// 					}
// 				}
// 			}
// 		}
		if (isIDC)
		{
err=11;
			CString str;
			BOOL isPost = (preq->m_Action==HTTP_POST);
			CString strInteract;
			if(isPost || (uri.Find('?') > 0))
				strInteract = INTERACT_DYNAMIC;
			else
				strInteract = INTERACT_STATIC;
			LPCTSTR ServerType = _T("WEB");
			LPCTSTR TypeState = TYPESTATE_DECIDED;
err=12;
			strDomain.MakeLower();
			CString tturl;
			tturl = strDomain + "_" + uri;
			if (tturl.Find("bbs") >= 0)
			{
				ServerType = _T("BBS");
				TypeState = TYPESTATE_POSSIBLE;
				strInteract = INTERACT_DYNAMIC;
			}
			else if (tturl.Find("blog")>=0)
			{
				ServerType = _T("BLOG");
				TypeState = TYPESTATE_POSSIBLE;
				strInteract = INTERACT_DYNAMIC;
			}
			else if (tturl.Find("chat")>=0)
			{
				ServerType = _T("CHAT");
				TypeState = TYPESTATE_POSSIBLE;
				strInteract = INTERACT_DYNAMIC;
			}
			if (preq->m_biphost)
			{
				str.Format("%03u.%03u.%03u.%03u\1%s\1%s\1WEB服务\1%05d\1%s\1%s\1%s\1", 
					cServerIp[0], cServerIp[1], cServerIp[2], cServerIp[3], strISP,
					(isPost?"post":"get"), port, ServerType, TypeState, strInteract);
			}
			else if (memcmp((LPCTSTR)strDomain, "www.", 4)!=0)
			{
				if (strDomain.Find('.')<0)
				{
					str.Format("%03u.%03u.%03u.%03u\1%s\1%s\1WEB服务\1%05d\1%s\1%s\1%s\1", 
						cServerIp[0], cServerIp[1], cServerIp[2], cServerIp[3], strISP,
						(isPost?"post":"get"), port, ServerType, TypeState, strInteract);
				}
				else
				{
					str.Format("%03u.%03u.%03u.%03u\1%s\1%s\1WEB服务\1%05d\1%s\1%s\1%s\1%s", 
						cServerIp[0], cServerIp[1], cServerIp[2], cServerIp[3], strISP,
						(isPost?"post":"get"), port, ServerType, TypeState, strInteract,
						strDomain);
				}
			}
			else
			{
				str.Format("%03u.%03u.%03u.%03u\1%s\1%s\1WEB服务\1%05d\1%s\1%s\1%s\1%s", 
					cServerIp[0], cServerIp[1], cServerIp[2], cServerIp[3], strISP,
					(isPost?"post":"get"), port, ServerType, TypeState, strInteract,
					strDomain);
			}

			preq->m_IDCInfo = str;
			v_IdcServerStat[HTTP_PROTOCOL].AddServer(str);
		}
// 		else
// 		{
// 			str.Format("%03u.%03u.%03u.%03u\1%s\1%s\1WEB服务\1%05d\1%s\1%s\1%s\1", 
// 				cServerIp[0], cServerIp[1], cServerIp[2], cServerIp[3], strISP,
// 				(isPost?"post":"get"), port, ServerType, TypeState, strInteract);
// 		}
//		m_HttpMap.AddHttpRequest(, str);
//		v_IdcServerStat[HTTP_PROTOCOL].AddServer(str);
	}
err=13;
	BOOL isSearch = TRUE;
	if (preq->m_Host.IsEmpty())
	{
		if (!preq->m_URL.IsEmpty())
		{
			CString tempurl;
			tempurl.Format("%s",preq->m_URL);
			if (tempurl.GetLength()>7)
			{
				tempurl=tempurl.Mid(7);
				int hh=tempurl.Find("/");
				if (hh>0)
				{
					preq->m_Host="http://"+tempurl.Left(hh);	
				}
				else
					preq->m_Host=preq->m_URL;
				preq->m_Host=ConverHttp2Html(preq->m_Host);
			}
			else
			{
				preq->m_Host.Format("%d.%d.%d.%d", cServerIp[0],
						cServerIp[1], cServerIp[2], cServerIp[3]);
				isSearch = FALSE;
			}
		}
		else
		{
			preq->m_Host.Format("%d.%d.%d.%d", cServerIp[0],
					cServerIp[1], cServerIp[2], cServerIp[3]);
			isSearch = FALSE;
		}
	}

err=14;
//	if(preq->m_URL.IsEmpty())
	if( preq->m_bproxy)
	{
		preq->m_URL=uri;
	}
	else
	{
		preq->m_URL="http://"+preq->m_Host+/*CString("/")+*/uri;
	}

err=15;
	preq->m_URL=ConverHttp2Html(preq->m_URL);		// google有多种情况, 转出的错
	preq->m_URI = uri;
	//处理uri中的热点关键字，存放到引擎关键字表中
//	if (isSearch && packh!=NULL && preq->Action==HTTP_GET && v_stat)
//	{
		splitnethot.AnalysNeyHot(uri,packh,preq->m_Host,header);
//	}
	return TRUE;
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "FormatHttpReqHeader() Error=%d!!", err);	
	}
	return FALSE;
}

// 写HttpUrl文件

/************************************************************************/
//	处理一个单包的Get数据,信息需要保存到文件中							*/
//	如果对上的是重点数据则返回FALSE,否则返回TRUE						*/
/************************************************************************/
// BOOL	ParserGetUrl(HttpCommInfo& Resquest, DATEHEADER& packh, const char* pPacket)
// {
// 	int error=0;
// //	// sleuth系统直接返回FALSE
// //	if (v_programtype == SYSTEM_SLEUTH)
// //		return FALSE;
// 	// 对照认证信息
// 	AAAInfo *pAAA;
// 	AAAInfo aaa;
// 	char sip[30];
// 	char dip[30];
// 	BOOL ret = FALSE;
// 	CString strurl;
// 	try
// 	{
// error=1;
// 		int isIndexIpUrl = 0;
// 		CaseInfo caseinfo;
// 		sprintf(sip, "%03d.%03d.%03d.%03d", packh.sip[0], packh.sip[1], packh.sip[2], packh.sip[3]);
// 		sprintf(dip, "%03d.%03d.%03d.%03d", packh.dip[0], packh.dip[1], packh.dip[2], packh.dip[3]);
// 		pAAA = GetAAAInfo(*((DWORD*)packh.sip), *((DWORD*)packh.dip), aaa, packh.tm);
// 		if (NULL != pAAA)
// 		{
// 			if (pAAA->CaseID != "")
// 				return FALSE;
// 		}
// 		else if (packh.vlanid!=0)
// 		{
// 			// 没有匹配上帐号就把vlan加入倒帐号字段中
// 			aaa.UserAccount1.Format("VLanID_%d", packh.vlanid);
// 			// 匹配Vlan的线索
// 			if (v_opencasehttpdown && SearchCaseByVlan(packh.vlanid, caseinfo))
// 				return FALSE;
// 		}
// 
// 		error=2;
// //		if (!v_opencasehttpdown)
// //		{
// //			strurl = Resquest.URL + ' ' + Resquest.Referer + ' ' + Resquest.Cookie +
// //				' ' + Resquest.Cookie2 + ' ' + Resquest.SetCookie2;	
// //			return SearchIndexByIPURL(sip, dip, strurl, strurl.GetLength());
// //		}
// 		if (!SearchCaseByIP(sip, dip, caseinfo))
// 		{
// error=3;
// 			// 对照MAC
// 			CString smac, dmac;
// 			FormatMac(packh.mac, smac, dmac);
// 			if (!SearchCaseByMAC(smac, dmac, caseinfo))
// 			{
// error=4;
// 				// 对照IP+URL
// 				strurl = Resquest.URL + ' ' + Resquest.Referer + ' ' + Resquest.Cookie +
// 					' ' + Resquest.Cookie2 + ' ' + Resquest.SetCookie2;	
// 				if (3!=SearchCaseByIPURL(sip, dip, strurl, strurl.GetLength(), caseinfo))
// 				{
// error=5;
// 					ret = TRUE;
// 					// 对照URL
// 					if (!SearchCaseByURL(strurl, strurl.GetLength(), caseinfo))
// 					{
// error=6;
// 						// 对照关键字
// 						if (!SearchCaseByKey(strurl, strurl.GetLength(), caseinfo))
// 						{
// error=7;
// 							if (Resquest.issave)
// 								UrlWriteStat(Resquest,packh,aaa.Phone1,aaa.UserAccount1);
// 							// 没有对上案件
// error=8;
// 							if (3!=SearchIndexByIPURL(sip, dip, strurl, strurl.GetLength()))
// 								return TRUE;
// 							else
// 								isIndexIpUrl = 50;
// 						}
// 					}
// 				}
// 			}
// 		}
// 
// 		// 对上了案件
// 		ASSERT(!caseinfo.CaseID.IsEmpty() || isIndexIpUrl==50);
// 		HttpSession *s= new HttpSession(packh, v_httpFrom, aaa, isIndexIpUrl);
// 		
// 		if(s>0)
// 		{
// 			pthisMissObj->m_SocketMap.SetAt(s->sip+"_"+s->dip, (SocketSession*&)s);
// 			s->InsertPacket(packh, pPacket);
// 		}
// error=9;
// 		if (isIndexIpUrl!=50 && ret && v_opencasehttpdown)	// 如果对上的是url 和 关键字
// 		{
// 			ASSERT(!caseinfo.CaseID.IsEmpty());
// 			// 写HttpUrl文件
// 			Resquest.Stm = packh.tm;
// 			CString smac, dmac;
// 			sprintf(sip, "%03d.%03d.%03d.%03d_%05d", packh.sip[0], packh.sip[1], packh.sip[2], packh.sip[3], packh.sport);
// 			sprintf(dip, "%03d.%03d.%03d.%03d_%05d", packh.dip[0], packh.dip[1], packh.dip[2], packh.dip[3], packh.dport);
// 			FormatMac(packh.mac, smac, dmac);
// error=10;
// 			ASSERT(FALSE);
// //			WriteIndexFIle(sip, dip,smac, dmac, &Resquest, aaa.UserAccount, aaa.Phone,
// //				caseinfo, Resquest.indexfilename, packh.type==ProxyTCP);
// //			CopyFileToTmpdir(caseinfo.CaseID, Resquest.Filename, Resquest.indexfilename, Resquest.attfiles,
// //				pthisMissObj->m_nowstm,	"httpurl", pthisMissObj->m_ServerPower, NULL);
// error=11;
// 		}
// 		
// 	}
// 	catch(...)
// 	{
// 		WriteLog(strHttp, "ParserGetUrl() Error!! error=%d strurl=%s",error,strurl);
// 		return TRUE;
// 	}
// 	return TRUE;
// }

// 写HttpUp对照文件
BOOL WriteIndexFIle(HttpCommInfo* pResquest,
		CaseInfo& caseinfo, DATEHEADER& packh, LPCTSTR pro)
{
/*lp	FILE *fp=0;
	fp=fopen(pResquest->m_indexfilename,"wb");
	if(!fp)
		return FALSE;
	try
	{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(230);
#endif

		char spytm[40];
		GetTimeString(packh.tm, "%Y-%m-%d %H:%M:%S", spytm, 40);
		CString sip,dip;
		sip.Format("%03d.%03d.%03d.%03d", packh.sip[0], packh.sip[1], packh.sip[2], packh.sip[3]);
		dip.Format("%03d.%03d.%03d.%03d", packh.dip[0], packh.dip[1], packh.dip[2], packh.dip[3]);
		CString sport, dport;
		sport.Format("%05d", packh.sport);
		dport.Format("%05d", packh.dport);
		CString smac, dmac;
		FormatMac(packh.mac, smac, dmac);
		BOOL isReal;
		if (caseinfo.LimitType > 200)
		{
			AddRealIP(sip, caseinfo.CaseID, caseinfo.LimitAddr, caseinfo.LimitType);
			caseinfo.LimitType -= 200;
			isReal = FALSE;
		}
		else if (caseinfo.LimitType > 100)
		{
			caseinfo.LimitType -= 100;
			isReal = TRUE;
		}
		else
		{
			isReal = FALSE;
		}

		CWordArray tmpwordarry;
		tmpwordarry.Add(packh.ipid);
		CString sipport,dipport;
		sipport.Format("%s_%s",sip,sport);
		dipport.Format("%s_%s",dip,dport);

		CString tmppos;
		if(GetProbeAAAInfo(sipport,dipport,smac,dmac,pAaaInfo,tmpwordarry,
			tmppos))
		{
			sip.Format("%s",sipport.Left(15));
			sport.Format("%s", sipport.Mid(16));
			dip.Format("%s", dipport.Left(15));
			dport.Format("%s",dipport.Mid(16));
		}
		else
			tmppos.Format("%s",DDEPOS);

		sip.Replace(".","_");
		dip.Replace(".","_");

		CString sipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)packh.sip)));
		CString dipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)packh.dip)));
		WriteIndexfileField(fp, "[1]\r\npro",	pro);
		WriteIndexfileField(fp, "spyaddr",		tmppos);
		WriteIndexfileField(fp, "tm",			spytm);
		WriteIndexfileField(fp, "sip",			sip);
		WriteIndexfileField(fp, "sport",		sport);
		WriteIndexfileField(fp, "dip",			dip);
		WriteIndexfileField(fp, "dport",		dport);
		WriteIndexfileField(fp, "sipaddr",	sipaddr);
		WriteIndexfileField(fp, "dipaddr",	dipaddr);
		WriteIndexfileField(fp, "datasrc",	pResquest->dataSrc);
		WriteIndexfileField(fp, "ISP",		pResquest->strISP);
		WriteIndexfileField(fp, "smac",			smac);
		WriteIndexfileField(fp, "dmac",			dmac);
		
		if (!caseinfo.CaseID.IsEmpty())
		{
			WriteIndexfileField(fp, "caseid",		caseinfo.CaseID);
			WriteIndexfileField(fp, "objid",		caseinfo.ObjectID);
			WriteIndexfileField(fp, "limitaddr",	caseinfo.LimitAddr);
			WriteIndexfileField(fp, "limittype",	caseinfo.LimitType);
		}
		else if (!pAaaInfo->CaseID.IsEmpty())
		{
			WriteIndexfileField(fp, "caseid",		pAaaInfo->CaseID);
			WriteIndexfileField(fp, "objid",		pAaaInfo->ObjectID);
			WriteIndexfileField(fp, "limitaddr",	pAaaInfo->LimitAddr);
			WriteIndexfileField(fp, "limittype",	pAaaInfo->LimitType);
		}

		WriteIndexfileField(fp, "account",	pAaaInfo->UserAccount1);
		WriteIndexfileField(fp, "phone",	pAaaInfo->Phone1);
		WriteIndexfileField(fp, "2account", pAaaInfo->UserAccount2);
		WriteIndexfileField(fp, "2phone",	pAaaInfo->Phone2);	
		if (packh.type == ProxyTCP)
			WriteIndexfileField(fp,  "代理5",	"TRUE");
		LPCTSTR Language = "";
		char *pSplitxt = NULL;
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(231);
#endif
		if (v_bLanguage)
		{
			// 对照语种
			CString tmps;
			int flen=GetFileLength(pResquest->Filename);
			if (flen > 0)
			{
				char* ptext;
				if (flen>FILEBUFFERSIZE-10)
				{
					ptext = tmps.GetBufferSetLength(flen+10);
					memset(ptext+flen,0,9);
				}
				else
				{
					ptext = FILEBUFFER;
					memset(ptext+flen,0,9);
				}
				
				FILE * fs=0;
				fs=fopen(pResquest->Filename, "rb");
				if(fs > 0)
				{
					UINT readsize=0;
					readsize=fread(ptext,1,flen,fs);
					if(readsize > 0 && readsize < 700)
					{
						pSplitxt = CHUNKBUFFER;
						memcpy(pSplitxt, ptext, readsize);
						pSplitxt[readsize+1] = 0;
					}
					fclose(fs);
					Language = CheckSPL(ptext, readsize);
				}
			}
		}
		else
		{
			CString tmps;
			int flen=GetFileLength(pResquest->Filename);
			if(flen > 0 && flen < 700)
			{
				pSplitxt = CHUNKBUFFER;
				memset(pSplitxt+flen,0,9);
				FILE * fs=0;
				fs=fopen(pResquest->Filename, "rb");
				if(fs > 0)
				{
					UINT readsize=0;
					readsize=fread(pSplitxt,1,flen,fs);
					fclose(fs);
				}
			}
		}
		WriteIndexfileField(fp, "filename",		GetShortName(pResquest->Filename));
		WriteIndexfileField(fp, "Language",		Language);
		WriteIndexfileField(fp, "content-type", pResquest->Content_Type);
		WriteIndexfileField(fp, "content-length", pResquest->Content_length);
		
		WriteIndexfileField(fp, "代理",			pResquest->bproxy? "TRUE" : "");
		WriteIndexfileField(fp, "url",			pResquest->URL);
		WriteIndexfileField(fp, "referer",		pResquest->Referer);
		WriteIndexfileField(fp, "cookie",		pResquest->Cookie);
		WriteIndexfileField(fp, "cookie2",		pResquest->Cookie2);
		WriteIndexfileField(fp, "setcookie2",	pResquest->SetCookie2);
		WriteIndexfileField(fp, "user-agent",	pResquest->useragent);
		if (isReal)	
			WriteIndexfileField(fp,"扩展","TRUE");
		fclose(fp);

		// HttpSession2.cpp中有与下面代码大部分相同的一段处理.
		CString strCrc32;
		if(v_stat && v_statHttpup && stricmp(pro, "httpup") == 0 && pSplitxt != NULL)
		{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(232);
#endif
			PostFieldinfoList outlist;
			CString strCrcBase;
			DWORD rSplit = SplitHttpup(
				pResquest->URL,
				pResquest->Content_Type, 
				pSplitxt,
				strCrcBase, 
				outlist);
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(233);
#endif
			if(rSplit)
			{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(234);
#endif
				DWORD dwURL = v_crc32Calc.GetCRC(pResquest->URL, strlen(pResquest->URL));
				strCrc32.Format("%08X_%08X", dwURL, v_crc32Calc.GetCRC(strCrcBase, strCrcBase.GetLength()));

				// Httpup 拆解活动历史
				sip.Replace("_",".");
				dip.Replace("_",".");

				HistoryRecord hisRec;
				hisRec.SetRecord(IDX_BEGTM,		spytm);
				hisRec.SetRecord(IDX_ENDTM,		spytm);
				hisRec.SetRecord(IDX_FORWTYPE,	"1");
				hisRec.SetRecord(IDX_IP1,		sip);
				hisRec.SetRecord(IDX_IP2,		dip);
				hisRec.SetRecord(IDX_PORT1,		sport);
				hisRec.SetRecord(IDX_PORT2,		dport);
				hisRec.SetRecord(IDX_ADDR1,		sipaddr);
				hisRec.SetRecord(IDX_ADDR2,		dipaddr);
				hisRec.SetRecord(IDX_DATASRC,	pResquest->dataSrc);
				hisRec.SetRecord(IDX_ACCOUNT1,	pAaaInfo->UserAccount1);
				hisRec.SetRecord(IDX_PHONE1,	pAaaInfo->Phone1);
				hisRec.SetRecord(IDX_ACCOUNT2,	pAaaInfo->UserAccount2);
				hisRec.SetRecord(IDX_PHONE2,	pAaaInfo->Phone2);
				hisRec.SetRecord(IDX_PRO,		pro);

				hisRec.SetRecord(IDX_URL,		pResquest->URL);
				hisRec.SetRecord(IDX_DOMAIN,	GetDomain(pResquest->URL));
				hisRec.SetRecord(IDX_CRCID,		strCrc32);
				hisRec.SetRecord(IDX_COUNT,		"1");
				CStringArray arrHisRec;
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(235);
#endif
			
				//从此处处理通过上传拆解出来的QQ密码对照相关的信息
				WriteQQPassToMap(outlist,spytm);

				if(GetHttpupHisRecArray(outlist, hisRec, arrHisRec))
				{
					if(arrHisRec.GetSize() > 0)
						WriteArrToData(HTTP_PROTOCOL, HISTORY_DATA, "", &arrHisRec);
				}
			}	// end if 拆解成功
		}
	}
	catch (...)
	{
		WriteLog(strHttp, "WriteHttpUpIndexFIle() Error!!");
		fclose(fp);
		return FALSE;
	}*/
	return TRUE;
}

/************************************************************************/
/*	处理一个单包的Post数据,信息需要保存到文件中							*/
/************************************************************************/
// BOOL ParserPostUrl(HttpCommInfo& Resquest,int headpos, DATEHEADER& packh, LPCTSTR pPacket)
// {
// 	// 对照认证信息
// 	AAAInfo *pAAA;
// 	AAAInfo aaa;
// 	char sip[32];
// 	char dip[32];
// 	BOOL bRubbish=FALSE;
// 	BOOL ret=TRUE;
// 	CaseInfo cinfo;
// 	try
// 	{
// 		
// 		CString strurl;
// 		BOOL isCase = FALSE;
// 		sprintf(sip, "%03d.%03d.%03d.%03d", packh.sip[0], packh.sip[1], packh.sip[2], packh.sip[3]);
// 		sprintf(dip, "%03d.%03d.%03d.%03d", packh.dip[0], packh.dip[1], packh.dip[2], packh.dip[3]);
// 		CString smac, dmac;
// 		FormatMac(packh.mac, smac, dmac);
// 		
// 		pAAA = GetAAAInfo(*((DWORD*)packh.sip), *((DWORD*)packh.dip), aaa, packh.tm);
// 		if (pAAA==NULL)
// 		{
// 			if (packh.vlanid!=0)
// 			{
// 				// 没有匹配上帐号就把vlan加入倒帐号字段中
// 				aaa.UserAccount1.Format("VLanID_%d", packh.vlanid);
// 				// 匹配Vlan的线索
// 				if (v_opencasehttpup && SearchCaseByVlan(packh.vlanid, cinfo))
// 				{
// 					isCase = TRUE;
// 				}
// 			}
// 		}
// 		if (v_opencasehttpup)
// 		{
// 			if (!aaa.CaseID.IsEmpty())
// 			{
// 				isCase = TRUE;
// 				cinfo.CaseID = aaa.CaseID;
// 				cinfo.ObjectID = aaa.ObjectID;
// 				cinfo.LimitType = aaa.LimitType;
// 				cinfo.LimitAddr = aaa.LimitAddr;
// 			}
// 			else
// 			{
// 				
// 				if (SearchCaseByIP(sip, dip, cinfo))
// 					isCase = TRUE;
// 				else
// 				{
// 					if (SearchCaseByMAC(smac, dmac, cinfo))
// 						isCase = TRUE;
// 					else
// 					{
// 						strurl = Resquest.URL + ' ' + Resquest.Referer + ' ' + Resquest.Cookie +
// 							' ' + Resquest.Cookie2 + ' ' + Resquest.SetCookie2;
// 						// 对照IP+URL
// 						if (3!=SearchCaseByIPURL(sip, dip, strurl, strurl.GetLength(), cinfo))
// 						{
// 							if (!SearchCaseByURL(strurl, strurl.GetLength(), cinfo))// 对照URL
// 								SearchCaseByKey(strurl, strurl.GetLength(), cinfo);	// 对照关键字
// 						}
// 					}
// 				}
// 			}
// 
// 		}
// 
// 		if (!isCase)
// 		{
// 			if(pAAA)
// 				bRubbish=SearchBruuishByAAA(pAAA->Phone1,pAAA->UserAccount1);
// 			if(!bRubbish)
// 				bRubbish=SearchBruuishByAAA(pAAA->Phone2,pAAA->UserAccount2);
// 			if(!bRubbish)
// 				bRubbish=SearchBurrishByIP(sip,dip);
// 		}
// 			
// 		IMSGPROXY imsgproxy;
// 		imsgproxy.m_dataSrc = Resquest.dataSrc;
// 		imsgproxy.packettm=packh.tm;
// 		imsgproxy.Direction=1;
// 		imsgproxy.sip.Format("%s_%05d",sip,packh.sport);
// 		imsgproxy.dip.Format("%s_%05d",dip,packh.dport);
// 		memcpy(imsgproxy.csip, packh.sip, 4);
// 		memcpy(imsgproxy.cdip, packh.dip, 4);
// 		imsgproxy.smac = smac;
// 		imsgproxy.dmac = dmac;
// 		imsgproxy.datalen=Resquest.Content_length;
// 		imsgproxy.pPacketData=(char*)pPacket+headpos;
// 		imsgproxy.proxytype = (packh.type == ProxyTCP)? 1 : 0;
// 		//处理即时消息
// 		if (InsertImsgProxy(imsgproxy, Resquest, DDEPOS, cinfo.CaseID,
// 			cinfo.LimitType, cinfo.ObjectID, cinfo.LimitAddr, 
// 			aaa.UserAccount1, aaa.Phone1, aaa.UserAccount2, aaa.Phone2))
// 			return TRUE;
// 		Resquest.emlfield.Empty();
// 		
// 		FILE *fp =0;
// 		try
// 		{
// 			SYSTEMTIME nowtm;
// 			GetLocalTime(&nowtm);
// 			Resquest.Filename.Format("%s%04d%02d%02d%02d%02d%02d_%s_%05d_%s_%05d_%u.httpup.txt",
// 				HttpTmpPath, nowtm.wYear, nowtm.wMonth, nowtm.wDay, nowtm.wHour, nowtm.wMinute, 
// 				nowtm.wSecond, sip, packh.sport, dip, packh.dport, rand()+nowtm.wMilliseconds);
// 			Resquest.indexfilename=Resquest.Filename+".indexfile.txt";
// 			fp = fopen(Resquest.Filename, "a+b");
// 			if (fp != NULL)
// 			{
// 				//写文件
// 				int wsize=0 ;
// 				if(Resquest.Content_Type.Find("urlencoded")>0)
// 				{
// 					int len = packh.datalen-headpos;
// 					memcpy(FILEBUFFER, pPacket+headpos, len);
// 					memset(FILEBUFFER+len, 0, 8);
// 					ConverHttp2Html(FILEBUFFER, len);
// 					len = strlen(FILEBUFFER);
// 					if(cinfo.CaseID.IsEmpty())
// 						SearchCaseByKey(FILEBUFFER, len, cinfo);
// 					wsize=fwrite(FILEBUFFER, 1, len,fp);
// 					fclose(fp);
// 					fp=0;
// 				}
// 				else if(!Resquest.ifcompressed)
// 				{
// 					if(v_opencasehttpup && cinfo.CaseID.IsEmpty())
// 						SearchCaseByKey(pPacket+headpos, packh.datalen-headpos, cinfo);
// 					wsize=fwrite(pPacket+headpos,1,packh.datalen-headpos,fp);
// 					fclose(fp);
// 					fp=0;
// 				}
// 				else
// 				{
// 					wsize=fwrite(pPacket+headpos,1,packh.datalen-headpos,fp);
// 					fclose(fp);
// 					fp=0;
// 					DealCompressedFile(&Resquest);
// 					if(v_opencasehttpup && cinfo.CaseID.IsEmpty())
// 						SearchCaseFileByKey(Resquest.Filename, cinfo);
// 				}
// 			}
// 			else
// 				return TRUE;
// 		}
// 		catch(...)
// 		{
// 			WriteLog(strHttp, "ParserPostUrl(1) Error!!");
// 			if(fp!=0)
// 				fclose(fp);
// 			return TRUE;
// 		}
// 		if(cinfo.CaseID.IsEmpty())
// 		{
// 			if (bRubbish || !v_openflaghttpup)
// 				return TRUE;
// 			else if(SearchBurrishByURL(strurl))
// 				return TRUE;
// 		}
// 
// 
// 		if (WriteIndexFIle(&Resquest, &aaa, cinfo, packh, "httpup"))
// 		{
// 			CopyFileToTmpdir(cinfo.CaseID, Resquest.Filename, Resquest.indexfilename,
// 				Resquest.attfiles, pthisMissObj->m_nowstm, "httpup", pthisMissObj->m_ServerPower,
// 				(v_openflaghttpup)? pthisMissObj->m_ServerPower+1 : NULL);
// 		}
// 		else
// 			ret= TRUE;
// 	}
// 	catch(...)
// 	{
// 		WriteLog(strHttp, "ParserPostUrl(2) Error!!");
// 		return TRUE;
// 	}
// 	return ret;
// }

/************************************************************************/
/*	处理一个单包的下行数据数据,(处理 Msn Yahoo Icqaol的http下行数据)	*/
/************************************************************************/
// BOOL	ParserHttpOk(HttpCommInfo& Response,int headpos, DATEHEADER& packh, const char* pPacket)
// {
// 	BOOL ret=TRUE;
// 	int error=0;
// 	try
// 	{
// 		//只判断
// 		if(Response.Content_Type=="application/x-msn-messenger" && 
// 			Response.Content_length>0){
// 		}
// 		else if(Response.Content_length>4 && 
// 			memcmp(pPacket+headpos,"YMSG",4)==0){
// 			
// 		}
// 		else if(Response.Content_length>8 && 
// 			memcmp(pPacket+headpos+4,"YMSG",4)==0){
// 			
// 		}	
// 		else
// 			return 0;
// 
// 		AAAInfo *pAAA;
// 		AAAInfo aaa;
// 		char sip[30];
// 		char dip[30];
// 		sprintf(sip, "%03d.%03d.%03d.%03d", packh.sip[0], packh.sip[1], packh.sip[2], packh.sip[3]);
// 		sprintf(dip, "%03d.%03d.%03d.%03d", packh.dip[0], packh.dip[1], packh.dip[2], packh.dip[3]);
// 		CString smac, dmac;
// 		FormatMac(packh.mac, smac, dmac);
// 		pAAA = GetAAAInfo(*((DWORD*)packh.sip), *((DWORD*)packh.dip), aaa, packh.tm);
// 		if (aaa.CaseID.IsEmpty())
// 		{
// error=2;
// 			CaseInfo caseinfo;
// 			if (packh.vlanid!=0 && pAAA==NULL)
// 			{
// 				// 没有匹配上帐号就把vlan加入倒帐号字段中
// 				aaa.UserAccount1.Format("VLanID_%d", packh.vlanid);
// 				// 匹配Vlan的线索
// 				if (SearchCaseByVlan(packh.vlanid, caseinfo))
// 				{
// 					aaa.CaseID = caseinfo.CaseID;
// 					aaa.ObjectID = caseinfo.ObjectID;
// 					aaa.LimitAddr = caseinfo.LimitAddr;
// 					aaa.LimitType = caseinfo.LimitType;
// 				}
// 			}
// 			if (aaa.CaseID.IsEmpty())
// 			{
// 				if (SearchCaseByIP(sip, dip, caseinfo) ||
// 					SearchCaseByMAC(smac, dmac, caseinfo))
// 				{
// 					aaa.CaseID = caseinfo.CaseID;
// 					aaa.ObjectID = caseinfo.ObjectID;
// 					aaa.LimitAddr = caseinfo.LimitAddr;
// 					aaa.LimitType = caseinfo.LimitType;
// 				}
// 			}
// 		}
// error=3;
// 
// 		IMSGPROXY imsgproxy;
// 		imsgproxy.m_dataSrc = Response.dataSrc;
// 		imsgproxy.packettm=packh.tm;
// 		imsgproxy.Direction=2;
// 		imsgproxy.sip.Format("%s_%05d", sip, packh.sport);
// 		imsgproxy.dip.Format("%s_%05d", dip, packh.dport);
// 		memcpy(imsgproxy.csip, packh.sip, 4);
// 		memcpy(imsgproxy.cdip, packh.dip, 4);
// 		imsgproxy.smac = smac;
// 		imsgproxy.dmac = dmac;
// 		
// 		//处理即时消息
// 		imsgproxy.datalen=Response.Content_length;
// 		imsgproxy.pPacketData=(char*)pPacket+headpos;
// 		imsgproxy.proxytype = (packh.type==ProxyTCP)? 1 : 0;
// 		if (InsertImsgProxy(imsgproxy, Response, DDEPOS, aaa.CaseID, aaa.LimitType,
// 			aaa.ObjectID, aaa.LimitAddr,			
// 			aaa.UserAccount1, aaa.Phone1, aaa.UserAccount2, aaa.Phone2))
// 			return -3;
// 		Response.emlfield.Empty();
// //		if(Response.Content_Type=="application/x-msn-messenger" &&
// //			Response.Content_length>3 && (!bRubbish || CaseId!=""))
// //		{
// //			imsgproxy.datalen=Response.Content_length;
// //			imsgproxy.pPacketData=(char*)pPacket+headpos;
// //			InsertMsnProxy(imsgproxy , CaseId,LimitType,ObjectID  ,LimitAddr,AllWatchFlg,UserAccount,Phone,DDEPOS);
// //			return 1;
// //		}
// //		else if	(Response.Content_length>4 && 	
// //			memcmp(pPacket+headpos,"YMSG",4)==0 && (!bRubbish || CaseId!=""))
// //		{
// //			imsgproxy.datalen=Response.Content_length;
// //			imsgproxy.pPacketData=(char*)pPacket+headpos;		
// //			InsertYahooProxy(imsgproxy , CaseId,LimitType,ObjectID  ,LimitAddr,AllWatchFlg,UserAccount,Phone,DDEPOS);
// //			return 1;
// //		}
// //		else if	(Response.Content_length>8 && 	
// //			memcmp(pPacket+headpos+4,"YMSG",4)==0 && (!bRubbish || CaseId!=""))
// //		{
// //			imsgproxy.datalen=Response.Content_length-4;
// //			imsgproxy.pPacketData=(char*)pPacket+headpos+4;		
// //			InsertYahooProxy(imsgproxy , CaseId,LimitType,ObjectID  ,LimitAddr,AllWatchFlg,UserAccount,Phone,DDEPOS);
// //			return 1;
// //		}
// 	}
// 	catch(...)
// 	{
// 		WriteLog(strHttp, "ParserHttpOk() Error!!");
// 		return TRUE;
// 	}
// 	return ret;
// 
// }



BOOL UrlWriteStat(HttpCommInfo& Resquest, DATEHEADER& packh,CString sphone,CString suseracount)
{
	return FALSE;
/*lp	if (v_stat && v_statHttpup)
	{
		CString tsip,tdip;
		CString sipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)packh.sip)));
		CString dipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)packh.dip)));
		tsip.Format("%03d.%03d.%03d.%03d_%05d", packh.sip[0], packh.sip[1], packh.sip[2], packh.sip[3], packh.sport);
		tdip.Format("%03d.%03d.%03d.%03d_%05d", packh.dip[0], packh.dip[1], packh.dip[2], packh.dip[3], packh.dport);
		char strtm[20];
		GetTimeString(packh.tm, "%Y-%m-%d %H:%M:%S", strtm, 20);
		HistoryRecord hisRec;
		hisRec.SetRecord(IDX_BEGTM,		strtm);
		hisRec.SetRecord(IDX_ENDTM,		strtm);
		hisRec.SetRecord(IDX_FORWTYPE,	"1");
		hisRec.SetRecord(IDX_IP1,		tsip);
		hisRec.SetRecord(IDX_IP2,		tdip);
		hisRec.SetRecord(IDX_PORT1,		tsip.Right(5));
		hisRec.SetRecord(IDX_PORT2,		tdip.Right(5));
		hisRec.SetRecord(IDX_ADDR1,		sipaddr);
		hisRec.SetRecord(IDX_ADDR2,		dipaddr);
		hisRec.SetRecord(IDX_DATASRC,	Resquest.dataSrc);
		hisRec.SetRecord(IDX_ACCOUNT1,	suseracount);
		hisRec.SetRecord(IDX_PHONE1,	sphone);
		hisRec.SetRecord(IDX_PRO,		"httpurl");
		hisRec.SetRecord(IDX_STATE,		"web访问");
		CString turl;
		turl.Format("%s",Resquest.URL);
		turl.Replace("http://","");
		turl.Replace("\1","");
		hisRec.SetRecord(IDX_URL,		turl);
		hisRec.SetRecord(IDX_DOMAIN,	Resquest.Host);	
		CString tempstr;
		if(hisRec.GetHisRecord(tempstr))
			WriteArrToData(HTTP_PROTOCOL, HISTORY_DATA, tempstr);
	}
	*/
	return TRUE;
}

// 写HttpUrl文件
// void WriteHttpUrlFile(PACKETHEADER& packheader, HttpCommInfo* pResquest, CString sipaddr, CString dipaddr)
// {
/*lp	int zFreeSpace, zPercentFree;
	zPercentFree = GetZFreeDisk(zFreeSpace);
	if(zPercentFree < 15)
	{
		HttpTmpPath.Format("x:\\xxx\\tmp\\dce2\\");
	}
	else
	{
		HttpTmpPath.Format("v:\\dce\\tmp\\dce2\\");
	}
	
	ASSERT(v_opencasehttpdown == TRUE);
	WarnInfo warninfo;
	FILE *fp=0;
	SYSTEMTIME nowtm;
	GetLocalTime(&nowtm);
	CString indexfile;
	indexfile.Format("%s%04d%02d%02d%02d%02d%02d_%s_%05u_%s_%05u.%u.httpurl.indexfile.txt", HttpTmpPath,
		nowtm.wYear, nowtm.wMonth, nowtm.wDay, nowtm.wHour, nowtm.wMinute, nowtm.wSecond,
		packheader.sip, packheader.sport, packheader.dip, packheader.dport, nowtm.wMilliseconds+rand());
		
	try
	{
		fp=fopen(indexfile,"ab");
		if(!fp)
			return ;
		BOOL isReal;
		CString strSection;
		if (aaa.LimitType > 200)
		{
			AddRealIP(packheader.sip, aaa.CaseID, aaa.LimitAddr, aaa.LimitType);
			aaa.LimitType -= 200;
			isReal = FALSE;
		}
		else if (aaa.LimitType > 100)
		{
			aaa.LimitType -= 100;
			isReal = TRUE;
		}
		else
			isReal = FALSE;
		packheader.sip.Replace(".","_");
		packheader.dip.Replace(".","_");

// 		CString sipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)csip)));
// 		CString dipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)cdip)));
		char spytm[40];
		GetTimeString(pResquest->Stm, "%Y-%m-%d %H:%M:%S", spytm, 40);
		WriteIndexfileField(fp, "[1]\r\npro=httpurl\r\nspyaddr", DDEPOS);
		WriteIndexfileField(fp, "sip",		packheader.sip);
		char strport[8];
		sprintf(strport, "%05u", packheader.sport);
		WriteIndexfileField(fp, "sport",	strport);
		WriteIndexfileField(fp, "dip",		packheader.dip);
		sprintf(strport, "%05u", packheader.dport);
		WriteIndexfileField(fp, "dport",	strport);
		WriteIndexfileField(fp, "sipaddr",	sipaddr);
		WriteIndexfileField(fp, "dipaddr",	dipaddr);
		WriteIndexfileField(fp, "datasrc",	pResquest->dataSrc);
		WriteIndexfileField(fp, "ISP",		pResquest->strISP);
		WriteIndexfileField(fp, "smac",		packheader.smac);
		WriteIndexfileField(fp, "dmac",		packheader.dmac);
		WriteIndexfileField(fp, "tm",		spytm);
		
		WriteIndexfileField(fp, "caseid",	aaa.CaseID);
		WriteIndexfileField(fp, "objid",	aaa.ObjectID);
		WriteIndexfileField(fp, "limitaddr",aaa.LimitAddr);
		WriteIndexfileField(fp, "limittype",aaa.LimitType);
		
		WriteIndexfileField(fp, "account",	aaa.UserAccount1);
		WriteIndexfileField(fp, "phone",	aaa.Phone1);
		WriteIndexfileField(fp, "2account", aaa.UserAccount2);
		WriteIndexfileField(fp, "2phone",	aaa.Phone2);
		if (packheader.type == ProxyTCP)
			WriteIndexfileField(fp,  "代理5",			"TRUE");
		
//		WriteIndexfileField(fp, "content-type",		pResquest->Content_Type);
//		WriteIndexfileField(fp, "content-length",	pResquest->Content_length);
		WriteIndexfileField(fp,  "代理",			pResquest->bproxy? "TRUE" : "");
		WriteIndexfileField(fp, "url",				pResquest->URL);
		WriteIndexfileField(fp, "referer",			pResquest->Referer);
		WriteIndexfileField(fp, "cookie",			pResquest->Cookie);
		WriteIndexfileField(fp, "cookie2",			pResquest->Cookie2);
		WriteIndexfileField(fp, "setcookie2",		pResquest->SetCookie2);
		WriteIndexfileField(fp, "user-agent",		pResquest->useragent);
		if (isReal)	WriteIndexfileField(fp, "扩展", "TRUE");
		fclose(fp);
		fp = NULL;
		CStringArray attfiles;

		//写告警
		WriteWarnAlarmArrToData(HTTP_PROTOCOL,
			warninfo.fenleiarr,warninfo.ruleidarr,warninfo.peopletypearr,
			warninfo.peoplearr,warninfo.namearr,warninfo.cardarr,
			warninfo.keytypearr,spytm,"","httpurl",
			warninfo.levelarr,spytm,spytm,DDEPOS,warninfo.dummytypeArr);

		//写文件
		CopyFileToTmpdir(aaa.CaseID, "", indexfile, attfiles, pthisMissObj->m_nowstm,
			"httpurl", pthisMissObj->m_ServerPower, NULL,
			warninfo.ruleidarr,warninfo.fenleiarr,warninfo.levelarr,warninfo.cardarr,
			warninfo.namearr,warninfo.peopletypearr,warninfo.peoplearr,warninfo.keytypearr,
			warninfo.IsOnlyAlarmArr,
			warninfo.dummytypeArr);
		
	}
	catch (...)
	{
		WriteLog(strHttp, "WriteHttpUrlFile() Error!!");
		if (fp!=NULL)
			fclose(fp);
	}*/
// }

BOOL    WriteQQPassToMap(PostFieldinfoList outlist,CString spytm)
{
// 	CString		strqqid, strqqpass;
// 	CString     str;
// 
// 	CStringArray  qqpassarr;
// 
// 	PostFieldinfoListIt beginpost,endpost,tmppost;
// 	beginpost	= outlist.begin();
// 	tmppost		= beginpost;
// 	endpost		= outlist.end();
// 
// 	CStringArray qqinfolist;
// 	CStringArray qqsplitinfolist;
// 	LPQQMapInfo   tmpinfo;
// 	CString      key;
// 
// 	while(tmppost!=endpost)
// 	{
// 		BOOL      bexist = FALSE;
// 
// 		if(!tmppost->strkey.IsEmpty())
// 		{
// 			if(QQInfo.Lookup(tmppost->strkey,tmpinfo))
// 			{
// 				//ID--->1
// 				if(tmppost->FieldType == 1)
// 				{
// 					for(int k=0;k<tmpinfo->uservaluelist.GetSize();k++)
// 					{
// 						if(tmpinfo->uservaluelist.GetAt(k).CompareNoCase(tmppost->strvalue) ==0)
// 						{
// 							bexist = TRUE;
// 							break;
// 						}
// 					}
// 
// 					if(!bexist)
// 						tmpinfo->uservaluelist.Add(tmppost->strvalue);
// 				}
// 				//PASS--->2
// 				else if(tmppost->FieldType == 2)
// 				{
// 					for(int k=0;k<tmpinfo->passvaluelist.GetSize();k++)
// 					{
// 						if(tmpinfo->passvaluelist.GetAt(k).CompareNoCase(tmppost->strvalue) ==0)
// 						{
// 							bexist = TRUE;
// 							break;
// 						}
// 					}
// 					
// 					if(!bexist)
// 						tmpinfo->passvaluelist.Add(tmppost->strvalue);
// 				}
// 
// 				QQInfo.SetAt(tmppost->strkey,tmpinfo);			
// 			}
// 			else
// 			{
// 				//ID--->1
// 				LPQQMapInfo	tmpmapinfo = new QQMapInfo;
// 				if(tmppost->FieldType == 1)
// 				{
// 					tmpmapinfo->uservaluelist.Add(tmppost->strvalue);
// 				}
// 				//PASS--->2
// 				else if(tmppost->FieldType == 2)
// 				{
// 					tmpmapinfo->passvaluelist.Add(tmppost->strvalue);
// 				}
// 				
// 				QQInfo.SetAt(tmppost->strkey,tmpmapinfo);
// 			}	
// 		}
// 
// 		tmppost++;		
// 	}
// 
// 	for(POSITION pos=QQInfo.GetStartPosition();pos!=NULL;)
// 	{	
// 		QQInfo.GetNextAssoc(pos,key,tmpinfo);
// 		
// 		if(tmpinfo->passvaluelist.GetSize()>0 && tmpinfo->uservaluelist.GetSize()>0)
// 		{
// 			for(int iuser=0;iuser<tmpinfo->uservaluelist.GetSize();iuser++)
// 			{
// 				for(int ipass=0;ipass<tmpinfo->passvaluelist.GetSize();ipass++)
// 				{
// 					str.Format("%s\1%s\1%s",tmpinfo->uservaluelist.GetAt(iuser),
// 						tmpinfo->passvaluelist.GetAt(ipass),spytm);
// 					
// 					qqpassarr.Add(str);
// 				}
// 			}
// 		}
// 	}
// 
// 	WriteArrToData(HTTP_PROTOCOL, QQPASS_DATA, "", &qqpassarr);
	return TRUE;
}

CString GetHttpInfo(HttpCommInfo& Request, BOOL bGetALL)
{
int error=0;
	try
	{
		CString str;
		int len = 0;
	//	if(bGetALL)
		{
			len = Request.m_URL.GetLength() + Request.m_Referer.GetLength()
			+ Request.m_Cookie.GetLength() + Request.m_Cookie2.GetLength()
			+ Request.m_SetCookie2.GetLength() + Request.m_Content_Type.GetLength()
			+ Request.m_useragent.GetLength() + Request.m_Language.GetLength() + Request.m_Charset.GetLength();
		}
	// 	else
	// 	{
	// 		len = Request.m_Host.GetLength();
	// 	}
error=1;
		len += 1000;
		char buf[4096];
		char* tp = buf;
		if (len >= (sizeof(buf)-20))
		{
			tp = str.GetBufferSetLength(len);
		}
		len = 0;
		int tmppos=g_moniterweb.GetMoniterweb(tp+len,Request.m_URL);
		len += tmppos;
error=2;
	//案件的url写详细的url，回溯的写域名部分，减少数据量。
	//对回溯部分匹配上监管网站的也写详细url
		if(bGetALL||tmppos)
		{
			len += WriteField(tp+len, "代理",			Request.m_bproxy? "TRUE" : "");
			len += WriteField(tp+len, "url",			Request.m_URL);
error=3;
			len += WriteField(tp+len, "referer",		Request.m_Referer);
			len += WriteField(tp+len, "cookie",			Request.m_Cookie);
			len += WriteField(tp+len, "cookie2",		Request.m_Cookie2);
			len += WriteField(tp+len, "setcookie2",		Request.m_SetCookie2);
			len += WriteField(tp+len, "content-type",	Request.m_Content_Type);
			len += WriteField(tp+len, "user-agent",		Request.m_useragent);
			len += WriteField(tp+len, "accept-language",Request.m_Language);
			len += WriteField(tp+len, "accept-charset",	Request.m_Charset);
			if (Request.m_bRbmm)
			{
				len += WriteField(tp+len, "rmb",	"true");
			}
			if(Request.m_iswap)//是wap
			{
				len += WriteField(tp+len, "wap",	"true");
			}

			if(!Request.m_WeiBoName.IsEmpty())
			{
				len+= WriteField(tp+len, "weiboname",	Request.m_WeiBoName);
			}

		}
		else
		{
			len += WriteField(tp+len, "代理",			Request.m_bproxy? "TRUE" : "");
			len += WriteField(tp+len, "url",			Request.m_URL);
			if(Request.m_iswap)//是wap
			{
				len += WriteField(tp+len, "wap",	"true");
			}
error=4;
			if(!Request.m_WeiBoName.IsEmpty())
			{
				len+= WriteField(tp+len, "weiboname",	Request.m_WeiBoName);
			}
		}
		return CString(tp, len);
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "GetHttpInfo Error!! error=%d",error);
	}
	return "";
}