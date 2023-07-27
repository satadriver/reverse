// HttpSession.cpp: implementation of the HttpSession class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "http.h"
// #include "TeSpread.h"
#include "HttpSession.h"
#include "HttpProxy.h"
#include "PublicFun.h"
#include "FreeGateDec.h"
#include "DealHttpGet.h"
#include "VoipMessage.h"
#include "PCUniqueCode.h"
#include "PhoneMap.h"
#include "PhoneNumCheat.h"
#include "PhoneMediaApp.h"
#include "PhoneAD.h"
#include "QQhttpSession.h"
#include "HttpProObserver.h"
//购物
#include "Shopping/Shopping.h"
#include "Contacts/Contacts.h"
//论坛
#include "Forum/Forum.h"
//助手
#include "Installed/Installed.h"
//邮箱
#include "EMail/EMail.h"
//旅行
#include "Traffic/Traffic.h"
//出行
#include "Taxi/Taxi.h"
#include "Running/Running.h"
//手机设备信息
#include "PhoneDevice/PhoneDevice.h"
//PC设备信心
#include "ComputerDevice/ComputerDevice.h"
//实名认证信息
#include "RealName/RealName.h"
#include "Chating/Chating.h"
//定位
#include "Location/Location.h"
//博客
#include "Blog/Blog.h"
//手机阅读
#include "Reading/Reading.h"
//云盘
#include "Cloud/YunPan.h"
//快递
#include "KuaiDi/KuaiDi.h"
//航空
#include "Airplane/Plane.h"
//搜索
#include "Search/Search.h"
#include "qqpim/QQSyncAsist.h"
//硬件特征码
#include "HardwareInfo/HardwareInfo.h"
//求职招聘
#include "Employ/Employ.h"
//站长工具
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

HttpSession::HttpSession(DATEHEADER& packh, int aT): SocketSession(packh, aT)
{
	m_ProcessTime = packh.m_tm;
	m_CSeqOffset = 0;
	m_CAckOffset = 0;
	m_PacketType = 1;
	m_FLAG = NOREQ;
	m_reqsortflag = 0;
	m_ressortflag = 0;
	m_spyaddr = g_DDEPOS;
	m_databufsize = 0;
	m_datalen = 0;
	m_databuffer = NULL;
	m_chunksize = 0;
	if(packh.m_sequence>MaxSeq)
	{
		m_CSeqOffset = SeqOffset;
	}
	if(packh.m_ack>MaxSeq)
	{
		m_CAckOffset = SeqOffset;
	}
	m_closetm = packh.m_tm + 60;

	// 处理IDC数据源
	if(m_dataSrc == DATASRC_OTHER)
	{
		// 匹配IDC数据源
		if ((v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP))
			|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
		{
			m_dataSrc = DATASRC_IDC;
		}
	}
}


HttpSession::~HttpSession()
{
	delete[] m_databuffer;
}

BOOL HttpSession::CloseSession(time_t nowtm)
{
	int err = 0;
	try
	{
 		if (m_PacketType == 10)
		{
err = 1;
			if (nowtm==0)
			{
err=2;
				// 清理当前下行的数据并找到新的请求
#ifdef	ADD_RUNSTATE_INFO
				v_httprunstate.AddState(200);
#endif
				NextSeqSessionForImsgProxy(m_lastpacktm-20);
				return FALSE;
			}
			else
			{
				// 清理所有的下行数据和请求
err=3;
#ifdef	ADD_RUNSTATE_INFO
				v_httprunstate.AddState(201);
#endif
				NextSeqSessionForImsgProxy(nowtm);
				return TRUE;
			}
		}

		if (m_reqsortflag != 0)
		{
err=4;
			m_ReqPacketlst.sort();
			m_reqsortflag = 0;
		}
		if (nowtm == 0)
		{
			// 会话还原完成或失败时调用
			// 搜索下一个HTTP请求
err=5;
			m_FLAG = NOREQ;
			FindNextRequest();
			nowtm = m_lastpacktm;
		}
		else
		{
			m_closetm += 60;		// 下一分钟继续调用该函数
		}
		
		do
		{
			int re = 0;
			if (m_FLAG == REQPROCING)
			{
				// 当前有没有完成的请求, 处理该请求
				if (!m_Requestline.m_HaveHeader)
				{
err=6;
					re = FindHttpHeaderFromList(&m_Requestline, m_ReqPacketlst, m_reqsortflag);
				}
				if (m_Requestline.m_Action == HTTP_POST)
				{
					if ((re == 0) && (m_Requestline.m_HaveHeader))
					{
						// 还原数据
err=7;
						re = ListToFile(m_ReqPacketlst, &m_Requestline, m_dceprocotol->m_HttpFileBuffer, m_reqsortflag);
					}
				}
				else
				{
					ASSERT(m_Requestline.m_Action == HTTP_GET);
					if ((re == 0) && (m_Requestline.m_HaveHeader)
						&& (!m_Responseline.m_HaveHeader))
					{
err=10;
						re = FindHttpHeaderFromList(&m_Responseline, m_ResponsePacketlst, m_ressortflag);
					}
					if ((re == 0) && (m_Requestline.m_HaveHeader) && (m_Responseline.m_HaveHeader))
					{
						// 还原数据
err=11;
						re = ListToFile(m_ResponsePacketlst, &m_Responseline, m_dceprocotol->m_HttpFileBuffer, m_ressortflag);
					}

					if ((re == 0) && (m_Requestline.m_HaveHeader))
					{

						DealMultiPackGet();
					}
				}
				if (re == 8)
				{
err=12;
					DealHttpTmpData();
				}
			}

			if (m_FLAG != REQPROCING)
			{
				// 如果当前没有正在处理的请求或该请求已经超时就搜索下一个HTTP请求
err=13;
				m_FLAG = NOREQ;
				FindNextRequest();
			}
			else if(nowtm > (m_ProcessTime + 90))
			{
				//  会话超时
				if (m_Requestline.m_Action == HTTP_POST)
				{
					if ((m_Requestline.m_HaveHeader)
						&& (m_ReqPacketlst.size() == 0)
						&& ((m_datalen>0) || !m_realfname.IsEmpty()))
					{
						// 最后一个包能还原多少算多少
						DealHttpTmpData();
					}
				}
				else
				{
					ASSERT(m_Requestline.m_Action == HTTP_GET);
					if ((m_Responseline.m_HaveHeader)
						&& (m_Requestline.m_HaveHeader)
						&& (m_ResponsePacketlst.size() == 0)
						&& ((m_datalen>0) || !m_realfname.IsEmpty()))
					{
						// 最后一个包能还原多少算多少
						DealHttpTmpData();
					}
				}
				m_FLAG = REQOVER;
			}
			else
			{
err=14;
				return FALSE;
			}
		}while(m_FLAG != NOREQ);
		// 没有找到HTTP请求
#ifdef		_DEBUG
		if(m_ReqPacketlst.size() > 0)
			printf("list size = %d\n", m_ReqPacketlst.size());
#endif
err=15;
		return TRUE;
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "CloseSession() Error=%d", err);
		return TRUE;
	}
}

int HttpSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{

#ifdef ADD_RUNSTATE_INFO
	AutoDestory AutoDebug(&v_httprunstate, 1, -1);
#endif
int error = 0;

	int re=0;

	try
	{
// #ifdef		DUMPHTTPPACKET
error =1;
		if (g_DumpPacketFlg)
		{
			v_HttpDumpHttpPacket.DumpOnePacket(packh, pPacket);
		}
	//	WriteLog(HTTPPRO, "ack : %u - seq : %u - nseq : %u\n", packh.m_ack, packh.m_sequence, m_Requestline.m_nseq);
// #endif
		m_lastpacktm = packh.m_tm;
error =2;
		if(memcmp(packh.m_sip, m_csip, 4) == 0)
		{
		//	packh.m_ack -= m_CAckOffset;
		//	packh.m_sequence -= m_CSeqOffset;
error=3;
			if (m_PacketType == 10)
			{
				// IMSG Proxy
				TcpPacket	Tcpp(packh, pPacket, 0, 0) ;
				m_ReqPacketlst.push_back(Tcpp);
				Tcpp.buf=0;
				if ((++m_reqsortflag&15)==15)
				{
#ifdef	ADD_RUNSTATE_INFO
					v_httprunstate.AddState(11);
#endif
					NextSeqSessionForImsgProxy(packh.m_tm - 20);
				}
				m_dceprocotol->m_HttpSessCount++;
				return 0;
			}
error=4;
		
			if(m_Requestline.m_fileseq==0)
			{
				char tmppack[16];
				memcpy(tmppack, pPacket, 12);
				memset(tmppack+12, 0, 4);
				strupr(tmppack);
				//2016-08-11更正
				if(strncmp(tmppack,"GET ",4) == 0 || strncmp(tmppack,"POST",4) == 0 || strncmp(tmppack,"PUT ",4) == 0)
				{
					if(strncmp(tmppack,"GET ",4) == 0 )
					{
						m_Requestline.m_Action=HTTP_GET;
					}
					else
						m_Requestline.m_Action=HTTP_POST;

 					m_Requestline.m_Ssequence=packh.m_sequence;
	  				m_Requestline.m_Sack=packh.m_ack;
					m_Requestline.m_nseq=packh.m_sequence /*+ m_Requestline.*/;
					m_Requestline.m_fileseq=packh.m_sequence;
					m_ProcessTime = packh.m_tm;
					m_FLAG = REQPROCING;
				}
				else
				{
					// 不是HTTP头
					return 0;
				}
			}
error =5;
			m_dceprocotol->m_HttpSessCount++;
			// 判断新的包的seq
// 			
			if(packh.m_sequence + packh.m_datalen < m_Requestline.m_nseq)
			{
				//!!!!!!
			//	DealDevUC(this,pPacket);
			//	DealPhoneMap(this,pPacket);
			//	DealPhoneMedia(this,pPacket);
			//	DealPhoneReadApp(this,pPacket);
			//	DealPhoneToolsApp(this,pPacket);
			//	DealPhoneAd(this,pPacket);
			
			//	DealPCMediaSoft(this,pPacket);
			//	DealPhoneNumCheat(this,pPacket);
				// 这个包不用了
				return 0;
			}
			else if (packh.m_sequence <= m_Requestline.m_nseq)
			{
				// 这个包就是在等待的包
				if(!m_Requestline.m_HaveHeader || m_Requestline.m_Action==HTTP_POST)
				{
error=6;
					if (m_Requestline.m_HaveHeader)
					{
						if ((packh.m_datalen > 5)
							&& ((memicmp(pPacket, "GET ", 4)==0) || (memicmp(pPacket, "POST ", 5)==0))
							)
						{
							// 后面的POST 或 GET 头
							re = 8;
//							ASSERT(FALSE);
							TcpPacket	Tcpp(packh, pPacket, 0, 0) ;
							m_ReqPacketlst.push_front(Tcpp);
							Tcpp.buf=0;
							m_reqsortflag++;
							return re;
						}
					}
error=7;
					// 插入包的数据缓冲区
					m_ProcessTime = packh.m_tm;
					re = InsertPacketToBuffer(pPacket, packh.m_sequence, packh.m_datalen, &m_Requestline);
error=8;
					if (re == 0) //(re >= 0) && (re != 8))
					{
						if (!m_Requestline.m_HaveHeader)
						{
							re = FindHttpHeaderFromList(&m_Requestline, m_ReqPacketlst, m_reqsortflag);
error=9;
						}
						
						if ((re == 0) && (m_Requestline.m_HaveHeader))
						{
							// 还原数据
							// ASSERT(m_Requestline.m_Action == HTTP_POST);
							re = ListToFile(m_ReqPacketlst, &m_Requestline, m_dceprocotol->m_HttpFileBuffer, m_reqsortflag);

							error=10;
						}
						
						if (m_Requestline.m_Action == HTTP_GET)
						{
							// add by cxl 切换到下一个请求包
							re = 7;
							// end
							return re;
						}
						
					}
					if (re == 8)
					{
						if (DealHttpTmpData())
						{
							return 8;
						}
						else
						{
							return -3;
						}
error=11;
					}
					else
					{
						if ((packh.m_datalen > 5)
						&& (memicmp(pPacket, "GET ", 4)==0))
						{			
							if (taxi::Taxi::ProcessTaxi(this,pPacket))
							{
								return -3;
							}
							if (forum::Forum::ProcessForum(this,pPacket))
							{
								return -3;
							}
                            //搜索
							if (search::Search::ProcessSearch(this,pPacket))
							{
								return -3;
							}
							
							if (running::Running::ProcessRunning(this,pPacket))
							{
								return -3;
							}
							if (phonedevice::PhoneDevice::ProcessPhoneDevice(this,pPacket))
							{
								return -3;
							}

							if (computerdevice::ComputerDevice::ProcessComputerDevice(this,pPacket))
							{
								return -3;
							}
							if (realname::RealName::ProcessRealName(this,pPacket))
							{
								return -3;
							}
							if (location::Location::ProcessLocation(this,pPacket))
							{
								return -3;
							}

							if (chating::Chating::ProcessChating(this,pPacket))
							{
								return -3;
							}
							if (yunpan::YunPan::ProcessYunPan(this,pPacket))
							{
								return -3;
							}
							//博客
							if (blog::Blog::ProcessBlog(this, pPacket))
							{
								return -3;
							}
							//快递
							if (kuaidi::KuaiDi::ProcessKuaidi(this, pPacket))
							{
								return -3;
							}
							//Airplane
							if (plane::Airplane::ProcessAirplane(this, pPacket))
							{
								return -3;
							}
							//HardwareInfo
							if (hardwareinfo::HardwareInfo::ProcessHardwareInfo(this, pPacket))
							{
								return -3;
							}
							//站长工具
							if (website::WebSite::ProcessWebSite(this,pPacket))
							{
								return -3;
							}
							//视频活动
							if (videoactivity::VideoActivity::ProcessVideoActivity(this,pPacket))
							{
								return -3;
							}
							//婚恋交友
							if (marriage::Marriage::ProcessMarriage(this,pPacket))
							{
								return -3;
							}
							//文件传输
							if (filetransfer::FileTransfer::ProcessFileTransfer(this,pPacket))
							{
								return -3;
							}
							//酒店预订
							if (hotel::Hotel::ProcessHotel(this,pPacket))
							{
								return -3;
							}
							//供求信息
							if (supply::Supply::ProcessSupply(this,pPacket))
							{
								return -3;
							}
							//第三方接口登入
							if (interfacelogin::InterfaceLogin::ProcessInterfaceLogin(this,pPacket))
							{
								return -3;
							}
							//软件下载
							if (software::Software::ProcessSoftware(this,pPacket))
							{
								return -3;
							}
							return -3;
						}
						return re;
					}
				}
			}
error=12;
			// 插入包到队列
			TcpPacket	Tcpp(packh, pPacket, 0, 0) ;
			m_ReqPacketlst.push_front(Tcpp);
			Tcpp.buf=0;
			m_reqsortflag++;
error =121;
			if ((m_reqsortflag > 30) && (m_Requestline.m_Action == HTTP_POST))
			{

				m_ReqPacketlst.sort();
error =122;
				m_reqsortflag = 0;
				if (!m_Requestline.m_HaveHeader)
				{
error =123;
					re = FindHttpHeaderFromList(&m_Requestline, m_ReqPacketlst, m_reqsortflag);
error=13;
				}
				if (m_Requestline.m_HaveHeader)
				{
error=131;
					// 还原数据
					re = ListToFile(m_ReqPacketlst, &m_Requestline, m_dceprocotol->m_HttpFileBuffer, m_reqsortflag);
error=14;
				}
				if (re == 8)
				{
					DealHttpTmpData();
error=14;
					return 8;
				}
			}
			return 0;
		}
		else
		{
			packh.m_ack -= m_CSeqOffset;
			packh.m_sequence -= m_CAckOffset;
			m_dceprocotol->m_HttpSessCount++;
			if (m_PacketType == 10 && m_Requestline.m_Action==HTTP_POST)
			{
				// 处理ImsgProxy代理的下行数据
//				m_FLAG='M';
				TcpPacket	Tcpp(packh, pPacket, 0, 0) ;
				m_ResponsePacketlst.push_back(Tcpp);
				Tcpp.buf=0;
				if ((++m_ressortflag&15)==15)
				{
#ifdef	ADD_RUNSTATE_INFO
					v_httprunstate.AddState(17);
#endif
error=15;					
					NextSeqSessionForImsgProxy(packh.m_tm - 20);
				}
				return 0;
			}
			//插入响应的lst
			if(packh.m_sequence+packh.m_datalen<=m_Responseline.m_nseq && m_Responseline.m_nseq>0)
				return 0;	
error=16;
			if (m_Responseline.m_fileseq==0 && m_Requestline.m_Action==HTTP_GET)
			{
				char tmppack[16];
				memcpy(tmppack, pPacket, 12);
				memset(tmppack+12, 0, 4);
				strupr(tmppack);
				if (packh.m_ack==m_Requestline.m_nseq || packh.m_sequence==m_Requestline.m_Sack)
				{				
					if(memcmp(tmppack,"HTTP/1.",7) == 0 && memcmp(tmppack+8," 200",4)==0  )
					{
						m_Responseline.m_Ssequence=packh.m_sequence;
						m_Responseline.m_Sack=packh.m_ack;
						m_Responseline.m_nseq=packh.m_sequence;
						m_Responseline.m_fileseq=packh.m_sequence;
// 						if(m_Responseline.m_Ssequence != m_Requestline.m_Sack)
// 							return -3;
					}
					else if(packh.m_ack<m_Requestline.m_nseq)
						return 0;
				}
error=17;				
				if(memcmp(tmppack,"HTTP/1.",7) == 0 && 
					memcmp(tmppack+8," 200",4)!=0  )
				{
					if(	m_Requestline.m_Action==HTTP_GET &&
						(packh.m_ack==m_Requestline.m_nseq ||
						packh.m_sequence==m_Requestline.m_Sack) )
					{
						if(memcmp(tmppack+8," 200",4)>0  )
							return -3;
						else
							return 0;
					}
					else
						return 0;
				}

			}
error=18;
			if (packh.m_sequence <= m_Responseline.m_nseq)
			{
				// 这个包就是在等待的包
				if(!m_Responseline.m_HaveHeader || m_Requestline.m_Action==HTTP_GET)
				{
					if (m_Responseline.m_HaveHeader)
					{
						if ((packh.m_datalen > 10)
							&& (memcmp(pPacket, "HTTP/1.", 7)==0)
							&& (pPacket[8] == 0x20))
						{
							// 后面的HTTP 回应头
							re = 8;
							TcpPacket	Tcpp(packh, pPacket, 0, 0) ;
							m_ResponsePacketlst.push_front(Tcpp);
							Tcpp.buf=0;
							m_ressortflag++;
							return re;
						}
					}
					// 插入包的数据缓冲区
					m_ProcessTime = packh.m_tm;
error=19;
					re = InsertPacketToBuffer(pPacket, packh.m_sequence, packh.m_datalen, &m_Responseline);
					if (re == 0) //(re >= 0) && (re != 8))
					{
						if (!m_Responseline.m_HaveHeader)
						{
error=20;
							re = FindHttpHeaderFromList(&m_Responseline, m_ResponsePacketlst, m_ressortflag);
						}
						if ((re == 0) && (m_Responseline.m_HaveHeader))
						{
							//  还原数据
							ASSERT(m_Requestline.m_Action == HTTP_GET);
error =21;
							re = ListToFile(m_ResponsePacketlst, &m_Responseline, m_dceprocotol->m_HttpFileBuffer, m_ressortflag);
						}
					}
					if (re == 8)
					{
error=22;
						if (DealHttpTmpData())
						{
							return 8;
						}
						else
						{
							return -3;
						}
					}
					else
					{
						return re;
					}
				}
			}
error=23;
			// 插入包到队列
			TcpPacket	Tcpp(packh, pPacket, 0, 0) ;
error =231;
			m_ResponsePacketlst.push_front(Tcpp);
error =232;
			Tcpp.buf=0;
			m_ressortflag++;
error =233;
			if((m_ressortflag > 30) && (m_Requestline.m_HaveHeader))
			{
error=24;
				m_ResponsePacketlst.sort();
				m_ressortflag = 0;
				if (!m_Responseline.m_HaveHeader)
				{
					re = FindHttpHeaderFromList(&m_Responseline, m_ResponsePacketlst, m_ressortflag);
error=25;
				}
				if (m_Responseline.m_HaveHeader && (m_Requestline.m_Action == HTTP_GET))
				{
					// 还原数据
					re = ListToFile(m_ResponsePacketlst, &m_Responseline, m_dceprocotol->m_HttpFileBuffer, m_ressortflag);
error=26;
				}
				if (re == 8)
				{
error=27;

					DealHttpTmpData();
				}
			}
			return re;
		}//endif
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "Http InertPacket Error!! %d",error);
		this->m_ResponsePacketlst.clear();
		this->m_ReqPacketlst.clear();
		re=-3;
	}
	return re;
}

