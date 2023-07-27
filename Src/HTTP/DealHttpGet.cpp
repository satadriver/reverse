// DealHttpGet.cpp: implementation of the DealHttpGet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "http.h"
#include "TeSpread.h"
#include "httpsession.h"
#include "publicfun.h"
#include "DealHttpGet.h"
#include "PostSplit.h"
#include "QQGet.h"
#include "VoipMessage.h"
#include "PCUniqueCode.h"
#include "PhoneMap.h"
#include "PhoneMediaApp.h"
#include "PhoneNumCheat.h"
#include "PhoneAD.h"
//邮箱
#include "EMail/EMail.h"
//论坛
#include "Forum/Forum.h"
//搜索
#include "Search/Search.h"
//打车工具
#include "Taxi/Taxi.h"
#include "Running/Running.h"
//手机设备信息
#include "PhoneDevice/PhoneDevice.h"
//电脑设备信息
#include "ComputerDevice/ComputerDevice.h"
#include "QQhttpSession.h"
//实名认证信息
#include "RealName/RealName.h"
//位置信息
#include "Location/Location.h"
//即时消息
#include "Chating/Chating.h"
//云盘
#include "Cloud/YunPan.h"
//快递
#include "KuaiDi/KuaiDi.h"
//航班
#include "Airplane/Plane.h"
//硬件特征信息
#include "HardwareInfo/HardwareInfo.h"
//博客
#include "Blog/Blog.h"
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

inline CString CreateImsgFilename(LPCTSTR sip, LPCTSTR dip, LPCTSTR suffix,HttpSession* s)
{
	CString fname;
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = s->m_dceprocotol->m_httpMissObj->GetTmpPath();
	fname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u.%s",
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute,
		now.wSecond, sip, dip, rand(), suffix);
	return fname;
}

//GET /url?sa=T&start=2&url=http%3A//soyolet.wx-e.com/ziliaoshoucang/diannaojishu/unicode02.htm HTTP/1.1
//Accept: */*
//Referer: http://www.google.com/search?hl=zh-CN&ie=UTF-8&oe=UTF-8&q=%E5%AD%97%E7%AC%A6%E9%9B%86+%E5%87%BD%E6%95%B0+C%2B%2B&lr=
//Accept-Language: zh-cn
//Accept-Encoding: gzip, deflate
//User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)
//Host: www.google.com
//Connection: Keep-Alive
//Cookie: PREF=ID=2487ee559e414dfc:TM=1071286718:LM=1071450637:TB=2:S=MrUoElGcJwI6kZAu

// 处理Get包的入口
// 一. 如果符合HttpDown抽样和代理, 就建会话
// 二. 如果符合重点参数(IP, MAC, Phone, Account), 就建会话
// 三. 解析Http头, 解析失败就返回
// 四. 如果符合IP+URL(Case/index), 就建会话
// 六. 如果是特殊浏览方式就写特殊浏览方式的url
// 五. 如果符合URL, KEY 就写HttpUrl
// 七. 结束 

/*************************************************
  函数名称:       ProcessHttpGet
  描述:          对httpget数据的处理
  输入参数:		 拆解httpget数据，比对案件，比对案件返回的信息包含那种类型的线索比对上的。根据比对
				上的线索类型，决定是建会话，还是继续判断是否是特浏，还是写httpurl。
  输出参数:         
  返回值:         
*************************************************/

