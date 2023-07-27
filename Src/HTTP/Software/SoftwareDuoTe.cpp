#include "StdAfx.h"
#include  "SoftwareDuoTe.h"

software::DuoTe::DuoTe(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "2345软件大全(原多特软件站)";	
}

software::DuoTe::~DuoTe()
{
	
}

int software::DuoTe::Is(HttpSession *http_session)
{
	//搜索 pc软件
	if(http_session->m_Requestline.m_Host.Find("so.duote.com")!=-1 && 
		http_session->m_Requestline.m_URI.Find("/cse/search")!=-1 ) 
	{
		return kWebSearch;
	}
	//搜索 安卓apk	
	if(http_session->m_Requestline.m_Host.Find("www.duote.com")!=-1 && 
		http_session->m_Requestline.m_URI.Find("/searchPhone.php")!=-1 ) 
	{
		return kWebSearch;
	}
	//下载 
	if(http_session->m_Requestline.m_Referer.Find("www.duote.com")!=-1 && 
		http_session->m_Requestline.m_URI.Find(".exe")!=-1) 
	{
		return kWebDown;
	}
	//下载 apk
	if(http_session->m_Requestline.m_Host.Find("app.2345.cn")!=-1 && 
	   http_session->m_Requestline.m_URI.Find(".apk")!=-1) 
	{
		return kApkDown;
	}
	return 0;
}

int software::DuoTe::Process(const char *packet,int action)
{
	//搜索
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
    //下载
	if (action == kWebDown)
	{
		return WebDown(packet);
	}
	//下载 apk
	if (action == kApkDown)
	{
		return ApkDown(packet);
	}
	return 1;
}
//搜索
int software::DuoTe::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   ms_->Replace(url,"?","&");
   url = ms_->UrlDecode(url);
   ms_->SplitStringToMap(url,"&","=");

   if (url.find("/cse/search") != -1)
   {
	   //软件名称
		softname_ = ms_->GetValueFromMapByKey("q",true);
   }
   else if (url.find("/searchPhone.php") != -1)
   {
	   //软件名称
		softname_ = ms_->GetValueFromMapByKey("so",true);
   }
   if (softname_.empty())
   {
	   return 0;
   }
   output_text_ =				
	   "mid=" + 	  	softname_ + "\r\n" + 	//软件名称	
	   "netmark=" + 	app_ + "\r\n" + 		//来源 	
	   "clientmsg=" + 	platformWeb + "\r\n" + 	//操作平台		
	   "state=搜索\r\n";						//状态 				
   WriteIndexFile();  
   return 0;
}
//下载
int software::DuoTe::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //软件名称
   if (url.find("/down/") != -1)
   {
	   softname_ = url.substr(url.find("/down/") + strlen("/down/"));
   }
   //软件名称
   if (url.find("/soft/") != -1)
   {
	   softname_ = url.substr(url.find("/soft/") + strlen("/soft/"));
   }
   //软件详情
   softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
   if (softname_.empty())
   {
	   return 0;
   }
   output_text_ =				
	   "mid=" + 	  	softname_ + "\r\n" + 	//软件名称	
	   "fileurl=" +     softurl_ + "\r\n" + 	//软件详情
	   "netmark=" + 	app_ + "\r\n" + 		//来源 	
	   "clientmsg=" + 	platformWeb + "\r\n" + 	//操作平台		
	   "state=下载\r\n";						//状态 
   WriteIndexFile();  
	return 0;
}
//下载 apk
int software::DuoTe::ApkDown(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	url = ms_->UrlDecode(url);
	//软件详情
	if (url.rfind("/") != -1)
	{
		softname_ = url.substr(url.rfind("/") + strlen("/"));
	}
	softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	if (softname_.empty())
	{
		return 0;
	}
	output_text_ =				
		"mid=" + 	  	softname_ + "\r\n" + 	//软件名称	
		"fileurl=" +     softurl_ + "\r\n" + 	//软件详情
		"netmark=" + 	app_ + "\r\n" + 		//来源 	
		"clientmsg=" + 	platformWeb + "\r\n" + 	//操作平台		
		"state=下载\r\n";						//状态 
	WriteIndexFile();  
	return 0;
}