int HttpSession::ParserUrl()
{
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(52);
#endif

	int error = 0;
	ASSERT(m_Requestline.m_HaveHeader!=0);
	try
	{
		BOOL tlcaseflag, tlwarnflag, tlindxflag;		// 临时存储特浏的还原开关
		BOOL IsSkype = FALSE;
		BOOL IsEspecial = FALSE;//是否是特浏
error=1;
		if (DealEspecialBrowse(this->m_Requestline,this))
		{
			if (m_Requestline.m_Action == HTTP_POST)
			{
				if (m_Requestline.m_Browser==TE_FREEGATE)
				{
					// 自由之门
					m_OpenCaseFlg = v_opencasefgurl;
					m_OpenIndxFlg = v_openflagfgurl;
					m_OpenWarnFlg = v_openwarnfgurl;
					tlcaseflag = v_opencasefgup;
					tlwarnflag = v_openwarnfgup;
					tlindxflag = v_openflagfgup;
				}
				else if (m_Requestline.m_Browser==TE_DYNAPASS)
				{
					// 动网通
					m_OpenCaseFlg = v_opencasedynaurl;
					m_OpenIndxFlg = v_openflagdynaurl;
					m_OpenWarnFlg = v_openwarndynaurl;
					tlcaseflag = v_opencasedynaup;
					tlwarnflag = v_openwarndynaup;
					tlindxflag = v_openflagdynaup;
				}
			}
			else
			{
				if (m_Requestline.m_Browser==TE_FREEGATE)
				{
					// 自由之门
					m_OpenCaseFlg = v_opencasefgurl;
					m_OpenIndxFlg = v_openflagfgurl;
					m_OpenWarnFlg = v_openwarnfgurl;
					tlcaseflag = v_opencasefgdn;
					tlwarnflag = v_openwarnfgdn;
					tlindxflag = v_openflagfgdn;
				}
				else if (m_Requestline.m_Browser==TE_DYNAPASS)
				{
					// 动网通
					m_OpenCaseFlg = v_opencasedynaurl;
					m_OpenIndxFlg = v_openflagdynaurl;
					m_OpenWarnFlg = v_openwarndynaurl;
					tlcaseflag = v_opencasedynadn;
					tlwarnflag = v_openflagdynadn;
					tlindxflag = v_openwarndynadn;
				}
			}
			IsEspecial = TRUE;
		}
		else if(DealSkype(m_Requestline, m_id))
		{
			IsSkype = TRUE;
		}
		else
		{
			if ((m_Requestline.m_Action==HTTP_GET && !v_opencasehttpdown)
				|| (m_Requestline.m_Action==HTTP_POST && !v_opencasehttpup))
			{
				return 1;
			}
			if (m_Requestline.m_Action == HTTP_POST)
			{
				m_OpenCaseFlg = v_opencasehttpup;
				m_OpenIndxFlg = v_openflaghttpup;
				m_OpenWarnFlg = v_openwarnhttpup;
			}
			else
			{
				m_OpenCaseFlg = v_opencasehttpdown;
				m_OpenIndxFlg = v_openflaghttpdown;
				m_OpenWarnFlg = v_openwarnhttpdown;
			}
		}
error=2;
		CString strurl;
		if (m_Requestline.m_Browser==TE_NOTHING)
		{
			strurl = m_Requestline.m_URL + ' ' + m_Requestline.m_Referer + ' ' + m_Requestline.m_Cookie +
			' ' + m_Requestline.m_Cookie2 + ' ' + m_Requestline.m_SetCookie2;
		}
		else
		{
			strurl = m_Requestline.m_URL + ' ' + m_Requestline.m_Referer;
		}
		strurl.MakeLower();	// 要匹配线索的URL必须转换成小写字符

		// 匹配案件和预警
		DWORD state = 0x10000;		// 需要匹配ip+URL
		SearchCaseOrWarn(this, (char*)(LPCTSTR)strurl, strurl.GetLength(), state);
error=3;
		if (/*(m_CaseArr.GetSize() == 0) && */(!m_rubbishflag))
		{
			// 匹配URL的屏蔽信息
			m_rubbishflag = SearchBurrishByURL(strurl);
		}
error=4;
		if (IsEspecial)
		{
			// 写特浏的URL文件
			WriteTeUrlIndexFile();

			// 置换还原开关
			m_OpenCaseFlg = tlcaseflag;
			m_OpenIndxFlg = tlindxflag;
			m_OpenWarnFlg = tlwarnflag;
		}
		else if(IsSkype)
		{
			WriteSkypeIndex();
		}
		else if(!m_Requestline.m_SearchType.IsEmpty() && 
			!m_Requestline.m_SearchKey.IsEmpty())
		{
			WriteSearchFile(FALSE);
			m_Requestline.m_SearchKeyType.Empty();
			m_Requestline.m_SearchKey.Empty();
			m_Requestline.m_SearchKey.Empty();
		}
		else 
		{
			// 是案件
			CString strSuffix;
			int filetype = CheckSuffix(m_Requestline.m_URL, strSuffix);
			// if (filetype != 1)	// 不是图片和脚本
			{
				if (m_CaseArr.GetSize() > 0 || v_httpurl)
				{
					// 写HttpUrl文件
					WriteHttpUrlFile((m_CaseArr.GetSize() > 0));
				}

			}
		}
error=5;
		// 清除案件和预警信息
		m_CaseArr.RemoveAll();
		m_WarnArr.RemoveAll();
		return 1;
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "ParserUrl() Error!!error=%d",error);
		return -3;
	}
}

