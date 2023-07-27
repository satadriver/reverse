// DealHttpPost.cpp: implementation of the DealHttpPost class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "http.h"
#include "TeSpread.h"
#include "httpsession.h"
#include "publicfun.h"
#include "FreeGateDec.h"
#include "PostSplit.h"
#include "HttpProxy.h"
//迅雷传
#include "thread.h"
#include "VoipMessage.h"
//手机地图
#include "PhoneMap.h"
#include "PhoneNumCheat.h"
#include "PhoneMediaApp.h"
#include "PhoneAD.h"
#include "HttpProObserver.h"
//邮箱
#include "EMail/EMail.h"
//购物
#include "Shopping/Shopping.h"
#include "Contacts/Contacts.h"
//论坛
#include "Forum/Forum.h"
#include "Taxi/Taxi.h"
#include "QQhttpSession.h"
#include "Chating/Chating.h"
//安装软件
#include "Installed/Installed.h"
//
#include "Running/Running.h"
//手机设备信息
#include "PhoneDevice/PhoneDevice.h"
//电脑设备信息
#include "ComputerDevice/ComputerDevice.h"
//实名认证
#include "RealName/RealName.h"
//位置信息
#include "Location/Location.h"
//阅读
#include "Reading/Reading.h"
//云盘
#include "Cloud/YunPan.h"
//快递
#include "KuaiDi/KuaiDi.h"
//航班
#include "Airplane/Plane.h"
//搜索
#include "Search/Search.h"

#include "qqpim/QQSyncAsist.h"
//出行工具
#include "Traffic/Traffic.h"
//硬件特征信息
#include "HardwareInfo/HardwareInfo.h"
//博客
#include "Blog/Blog.h"
//招聘
#include "Employ/Employ.h"
//网站管理
#include "WebSite/WebSite.h"
//视频活动
#include "VideoActivity/VideoActivity.h"
//婚恋交友
#include "Marriage/Marriage.h"
//文件传输
#include "FileTransfer/FileTransfer.h"
//酒店预订
#include "Hotel/Hotel.h"
//供求关系
#include "Supply/Supply.h"
//第三方接口登入
#include "InterfaceLogin/InterfaceLogin.h"
//软件下载
#include "Software/Software.h"

inline CString CreateImsgFilename(LPCTSTR sip, LPCTSTR dip, LPCTSTR suffix,HttpSession* httpsession)
{
	CString fname;
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = httpsession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	fname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u.%s",
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute,
		now.wSecond, sip, dip, rand(), suffix);
	return fname;
}

BOOL IsheyooPacket(HttpCommInfo& Request, DATEHEADER& packh,
					LPCTSTR pPacket, HttpSession& httpsession);
BOOL Is66callPacket(HttpCommInfo& Request, DATEHEADER& packh,
					LPCTSTR pPacket, HttpSession& httpsession);
BOOL IsPopoPacket(HttpCommInfo& Request, DATEHEADER& packh,
					LPCTSTR pPacket, HttpSession& httpsession);			// 判断是不是网易popo的包
BOOL IsteltelPacket(HttpCommInfo& Request, DATEHEADER& packh,
					LPCTSTR pPacket, HttpSession& httpsession);
// 处理Post的包
//POST /in.jsp HTTP/1.1
//Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, application/msword, application/x-shockwave-flash, */*
//Referer: http://sms.163.com/usererror.php?paths=%2Fservice%2Fsendmsg_pop.php%3Fbd%3D%26send%3D1%26phone%3D13864301553%26message%3D%D7%A3%C4%E3%CA%A5%B5%AE%BF%EC%C0%D6%A3%A1%26send_type%3D
//Accept-Language: zh-cn
//Content-Type: application/x-www-form-urlencoded
//Accept-Encoding: gzip, deflate
//User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)
//Host: reg4.163.com
//Content-Length: 295
//Connection: Keep-Alive
//Cache-Control: no-cache
//Cookie: nn_V=846750514; NETEASE_SSN=13806434228; URSJESSIONID=afNW-Pcym8yc
//
//url=http%3A%2F%2Fsms.163.com%2Fservice%2Fredirect.php%3Foldpaths%3D%252Fservice%252Fsendmsg_pop.php%253Fbd%253D%2526send%253D1%2526phone%253D13864301553%2526message%253D%25D7%25A3%25C4%25E3%25CA%25A5%25B5%25AE%25BF%25EC%25C0%25D6%25A3%25A1%2526send_type%253D&username=13806434228&password=386660