void ProcessHttpGet(DATEHEADER& packh, const char* packet,HttpProtocol* httpprotocl)
{
	int err = 0;
	try
	{
#ifdef	ADD_RUNSTATE_INFO
		AutoDestory AutoDebug(&v_httprunstate, 100, -100);
#endif
		if(v_programtype == SYSTEM_DEPARTMENT||
		v_programtype == SYSTEM_HOTEL)
		{
			DealQQGet(packh,packet);
		}
err =1;
		CString text;	// 需要匹配案件和预警的串
		BOOL IsEspecialBrowse = FALSE;		// 是否是特浏
		HttpSession httpsession(packh, g_httpFrom);
		httpsession.m_dceprocotol=httpprotocl;
		HttpCommInfo* pRequest = &httpsession.m_Requestline;
		CString strsubpro="httpurl";

err=3;
		BOOL unixhead=FALSE;
		BOOL sessionflag=false;
		int i=Search4ByteFind(packet, packh.m_datalen);//0d 0a 0d 0a
		if (i<0)
		{
			i=Search2ByteFind(packet, packh.m_datalen);// 0a 0a
			unixhead=TRUE;
		}	
err=4;
		if(i < 0)
		{
			// Http头不完整
err=43;
			if (packh.m_datalen<=13)
			{ // 动网通一次只发送5－13个字节
				httpprotocl->m_GetSessCount++;
				HttpSession *s= new HttpSession(packh, g_httpFrom/*, isIndexIpUrl*/);
				s->m_dceprocotol=httpprotocl;
				if(s>0)
				{
					if (-2 >= s->InsertPacket(packh, packet))
						delete s;
					else
					{
						/*
						char key[64] = {0};
						sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d_%d",
							packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
							packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_sequence);
						m_httpMissObj->m_SocketMap.SetAt(key, (SocketSession*&)s);*/
						httpprotocl->m_httpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
					}
				}
				return;
			}
		}
		else
		{
			memcpy(httpprotocl->m_HttpFileBuffer, packet, packh.m_datalen);
			memset(httpprotocl->m_HttpFileBuffer+packh.m_datalen, 0, 32);
err=41;
			if(!FormatHttpReqHeader(*pRequest, httpprotocl->m_HttpFileBuffer, i, unixhead, packh.m_dip, packh.m_dport, &packh))
				return ;	// 解析Http头失败 丢弃该包
err=42;
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
err=43;

//			Deal360MID(*pRequest, httpsession);		// 如果有,输出360唯一值
//			DealBaidu dealbaidu;
			// 判断并处理特殊浏览方式的包

			if (DealEspecialBrowse(*pRequest,&httpsession))
			{
				IsEspecialBrowse = TRUE;
				if (pRequest->m_Browser==TE_FREEGATE)
				{
					strsubpro="freegateup";
					// 自由之门
					httpsession.m_OpenCaseFlg = v_opencasefgurl;
					httpsession.m_OpenWarnFlg = v_openwarnfgurl;
					httpsession.m_OpenIndxFlg = v_openflagfgurl;
					if(pRequest->m_BrowserVer >= 0x603)
					{
						// 获得一个定位包
						ASSERT(FALSE);
//						CreateFreeGate63(header.sip, header.dip, TRUE);
						return;
					}
				}
				else if (pRequest->m_Browser==TE_DYNAPASS)
				{
					strsubpro="dynapassup";
					// 动网通
					httpsession.m_OpenCaseFlg = v_opencasedynaurl;
					httpsession.m_OpenWarnFlg = v_openwarndynaurl;
					httpsession.m_OpenIndxFlg = v_openflagdynaurl;
				}
			}
			else if(DealSkype(*pRequest,(httpsession.m_id)))
			{
				httpsession.WriteSkypeIndex();
			}
			else if (IsPopoLogin(*pRequest, /*packh, packet+headpos, */httpsession))
			{
				return;
			}
			else if (IsWebQQLogin(*pRequest, /*packh, packet+headpos, */httpsession))
			{
				return;
			}
	
			else if(DealTaoBaoWangWang(*pRequest,(httpsession.m_id)))
			{
				httpsession.WriteTaoBaoIndex();
			}
		    //网页开放接口数据解析
/*
			else if(iswebopenapi_packet(*pRequest, httpsession))
			{
				return;
			}
*/
			else if (int iValue = IsQQhttpSession(&httpsession))
			{
				ProcessQQhttpSession(&httpsession,packet,iValue) ;
			}
			

			else if (DealVoipCall(&httpsession,packet))
			{
				return ;
			}

			else if (DealDevUC(&httpsession,packet))
			{
				return ;
			}
			else if (DealPhoneMap(&httpsession,packet))
			{
				return ;
			}
			else if (DealPhoneMedia(&httpsession,packet))
			{
				return ;
			}
		
			else if (DealPhoneNumCheat(&httpsession,packet))
			{
				return ;
			}
			else if (DealPhoneAd(&httpsession,packet))
			{
				return ;
			}
			else if (email::EMail::ProcessEMail(&httpsession,packet))
			{
				return ;
			}
			else if (forum::Forum::ProcessForum(&httpsession,packet))
			{
				return ;
			}
            //搜索引擎
			else if (search::Search::ProcessSearch(&httpsession,packet))
			{
				return ;
			}
			else if (taxi::Taxi::ProcessTaxi(&httpsession,packet))
			{
				return ;
			}			
			else if (running::Running::ProcessRunning(&httpsession,packet))
			{
				return ;
			}
			else if (phonedevice::PhoneDevice::ProcessPhoneDevice(&httpsession,packet))
			{
				return ;
			}
			else if (computerdevice::ComputerDevice::ProcessComputerDevice(&httpsession,packet))
			{
				return ;
			}
			else if (realname::RealName::ProcessRealName(&httpsession,packet))
			{
				return ;
			}
			else if (location::Location::ProcessLocation(&httpsession,packet))
			{
				return ;
			}
			else if (chating::Chating::ProcessChating(&httpsession,packet))
			{
				return ;
			}
			else if (yunpan::YunPan::ProcessYunPan(&httpsession,packet))
			{
				return ;
			}
			else if (blog::Blog::ProcessBlog(&httpsession,packet))
			{
				return ;
			}
			else if (kuaidi::KuaiDi::ProcessKuaidi(&httpsession,packet))
			{
				return ;
			}
			else if (plane::Airplane::ProcessAirplane(&httpsession,packet))
			{
				return ;
			}
			else if (hardwareinfo::HardwareInfo::ProcessHardwareInfo(&httpsession,packet))
			{
				return ;
			}
			//站长工具	
			else if (website::WebSite::ProcessWebSite(&httpsession,packet))
			{
				return ;
			}
			//视频活动	
			else if (videoactivity::VideoActivity::ProcessVideoActivity(&httpsession,packet))
			{
				return ;
			}
			//婚恋交友	
			else if (marriage::Marriage::ProcessMarriage(&httpsession,packet))
			{
				return ;
			}
			//文件传输
			else if (filetransfer::FileTransfer::ProcessFileTransfer(&httpsession,packet))
			{
				return ;
			}
			//酒店预订
			else if (hotel::Hotel::ProcessHotel(&httpsession,packet))
			{
				return ;
			}
			//供求信息
			else if (supply::Supply::ProcessSupply(&httpsession,packet))
			{
				return ;
			}
			//第三方接口登入
			else if (interfacelogin::InterfaceLogin::ProcessInterfaceLogin(&httpsession,packet))
			{
				return ;
			}
			//软件下载
			else if (software::Software::ProcessSoftware(&httpsession,packet))
			{
				return ;
			}

err=44;

			pRequest->m_URL = ConverHttp2Html(pRequest->m_URL);
err=441;

			pRequest->m_Referer = ConverHttp2Html(pRequest->m_Referer);
err=442;

			text.Format("%s  %s  %s  %s  %s", pRequest->m_URL, pRequest->m_Referer,
				pRequest->m_Cookie, pRequest->m_Cookie2, pRequest->m_SetCookie2);
			text.MakeLower();	// 要匹配线索的URL必须转换成小写字符
		}
err=45;

		// 匹配案件和预警
		DWORD state = 0x10000;		// 需要匹配ip+URL
		SearchCaseOrWarn(&httpsession, (char*)(LPCTSTR)text, text.GetLength(), state,strsubpro);
err=46;

		if (/*(httpsession.m_CaseArr.GetSize() == 0) && */(!httpsession.m_rubbishflag))
		{
			// 匹配URL的屏蔽信息
			httpsession.m_rubbishflag = SearchBurrishByURL(text);
		}
err=47;

		//中上了ip，mac，帐号，urlip类型线索，这些类型线索如没中，中的关键字类型线索不建会话
		//因为这种数据dde只放进了头
		if ((state & 63) && httpsession.m_CaseArr.GetSize() > 0)
		{
			// 建会话
			httpprotocl->m_GetSessCount++;
			HttpSession *s= new HttpSession(packh, g_httpFrom/*, isIndexIpUrl*/);
			s->m_dceprocotol=httpprotocl;
			if(s>0)
			{
				if (-2 >= s->InsertPacket(packh, packet))
				{
					delete s;
				}
				else
				{
					/*
					char key[64] = {0};
					sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d_%d",
						packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
						packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_sequence);
						m_httpMissObj->m_SocketMap.SetAt(key, (SocketSession*&)s);*/
					httpprotocl->m_httpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
				}
			}
			return;
		}
		else if (SearchIndexByIPURL(httpsession.m_sip.Left(15), httpsession.m_dip.Left(15), text, text.GetLength()))
		{
			// 符合预警的IP+URL
			// 建会话
			httpprotocl->m_GetSessCount++;
			HttpSession *s= new HttpSession(packh, g_httpFrom/*, isIndexIpUrl*/);
			s->m_dceprocotol=httpprotocl;
			if(s>0)
			{
				s->m_PacketType = 15;
				if (-2 >= s->InsertPacket(packh, packet))
				{
					delete s;
				}
				else
				{
					/*
					char key[64] = {0};
					sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d_%d",
						packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
						packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_sequence);
						m_httpMissObj->m_SocketMap.SetAt(key, (SocketSession*&)s);*/
					httpprotocl->m_httpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
				}
			}
			return;
		}
		else if (SearchIndexByIPPORT(httpsession.m_sip, httpsession.m_dip))
		{
			// 符合预警的IP+PORT
			// 建会话
			httpprotocl->m_GetSessCount++;
			HttpSession *s= new HttpSession(packh, g_httpFrom/*, isIndexIpUrl*/);
			s->m_dceprocotol=httpprotocl;
			if(s>0)
			{
				s->m_PacketType = 20;
				if (-2 >= s->InsertPacket(packh, packet))
				{
					delete s;
				}
				else
				{
					/*
					char key[64] = {0};
					sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d_%d",
						packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
						packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_sequence);
						m_httpMissObj->m_SocketMap.SetAt(key, (SocketSession*&)s);*/
					httpprotocl->m_httpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
				}
			}
			return;
		}
err=48;

		if (IsEspecialBrowse)
		{
err=481;

			CString suffix;
			pRequest->m_filetype = CheckSuffix(pRequest->m_URL, suffix);
			if ((pRequest->m_filetype != 1)
				&& ((httpsession.m_OpenIndxFlg)
					|| (httpsession.m_CaseArr.GetSize() > 0)
					|| (httpsession.m_WarnArr.GetSize() > 0)))
			{
				httpprotocl->m_HttpurlCount++;
				httpsession.WriteTeUrlIndexFile();
			}
err=50;

// 			if(CheckFreeGate63(header.sip, header.dip))
// 				return ;	// 自由之门Get不处理
			return;
		}
		else
		{
err=482;

			// 如果是案件就写HttpUrl
			CString strSuffix;
			int filetype = CheckSuffix(pRequest->m_URL, strSuffix);
err=483;
			if (!httpsession.m_Requestline.m_SearchType.IsEmpty())
			{
				httpsession.WriteSearchFile(FALSE);
			}
			if (filetype != 1)	// 不是图片和脚本
			{
				httpsession.SetIdcDomain();
			// 拆解上传
				SplitHttpDownToHis(pRequest->m_URL, pRequest->m_Content_Type, &httpsession);

				if (httpsession.m_CaseArr.GetSize() > 0 || httpsession.m_WarnArr.GetSize() > 0  || v_httpurl)
				{
err=484;

					httpsession.WriteHttpUrlFile((httpsession.m_CaseArr.GetSize() > 0 && v_opencasehttpdown)||(httpsession.m_WarnArr.GetSize() > 0));
				}
			}
err=49;

// 			if (httpsession.m_CaseArr.GetSize() > 0 && v_opencasehttpdown)
// 			{
// 				SYSTEMTIME now;
// 				GetLocalTime(&now);
// 				CString tmppath = g_httpMissObj->GetTmpPath();
// 				httpsession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_httpurl.indexfile.txt",
// 					tmppath, now.wYear, now.wMonth, now.wDay,
// 					now.wHour, now.wMinute, now.wSecond, httpsession.m_sip, httpsession.m_dip, ++g_httpFileNum);
// 				CString text = GetHttpInfo(*pRequest);
// 				httpsession.WriteIndexFile("httpurl", "httpurl", text, g_httpMissObj);
// 				g_HttpurlCount++;
// 			}
		}
	}
	catch(...)
	{
		WriteLog(HTTPPRO, "ProcessHttpGet() Error = %d!!", err);
	}
}