BOOL HttpSession::NextSeqSessionForImsgProxy(time_t ptm)
{
	// 只处理POST
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(4);
#endif
	
	BOOL re=0;
	TcpPacket* tp;
	int buflen = 0;
	DWORD BufSeq=0;
	DWORD BufNextSeq = 0;
	time_t stm;
	BOOL ins=FALSE;
	if (m_ReqPacketlst.size() > 0)
	{
		if (m_reqsortflag > 0)
		{
			m_ReqPacketlst.sort();
			m_reqsortflag = 0;
		}
		TcpPacketListIt reqend, reqitem;
		reqitem = m_ReqPacketlst.begin();
		if (reqitem->tm <= ptm)
		{
			while(m_ReqPacketlst.size()>0)
			{
				reqitem = m_ReqPacketlst.begin();
				tp = &(*reqitem);
				if (tp->Slen > 12 && memcmp(tp->buf, "POST ", 5)==0)
				{
					if (tp->tm <= ptm)
					{
						if (buflen>0)
						{
#ifdef	ADD_RUNSTATE_INFO
							v_httprunstate.AddState(41);
#endif
							memset(m_dceprocotol->m_HttpFileBuffer+buflen, 0, 8);
							BOOL unixhead=FALSE;
							int i=Search4ByteFind(m_dceprocotol->m_HttpFileBuffer, buflen);//0d 0a 0d 0a
							if (i<0)
							{
								i=Search2ByteFind(m_dceprocotol->m_HttpFileBuffer, buflen);// 0a 0a
								unixhead=TRUE;
							}
							HttpCommInfo Response;
							Response.m_Content_length=buflen-i;
							if(FormatHttpReqHeader(Response,m_dceprocotol->m_HttpFileBuffer,i,unixhead,m_cdip, atoi(m_dip.Right(5))))
							{
								// imsg proxy
								Response.m_IDCInfo.Empty();
//								Response.m_dataSrc = s->m_dataSrc;
								if ((Response.m_Content_length+i <= buflen) && (Response.m_Content_length>8))
								{
									ASSERT(i <= buflen-Response.m_Content_length);
									IMSGPROXY imsgproxy;
									imsgproxy.m_dataSrc = m_dataSrc;
									imsgproxy.packettm = stm;
									imsgproxy.Direction=1;
									imsgproxy.sip = m_sip;
									imsgproxy.dip = m_dip;
									memcpy(imsgproxy.csip, m_csip, 4);
									memcpy(imsgproxy.cdip, m_cdip, 4);
									imsgproxy.smac = m_smac;
									imsgproxy.dmac = m_dmac;
									imsgproxy.datalen=Response.m_Content_length;
									imsgproxy.pPacketData=m_dceprocotol->m_HttpFileBuffer+i;
									imsgproxy.proxytype = m_ProxyType;
#ifdef	ADD_RUNSTATE_INFO
									v_httprunstate.AddState(42);
#endif
									InsertImsgProxy(imsgproxy, Response, m_spyaddr, this);
								}
							}
							buflen = 0;
						}
 						stm = tp->tm;
						BufSeq = tp->Ssequence;
						memcpy(m_dceprocotol->m_HttpFileBuffer, tp->buf, tp->Slen);
						buflen = tp->Slen;
						BufNextSeq = tp->Nextsequence;
						m_ReqPacketlst.erase(reqitem);
					}
					else
					{
						break;
					}
				}
				else
				{
					if (buflen>0)
					{
						if (BufNextSeq == tp->Ssequence)
						{
							memcpy(m_dceprocotol->m_HttpFileBuffer+buflen, tp->buf, tp->Slen);
							buflen += tp->Slen;
							BufNextSeq = tp->Nextsequence;
						}
//						else if (BufNextSeq < tp->Ssequence)
//						{
//							// 不连续
//							break;
//						}
					}
					m_ReqPacketlst.erase(reqitem);
				}
			}
		}
		if (buflen>0)
		{
			memset(m_dceprocotol->m_HttpFileBuffer+buflen, 0, 8);
			BOOL unixhead=FALSE;
			int i=Search4ByteFind(m_dceprocotol->m_HttpFileBuffer, buflen);//0d 0a 0d 0a
			if (i<0)
			{
				i=Search2ByteFind(m_dceprocotol->m_HttpFileBuffer, buflen);// 0a 0a
				unixhead=TRUE;
			}
			if (i>0)
			{
				HttpCommInfo Response;
				Response.m_Content_length=buflen-i;
				if(FormatHttpReqHeader(Response,m_dceprocotol->m_HttpFileBuffer,i,unixhead, m_cdip, atoi(m_dip.Right(5))))
				{
					// imsg proxy
					Response.m_IDCInfo.Empty();
//					Response.m_dataSrc = s->m_dataSrc;
					if ((Response.m_Content_length<=buflen-i) && (Response.m_Content_length>8))
					{
						ASSERT(i <= buflen-Response.m_Content_length);
						IMSGPROXY imsgproxy;
						imsgproxy.m_dataSrc = m_dataSrc;
						imsgproxy.packettm=stm;
						imsgproxy.Direction=1;
						imsgproxy.sip = m_sip;
						imsgproxy.dip = m_dip;
						memcpy(imsgproxy.csip, m_csip, 4);
						memcpy(imsgproxy.cdip, m_cdip, 4);
						imsgproxy.smac = m_smac;
						imsgproxy.dmac = m_dmac;
						imsgproxy.datalen=Response.m_Content_length;
						imsgproxy.pPacketData=m_dceprocotol->m_HttpFileBuffer+i;
						imsgproxy.proxytype = m_ProxyType;
#ifdef	ADD_RUNSTATE_INFO
						v_httprunstate.AddState(43);
#endif
						InsertImsgProxy(imsgproxy, Response, m_spyaddr, this);
					}
				}
			}
//			TcpPacket packet;
//			packet.tm = stm;
//			packet.Ssequence = BufSeq;
//			packet.buf = new char[buflen];
//			packet.Slen = buflen;
//			packet.Nextsequence = BufNextSeq;
//			s->ReqPacketlst.push_front(packet);
//			s->reqPacketCount++;
//			packet.buf = NULL;
		}
	}

#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(44);
#endif
	
	//  只处理HTTP 200 ok
	if (m_ResponsePacketlst.size() > 0)
	{
		if (m_ressortflag > 0)
		{
			m_ResponsePacketlst.sort();
			m_ressortflag = 0;
		}
		// 遍历HTTP下行的数据队列
		TcpPacketListIt resend, resitem;
		resitem = m_ResponsePacketlst.begin();
		if (resitem->tm <= ptm)
		{
			tp=NULL;
			buflen = 0;
			BufSeq=0;
			BufNextSeq = 0;
			while(m_ResponsePacketlst.size()>0)
			{
				resitem = m_ResponsePacketlst.begin();
				tp = &(*resitem);
				if (tp->Slen > 12 && memcmp(tp->buf, "HTTP/1.", 7)==0 && memcmp(tp->buf+8, " 200", 4)==0)
				{
					if (tp->tm <= ptm)
					{
						if (buflen>0)
						{
#ifdef	ADD_RUNSTATE_INFO
							v_httprunstate.AddState(45);
#endif
							memset(m_dceprocotol->m_HttpFileBuffer+buflen, 0, 8);
							BOOL unixhead=FALSE;
							int i=Search4ByteFind(m_dceprocotol->m_HttpFileBuffer, buflen);//0d 0a 0d 0a
							if (i<0)
							{
								i=Search2ByteFind(m_dceprocotol->m_HttpFileBuffer, buflen);// 0a 0a
								unixhead=TRUE;
							}
							HttpCommInfo Response;
							Response.m_Content_length=buflen-i;
							if(FormatHttpReqHeader(Response,m_dceprocotol->m_HttpFileBuffer,i,unixhead,m_cdip, atoi(m_dip.Right(5))))
							{
								// imsg proxy http down
								Response.m_IDCInfo.Empty();
//								Response.m_dataSrc = s->m_dataSrc;
								if ((Response.m_Content_length+i <= buflen) && (Response.m_Content_length>8))
								{
									ASSERT(i <= buflen-Response.m_Content_length);
									IMSGPROXY imsgproxy;
									imsgproxy.m_dataSrc = m_dataSrc;
									imsgproxy.packettm=stm;
									imsgproxy.Direction=2;
									imsgproxy.sip = m_sip;
									imsgproxy.dip = m_dip;
									memcpy(imsgproxy.csip, m_csip, 4);
									memcpy(imsgproxy.cdip, m_cdip, 4);
									imsgproxy.smac = m_smac;
									imsgproxy.dmac = m_dmac;
									imsgproxy.datalen=Response.m_Content_length;
									imsgproxy.pPacketData=m_dceprocotol->m_HttpFileBuffer+i;
									imsgproxy.proxytype = m_ProxyType;
#ifdef	ADD_RUNSTATE_INFO
									v_httprunstate.AddState(46);
#endif
									InsertImsgProxy(imsgproxy, Response, m_spyaddr, this);
								}
							}
							buflen = 0;
						}
						stm = tp->tm;
						BufSeq = tp->Ssequence;
						memcpy(m_dceprocotol->m_HttpFileBuffer, tp->buf, tp->Slen);
						buflen = tp->Slen;
						BufNextSeq = tp->Nextsequence;
						m_ResponsePacketlst.erase(resitem++);
					}
					else
						break;
				}
				else
				{
					if (buflen>0)
					{
						if (BufNextSeq == tp->Ssequence)
						{
							memcpy(m_dceprocotol->m_HttpFileBuffer+buflen, tp->buf, tp->Slen);
							buflen += tp->Slen;
							BufNextSeq = tp->Nextsequence;
						}
//						else if (BufNextSeq < tp->Ssequence)
//						{
//							break;
//						}
					}
					m_ResponsePacketlst.erase(resitem++);
				}
			}

			if (buflen>0)
			{
				memset(m_dceprocotol->m_HttpFileBuffer+buflen, 0, 8);
				BOOL unixhead=FALSE;
				int i=Search4ByteFind(m_dceprocotol->m_HttpFileBuffer, buflen);//0d 0a 0d 0a
				if (i<0)
				{
					i=Search2ByteFind(m_dceprocotol->m_HttpFileBuffer, buflen);// 0a 0a
					unixhead=TRUE;
				}
				if (i>0)
				{
					HttpCommInfo Response;
					Response.m_Content_length=buflen-i;
					if(FormatHttpReqHeader(Response,m_dceprocotol->m_HttpFileBuffer,i,unixhead,m_cdip, atoi(m_dip.Right(5))))
					{
						// imsg proxy http down
//						Response.m_dataSrc = s->m_dataSrc;
						if (Response.m_Content_length<=buflen-i)
						{
							if (Response.m_Content_length>8)
							{
								ASSERT(i <= buflen-Response.m_Content_length);
								IMSGPROXY imsgproxy;
								imsgproxy.m_dataSrc = m_dataSrc;
								imsgproxy.packettm=stm;
								imsgproxy.Direction=2;
								imsgproxy.sip = m_sip;
								imsgproxy.dip = m_dip;
								memcpy(imsgproxy.csip, m_csip, 4);
								memcpy(imsgproxy.cdip, m_cdip, 4);
								imsgproxy.smac = m_smac;
								imsgproxy.dmac = m_dmac;
								imsgproxy.datalen=Response.m_Content_length;
								imsgproxy.pPacketData=m_dceprocotol->m_HttpFileBuffer+i;
								imsgproxy.proxytype = m_ProxyType;
#ifdef	ADD_RUNSTATE_INFO
								v_httprunstate.AddState(47);
#endif
								InsertImsgProxy(imsgproxy, Response, m_spyaddr, this);
							}
							return TRUE;
						}
					}
				}
				TcpPacket packet;
				packet.tm = stm;
				packet.Ssequence = BufSeq;
				packet.buf = new char[buflen];
				if (packet.buf == NULL)
				{
					WriteLog(HTTPPRO, "new error new err1");

				}

				packet.Slen = buflen;
				packet.Nextsequence = BufNextSeq;
				m_ResponsePacketlst.push_front(packet);
				packet.buf = NULL;
			}
		}
	}

	return re;
}