void ProcessHttpPost(DATEHEADER& packh, const char* packet,HttpProtocol* httpprotocl)
{
	int error=0;
	try
	{
#ifdef	ADD_RUNSTATE_INFO
		AutoDestory AutoDebug(&v_httprunstate, 200, -200);
#endif
		// 判断Http头是否完整
		HttpSession httpsession(packh, g_httpFrom);
		httpsession.m_dceprocotol=httpprotocl;
		BOOL unixhead=FALSE;
		BOOL sessionflag=FALSE;
		HttpCommInfo* pRequest = &httpsession.m_Requestline;
		int i=Search4ByteFind(packet, packh.m_datalen); //0d 0a 0d 0a

		if (i<0)
		{
			i=Search2ByteFind(packet, packh.m_datalen); // 0a 0a
			unixhead=TRUE;
		}
error=1;
		if (i>0)
		{
			// 解析Http头
			memcpy(httpprotocl->m_HttpFileBuffer, packet, packh.m_datalen);
			memset(httpprotocl->m_HttpFileBuffer + packh.m_datalen, 0, 8);
			if (!FormatHttpReqHeader(*pRequest, httpprotocl->m_HttpFileBuffer, i, unixhead, packh.m_dip, packh.m_dport, &packh))
			{
				return ; // Http头解析失败
			}
			if (!pRequest->m_IDCInfo.IsEmpty())
			{
				// 是IDC的数据
				SOCKETKEY	key;
				memcpy(&key.sip, packh.m_sip, 4);
				memcpy(&key.dip, packh.m_dip, 4);
				key.sport = packh.m_sport;
				key.dport = packh.m_dport;
				g_HttpMap.AddHttpRequest(key, pRequest->m_IDCInfo);
				pRequest->m_IDCInfo.Empty();
			}
error=2;
			// 判断是不是即时消息代理
			if (IsImsgProxy(pRequest, packet+i, packh.m_datalen-i))
			{
				// 是即时消息代理就建立会话处理
				httpprotocl->m_PostSessCount++;
				HttpSession* s = CreateSession(packh, g_httpFrom,httpprotocl);
				s->m_dceprocotol=httpprotocl;
				if (s)
				{
					/*
					char key[64] = {0};
					sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d_%d",
						packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
						packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_sequence);
						m_httpMissObj->m_SocketMap.SetAt(key, (SocketSession*&)s);*/
					s->m_PacketType = 10;
					httpprotocl->m_httpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
					s->InsertPacket(packh, packet);
				}
				return;
			}
error=3;
		//	Request.m_dataSrc = GetDataSrc(packh, g_httpFrom, Request.m_strISP);

			// 判断数据是否完整
			if (packh.m_datalen >= i+pRequest->m_Content_length)
			{
				// 单包处理该Post
				httpprotocl->m_HttpupCount++;
#ifdef		DUMPHTTPPACKET
//				v_HttpDumpHttpPacket.DumpOnePacket(packh, packet);
#endif
				httpsession.m_spyaddr = g_DDEPOS;
				if(httpsession.m_Requestline.m_strBoundary.IsEmpty())
				{
// 					static int n = 1;
// 					printf("POST DANBAO : %d\n",n);
// 					n++;
					ParserPostData(*pRequest, i, packh, packet, httpsession);
				}
				else
				{
					httpprotocl->m_PostSessCount++;
					HttpSession* s = CreateSession(packh, g_httpFrom,httpprotocl);
					s->m_dceprocotol=httpprotocl;
					if (s)
					{
						/*
						char key[64] = {0};
						sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d_%d",
							packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
							packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_sequence);
						m_httpMissObj->m_SocketMap.SetAt(key, (SocketSession*&)s);*/
						httpprotocl->m_httpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
						s->InsertPacket(packh, packet);
					}
				}
				return;
			}
		}
error=4;
#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(210);
#endif

		// 是一个多包的POST就建立会话处理
		// 说明: 以前的逻辑是标准端口, 代理, 重点线索(包括IP+URL)的才建会话
		// 由于从日志上看(泉州, 博客网)不符合上述条件的包较少所以就改为只要是多包的POST就建会话处理
		httpprotocl->m_PostSessCount++;
		HttpSession* s = CreateSession(packh, g_httpFrom,httpprotocl);
		s->m_dceprocotol=httpprotocl;
		if (s)
		{
			/*
			char key[64] = {0};
			sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d_%d",
				packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
				packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_sequence);
						m_httpMissObj->m_SocketMap.SetAt(key, (SocketSession*&)s);*/
			httpprotocl->m_httpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
			s->InsertPacket(packh, packet);
		}
	}
	catch(...)
	{
		WriteLog(HTTPPRO, "ProcessHttpPost() Error!!=%d",error);
	}
	return;
}

