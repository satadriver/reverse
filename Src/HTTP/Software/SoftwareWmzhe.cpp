#include "StdAfx.h"
#include  "SoftwareWmzhe.h"

software::Wmzhe::Wmzhe(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "完美下载";	
}

software::Wmzhe::~Wmzhe()
{
	
}

int software::Wmzhe::Is(HttpSession *http_session)
{
	//搜索
	if((http_session->m_Requestline.m_Host.Find("s.wmzhe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cse/search")!=-1)) 
	{
		return kWebSearch;
	}
	//下载 exe
	if(http_session->m_Requestline.m_Host.Find(".url.") != -1 &&
		http_session->m_Requestline.m_URI.Find(".exe") != -1 &&
		http_session->m_Requestline.m_Referer.Find("www.wmzhe.com/soft") != -1) 
	{
		return kWebDown;
	}
	//下载 zip
	if (http_session->m_Requestline.m_Host.Find(".wmzhe.com") != -1 &&
		http_session->m_Requestline.m_URI.Find(".zip") != -1 &&
		http_session->m_Requestline.m_Referer.Find("www.wmzhe.com/soft") != -1)
	{
		return kWebDown;
	}
	//android 下载
	if( http_session->m_Requestline.m_Host.Find(".wmzhe.com")!=-1 && 
		http_session->m_Requestline.m_URI.Find(".apk")!=-1 &&
		http_session->m_Requestline.m_Referer.Find("www.wmzhe.com/android/") != -1) 
	{
		return kWebDown;
	}
	return 0;
}

int software::Wmzhe::Process(const char *packet,int action)
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
	return 1;
}
//搜索
int software::Wmzhe::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //软件名称
   softname_ = ms_->GetValueFromMapByKey("q",true);
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
int software::Wmzhe::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //软件名称
   if (url.rfind("/") != -1)
   {
	   softname_ = url.substr(url.rfind("/") + strlen("/"));
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