//*************************************************
//	函数名称:		FindHttpHeader
//	描述:			
//	输入参数:		
//	输出参数:		
//	返回值:			
//	其它:			
//*************************************************
int HttpSession::FindHttpHeader(char* pData, int datalen, HttpCommInfo* pheader)
{
	ASSERT(!pheader->m_HaveHeader);

	int headlen = -1;		// HTTP头的长度
	BOOL unixhead = FALSE;
	headlen = Search4ByteFind(pData, datalen);//0d 0a 0d 0a
	if (headlen < 0)
	{
		headlen = Search2ByteFind(pData, datalen);// 0a 0a
		unixhead=TRUE;
	}

	if (headlen > 0)
	{
		// HTTP头全了, 解析HTTP头
		if(!FormatHttpReqHeader(*pheader, pData, headlen, unixhead, m_cdip, atoi(m_dip.Right(5))))
		{
			return -3;		// HTTP头解析失败
		}
		pheader->m_HaveHeader = true;
		ASSERT(pheader->m_HaveHeader);
		// HTTP头解析成功
		if ((pheader->m_Action == HTTP_GET)
			&& (pheader == &this->m_Requestline))
		{
			// 处理HTTPURL
			ParserUrl();
		}
		return headlen;
	}

	// HTTP头还不全
	return 0;
}


//*************************************************
//	函数名称:		
//	描述:			
//	输入参数:		
//	输出参数:		
//	返回值:			
//	其它:			
//*************************************************
int HttpSession::InsertPacketToBuffer(const char* pPacket,
				DWORD seq, int packetlen, HttpCommInfo* pheader)
{
int error=0;
	try
	{
	

		BOOL bCanFind = FALSE;
		if (!pheader->m_HaveHeader)
		{
			bCanFind = TRUE;
			// HTTP头还没有解析成功
			if (m_datalen == 0)
			{
				bCanFind = FALSE;
error =1;
				int ret = FindHttpHeader((char*)pPacket, packetlen, pheader);

				if (ret > 0)
				{
					// 找到了 返回头的长度
					pheader->m_fileseq += ret;
					pheader->m_nseq += ret;
					if (pheader->m_Action == HTTP_GET)
					{
						ASSERT(m_datalen == 0);
						// 不处理一个包多个GET的现象
						return ret;	// 
					}
				}
				else if (ret < 0)
				{
					return ret;		// 出错了
				}
			}
		}
error=2;
		// 处理数据


		int copysize = packetlen - (pheader->m_nseq - seq);


		if (copysize <= 0)
		{
			return 0;
		}
error=3;
		if ((copysize + m_datalen) > m_databufsize - 20)
		{
			// 缓冲区不够了
			int newsize = 0;
			if ((m_databufsize >= DATABUFFER8KSIZE)
				&& (!pheader->m_HaveHeader))
			{
				copysize = DATABUFFER8KSIZE - m_datalen;
			}
			else if (m_databufsize < DATABUFFER2KSIZE)
			{
				newsize = DATABUFFER2KSIZE;
			}
			else
			{
error=3;
				newsize = m_databufsize * 2;
//				printf("ack : %u - newsize : %d\n",pheader->m_Sack, newsize);
			//	if (newsize >= (2 * 1024 * 1024)) //GAOFEI
				if (newsize >= (10 * 1024 * 1024)) 
				{
					ASSERT(pheader->m_HaveHeader);
	// 				if ((pheader->m_Content_length > DATABUFFER16KSIZE)
	// 					|| (pheader->m_ifchunked))
					{
						newsize = 0;
error=4;
						int re = WriteDataToTmpFile((char*)pPacket + packetlen - copysize, copysize,
							0, pheader->m_ifchunked);
	//					if (0 > WriteDataToTmpFile(NUL, 0, 0, pheader->m_ifchunked))
						pheader->m_nseq += copysize;
						copysize = 0;
						if (re == 8)
						{
							return 8;
						}
						else if (re < 0)
						{
							return -2;
						}
					}
				}
			}
error=5;
			if (newsize > 0)
			{
				char* tp = new char[newsize];
				if (tp == NULL)
				{
					WriteLog(HTTPPRO, "new error new err2");

					return -6;		// 内存分配失败
				}
error=51;
				memset(tp + newsize - 16, 0, 16);
				int tempsize = m_datalen>newsize?newsize:m_datalen;
				memcpy(tp, m_databuffer, tempsize);
				delete[] m_databuffer;
				m_databuffer = tp;
				m_databufsize = newsize;
			}
		}
error=6;
		if (copysize > 0 && m_databufsize >= m_datalen+copysize )
		{
			memcpy(m_databuffer + m_datalen,
				pPacket + packetlen - copysize, copysize);
			pheader->m_nseq += copysize;
			m_datalen += copysize;
		}
		
		if (bCanFind && !pheader->m_HaveHeader)
		{
error=7;
			// 再找一次头
			int ret = FindHttpHeader(m_databuffer, m_datalen, pheader);
error=9;
			if (ret > 0)
			{
				// 找到了 返回头的长度
				pheader->m_fileseq += ret;
				memmove(m_databuffer, m_databuffer + ret, m_datalen - ret);
				m_datalen -= ret;
				if (pheader->m_nseq != seq + packetlen)
				{
error=8;
					ret = InsertPacketToBuffer(pPacket, seq, packetlen, pheader);
				}
			}
			else if (ret < 0)
			{
				return ret;		// 出错了
			}
			else if (ret == 0)
			{
				if (m_datalen>=8000)
				{
					return -3;		// 放弃找HTTP头
				}
				return 0;
			}
		}
error=10;
		if ((pheader->m_HaveHeader) && (pheader->m_Action != HTTP_GET))
		{
			if ((pheader->m_HaveHeader)
				&& ((pheader->m_nseq - pheader->m_fileseq) >= pheader->m_Content_length))
			{
				// 还原完成了
				if (pheader->m_Sack == 2278353100)
				{
					printf("ack 2278353100 finished\n");
				}
				return 8;
			}
error=11;
			if ((pheader->m_ifchunked) && (m_datalen>=7)
				&& (memcmp(m_databuffer+m_datalen-7, "\r\n\r\n\r\n", 7) == 0))
			{
				// 还原完成了
				return 8;
			}
		}
		return 0;
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "InsertPacketToBuffer error new err=%d",error);

	}
	return 0;

}

int HttpSession::FindHttpHeaderFromList(HttpCommInfo* pheader, TcpPacketList& TcpList, UINT& sortflg)
{
int error=0;
	try
	{
		if(TcpList.size() <= 0)
		{
			sortflg = 0;
			return 0;	// 队列中没有包
		}
		if(sortflg > 6)
		{
			TcpList.sort();
			sortflg = 0;
		}
error=1;
		TcpPacketListIt end, item, tmpitem;
		item = TcpList.begin();
		end = TcpList.end();
		int num = 0;
	//	int loopnum = 0;
		BOOL conte = FALSE;
	#ifdef		_DEBUG
		TcpPacket* p1;
	#endif
		while(item != end)
		{
	#ifdef		_DEBUG
			p1 = &*item;
	#endif
			tmpitem = item;
			item++;
			num++;
			if (tmpitem->Ssequence <= pheader->m_nseq)
			{
				if(tmpitem->Nextsequence > pheader->m_nseq)
				{
error=2;
					int re = InsertPacketToBuffer(tmpitem->buf, tmpitem->Ssequence, tmpitem->Slen, pheader);
					TcpList.erase(tmpitem);
					if (num <= sortflg)
					{
						sortflg--;		// 没有排序的包
						item = TcpList.begin();
						num = 0;
						conte = FALSE;
					}
					else
					{
						conte = TRUE;
						if (item == end)
						{
							item = TcpList.begin();
						}
					}
					if ((pheader->m_HaveHeader) || (re < 0))
					{
						return re;	// 找到头了>0 或 出错了<0
					}
				}
				else
				{
					TcpList.erase(tmpitem);
				}
			}
			else
			{
error=4;
				// 不连续
				if (num > sortflg)
				{
					if (conte)
					{
						conte = FALSE;
						item = TcpList.begin();	// 重新继续找
						num = 0;
						continue;
					}
					return 0;
				}
			}
		}
		return 0;
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "HttpSession::FindHttpHeaderFromList error new err=%d",error);	
	}
	return 0;

}