/************************************************************************/
/*	处理一个单包的Post数据,信息需要保存到文件中							*/
/************************************************************************/
BOOL ParserPostData(HttpCommInfo& Request, int headpos, DATEHEADER& packh,
					LPCTSTR pPacket, HttpSession& httpsession)
{
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(220);
#endif
	// 对照认证信息
// 	AAAInfo aaa;
// 	WarnInfo warninfo;
	CString pro = "";
	try
	{
		// 判断并处理特殊浏览方式的包
		if (DealEspecialBrowse(Request,&httpsession))
		{
			if (Request.m_Browser==TE_FREEGATE)
			{
				// 自由之门
				pro = "freegateup";
				httpsession.m_OpenCaseFlg = v_opencasefgup;
				httpsession.m_OpenIndxFlg = v_openflagfgup;
				httpsession.m_OpenWarnFlg = v_openwarnfgup;
			}
			else if (Request.m_Browser==TE_DYNAPASS)
			{
				// 动网通
				pro = "dynapassup";
				httpsession.m_OpenCaseFlg = v_opencasedynaup;
				httpsession.m_OpenIndxFlg = v_openflagdynaup;
				httpsession.m_OpenWarnFlg = v_openwarndynaup;
			}
			Request.m_URL = ConverHttp2Html(Request.m_URL);
			Request.m_Referer = ConverHttp2Html(Request.m_Referer);
		}
		else
		{
// 			if(CheckFreeGate63(httpsession.m_sip.Left(15), httpsession.m_dip.Left(15)))
// 			{
// 				// 是自由之门6.3 
// 				pro = "freegateup";
// 				httpsession.m_OpenCaseFlg = v_opencasefgup;
// 				httpsession.m_OpenIndxFlg = v_openflagfgup;
// 				httpsession.m_OpenWarnFlg = v_openwarnfgup;
// 				Request.m_Browser = TE_FREEGATE;
// 				Request.m_BrowserVer = 0x603;
// 				pro = "freegateup";
// 			}
// 			else
//			HttpProObserver *hpo = HttpProObserver::Instance(); 
//			hpo->Process(&httpsession,pPacket+headpos);

			// 判断是不是网易泡泡的包
			if (IsPopoPacket(Request, packh, pPacket+headpos, httpsession))
			{
				return TRUE;
			}
			else if (ProcQQPimHttpUpPacket(&httpsession,packh,pPacket,TRUE))
			{
				return TRUE;
			}
			else if (IsheyooPacket(Request, packh, pPacket+headpos, httpsession))
			{
				return TRUE;
			}
			else if (IsteltelPacket(Request, packh, pPacket+headpos, httpsession))
			{
				return TRUE;
			}
			else if (Is66callPacket(Request, packh, pPacket+headpos, httpsession))
			{
				return TRUE;
			}
			else if (isThreadPacket(httpsession))
			{
				if(thunder_DealThunderSend(httpsession))
				{
					return TRUE;
				}			
			}
			else if (isThreadLoginPacket(httpsession))
			{
				if(thunder_DealThunderLogin(httpsession,pPacket+headpos))
				{
					return TRUE;
				}
			}
			/*
			else if(isMobileWeiboPostPacket(&httpsession))
			{
				if(mobileweibo_DealWeiboIndex(&httpsession,pPacket+headpos))
				{
					return TRUE;
				}
			}
			else if(isPapaPacket(&httpsession))
			{
				if(papa_DealWeiboIndex(&httpsession,pPacket+headpos))
				{
					return TRUE;
				}
			}	
			else if( int nWeChatCase = isWeixinPacket(&httpsession))
			{
				if(Weixin_DealPost(&httpsession,pPacket+headpos))
				{
					return TRUE;
				}
			}
			//处理微博163数据
			else if(WeiboWangyi_is_weibo_packet(&httpsession))
			{
				WeiboWangyi wbwy;
				if(wbwy.process_weibo(&httpsession,pPacket+headpos))
				{
					return TRUE;
				}
			}
			*/
			/*
			else if(isDidiPostPacket(&httpsession))
			{
				if (ProcessDiDiTaxiTextOrder(&httpsession,pPacket+headpos))
				{
					return TRUE;
				}
			}*/
			else if (int nValue = IsVoipALITONGMessage(&httpsession))
			{
				if (VoipALITONGMessage(&httpsession,pPacket+headpos,nValue))
				{
					return TRUE;
				}
			}
			else if (IsVoipZHCALLMessage(&httpsession))
			{
				if (VoipZHCALLMessage(&httpsession,pPacket+headpos))
				{
					return TRUE;
				}
			}
			else if (IsVoip97CALLMessage(&httpsession))
			{
				if (Voip97CALLMessage(&httpsession,pPacket+headpos))
				{
					return TRUE;
				}
			}
			else if (IsVoipDYTMessage(&httpsession))
			{
				if (VoipDYTMessage(&httpsession,pPacket+headpos))
				{
					return TRUE;
				}
			}
			//qq 会话
			else if(int iValue = IsQQhttpSession(&httpsession))
			{
				ProcessQQhttpSession(&httpsession,pPacket,iValue) ;
			}
			else if ( DealVoipCall(&httpsession,pPacket+headpos) )
			{
				return TRUE;
			}		
			else if (DealPhoneMap(&httpsession,pPacket))
			{
				return TRUE;
			}
			else if (DealPhoneMedia(&httpsession,pPacket))
			{
				return TRUE;
			}
			else if (DealPhoneNumCheat(&httpsession,pPacket))
			{
				return TRUE;
			}
			else if (DealPhoneAd(&httpsession,pPacket))
			{
				return TRUE;
			}
			//邮箱
			else if (email::EMail::ProcessEMail(&httpsession,pPacket))
			{
				return TRUE;
			}
			//购物包
			else if (shopping::Shopping::ProcessShopping(&httpsession,pPacket))
			{
				return TRUE;
			}
			//会话
			else if (contacts::Contacts::ProcessContacts(&httpsession,pPacket))
			{
				return TRUE;
			}
			//论坛
			else if (forum::Forum::ProcessForum(&httpsession,pPacket))
			{
				return TRUE;
			}
			//搜索引擎
			else if (search::Search::ProcessSearch(&httpsession,pPacket))
			{
				return TRUE;
			}
			//出租
			else if (taxi::Taxi::ProcessTaxi(&httpsession,pPacket))
			{
				return TRUE;
			}
			//聊天
			else if (chating::Chating::ProcessChating(&httpsession,pPacket))
			{
				return TRUE;
			}
			//助手
			else if (installed::Installed::ProcessInstalled(&httpsession,pPacket))
			{
				return TRUE;
			}
			//运行
			else if (running::Running::ProcessRunning(&httpsession,pPacket))
			{
				return TRUE;
			}
			//手机设备
			else if (phonedevice::PhoneDevice::ProcessPhoneDevice(&httpsession,pPacket))
			{
				return TRUE;
			}
			//Airplane
			else if (plane::Airplane::ProcessAirplane(&httpsession,pPacket))
			{
				return TRUE;
			}
			//电脑设备
			else if (computerdevice::ComputerDevice::ProcessComputerDevice(&httpsession,pPacket))
			{
				return TRUE;
			}
			//真实姓名
			else if (realname::RealName::ProcessRealName(&httpsession,pPacket))
			{
				return TRUE;
			}
			//定位信息
			else if (location::Location::ProcessLocation(&httpsession,pPacket))
			{
				return TRUE;
			}
			//博客
			else if (blog::Blog::ProcessBlog(&httpsession,pPacket))
			{
				return TRUE;
			}
			//阅读
			else if (reading::Reading::ProcessReading(&httpsession,pPacket))
			{
				return TRUE;
			}
			//云盘
			else if (yunpan::YunPan::ProcessYunPan(&httpsession,pPacket))
			{
				return TRUE;
			}
			//旅行 
			else if (Traffic::ProcessTraffic(&httpsession,pPacket))
			{
				return TRUE;
			}
			//快递
			else if (kuaidi::KuaiDi::ProcessKuaidi(&httpsession,pPacket))
			{
				return TRUE;
			}
			//HardwareInfo
			else if (hardwareinfo::HardwareInfo::ProcessHardwareInfo(&httpsession,pPacket))
			{
				return TRUE;
			}
			//Employ
			else if (employ::Employ::ProcessEmploy(&httpsession,pPacket))
			{
				return TRUE;
			}
			//站长工具	
			else if (website::WebSite::ProcessWebSite(&httpsession,pPacket))
			{
				return TRUE;
			}
			//视频活动	
			else if (videoactivity::VideoActivity::ProcessVideoActivity(&httpsession,pPacket))
			{
				return TRUE;
			}
			//婚恋交友
			else if (marriage::Marriage::ProcessMarriage(&httpsession,pPacket))
			{
				return TRUE;
			}
			//文件传输
			else if (filetransfer::FileTransfer::ProcessFileTransfer(&httpsession,pPacket))
			{
				return TRUE;
			}	
			//酒店预订
			else if (hotel::Hotel::ProcessHotel(&httpsession,pPacket))
			{
				return TRUE;
			}
			//供求信息
			else if (supply::Supply::ProcessSupply(&httpsession,pPacket))
			{
				return TRUE;
			}
			//第三方接口登入
			else if (interfacelogin::InterfaceLogin::ProcessInterfaceLogin(&httpsession,pPacket))
			{
				return TRUE;
			}
			//软件下载
			else if (software::Software::ProcessSoftware(&httpsession,pPacket))
			{
				return TRUE;
			}
			else
			{
				// 一般上传
				httpsession.m_OpenCaseFlg = v_opencasehttpup;
				httpsession.m_OpenIndxFlg = v_openflaghttpup;
				httpsession.m_OpenWarnFlg = v_openwarnhttpup;
				Request.m_Browser = TE_NOTHING;
				pro = "httpup";
			}
		}
		CString strurl;
		BOOL isCase = FALSE;
		BOOL isCDMA = FALSE;
		CString smac, dmac;

		if (Request.m_Browser != TE_NOTHING)
		{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(221);
#endif
			if ((Request.m_Browser == TE_FREEGATE)
				&& (Request.m_BrowserVer < 0x603))
			{
				strurl = Request.m_URL + ' ' + Request.m_Referer;
				// 解密数据
				FreeGateDec fnet;
				fnet.InitDataKey(Request.m_FreenetKey1, Request.m_FreenetKey2);
				int len = packh.m_datalen-headpos;
				fnet.DecodeData((char*)pPacket+headpos, len);
			}
		}
		else
		{
// 			if (Request.m_useragent.Find("Windows") == -1)
// 			{
// 				return FALSE;
// 			}
// 			strurl = Request.m_URL + ' ' + Request.m_Referer + ' ' + Request.m_Cookie +
// 				' ' + Request.m_Cookie2 + ' ' + Request.m_SetCookie2;
			strurl.Format("%s  %s  %s  %s  %s", Request.m_URL, Request.m_Referer,
				Request.m_Cookie, Request.m_Cookie2, Request.m_SetCookie2);
		}

		strurl.MakeLower();	// 要匹配线索的URL必须转换成小写字符
		if (Request.m_Browser == TE_NOTHING)
		{
			IMSGPROXY imsgproxy;
			imsgproxy.m_dataSrc=httpsession.m_dataSrc;
			imsgproxy.packettm=packh.m_tm;
			imsgproxy.Direction=1;
			imsgproxy.sip = httpsession.m_sip;
			imsgproxy.dip = httpsession.m_dip;
			memcpy(imsgproxy.csip, packh.m_sip, 4);
			memcpy(imsgproxy.cdip, packh.m_dip, 4);
			imsgproxy.smac = smac;
			imsgproxy.dmac = dmac;
			imsgproxy.datalen=Request.m_Content_length;
			imsgproxy.pPacketData=(char*)pPacket+headpos;
			imsgproxy.proxytype = (packh.m_type == ProxyTCP)? 1 : 0;
			//处理即时消息
			if (InsertImsgProxy(imsgproxy, Request, g_DDEPOS, &httpsession))
			{
				return TRUE;
			}
		}

		//实体内容写入文件
		int len = packh.m_datalen-headpos;
		if (len <= 0)
		{
			return TRUE;
		}
		SYSTEMTIME now;
		GetLocalTime(&now);
		httpsession.m_realfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.txt",
			httpsession.m_dceprocotol->m_httpMissObj->GetTmpPath(), now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, httpsession.m_sip, httpsession.m_dip, ++httpsession.m_dceprocotol->m_httpFileNum, pro);
		httpsession.m_indexfname = httpsession.m_realfname + ".indexfile.txt";
		FILE *fp = NULL;
		fp = fopen(httpsession.m_realfname, "a+b");
		if (fp != NULL)
		{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(223);
#endif
			//写文件
			int wsize=0 ;
			if(Request.m_Content_Type.Find("urlencoded")>0)
			{
				memcpy(httpsession.m_dceprocotol->m_HttpFileBuffer, pPacket+headpos, len);
				memset(httpsession.m_dceprocotol->m_HttpFileBuffer+len, 0, 8);
				ConverHttp2Html(httpsession.m_dceprocotol->m_HttpFileBuffer, len);
				len = strlen(httpsession.m_dceprocotol->m_HttpFileBuffer);
				wsize = fwrite(httpsession.m_dceprocotol->m_HttpFileBuffer, 1, len,fp);
				fclose(fp);
				fp=0;
				if (wsize <= 0)
				{
					return TRUE;
				}
			}
			else if(!Request.m_ifcompressed)
			{
				wsize = fwrite(pPacket+headpos,1,packh.m_datalen-headpos,fp);
				fclose(fp);
				fp=0;
				if (wsize <= 0)
				{
					return TRUE;
				}
			}
			else
			{
				wsize = fwrite(pPacket+headpos,1,packh.m_datalen-headpos,fp);
				fclose(fp);
				fp=0;
				if (wsize <= 0)
				{
					return TRUE;
				}
//				DealCompressedFile(&Request);		
			}
		}
		else
		{
			return TRUE;
		}
	
		// 2:05
		// 匹配案件和预警
		DWORD state = 0x10000;		// 需要匹配ip+URL
		SearchCaseOrWarn(&httpsession, (char*)(LPCTSTR)strurl, strurl.GetLength(), state,pro);

		if (/*(httpsession.m_CaseArr.GetSize() == 0) && */(!httpsession.m_rubbishflag))
		{
			// 匹配URL的屏蔽信息
			httpsession.m_rubbishflag = SearchBurrishByURL(strurl);
		}

		if (Request.m_Browser == TE_WUJIE)
		{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(222);
#endif
			httpsession.WriteTeUrlIndexFile();
//			WriteTeUrlIndexFile(packh, g_httpFrom, httpsession, Request.m_BrowserVer);
			return TRUE;
		}

		// 2:30
		// 匹配语种
		httpsession.CheckLanguage(httpsession.m_dceprocotol->m_HttpFileBuffer, FILEBUFFERSIZE);

		// 拆解上传
		SplitHttpupToHis(Request.m_URL, Request.m_Content_Type, &httpsession, pro, packh,Request.m_Host, Request.m_bproxy);

		if(!Request.m_WeiBoName.IsEmpty())
		{
			pro=Request.m_WeiBoName;
		}
		// 匹配IDC数据源
		// 3:15
//		return TRUE;
		if (Request.m_Browser != TE_NOTHING)
		{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(224);
#endif
			// 写动网通和自由之门的文件
			//httpsession.WriteTeUrlIndexFile();
			WriteFreeGateFile(packh, Request, httpsession, g_DDEPOS, pro);
		}
		else
		{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(225);
#endif

			CString text = GetHttpInfo(Request);
			if (httpsession.m_CaseArr.GetSize()==0)
			{
				CString useragent = Request.m_useragent;
				useragent.MakeLower();
				// 非案件 非浏览器发出POST丢弃
				if (-1 == (g_ExplorerType1.find(useragent, useragent.GetLength()))
					&& (-1 == (g_ExplorerType2.find(useragent, useragent.GetLength()))))
				{
			//		return TRUE;
				}
				if(Request.m_Content_Type.CompareNoCase("application/octet-stream") == 0 )
					return TRUE;	/// 上传且二进制流不输出
			}
// 			int hlen = WriteField(g_HttpFileBuffer,		"url",			Request.m_URL);
// 			hlen += WriteField(g_HttpFileBuffer+hlen,	"referer",		Request.m_Referer);
// 			hlen += WriteField(g_HttpFileBuffer+hlen,	"cookie",		Request.m_Cookie);
// 			hlen += WriteField(g_HttpFileBuffer+hlen,	"cookie2",		Request.m_Cookie2);
// 			hlen += WriteField(g_HttpFileBuffer+hlen,	"setcookie2",	Request.m_SetCookie2);
// 			hlen += WriteField(g_HttpFileBuffer+hlen,	"user-agent",	Request.m_useragent);
// 			memset(g_HttpFileBuffer+hlen, 0, 8);
			httpsession.WriteIndexFile(pro, pro, text, httpsession.m_dceprocotol->m_httpMissObj);
		}
		// 5:20
	}
	catch(...)
	{
		WriteLog(HTTPPRO, "ParserPostData Error!!");
	}
	return TRUE;
}