// 匹配IP+URL 返回0未匹配上 1是Case 2是Index 3是Only Case/Index IP
// URL必须转换成小写字符
// int SearchIPUrl(LPCTSTR ip1, LPCTSTR ip2, LPCTSTR url, int urllen, CaseInfo& caseinfo)
// {
// 	int r1 = SearchCaseByIPURL(ip1, ip2, url, urllen, caseinfo);
// 	if (r1 == 3)
// 		return 1;
// 	int r2 = SearchIndexByIPURL(ip1, ip2, url, urllen);
// 	if (r2 == 3)
// 		return 2;
// 	if (r1==1 || r2==1)
// 		return 3;
// 	return 0;
// }
// 
// 匹配URL, Key 返回TRUE是Case FALSE未匹配上
// BOOL SearchUrlAndKey(LPCTSTR url, int urllen, CaseInfo& caseinfo)
// {
// 	if (!SearchCaseByURL(url, urllen, caseinfo))
// 	{
// 		if (!SearchCaseByKey(url, urllen, caseinfo))
// 		{
// 			return FALSE;
// 		}
// 	}
// 	return TRUE;
// }
//----------------------------------------------------------

// 判断并处理skype数据
// 程序先写完，但不处理，等需要时放开
// 2处调用, DealHttpGet.cpp中处理单包Get, HttpSession.cpp ParserUrl处理会话中的Get
BOOL DealSkype(HttpCommInfo& Request,CString& id)
{
	// Host不是dip的就不处理
	CString tmpuri = Request.m_URI;
	CString tmphost = Request.m_Host;
//	WriteLog("dce","tmpuri:%s,tmphost:%s\r\n",tmpuri,tmphost);
	
	BOOL IsSkype = FALSE;
#ifndef		_DEBUG
//	Request.m_URI.Empty();
#endif
	if (Request.m_biphost)
		return FALSE;
	if (memcmp(tmphost,"skype.tom.com",13) == 0)
	{
		//Request URI: /user/index.php?skypeid=apapeng&str=6ca5b8498749ef5920b7943f7bde108f
		int begpos = tmpuri.Find("skype_id=");
		if (begpos >= 0)
		{
			tmpuri = tmpuri.Mid(begpos+9);
			int endpos = tmpuri.Find("&");
			if (endpos >= 0)
			{
				id = CString(tmpuri,endpos);
				IsSkype = TRUE;
			} 
			else //URL: http://skype.tom.com/ad/pop/fanke_110214_3/?skype_ver=5.1.4.98&skype_id=suyang1865311 
			{
				id = tmpuri;
				IsSkype = TRUE;
			}
// 			int endpos = tmpuri.Find("&");
// 			if (endpos >= 0)
// 			{
// 				id.Format("%s",tmpuri.Mid(begpos+8),endpos-begpos-8);
				// 得到的id错, 得到了 apapeng&str=6ca5b8498749ef5920b7943f7bde108f
// 				IsSkype = TRUE;
// 			}
		}
	}
	else if (memcmp(tmphost,"skypetools1.tom.com",19) ==0)
	{
		//http://skypetools1.tom.com/en_return_time.php?login_id=suyang1865311&code=730b1bdab3ec63d66fc0b6ad85387592
		int begpos = tmpuri.Find("login_id=");
		if (begpos >= 0)
		{
			tmpuri = tmpuri.Mid(begpos+9);
			int endpos = tmpuri.Find("&");
			if(endpos >= 0)
			{
				id = CString(tmpuri,endpos);
				IsSkype = TRUE;
			}
		}
	}	
	else if (memcmp(tmphost,"news.tom.com",12) ==0)
	{
		//http://news.tom.com/skype_miniportal_2011.html?skype_id=suyang1865311&cth=11&skype_ver=5.1.4.98&entrypoint=0
		int begpos = tmpuri.Find("skype_id=");
		if (begpos >= 0)
		{
			tmpuri = tmpuri.Mid(begpos+9);
			int endpos = tmpuri.Find("&");
			if(endpos >= 0){
				id = CString(tmpuri,endpos);
				IsSkype = TRUE;
			}
		}
	}
	//Host: hao.tom.com\r\n
	else if (strstr(tmphost,".tom.com") != NULL) //&& memcmp(tmpuri,"/skype/",7) == 0)
	{
		//Request URI: /skype/?skype_ver=4.2.4.100&skype_id=coolstar14&port=40279632 HTTP/1.1\r\n
		int begpos = tmpuri.Find("skype_id=");
		if (begpos >= 0)
		{
			tmpuri = tmpuri.Mid(begpos+9);
			int endpos = tmpuri.Find("&");
			if (endpos >= 0)
			{
				id = CString(tmpuri,endpos);
				IsSkype = TRUE;
			}
		}
	}
	//Host: ui.skype.com\r\n
	else if (memcmp(tmphost,"ui.skype.com",12) == 0)  //ver 3.8.0.154
	{
		//Request URI: /ui/0/3.8.0.154/en/getlatestversion?ver=3.8.0.154&uhash=178d2b6722d6b88fb6c46480df5e5e256
		if (tmpuri.Find("en/getlatestversion?ver=")>=0)
		{
			IsSkype = TRUE;
		}
	}
	else if (memcmp(tmphost,"game.tom.com",12) == 0)  // ver: 4.0.4.177
	{
		//Request URI: /service/skype_bind_service_new.php?cmd=querypassport&my_skype_id=web880&my_skype_ver=4.0.4.177&sign=285f25a0b76899355da94350deb3ecfe HTTP/1.1
		int begpos = tmpuri.Find("my_skype_id=");
		if (begpos >= 0)
		{
			CString tmpstr;
			tmpstr.Format("%s",tmpuri.Mid(begpos+12));

			int endpos = tmpstr.Find("&");
			if (endpos >= 0)
			{
				id.Format("%s",tmpstr.Left(endpos));
				IsSkype = TRUE;
			}
		}
	}
	else if (memcmp(tmphost,"api.skype.com",13) == 0) //ver : 6.16
	{
		int begpos = tmpuri.Find("users/");
		int endpos = tmpuri.Find("/profile/avatar");
		int offset = strlen("users/");
		if ( (begpos >= 0) && (endpos >= 0))
		{
			id.Format("%s",tmpuri.Mid(begpos+offset,endpos-begpos-offset));
			if (id != "webi_edu")
			{
				IsSkype = TRUE;
			}
			
		}
	}

	return IsSkype;
}