int HttpSession::ListToFile(TcpPacketList& TcpList, HttpCommInfo* pheader, char* buf, UINT& sortflg)
{
int err = 0;
	UINT dlen = 0;

	try
	{
		if(TcpList.size() <= 0)
		{
			sortflg = 0;
			return 0;	// 队列中没有包
		}
err=1;
		if(sortflg > 8)
		{
			TcpList.sort();
			sortflg = 0;
		}
err=2;
		TcpPacketListIt end, item, tmpitem;
		item = TcpList.begin();
		end = TcpList.end();
		int num = 0;
	//	int loopnum = 0;
		BOOL conte = FALSE;
		int re = 0;
	#ifdef		_DEBUG
		TcpPacket* p1;
	#endif
err=3;
		while(item != end)
		{
	#ifdef		_DEBUG
			p1 = &*item;
	#endif
			tmpitem = item;
			item++;
			num++;
			if (tmpitem->Ssequence <= pheader->m_nseq)
			{
err=4;
				if(tmpitem->Nextsequence > pheader->m_nseq)
				{
err=5;
					UINT copysize = tmpitem->Nextsequence - pheader->m_nseq;
					if (copysize > 2000)
					{
						ASSERT(FALSE);
						WriteLog(HTTPPRO, "copysize1 = %u", copysize);
						return -3;
					}

					if (dlen+ copysize > FILEBUFFERSIZE)
					{
						ASSERT(FALSE);
						WriteLog(HTTPPRO, "dlen > %u,num:%d", FILEBUFFERSIZE,num);
						return -3;
					}

					if(pheader == &m_Requestline)
					{
						// 请求队列
						if ((tmpitem->Slen>5)
							&& ((memcmp(tmpitem->buf, "GET ", 4)==0)
								|| (memcmp(tmpitem->buf, "POST ", 5)==0)))
						{
							re = 8;
//							ASSERT(FALSE);
							break;
						}
					}
					else
					{
						// 回应队列
						if ((tmpitem->Slen>10)
							&& (memcmp(tmpitem->buf, "HTTP/1.", 7)==0)
							&& (tmpitem->buf[8] == 0x20))
						{
							re = 8;
//							ASSERT(FALSE);
							break;
						}
					}

					if (FILEBUFFERSIZE > dlen+copysize)
					{
						memcpy(buf + dlen, tmpitem->buf + tmpitem->Slen - copysize, copysize);
					}

err=51;
					m_ProcessTime = tmpitem->tm;
					TcpList.erase(tmpitem);
					pheader->m_nseq += copysize;
					dlen += copysize;
err=52;
					if (num <= sortflg)
					{
						sortflg--;		// 没有排序的包
						item = TcpList.begin();
						num = 0;
						conte = FALSE;
					}
					else
					{
						conte = TRUE;
						if (item == end)
						{
							item = TcpList.begin();
						}
					}
err=53;
					if ((pheader->m_Content_length > 0)
						&& ((pheader->m_nseq - pheader->m_fileseq) >= pheader->m_Content_length))

					{
						re = 8;
						break;
					}
err=54;
					if ((pheader->m_ifchunked) && (dlen>=7)
						&& (memcmp(buf+dlen-7, "\r\n0\r\n\r\n", 7) == 0))
					{
						re = 8;
						break;
					}

					if (dlen >= FILEBUFFERSIZE-3000)
					{
						// 缓冲区满了
						break;
					}
				}
				else
				{
err=6;
					TcpList.erase(tmpitem);
				}
			}
			else
			{
				// 不连续
				if (num > sortflg)
				{
					if (conte)
					{
err=7;
						conte = FALSE;
						item = TcpList.begin();	// 重新继续找
						num = 0;
						continue;
					}
					re = -1;
					break;
				}
			}
		}

err=8;
		if (dlen <= 0)
		{
			return re;
		}
		
err=9;
		if (!pheader->m_ifchunked && (pheader->m_Content_length < DATABUFFER16KSIZE - 20))
		{
err=10;
			if (m_databufsize < pheader->m_Content_length + 20)
			{
err=11;
				int newsize = pheader->m_Content_length + 20;
				char* tp = new char[newsize];
				if (tp == NULL)
				{
					WriteLog(HTTPPRO, "new error new err3");

					return -3;	// 内存分配失败
				}
				if (m_datalen > newsize)
				{
					ASSERT(FALSE);
					WriteLog(HTTPPRO, "m_datalen 大于 newsize");
					return -3;
				}
				memset(tp + newsize - 20, 0, 20);
				memcpy(tp, m_databuffer, m_datalen>newsize?newsize:m_datalen);
				delete[] m_databuffer;
				m_databuffer = tp;
				m_databufsize = newsize;
			}
			UINT copysize = min(m_databufsize-20-m_datalen, dlen);
			if (copysize > DATABUFFER16KSIZE)
			{
				ASSERT(FALSE);
				WriteLog(HTTPPRO, "copysize2 = %u", copysize);
				return -3;
			}
			if (m_databufsize >= m_datalen+copysize)
			{
				memcpy(m_databuffer+m_datalen, buf, copysize);
				m_datalen += copysize;
			}
		}
		else if ((dlen + m_datalen + 10) <= m_databufsize)
		{
err=12;
			// add by cxl
// 			if (dlen > DATABUFFER16KSIZE)
// 			{
// 				ASSERT(FALSE);
// 				WriteLog(HTTPPRO, "dlen1 = %u", dlen);
// 				return -3;
// 			}
			// end

			if (m_databuffer == NULL)
			{
				ASSERT(FALSE);
				WriteLog(HTTPPRO, "m_databuffer = NULL");
				return -3;

			}
			if (m_databufsize >= m_datalen+dlen)
			{
				memcpy(m_databuffer + m_datalen, buf, dlen);
	err=121;
				m_datalen += dlen;
			}
		}
		else if (dlen + m_datalen < DATABUFFER8KSIZE - 20)
		{
err=13;
			ASSERT(m_databufsize < DATABUFFER8KSIZE);
			int newsize = DATABUFFER8KSIZE;
			char* tp = new char[newsize];
			if (tp == NULL)
			{
				WriteLog(HTTPPRO, "new error new err3");
				return -3;	// 内存分配失败
			}
			
			if (m_datalen > newsize)
			{
				ASSERT(FALSE);
				WriteLog(HTTPPRO, "2m_datalen 大于 newsize");
				return -3;
			}

			memset(tp + newsize - 20, 0, 20);

			memcpy(tp, m_databuffer, m_datalen);
			delete[] m_databuffer;
			m_databuffer = tp;
			m_databufsize = newsize;
			if (dlen > DATABUFFER16KSIZE)
			{
				ASSERT(FALSE);
				WriteLog(HTTPPRO, "dlen2 = %u", dlen);
				return -3;
			}
			if (m_databufsize >= dlen+m_datalen)
			{
				memcpy(m_databuffer + m_datalen, buf, dlen);
				m_datalen += dlen;
			}
		}
		else
		{
err=14;
			// 写文件
			re = WriteDataToTmpFile(buf, dlen, re, pheader->m_ifchunked);
		}

		return re;

	}
	catch(...)
	{
		WriteLog(HTTPPRO, "ListToFile() Error=%d!,m_datalen=%d,m_databufsize=%d,dlen=%d", err,m_datalen,m_databufsize,dlen);
		v_DceRunFlg = 0;
		return -3;
	}
}