//*************************************************
//	函数名称:		IsPopoPacket
//	描述:			判断是不是网易popo的包
//	输入参数:		Request Http头信息
//					packh 包头
//					pPacket POST数据
//					httpsession http会话
//	输出参数:		
//	返回值:			是网易popo的包返回TRUE 否则返回FALSE
//	其它:			如果是网易popo的包那么就不再按照上传处理了
//*************************************************
BOOL IsPopoPacket(HttpCommInfo& Request, DATEHEADER& packh,
					LPCTSTR pPacket, HttpSession& httpsession)
{

	static Search SerUser("username=");
	int pos = SerUser.find(pPacket);
//		WriteLog(HTTPPRO, "IsPopoPacket %d,---%s----%s",pos,Request.m_Host,Request.m_URI);

	if ((pos >= 0)
		&& (Request.m_Host.CompareNoCase("reg.163.com") == 0)
		&& (Request.m_URL.Find("/logins.jsp?&url=http://fm163.163.com/coremail/fcg/ntesdoor2") >= 0))
	{
//				WriteLog(HTTPPRO, "IsPopoPacket1");

		char* tp = strchr(pPacket+pos+9, '&');
		if (tp != NULL)
		{
			*tp = 0;
			tp = (char*)pPacket + pos + 9;		// popoID
//		WriteLog(HTTPPRO, "IsPopoPacket2");

			// 写对照文件
			httpsession.m_OpenIndxFlg = v_openflaghf;
			httpsession.m_OpenCaseFlg = v_opencasehf;
			httpsession.m_OpenWarnFlg = v_openwarnhf;
			CString userid;
			userid.Format("%s@163.com", tp);
			// 匹配案件、预警和屏蔽信息
			DWORD state = 0;
			SearchCaseOrWarn(&httpsession, userid, userid.GetLength(), state,"imsg_popo");

			// 处理IDC数据源
			CString tmpisp;
			if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_cdip), tmpisp))
			{
				if (httpsession.m_dataSrc == DATASRC_OTHER)
				{
					httpsession.m_dataSrc = DATASRC_IDC;
					httpsession.m_strISP = tmpisp;
				}
				if (v_statIdc)
				{
					CString ttstr;
					ttstr.Format("%s\1%s\1popo\1即时消息\1%s\1网易popo\1%s\1\1", 
						httpsession.m_dip.Left(15), httpsession.m_strISP, httpsession.m_dip.Right(5), TYPESTATE_DECIDED);
					v_IdcServerStat[HTTP_PROTOCOL].AddServer(ttstr);
				}
			}
			else if ((httpsession.m_dataSrc == DATASRC_OTHER)
				&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_csip), httpsession.m_strISP)))
			{
				httpsession.m_dataSrc = DATASRC_IDC;
			}

			// 输出活动历史
			if (v_statGame)
			{
				HistoryRecord hisRec;
 				hisRec.SetRecord(IDX_FORWTYPE,	"1");
 				hisRec.SetRecord(IDX_ID,		userid);
				hisRec.SetRecord(IDX_USER,		userid);

				hisRec.SetRecord(IDX_STATE, "上线");
				httpsession.WriteHisData("imsg", "imsg_popo", &hisRec, httpsession.m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);//, HISTORY_DATA);
			}

			// 输出文件
			CString txt;
			txt.Format("user= %s\r\nstat= 上线\r\n", userid);
			httpsession.m_indexfname = CreateImsgFilename(httpsession.m_sip, httpsession.m_dip, "imsg_popo.indexfile.txt",&httpsession);
			httpsession.WriteIndexFile("imsg_popo", "imsg_popo", txt, httpsession.m_dceprocotol->m_httpMissObj);
			httpsession.m_CaseArr.RemoveAll();
			httpsession.m_WarnArr.RemoveAll();
			return TRUE;
		}
	}
	char tp[100];
	memset(tp,0,100);
	if ((Request.m_Host.CompareNoCase("blog.163.com") == 0)
		&& (Request.m_URL.Find("/blogservice/ToPopoService/") >= 0))
	{
		char* tpbegin = strstr(pPacket,"<userName>");
		if (tpbegin!=NULL)
		{
			char* tpend = strstr(tpbegin,".com</userName>");
			if (tpend == NULL)
			{
				return false;
			}
			int copylen = 0;
			copylen = tpend-tpbegin-6;
			if (copylen > 100)
			{
				copylen = 100;
			}
			memcpy(tp,tpbegin+10,copylen);
		}
		else
		{
			return false;
		}
		// 写对照文件
		httpsession.m_OpenIndxFlg = v_openflaghf;
		httpsession.m_OpenCaseFlg = v_opencasehf;
		httpsession.m_OpenWarnFlg = v_openwarnhf;
		CString userid;
		userid.Format("%s", tp);
		// 匹配案件、预警和屏蔽信息
		DWORD state = 0;
		SearchCaseOrWarn(&httpsession, userid, userid.GetLength(), state,"imsg_popo");

		// 处理IDC数据源
		CString tmpisp;
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_cdip), tmpisp))
		{
			if (httpsession.m_dataSrc == DATASRC_OTHER)
			{
				httpsession.m_dataSrc = DATASRC_IDC;
				httpsession.m_strISP = tmpisp;
			}
			if (v_statIdc)
			{
				CString ttstr;
				ttstr.Format("%s\1%s\1popo\1即时消息\1%s\1网易popo\1%s\1\1", 
					httpsession.m_dip.Left(15), httpsession.m_strISP, httpsession.m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[HTTP_PROTOCOL].AddServer(ttstr);
			}
		}
		else if ((httpsession.m_dataSrc == DATASRC_OTHER)
			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_csip), httpsession.m_strISP)))
		{
			httpsession.m_dataSrc = DATASRC_IDC;
		}

		// 输出活动历史
		if (v_statGame)
		{
			HistoryRecord hisRec;
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		userid);
			hisRec.SetRecord(IDX_USER,		userid);

			hisRec.SetRecord(IDX_STATE, "上线");
			httpsession.WriteHisData("imsg", "imsg_popo", &hisRec, httpsession.m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);//, HISTORY_DATA);
		}

		// 输出文件
		CString txt;
		txt.Format("user= %s\r\nstat= 上线\r\n", userid);
		httpsession.m_indexfname = CreateImsgFilename(httpsession.m_sip, httpsession.m_dip, "imsg_popo.indexfile.txt",&httpsession);
		httpsession.WriteIndexFile("imsg_popo", "imsg_popo", txt, httpsession.m_dceprocotol->m_httpMissObj);
		httpsession.m_CaseArr.RemoveAll();
		httpsession.m_WarnArr.RemoveAll();
		return TRUE;
	}
	return FALSE;
}



