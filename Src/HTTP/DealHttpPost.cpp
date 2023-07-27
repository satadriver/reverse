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
//Ѹ�״�
#include "thread.h"
#include "VoipMessage.h"
//�ֻ���ͼ
#include "PhoneMap.h"
#include "PhoneNumCheat.h"
#include "PhoneMediaApp.h"
#include "PhoneAD.h"
#include "HttpProObserver.h"
//����
#include "EMail/EMail.h"
//����
#include "Shopping/Shopping.h"
#include "Contacts/Contacts.h"
//��̳
#include "Forum/Forum.h"
#include "Taxi/Taxi.h"
#include "QQhttpSession.h"
#include "Chating/Chating.h"
//��װ���
#include "Installed/Installed.h"
//
#include "Running/Running.h"
//�ֻ��豸��Ϣ
#include "PhoneDevice/PhoneDevice.h"
//�����豸��Ϣ
#include "ComputerDevice/ComputerDevice.h"
//ʵ����֤
#include "RealName/RealName.h"
//λ����Ϣ
#include "Location/Location.h"
//�Ķ�
#include "Reading/Reading.h"
//����
#include "Cloud/YunPan.h"
//���
#include "KuaiDi/KuaiDi.h"
//����
#include "Airplane/Plane.h"
//����
#include "Search/Search.h"

#include "qqpim/QQSyncAsist.h"
//���й���
#include "Traffic/Traffic.h"
//Ӳ��������Ϣ
#include "HardwareInfo/HardwareInfo.h"
//����
#include "Blog/Blog.h"
//��Ƹ
#include "Employ/Employ.h"
//��վ����
#include "WebSite/WebSite.h"
//��Ƶ�
#include "VideoActivity/VideoActivity.h"
//��������
#include "Marriage/Marriage.h"
//�ļ�����
#include "FileTransfer/FileTransfer.h"
//�Ƶ�Ԥ��
#include "Hotel/Hotel.h"
//�����ϵ
#include "Supply/Supply.h"
//�������ӿڵ���
#include "InterfaceLogin/InterfaceLogin.h"
//�������
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
					LPCTSTR pPacket, HttpSession& httpsession);			// �ж��ǲ�������popo�İ�
BOOL IsteltelPacket(HttpCommInfo& Request, DATEHEADER& packh,
					LPCTSTR pPacket, HttpSession& httpsession);
// ����Post�İ�
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
		// �ж�Httpͷ�Ƿ�����
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
			// ����Httpͷ
			memcpy(httpprotocl->m_HttpFileBuffer, packet, packh.m_datalen);
			memset(httpprotocl->m_HttpFileBuffer + packh.m_datalen, 0, 8);
			if (!FormatHttpReqHeader(*pRequest, httpprotocl->m_HttpFileBuffer, i, unixhead, packh.m_dip, packh.m_dport, &packh))
			{
				return ; // Httpͷ����ʧ��
			}
			if (!pRequest->m_IDCInfo.IsEmpty())
			{
				// ��IDC������
				SOCKETKEY	key;
				memcpy(&key.sip, packh.m_sip, 4);
				memcpy(&key.dip, packh.m_dip, 4);
				key.sport = packh.m_sport;
				key.dport = packh.m_dport;
				g_HttpMap.AddHttpRequest(key, pRequest->m_IDCInfo);
				pRequest->m_IDCInfo.Empty();
			}