int HttpSession::WriteDataToTmpFile(char* buf, int len, int overflg, BOOL bChunked)
{
	int err = 0;
	// 保存数据到文件 Chunk传输在这里处理
	try
	{
err=1;
		int re = overflg;
		if (m_realfname.IsEmpty())
		{
			if (!CreateFilename(this))
			{
				return -2;
			}
		}
		ASSERT(!m_realfname.IsEmpty());
err=2;
		FILE* pf = fopen(m_realfname, "ab");
		if (pf == NULL)
		{
			return -2;
		}
		if (bChunked)
		{
err=3;
			int pos = 0;
			if (m_chunksize > 0)
			{
				int writesize = min(m_chunksize, m_datalen);
				if (writesize > 0)
				{
					fwrite(m_databuffer, 1, writesize, pf);
					m_datalen -= writesize;
					m_chunksize -= writesize;
					pos += writesize;
				}
				if (m_chunksize > 0)
				{
					writesize = min(m_chunksize, len);
					if (writesize > 0)
					{
						fwrite(buf, 1, writesize, pf);
						len -= writesize;
						buf += writesize;
						m_chunksize -= writesize;
					}
				}
			}
err=4;
			while (m_chunksize == 0)
			{
				int chunksize = -1;
				if (m_datalen > 0)
				{
					if(m_datalen > m_databufsize || pos > m_databufsize)
					{
						WriteLog(HTTPPRO, "wrtietmpdata error size5");
						return -3;
					}
					if (m_datalen < 16)
					{
err=5;
						memmove(m_databuffer, m_databuffer+pos, m_datalen);
						int copysize = min(20, len);
						if (copysize > 0)
						{
err=6;
							if (m_databufsize < m_datalen+copysize)
							{
								WriteLog(HTTPPRO, "wrtietmpdata error size4");
								return -3;
							}
							memcpy(m_databuffer + m_datalen, buf, copysize);
							m_datalen += copysize;

							buf += copysize;
							len -= copysize;
						}
						pos = 0;
					}
err=7;
					if (memcmp(m_databuffer+pos, "\r\n", 2) == 0)
					{
						pos += 2;
						m_datalen -= 2;
					}
err=8;
					if (m_databufsize < pos+ min(m_datalen, 16))
					{
						WriteLog(HTTPPRO, "wrtietmpdata error size3");
						return -3;
					}
					int tpos = SearchCRLF.find(m_databuffer + pos, min(m_datalen, 16));	// 0d 0a
					if (tpos < 0)
					{
						re = -3;
						break;
					}
					if(sscanf(m_databuffer + pos, "%x", &chunksize) <= 0)
					{
						re = -3;
						break;
					}
					pos += tpos + 2;
					m_datalen -= tpos + 2;

					if (pos > m_databufsize)
					{
						WriteLog(HTTPPRO, "wrtietmpdata error size2");
						return -3;
					}

					if (chunksize<0 || chunksize>=0x1000000)
					{
						re = -3;
						break;
					}
					if (chunksize == 0)
					{
						re = 8;
						m_datalen = 0;
						break;
					}
err=9;
					m_chunksize = chunksize;
					int writesize = min(m_chunksize, m_datalen);
					

					if (m_databufsize < pos+writesize)
					{

						WriteLog(HTTPPRO, "wrtietmpdata error size1");
						return -3;
					}
err=91;
					if (writesize > 0)
					{
						fwrite(m_databuffer + pos, 1, writesize, pf);
						m_datalen -= writesize;
						m_chunksize -= writesize;
						pos += writesize;
					}
err=92;
					if ((m_chunksize > 0) && (len > 0))
					{
						writesize = min(m_chunksize, len);
						if (writesize > 0)
						{
							fwrite(buf, 1, writesize, pf);
							len -= writesize;
							buf += writesize;
							m_chunksize -= writesize;
						}
					}
				}
				else if (len > 2)
				{
	// 				if (len < 100)
err=10;
					if (memcmp(buf, "\r\n", 2) == 0)
					{
						buf += 2;
						len -= 2;
					}
err=101;
					int tpos = SearchCRLF.find(buf, min(len, 16));	// 0d 0a
					if (tpos < 0)
					{
						re = -3;
						break;
					}
err=102;
					if(sscanf(buf, "%x", &chunksize) <= 0)
					{
						re = -3;
						break;
					}
					buf += tpos + 2;
					len -= tpos + 2;
err=103;
					if (chunksize<0 || chunksize>=0x1000000)
					{
						re = -3;
						break;
					}
err=104;
					if (chunksize == 0)
					{
						re = 8;
						m_datalen = 0;
						break;
					}
					m_chunksize = chunksize;
err=105;
					int writesize = min(m_chunksize, len);
					if (writesize > 0)
					{
						fwrite(buf, 1, writesize, pf);
						len -= writesize;
						buf += writesize;
						m_chunksize -= writesize;
					}
				}
				else
				{
					if ((len > 0) && (m_databuffer != NULL))
					{
						ASSERT(m_datalen == 0);
						memcpy(m_databuffer, buf, len);
						m_datalen = len;
					}
					break;
				}
			}
		}
		else
		{
err=11;
			if (m_datalen > 0)
			{
				fwrite(m_databuffer, 1, m_datalen, pf);
				m_datalen = 0;
			}
			if (len > 0)
			{
				fwrite(buf, 1, len, pf);
			}
		}
err=12;
		if (ftell(pf) > Maxfilelen)
		{
#ifdef _DEBUG
			printf("%s_%s 会话大小超标, 放弃\r\n", m_sip, m_dip);
#endif
			WriteLog(HTTPPRO, "会话大小超标, 放弃");
			re = -2;	// 文件太大
		}
		fclose(pf);
		pf = NULL;
		return re;
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "WriteDataToTmpFile() Error=%d", err);
		return -3;
	}
}