BOOL IsheyooPacket(HttpCommInfo& Request, DATEHEADER& packh,
					LPCTSTR pPacket, HttpSession& httpsession)
{
	if ((Request.m_Host.CompareNoCase("www.heyyo.com") == 0)
		&& (Request.m_URL.Find("/cgi-bin/clientbizcenter.cgi") >= 0))
	{
		char * phttpdata=(char*)pPacket;

		char *pp=strstr(phttpdata,"username=");
		if(pp==NULL)
			return FALSE;
		char* pp2=strstr(pp,"&");
		if(pp2==NULL)
			return FALSE;
		CString username=CString(pp+9,pp2-pp-9);

		 
    	if (username.IsEmpty())
		{
			return FALSE;
		}
		// 写对照文件
		httpsession.m_OpenIndxFlg = v_openflaghf;
		httpsession.m_OpenCaseFlg = v_opencasehf;
		httpsession.m_OpenWarnFlg = v_openwarnhf;
		// 匹配案件、预警和屏蔽信息
		DWORD state = 0;
		SearchCaseOrWarn(&httpsession, username, username.GetLength(), state,"voip_heyoo");

		// 处理IDC数据源
		CString tmpisp;
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_cdip), tmpisp))
		{
			if (httpsession.m_dataSrc == DATASRC_OTHER)
			{
				httpsession.m_dataSrc = DATASRC_IDC;
				httpsession.m_strISP = tmpisp;
			}
			if (v_statIdc)
			{
				CString ttstr;
				ttstr.Format("%s\1%s\1heyoo\1即时消息\1%s\1和悦网络电话\1%s\1\1", 
					httpsession.m_dip.Left(15), httpsession.m_strISP, httpsession.m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[HTTP_PROTOCOL].AddServer(ttstr);
			}
		}
		else if ((httpsession.m_dataSrc == DATASRC_OTHER)
			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_csip), httpsession.m_strISP)))
		{
			httpsession.m_dataSrc = DATASRC_IDC;
		}

		// 输出活动历史
		if (v_statImsgOther)
		{
			HistoryRecord hisRec;
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		username);
			hisRec.SetRecord(IDX_USER,		username);

			hisRec.SetRecord(IDX_STATE, "上线");
			httpsession.WriteHisData("voip_heyoo", "voip_heyoo", &hisRec, httpsession.m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);//, HISTORY_DATA);
		}

		// 输出文件
		CString txt;
		txt.Format("user= %s\r\nstat= 上线\r\n", username);
		httpsession.m_indexfname = CreateImsgFilename(httpsession.m_sip, httpsession.m_dip, "imsg_heyoo.indexfile.txt",&httpsession);
		httpsession.WriteIndexFile("voip_heyoo", "voip_heyoo", txt, httpsession.m_dceprocotol->m_httpMissObj);
		httpsession.m_CaseArr.RemoveAll();
		httpsession.m_WarnArr.RemoveAll();
		return TRUE;
	}
	return FALSE;
}