//处理方法同skype
BOOL DealTaoBaoWangWang(HttpCommInfo& Request,CString& id)
{
	// Host不是dip的就不处理
	CString tmpuri = Request.m_URI;
	CString tmphost = Request.m_Host;
	BOOL IsTaoBao = FALSE;
#ifndef		_DEBUG
//	Request.m_URI.Empty();
#endif
	if (Request.m_biphost)
		return FALSE;

	//Request URI: /imlogingw/tcp60login?ver=6.30.06&loginId=cntaobaotaobaotest HTTP/1.1
	if (memcmp(tmphost,"newallot.im.alisoft.com",23) == 0)
	{
		if (tmpuri.Find("tcp60login") > 0)
		{
			CString tmpurist;
			int begpos = tmpuri.Find("loginId=");
			if (begpos >= 0)
			{
				int endpos = tmpuri.Find(" ");
				if (endpos >= 0)
				{
					tmpurist = tmpuri.Left(endpos);
					id.Format("%s",tmpurist.Mid(begpos+8));
					id.Replace("cntaobao","");
				}
				else if (endpos == -1)
				{
					id.Format("%s",tmpuri.Mid(begpos+8));
					id.Replace("cntaobao","");
				}
			}
			IsTaoBao = TRUE;
		}
	}
	return IsTaoBao;
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
// GET /popo/update/verctrl/MainVersion.jsp?popo=2085&main=8.0.1.68 HTTP/1.1
// User-Agent: MyPopo
// Host: dl.popo.163.com
// Cache-Control: no-cache
// Cookie: _ntes_nnid=ee1b072727729f4ca978edc0e81fb16c,0; theaddr=%u5C71%u4E1C; NTES_adMenuNum=2; MAIL163_SSN=apapeng; Province=0530; City=0533; vjuids=217ce35c1.11ff95ba089.0.a437c933086fd; vjlast=1236839146,1236839146,30; _ntes_nuid=ee1b072727729f4ca978edc0e81fb16c
BOOL IsPopoLogin(HttpCommInfo& Request, HttpSession& httpsession)
{

//		WriteLog(HTTPPRO, "IsPopoPacket %d,---%s----%s",pos,Request.m_Host,Request.m_URI);

	if ((Request.m_Host.CompareNoCase("dl.popo.163.com") == 0)
		&& (Request.m_URL.Find("/popo/update/verctrl/MainVersion.jsp?popo=") >= 0))
	{
//				WriteLog(HTTPPRO, "IsPopoPacket1");
		int ifind = Request.m_Cookie.Find("MAIL163_SSN=");
		if (ifind<0)
		{
			return FALSE;
		}
		CString tmpuser;
		tmpuser=Request.m_Cookie.Mid(ifind+12);
		ifind= tmpuser.Find(";");
		
		CString userid;
		if (ifind>0)
		{
			userid.Format("%s@163.com", tmpuser.Left(ifind));
		}
		else
		{
			userid.Format("%s163.com",tmpuser);
		}
		if (userid.IsEmpty())
		{
			return FALSE;
		}
		// 写对照文件
		httpsession.m_OpenIndxFlg = v_openflaghf;
		httpsession.m_OpenCaseFlg = v_opencasehf;
		httpsession.m_OpenWarnFlg = v_openwarnhf;
		// 匹配案件、预警和屏蔽信息
		DWORD state = 0;
		SearchCaseOrWarn(&httpsession, userid, userid.GetLength(), state);

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

// lGET /loginproxy.html?login_level=3 HTTP/1.1
// Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/msword, application/vnd.ms-excel, *
// Referer: http://web.qq.com/
// Accept-Language: zh-cn
// Accept-Encoding: gzip, deflate
// User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)
// Host: web.qq.com
// Connection: Keep-Alive
// Cookie: adid=909972432; adSP=kEZFUnE53Dp7bPMqX5Sz3qRZiTmRVYM3D6Y0gQMGXyU=_20459_326830_1302059125_; 
//adVer=3439; ac=1,008,017; o_cookie=909972432; pgv_pvid=5044059122; pgv_flv=10.0; pgv_r_cookie=111854888744; 
//pt2gguin=o0914817713; uin_cookie=909972432; euin_cookie=DA1FF0EF667B6B901FEFDA8342188888C4F81FF9F3973F8C; 
//pgv_info=pgvReferrer=&ssid=s333908505; ptui_width=370; ptui_height=199; uin=o0914817713; 
//skey=@y73eLxTTy; ptisp=ctc; ptwebqq=48523c58a7d9b3e98e61324bd5819dea6851d3a562b2a591282a1e47de2a74be

//确定webqq登陆成功, qq号为 o_cookie=的值. host为web.qq.com, url为 /loginproxy.html?login_level=开始
BOOL IsWebQQLogin( HttpCommInfo& Request, HttpSession& httpsession )
{
	if ((Request.m_Host.CompareNoCase("web.qq.com") == 0)
		&& (Request.m_URL.Find("/loginproxy.html?login_level=") >= 0))
	{
		//				WriteLog(HTTPPRO, "IsPopoPacket1");
		int ifind = Request.m_Cookie.Find("o_cookie=");
		if (ifind<0)
		{
			return FALSE;
		}
		CString tmpuser;
		tmpuser=Request.m_Cookie.Mid(ifind+9);
		ifind= tmpuser.Find(";");
		
		CString userid;
		if (ifind>0)
			userid.Format("%s", tmpuser.Left(ifind));
		else
			userid.Format("%s",tmpuser);
		if (userid.IsEmpty())
		{
			return FALSE;
		}

		// 写对照文件
		httpsession.m_OpenIndxFlg = v_openflaghf;
		httpsession.m_OpenCaseFlg = v_opencasehf;
		httpsession.m_OpenWarnFlg = v_openwarnhf;
		// 匹配案件、预警和屏蔽信息
		DWORD state = 0;
		SearchCaseOrWarn(&httpsession, userid, userid.GetLength(), state);
		
		// 处理IDC数据源
// 		CString tmpisp;
// 		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_cdip), tmpisp))
// 		{
// 			if (httpsession.m_dataSrc == DATASRC_OTHER)
// 			{
// 				httpsession.m_dataSrc = DATASRC_IDC;
// 				httpsession.m_strISP = tmpisp;
// 			}
// 			if (v_statIdc)
// 			{
// 				CString ttstr;
// 				ttstr.Format("%s\1%s\1popo\1即时消息\1%s\1网易popo\1%s\1\1", 
// 					httpsession.m_dip.Left(15), httpsession.m_strISP, httpsession.m_dip.Right(5), TYPESTATE_DECIDED);
// 				v_IdcServerStat[HTTP_PROTOCOL].AddServer(ttstr);
// 			}
// 		}
// 		else if ((httpsession.m_dataSrc == DATASRC_OTHER)
// 			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_csip), httpsession.m_strISP)))
// 		{
// 			httpsession.m_dataSrc = DATASRC_IDC;
// 		}
		
		// 输出活动历史
// 		if (v_statGame)
// 		{
// 			HistoryRecord hisRec;
// 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
// 			hisRec.SetRecord(IDX_ID,		userid);
// 			hisRec.SetRecord(IDX_USER,		userid);
// 			
// 			hisRec.SetRecord(IDX_STATE, "上线");
// 			httpsession.WriteHisData("imsg", "imsg_webqq", &hisRec, httpsession.m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);//, HISTORY_DATA);
// 		}
		
		// 输出文件
		CString txt;
		txt.Format("user= %s\r\nstat= 上线\r\nprotype= webqq\r\n", userid);
		httpsession.m_indexfname = CreateImsgFilename(httpsession.m_sip, httpsession.m_dip, "imsg_oicq.indexfile.txt",&httpsession);
		httpsession.WriteIndexFile("imsg_oicq", "imsg_oicq", txt, httpsession.m_dceprocotol->m_httpMissObj);
		httpsession.m_CaseArr.RemoveAll();
		httpsession.m_WarnArr.RemoveAll();
		return TRUE;
	}
	return FALSE;
}

BOOL Deal360MID( HttpCommInfo& Request, HttpSession &httpsession )
{
	
// GET /qtup/s?mid=c042e713e1adf022f1df1ee5eb86440e&v=8.0.0.2001&.... HTTP/1.1
// Accept: */
// Accept-Encoding: gzip, deflate
// User-Agent: Mozilla/4.0 (NoExplorer; BindOk; ....)
// Host: stat.360safe.com
// Connection: Keep-Alive

	/*
	static time_t tmcheck = time(0);
	CString strmid;
	if ((Request.m_Host.CompareNoCase("stat.360safe.com") == 0)
		&& (Request.m_URL.Find("/qtup/s?mid=") >= 0))
	{
		int ifind = Request.m_URL.Find("/qtup/s?mid=");
		CString tmpuser;
		tmpuser=Request.m_URL.Mid(ifind+12);
		ifind= tmpuser.Find("&");
		if (ifind>0)
			strmid.Format("%s", tmpuser.Left(ifind));
		else
			strmid.Format("%s",tmpuser);
		if(strmid.GetLength() != 32)
			return FALSE;
	}
	if(strmid.IsEmpty())
		return FALSE;

	map<CString,time_t>::iterator It;
	It = g_uniqmid.find(strmid);
	if(It != g_uniqmid.end())
	{
		It->second = time(0);
	}
	else
	{
		if(FALSE == httpsession.m_AuthInfo.m_UserAccount1.IsEmpty())
		{
// 格式: 时间 类型 唯一值 上网帐号 ID号   
// 类型为 360或qq  .  
// 当为360时必须对上上网帐号,ID为空.  
// qq时可以不需要对上上网帐号,id为qq号.  
			CString tmpbcp;
			tmpbcp.Format("%s\t360\t%s\t%s\t%s", 
				CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"), 
				strmid, 
				httpsession.m_AuthInfo.m_UserAccount1,
				"");
			g_uniqmid[strmid] = time(0);
			WriteArrToData(HTTP_PROTOCOL, UNIMID_DATA, tmpbcp, NULL, NULL);
		}
	}

	if(abs(time(0)-tmcheck) > 10*60)
	{
		if(g_uniqmid.size() > 10000)
		{
			WriteLog("dce", "360 uniq mid map have item %d", g_uniqmid.size());
		}

		CStringArray arrErase;
		time_t tmnow = time(0);
		for (It = g_uniqmid.begin(); It != g_uniqmid.end(); It++)
		{
			if(abs(tmnow-It->second) > 30*60)
			{
				// 30分钟未再出现清理
				arrErase.Add(It->first);
			}
		}
		for (int iie = 0; iie < arrErase.GetSize() ; iie++)
		{
			g_uniqmid.erase(arrErase[iie]);
		}
	}
*/
	return TRUE;
}