//解析会话
BOOL HttpSession::DealHttpTmpData()
{
	m_FLAG = REQOVER;
	HttpCommInfo* pheader = (m_Requestline.m_Action == HTTP_POST)? &m_Requestline : &m_Responseline;
	int datalen;
	if ((!m_realfname.IsEmpty()) || (pheader->m_ifchunked))
	{
		if (m_datalen > 0)
		{
			WriteDataToTmpFile(NULL, 0, 8, pheader->m_ifchunked);
		}
		datalen = GetFileLength(m_realfname);
	}
	else
	{
		datalen = m_datalen;
	}
	if(((datalen <= 5 || datalen > Maxfilelen) || (m_Requestline.m_Action == HTTP_GET && datalen < 50)) && m_Requestline.m_Host.Find("a.mobile.yy.com")==-1)
	{
		return FALSE;	// 文件太大或太小
	}
	CString tmpstr;
	char* pData;
	BOOL bReadFile;
	BOOL bWriteFile;
	if(datalen == m_datalen)
	{
		pData = m_databuffer;
		memset(pData + m_datalen, 0, 8);
		bReadFile = FALSE;
		bWriteFile = TRUE;
	}
	else 
	{
		bReadFile = TRUE;
		bWriteFile = FALSE;
	}
	CFile file;
	if ((m_Requestline.m_Browser == TE_NOTHING)
		&& (datalen == m_datalen))
	{
		IMSGPROXY imsgproxy;
		imsgproxy.m_dataSrc=m_dataSrc;
		imsgproxy.packettm=m_ProcessTime;
		imsgproxy.Direction=1;
		imsgproxy.sip = m_sip;
		imsgproxy.dip = m_dip;
		memcpy(imsgproxy.csip, m_csip, 4);
		memcpy(imsgproxy.cdip, m_cdip, 4);
		imsgproxy.smac = m_smac;
		imsgproxy.dmac = m_dmac;
		imsgproxy.datalen=m_datalen;
		imsgproxy.pPacketData=m_databuffer;
		imsgproxy.proxytype = m_ProxyType;
		//处理即时消息
		if (InsertImsgProxy(imsgproxy, m_Requestline, m_spyaddr, this))
		{
			m_PacketType = 10;
			return TRUE;
		}
	}
	if ((m_Requestline.m_Browser==TE_DYNAPASS) || 
		(m_Requestline.m_Browser==TE_FREEGATE && m_Requestline.m_BrowserVer!=0x603))
	{
		// 还原特殊浏览方式的数据
		if (bReadFile)
		{
			// 读文件
			if (datalen < FILEBUFFERSIZE - 20)
			{
				pData = m_dceprocotol->m_HttpFileBuffer;
			}
			else
			{
				pData = tmpstr.GetBufferSetLength(datalen + 10);
			}
			file.Open(m_realfname, CFile::modeReadWrite);
			if (file.m_hFile == CFile::hFileNull)
			{
				return FALSE;
			}
			file.SeekToBegin();
			datalen = file.Read(pData, datalen);
			if (datalen <= 0)
			{
				file.Close();
				return FALSE;
			}
			memset(pData + datalen, 0, 8);
			bReadFile = FALSE;
		}
		// 解密数据
		FreeGateDec fnet;
		fnet.InitDataKey(m_Requestline.m_FreenetKey1, m_Requestline.m_FreenetKey2);
		fnet.DecodeData(pData, datalen);
		bWriteFile = TRUE;		// 数据需要回写到文件中
	}

	if (pheader->m_Content_Type.Find("urlencoded") > 0)
	{
		if (bReadFile)
		{
			// 读文件
			if (datalen < FILEBUFFERSIZE - 20)
			{
				pData = m_dceprocotol->m_HttpFileBuffer;
			}
			else
			{
				pData = tmpstr.GetBufferSetLength(datalen + 10);
			}
			file.Open(m_realfname, CFile::modeReadWrite);
			if (file.m_hFile == CFile::hFileNull)
			{
				return FALSE;
			}
			file.SeekToBegin();
			datalen = file.Read(pData, datalen);
			if (datalen <= 0)
			{
				file.Close();
				return FALSE;
			}
			memset(pData + datalen, 0, 8);
			bReadFile = FALSE;
		}
		ASSERT (datalen > 0);
		//need to test !!!////////////////////////////////////////////////////////////////////////
	//	ConverHttp2Html(pData, datalen);
		datalen = strlen(pData);
		bWriteFile = TRUE;
	}
	
//	HttpProObserver *hpo = HttpProObserver::Instance(); 
//	hpo->Process(this,m_databuffer);
    //判断类型
	if (email::EMail::ProcessEMail(this,m_databuffer))
	{
		return TRUE;
	}
	//购物
	if (shopping::Shopping::ProcessShopping(this,m_databuffer))
	{
		return TRUE;
	}
	//Employ
	else if (employ::Employ::ProcessEmploy(this,m_databuffer))
	{
		return TRUE;
	}
	if (contacts::Contacts::ProcessContacts(this,m_databuffer))
	{
		return TRUE;
	}
	
	if (forum::Forum::ProcessForum(this,m_databuffer))
	{
		return TRUE;
	}
	
	if (taxi::Taxi::ProcessTaxi(this,m_databuffer))
	{
		return TRUE;
	}
	//安装助手
	if (installed::Installed::ProcessInstalled(this,m_databuffer))
	{
		return TRUE;
	}

	if (running::Running::ProcessRunning(this,m_databuffer))
	{
		return TRUE;
	}

	if (phonedevice::PhoneDevice::ProcessPhoneDevice(this,m_databuffer))
	{
		return TRUE;
	}

	if (computerdevice::ComputerDevice::ProcessComputerDevice(this,m_databuffer))
	{
		return TRUE;
	}

	if (realname::RealName::ProcessRealName(this,m_databuffer))
	{
		return TRUE;
	}

	if (chating::Chating::ProcessChating(this,m_databuffer))
	{
		return TRUE;
	}

	if (location::Location::ProcessLocation(this,m_databuffer))
	{
		return TRUE;
	}
	//博客
	if (blog::Blog::ProcessBlog(this,m_databuffer))
	{
		return TRUE;
	}
    //阅读
	if (reading::Reading::ProcessReading(this,m_databuffer))
	{
		return TRUE;
	}
	//云盘
	if (yunpan::YunPan::ProcessYunPan(this,m_databuffer))
	{
		return TRUE;
	}
	//旅行
	if (Traffic::ProcessTraffic(this, m_databuffer))
	{
		return TRUE;
	}
	//快递
	if (kuaidi::KuaiDi::ProcessKuaidi(this, m_databuffer))
	{
		return TRUE;
	}
	//Airplane
	if (plane::Airplane::ProcessAirplane(this, m_databuffer))
	{
		return TRUE;
	}
	//HardwareInfo
	if (hardwareinfo::HardwareInfo::ProcessHardwareInfo(this, m_databuffer))
	{
		return TRUE;
	}
	//站长工具
    if (website::WebSite::ProcessWebSite(this,m_databuffer))
    {
		return TRUE;
    }
	//视频活动
    if (videoactivity::VideoActivity::ProcessVideoActivity(this,m_databuffer))
    {
		return TRUE;
    }
	//婚恋交友
    if (marriage::Marriage::ProcessMarriage(this,m_databuffer))
    {
		return TRUE;
    }
	//文件传输
    if (filetransfer::FileTransfer::ProcessFileTransfer(this,m_databuffer))
    {
		return TRUE;
    }
	//酒店预订
    if (hotel::Hotel::ProcessHotel(this,m_databuffer))
    {
		return TRUE;
    }
	//供求关系
	if (supply::Supply::ProcessSupply(this,m_databuffer))
	{
		return TRUE;
	}
	//第三方接口登入
	if (interfacelogin::InterfaceLogin::ProcessInterfaceLogin(this,m_databuffer))
	{
		return TRUE;
	}
	//软件下载
	if (software::Software::ProcessSoftware(this,m_databuffer))
	{
		return TRUE;
	}
	if (pheader->m_Content_Type.Find("urlencoded") > 0)
	{
		ConverHttp2Html(pData, datalen);
		datalen = strlen(pData);
	}
	DATEHEADER packh;
	if (ProcQQPimHttpUpPacket(this,packh,m_databuffer,FALSE))
	{
		return TRUE;
 	}
	//	
	if (bWriteFile)
	{
		if (file.m_hFile == CFile::hFileNull)
		{
			if (!CreateFilename(this))
			{
				return FALSE;
			}
			if (!file.Open(m_realfname, CFile::modeCreate | CFile::modeReadWrite))
			{
				return FALSE;
			}
		}
		file.SeekToBegin();
		file.Write(pData, datalen);
		file.SetLength(datalen);
	}
	if (file.m_hFile != CFile::hFileNull)
	{
		file.Close();
	}
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(82);
#endif
	int re = DealHotMailFile(this, pheader);
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(88);
#endif
	if(re!=8)
	{
		return FALSE;
	}
	DealCompressedFile(m_realfname, pheader);
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(89);
#endif
	CString attflst;
//	int ires=DealWebMail(this, pheader, attflst);
//	if (ires == 2)
//	{
//		return TRUE;
//	}
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(83);
#endif
	if ((m_realfname.Right(4).CompareNoCase(".rar")==0) ||
		(m_realfname.Right(4).CompareNoCase(".zip")==0))
	{
		int index =  m_attfiles.Add(m_realfname+"*1");
#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(84);
#endif
		if (!DealRarFile(m_realfname, GetShortName(m_realfname), m_attfiles, "dce2", attflst))
		{
#ifdef	ADD_RUNSTATE_INFO
			v_httprunstate.AddState(85);
#endif
			CString& s = m_attfiles.ElementAt(index);
			ASSERT(s.GetAt(s.GetLength()-2)=='*');
			s.SetAt(s.GetLength()-1, '4');
		}
		else
		{
			CString& s = m_attfiles.ElementAt(index);
			ASSERT(s.GetAt(s.GetLength()-2)=='*');
			s.SetAt(s.GetLength()-1, '3');
		}
	}
	//处理附件 关键字过滤 
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(86);
#endif
//	FilterFile(pheader);
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(87);
#endif
		//处理一下是否是淘宝的登录包
	if (m_Requestline.m_Action == HTTP_POST)
	{
		if (m_Requestline.m_URL.Find("/member/login.jhtml") >=0 &&
			m_Requestline.m_Host.Find(".taobao.com") >= 0)
		{
//			FormatTaobaoLogin()
		}
	}

 	DumpHttpFile(attflst);
	return TRUE;
}

void HttpSession::FindNextRequest()
{
	// 首先清理上一个会话的状态
	m_Requestline.Clear();
	m_Responseline.Clear();
	m_datalen = 0;
	if (!m_indexfname.IsEmpty())
	{
		DeleteFile(m_indexfname);
		m_indexfname.Empty();
	}

	if (!m_realfname.IsEmpty())
	{
		DeleteFile(m_realfname);
		m_realfname.Empty();
	}

	for (int i=0; i<m_attfiles.GetSize(); i++)
	{
		CString& s = m_attfiles.ElementAt(i);	
		// 以引用的方式操作数组中的字符串
		// 这样相当于直接操作数组中的字符串
		// 注意对其中任何一个变量的改变都会影响到另外一个变量
		if (s.GetLength()>2)
		{
			if (s.GetAt(s.GetLength()-2)=='*')
				s.ReleaseBuffer(s.GetLength()-2);
			DeleteFile(s);
		}
	}
	m_attfiles.RemoveAll();

	// 查找下一个"GET"或"POST"
	if (m_reqsortflag != 0)
	{
		m_ReqPacketlst.sort();
		m_reqsortflag = 0;
	}
	TcpPacketListIt item, tmpitem, end;
	item = m_ReqPacketlst.begin();
	end = m_ReqPacketlst.end();
	
	while((m_FLAG == NOREQ) && (item != end))
	{
		tmpitem = item;
		item++;
#ifdef		_DEBUG
		TcpPacket* p1 = &*tmpitem;
#endif
		if(memcmp(tmpitem->buf, "GET ", 4) == 0)
		{
			m_FLAG = REQPROCING;
			m_Requestline.m_Action=HTTP_GET;
			m_Requestline.m_Ssequence = tmpitem->Ssequence;
			m_Requestline.m_Sack = tmpitem->Sack;
			m_Requestline.m_nseq = tmpitem->Ssequence;
			m_Requestline.m_fileseq = tmpitem->Ssequence;
			m_ProcessTime = tmpitem->tm;
			InsertPacketToBuffer(tmpitem->buf, tmpitem->Ssequence,
				tmpitem->Slen, &m_Requestline);
			m_Responseline.m_fileseq = m_Requestline.m_Sack;
			m_Responseline.m_nseq = m_Requestline.m_Sack;
		}
		else if (memcmp(tmpitem->buf, "POST ", 5) == 0)
		{
			m_FLAG = REQPROCING;
			m_Requestline.m_Action = HTTP_POST;
			m_Requestline.m_Ssequence = tmpitem->Ssequence;
			m_Requestline.m_Sack = tmpitem->Sack;
			m_Requestline.m_nseq = tmpitem->Ssequence;
			m_Requestline.m_fileseq = tmpitem->Ssequence;
			m_ProcessTime = tmpitem->tm;
			InsertPacketToBuffer(tmpitem->buf, tmpitem->Ssequence,
				tmpitem->Slen, &m_Requestline);
		}
		m_ReqPacketlst.erase(tmpitem);
	}
	return ;
}


BOOL HttpSession::DealMultiPackGet()
{
	if (realname::RealName::ProcessRealName(this,m_databuffer))
	{
		return TRUE;
	}

	if (search::Search::ProcessSearch(this,m_databuffer))
	{
		return TRUE;
	}
	return TRUE;
}