BOOL Is66callPacket(HttpCommInfo& Request, DATEHEADER& packh,
					LPCTSTR pPacket, HttpSession& httpsession)
{
	if ((Request.m_Host.CompareNoCase("phone.66call.net") == 0)
		&& (Request.m_URL.Find("/voipcall/login.php") >= 0))
	{
		char * phttpdata=(char*)pPacket;

		char *pp=strstr(phttpdata,"name=");
		if(pp==NULL)
			return FALSE;
		char* pp2=strstr(pp,"&");
		if(pp2==NULL)
			return FALSE;
		CString username=CString(pp+5,pp2-pp-5);

		if (username.IsEmpty())
		{
			return FALSE;
		}
		// 写对照文件
		httpsession.m_OpenIndxFlg = v_openflaghf;
		httpsession.m_OpenCaseFlg = v_opencasehf;
		httpsession.m_OpenWarnFlg = v_openwarnhf;
		// 匹配案件、预警和屏蔽信息
		DWORD state = 0;
		SearchCaseOrWarn(&httpsession, username, username.GetLength(), state,"voip_66call");

		// 处理IDC数据源
		CString tmpisp;
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_cdip), tmpisp))
		{
			if (httpsession.m_dataSrc == DATASRC_OTHER)
			{
				httpsession.m_dataSrc = DATASRC_IDC;
				httpsession.m_strISP = tmpisp;
			}
			if (v_statIdc)
			{
				CString ttstr;
				ttstr.Format("%s\1%s\166call\1即时消息\1%s\166call网络电话\1%s\1\1", 
					httpsession.m_dip.Left(15), httpsession.m_strISP, httpsession.m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[HTTP_PROTOCOL].AddServer(ttstr);
			}
		}
		else if ((httpsession.m_dataSrc == DATASRC_OTHER)
			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_csip), httpsession.m_strISP)))
		{
			httpsession.m_dataSrc = DATASRC_IDC;
		}

		// 输出活动历史
		if (v_statImsgOther)
		{
			HistoryRecord hisRec;
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		username);
			hisRec.SetRecord(IDX_USER,		username);

			hisRec.SetRecord(IDX_STATE, "上线");
			httpsession.WriteHisData("voip_66call", "voip_66call", &hisRec, httpsession.m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);//, HISTORY_DATA);
		}

		// 输出文件
		CString txt;
		txt.Format("user= %s\r\nstat= 上线\r\n", username);
		httpsession.m_indexfname = CreateImsgFilename(httpsession.m_sip, httpsession.m_dip, "imsg_66call.indexfile.txt",&httpsession);
		httpsession.WriteIndexFile("voip_66call", "voip_66call", txt, httpsession.m_dceprocotol->m_httpMissObj);
		httpsession.m_CaseArr.RemoveAll();
		httpsession.m_WarnArr.RemoveAll();
		return TRUE;
	}
	return FALSE;
}