error=2;
			// �ж��ǲ��Ǽ�ʱ��Ϣ����
			if (IsImsgProxy(pRequest, packet+i, packh.m_datalen-i))
			{
				// �Ǽ�ʱ��Ϣ����ͽ����Ự����
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

			// �ж������Ƿ�����
			if (packh.m_datalen >= i+pRequest->m_Content_length)
			{
				// ���������Post
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

		// ��һ�������POST�ͽ����Ự����
		// ˵��: ��ǰ���߼��Ǳ�׼�˿�, ����, �ص�����(����IP+URL)�ĲŽ��Ự
		// ���ڴ���־�Ͽ�(Ȫ��, ������)���������������İ��������Ծ͸�ΪֻҪ�Ƕ����POST�ͽ��Ự����
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
/*	����һ��������Post����,��Ϣ��Ҫ���浽�ļ���							*/
/************************************************************************/
BOOL ParserPostData(HttpCommInfo& Request, int headpos, DATEHEADER& packh,
					LPCTSTR pPacket, HttpSession& httpsession)
{
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(220);
#endif
	// ������֤��Ϣ
// 	AAAInfo aaa;
// 	WarnInfo warninfo;
	CString pro = "";
	try
	{
		// �жϲ��������������ʽ�İ�
		if (DealEspecialBrowse(Request,&httpsession))
		{
			if (Request.m_Browser==TE_FREEGATE)
			{
				// ����֮��
				pro = "freegateup";
				httpsession.m_OpenCaseFlg = v_opencasefgup;
				httpsession.m_OpenIndxFlg = v_openflagfgup;
				httpsession.m_OpenWarnFlg = v_openwarnfgup;
			}
			else if (Request.m_Browser==TE_DYNAPASS)
			{
				// ����ͨ
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
// 				// ������֮��6.3 
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

			// �ж��ǲ����������ݵİ�
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
			//����΢��163����
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
			//qq �Ự
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
			//����
			else if (email::EMail::ProcessEMail(&httpsession,pPacket))
			{
				return TRUE;
			}
			//�����
			else if (shopping::Shopping::ProcessShopping(&httpsession,pPacket))
			{
				return TRUE;
			}
			//�Ự
			else if (contacts::Contacts::ProcessContacts(&httpsession,pPacket))
			{
				return TRUE;
			}
			//��̳
			else if (forum::Forum::ProcessForum(&httpsession,pPacket))
			{
				return TRUE;
			}
			//��������
			else if (search::Search::ProcessSearch(&httpsession,pPacket))
			{
				return TRUE;
			}
			//����
			else if (taxi::Taxi::ProcessTaxi(&httpsession,pPacket))
			{
				return TRUE;
			}
			//����
			else if (chating::Chating::ProcessChating(&httpsession,pPacket))
			{
				return TRUE;
			}
			//����
			else if (installed::Installed::ProcessInstalled(&httpsession,pPacket))
			{
				return TRUE;
			}
			//����
			else if (running::Running::ProcessRunning(&httpsession,pPacket))
			{
				return TRUE;
			}
			//�ֻ��豸
			else if (phonedevice::PhoneDevice::ProcessPhoneDevice(&httpsession,pPacket))
			{
				return TRUE;
			}
			//Airplane
			else if (plane::Airplane::ProcessAirplane(&httpsession,pPacket))
			{
				return TRUE;
			}
			//�����豸
			else if (computerdevice::ComputerDevice::ProcessComputerDevice(&httpsession,pPacket))
			{
				return TRUE;
			}
			//��ʵ����
			else if (realname::RealName::ProcessRealName(&httpsession,pPacket))
			{
				return TRUE;
			}
			//��λ��Ϣ
			else if (location::Location::ProcessLocation(&httpsession,pPacket))
			{
				return TRUE;
			}
			//����
			else if (blog::Blog::ProcessBlog(&httpsession,pPacket))
			{
				return TRUE;
			}
			//�Ķ�
			else if (reading::Reading::ProcessReading(&httpsession,pPacket))
			{
				return TRUE;
			}
			//����
			else if (yunpan::YunPan::ProcessYunPan(&httpsession,pPacket))
			{
				return TRUE;
			}
			//���� 
			else if (Traffic::ProcessTraffic(&httpsession,pPacket))
			{
				return TRUE;
			}
			//���
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
			//վ������	
			else if (website::WebSite::ProcessWebSite(&httpsession,pPacket))
			{
				return TRUE;
			}
			//��Ƶ�	
			else if (videoactivity::VideoActivity::ProcessVideoActivity(&httpsession,pPacket))
			{
				return TRUE;
			}
			//��������
			else if (marriage::Marriage::ProcessMarriage(&httpsession,pPacket))
			{
				return TRUE;
			}
			//�ļ�����
			else if (filetransfer::FileTransfer::ProcessFileTransfer(&httpsession,pPacket))
			{
				return TRUE;
			}	
			//�Ƶ�Ԥ��
			else if (hotel::Hotel::ProcessHotel(&httpsession,pPacket))
			{
				return TRUE;
			}
			//������Ϣ
			else if (supply::Supply::ProcessSupply(&httpsession,pPacket))
			{
				return TRUE;
			}
			//�������ӿڵ���
			else if (interfacelogin::InterfaceLogin::ProcessInterfaceLogin(&httpsession,pPacket))
			{
				return TRUE;
			}
			//�������
			else if (software::Software::ProcessSoftware(&httpsession,pPacket))
			{
				return TRUE;
			}
			else
			{
				// һ���ϴ�
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
				// ��������
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

		strurl.MakeLower();	// Ҫƥ��������URL����ת����Сд�ַ�
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
			//����ʱ��Ϣ
			if (InsertImsgProxy(imsgproxy, Request, g_DDEPOS, &httpsession))
			{
				return TRUE;
			}
		}

		//ʵ������д���ļ�
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
			//д�ļ�
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
		// ƥ�䰸����Ԥ��
		DWORD state = 0x10000;		// ��Ҫƥ��ip+URL
		SearchCaseOrWarn(&httpsession, (char*)(LPCTSTR)strurl, strurl.GetLength(), state,pro);

		if (/*(httpsession.m_CaseArr.GetSize() == 0) && */(!httpsession.m_rubbishflag))
		{
			// ƥ��URL��������Ϣ
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
		// ƥ������
		httpsession.CheckLanguage(httpsession.m_dceprocotol->m_HttpFileBuffer, FILEBUFFERSIZE);

		// ����ϴ�
		SplitHttpupToHis(Request.m_URL, Request.m_Content_Type, &httpsession, pro, packh,Request.m_Host, Request.m_bproxy);

		if(!Request.m_WeiBoName.IsEmpty())
		{
			pro=Request.m_WeiBoName;
		}
		// ƥ��IDC����Դ
		// 3:15
//		return TRUE;
		if (Request.m_Browser != TE_NOTHING)
		{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(224);
#endif
			// д����ͨ������֮�ŵ��ļ�
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
				// �ǰ��� �����������POST����
				if (-1 == (g_ExplorerType1.find(useragent, useragent.GetLength()))
					&& (-1 == (g_ExplorerType2.find(useragent, useragent.GetLength()))))
				{
			//		return TRUE;
				}
				if(Request.m_Content_Type.CompareNoCase("application/octet-stream") == 0 )
					return TRUE;	/// �ϴ��Ҷ������������
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
//	��������:		IsPopoPacket
//	����:			�ж��ǲ�������popo�İ�
//	�������:		Request Httpͷ��Ϣ
//					packh ��ͷ
//					pPacket POST����
//					httpsession http�Ự
//	�������:		
//	����ֵ:			������popo�İ�����TRUE ���򷵻�FALSE
//	����:			���������popo�İ���ô�Ͳ��ٰ����ϴ�������
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

			// д�����ļ�
			httpsession.m_OpenIndxFlg = v_openflaghf;
			httpsession.m_OpenCaseFlg = v_opencasehf;
			httpsession.m_OpenWarnFlg = v_openwarnhf;
			CString userid;
			userid.Format("%s@163.com", tp);
			// ƥ�䰸����Ԥ����������Ϣ
			DWORD state = 0;
			SearchCaseOrWarn(&httpsession, userid, userid.GetLength(), state,"imsg_popo");

			// ����IDC����Դ
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
					ttstr.Format("%s\1%s\1popo\1��ʱ��Ϣ\1%s\1����popo\1%s\1\1", 
						httpsession.m_dip.Left(15), httpsession.m_strISP, httpsession.m_dip.Right(5), TYPESTATE_DECIDED);
					v_IdcServerStat[HTTP_PROTOCOL].AddServer(ttstr);
				}
			}
			else if ((httpsession.m_dataSrc == DATASRC_OTHER)
				&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_csip), httpsession.m_strISP)))
			{
				httpsession.m_dataSrc = DATASRC_IDC;
			}

			// ������ʷ
			if (v_statGame)
			{
				HistoryRecord hisRec;
 				hisRec.SetRecord(IDX_FORWTYPE,	"1");
 				hisRec.SetRecord(IDX_ID,		userid);
				hisRec.SetRecord(IDX_USER,		userid);

				hisRec.SetRecord(IDX_STATE, "����");
				httpsession.WriteHisData("imsg", "imsg_popo", &hisRec, httpsession.m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);//, HISTORY_DATA);
			}

			// ����ļ�
			CString txt;
			txt.Format("user= %s\r\nstat= ����\r\n", userid);
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
		// д�����ļ�
		httpsession.m_OpenIndxFlg = v_openflaghf;
		httpsession.m_OpenCaseFlg = v_opencasehf;
		httpsession.m_OpenWarnFlg = v_openwarnhf;
		CString userid;
		userid.Format("%s", tp);
		// ƥ�䰸����Ԥ����������Ϣ
		DWORD state = 0;
		SearchCaseOrWarn(&httpsession, userid, userid.GetLength(), state,"imsg_popo");

		// ����IDC����Դ
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
				ttstr.Format("%s\1%s\1popo\1��ʱ��Ϣ\1%s\1����popo\1%s\1\1", 
					httpsession.m_dip.Left(15), httpsession.m_strISP, httpsession.m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[HTTP_PROTOCOL].AddServer(ttstr);
			}
		}
		else if ((httpsession.m_dataSrc == DATASRC_OTHER)
			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_csip), httpsession.m_strISP)))
		{
			httpsession.m_dataSrc = DATASRC_IDC;
		}

		// ������ʷ
		if (v_statGame)
		{
			HistoryRecord hisRec;
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		userid);
			hisRec.SetRecord(IDX_USER,		userid);

			hisRec.SetRecord(IDX_STATE, "����");
			httpsession.WriteHisData("imsg", "imsg_popo", &hisRec, httpsession.m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);//, HISTORY_DATA);
		}

		// ����ļ�
		CString txt;
		txt.Format("user= %s\r\nstat= ����\r\n", userid);
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
		// д�����ļ�
		httpsession.m_OpenIndxFlg = v_openflaghf;
		httpsession.m_OpenCaseFlg = v_opencasehf;
		httpsession.m_OpenWarnFlg = v_openwarnhf;
		// ƥ�䰸����Ԥ����������Ϣ
		DWORD state = 0;
		SearchCaseOrWarn(&httpsession, username, username.GetLength(), state,"voip_heyoo");

		// ����IDC����Դ
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
				ttstr.Format("%s\1%s\1heyoo\1��ʱ��Ϣ\1%s\1��������绰\1%s\1\1", 
					httpsession.m_dip.Left(15), httpsession.m_strISP, httpsession.m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[HTTP_PROTOCOL].AddServer(ttstr);
			}
		}
		else if ((httpsession.m_dataSrc == DATASRC_OTHER)
			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_csip), httpsession.m_strISP)))
		{
			httpsession.m_dataSrc = DATASRC_IDC;
		}

		// ������ʷ
		if (v_statImsgOther)
		{
			HistoryRecord hisRec;
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		username);
			hisRec.SetRecord(IDX_USER,		username);

			hisRec.SetRecord(IDX_STATE, "����");
			httpsession.WriteHisData("voip_heyoo", "voip_heyoo", &hisRec, httpsession.m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);//, HISTORY_DATA);
		}

		// ����ļ�
		CString txt;
		txt.Format("user= %s\r\nstat= ����\r\n", username);
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
		// д�����ļ�
		httpsession.m_OpenIndxFlg = v_openflaghf;
		httpsession.m_OpenCaseFlg = v_opencasehf;
		httpsession.m_OpenWarnFlg = v_openwarnhf;
		// ƥ�䰸����Ԥ����������Ϣ
		DWORD state = 0;
		SearchCaseOrWarn(&httpsession, username, username.GetLength(), state,"voip_66call");

		// ����IDC����Դ
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
				ttstr.Format("%s\1%s\166call\1��ʱ��Ϣ\1%s\166call����绰\1%s\1\1", 
					httpsession.m_dip.Left(15), httpsession.m_strISP, httpsession.m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[HTTP_PROTOCOL].AddServer(ttstr);
			}
		}
		else if ((httpsession.m_dataSrc == DATASRC_OTHER)
			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_csip), httpsession.m_strISP)))
		{
			httpsession.m_dataSrc = DATASRC_IDC;
		}

		// ������ʷ
		if (v_statImsgOther)
		{
			HistoryRecord hisRec;
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		username);
			hisRec.SetRecord(IDX_USER,		username);

			hisRec.SetRecord(IDX_STATE, "����");
			httpsession.WriteHisData("voip_66call", "voip_66call", &hisRec, httpsession.m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);//, HISTORY_DATA);
		}

		// ����ļ�
		CString txt;
		txt.Format("user= %s\r\nstat= ����\r\n", username);
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
		// д�����ļ�
		httpsession.m_OpenIndxFlg = v_openflaghf;
		httpsession.m_OpenCaseFlg = v_opencasehf;
		httpsession.m_OpenWarnFlg = v_openwarnhf;
		// ƥ�䰸����Ԥ����������Ϣ
		DWORD state = 0;
		SearchCaseOrWarn(&httpsession, username, username.GetLength(), state,"voip_teltel");

		// ����IDC����Դ
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
				ttstr.Format("%s\1%s\1teltel\1��ʱ��Ϣ\1%s\1teltel����绰\1%s\1\1", 
					httpsession.m_dip.Left(15), httpsession.m_strISP, httpsession.m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[HTTP_PROTOCOL].AddServer(ttstr);
			}
		}
		else if ((httpsession.m_dataSrc == DATASRC_OTHER)
			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)httpsession.m_csip), httpsession.m_strISP)))
		{
			httpsession.m_dataSrc = DATASRC_IDC;
		}

		// ������ʷ
		if (v_statImsgOther)
		{
			HistoryRecord hisRec;
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		username);
			hisRec.SetRecord(IDX_USER,		username);

			hisRec.SetRecord(IDX_STATE, "����");
			httpsession.WriteHisData("voip_teltel", "voip_teltel", &hisRec, httpsession.m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);//, HISTORY_DATA);
		}

		// ����ļ�
		CString txt;
		txt.Format("user= %s\r\nstat= ����\r\n", username);
		httpsession.m_indexfname = CreateImsgFilename(httpsession.m_sip, httpsession.m_dip, "imsg_teltel.indexfile.txt",&httpsession);
		httpsession.WriteIndexFile("voip_teltel", "voip_teltel", txt, httpsession.m_dceprocotol->m_httpMissObj);
		httpsession.m_CaseArr.RemoveAll();
		httpsession.m_WarnArr.RemoveAll();
		return TRUE;
	}
	return FALSE;
}