BOOL IsteltelPacket(HttpCommInfo& Request, DATEHEADER& packh,
					LPCTSTR pPacket, HttpSession& httpsession)
{
	if ((Request.m_Host.CompareNoCase("log.teltel.com") == 0)
		&& (Request.m_URL.Find("/ClientLogServer.php") >= 0))
	{
		char * phttpdata=(char*)pPacket;

		char *pp=strstr(phttpdata,"accountid=");
		if(pp==NULL)
			return FALSE;
		char* pp2=strstr(pp,"&");
		if(pp2==NULL)
			return FALSE;
		CString username=CString(pp+10,pp2-pp-10);

		if (username.IsEmpty())
		{
			return FALSE;
		}
		// 写对照文件
		httpsession.m_OpenIndxFlg = v_openflaghf;
		httpsession.m_OpenCaseFlg = v_opencasehf;
		httpsession.m_OpenWarnFlg = v_openwarnhf;
		// 匹配案件、预警和屏蔽信息
		DWORD state = 0;
		SearchCaseOrWarn(&httpsession, username, username.GetLength(), state,"voip_teltel");

		// 处理IDC数据源
		CString tmpisp;
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_cdip), tmpisp))
		{
			if (httpsession.m_dataSrc == DATASRC_OTHER)
			{
				httpsession.m_dataSrc = DATASRC_IDC;
				httpsession.m_strISP = tmpisp;
			}
			if (v_statIdc)
			{
				CString ttstr;
				ttstr.Format("%s\1%s\1teltel\1即时消息\1%s\1teltel网络电话\1%s\1\1", 
					httpsession.m_dip.Left(15), httpsession.m_strISP, httpsession.m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[HTTP_PROTOCOL].AddServer(ttstr);
			}
		}
		else if ((httpsession.m_dataSrc == DATASRC_OTHER)
			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_csip), httpsession.m_strISP)))
		{
			httpsession.m_dataSrc = DATASRC_IDC;
		}

		// 输出活动历史
		if (v_statImsgOther)
		{
			HistoryRecord hisRec;
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		username);
			hisRec.SetRecord(IDX_USER,		username);

			hisRec.SetRecord(IDX_STATE, "上线");
			httpsession.WriteHisData("voip_teltel", "voip_teltel", &hisRec, httpsession.m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);//, HISTORY_DATA);
		}

		// 输出文件
		CString txt;
		txt.Format("user= %s\r\nstat= 上线\r\n", username);
		httpsession.m_indexfname = CreateImsgFilename(httpsession.m_sip, httpsession.m_dip, "imsg_teltel.indexfile.txt",&httpsession);
		httpsession.WriteIndexFile("voip_teltel", "voip_teltel", txt, httpsession.m_dceprocotol->m_httpMissObj);
		httpsession.m_CaseArr.RemoveAll();
		httpsession.m_WarnArr.RemoveAll();
		return TRUE;
	}
	return FALSE;
}