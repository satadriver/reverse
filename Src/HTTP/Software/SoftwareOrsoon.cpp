#include "StdAfx.h"
#include  "SoftwareOrsoon.h"

software::Orsoon::Orsoon(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "未来软件园";	
}

software::Orsoon::~Orsoon()
{
	
}

int software::Orsoon::Is(HttpSession *http_session)
{
	//搜索
	if(http_session->m_Requestline.m_Host.Find("s.orsoon.com")!=-1 && 
		http_session->m_Requestline.m_URI.Find("/cse/search")!=-1 ) 
	{
		return kWebSearch;
	}
	//下载 1
	if(((http_session->m_Requestline.m_Host.Find(".qpzqxz.com")!=-1)) && 
		(http_session->m_Requestline.m_URI.Find("/download/")!=-1)) 
	{
		return kWebDown;
	}
	//下载 2
	if( http_session->m_Requestline.m_Host.Find("url.7wkw.com") !=-1 && 
	    http_session->m_Requestline.m_URI.Find("/down/")!=-1 &&
		http_session->m_Requestline.m_Referer.Find("http://www.orsoon.com/Soft/") != -1) 
	{
		return kWebDown;
	}
	//下载 3
	if(((http_session->m_Requestline.m_Host.Find(".orsoon.com:")!=-1)) && 
		(http_session->m_Requestline.m_URI.Find("/")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::Orsoon::Process(const char *packet,int action)
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
int software::Orsoon::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   ms_->Replace(url,"?","&");
   url = ms_->UrlDecode(url);
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
int software::Orsoon::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   string host = http_session_->m_Requestline.m_Host.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //软件名称
   if (url.find("/download/") != -1)
   {
	   softname_ = url.substr(url.find("/download/") + strlen("/download/"));
   }
   //软件名称
   else if (url.find("/down/") != -1)
   {
	   softname_ = url.substr(url.find("/down/") + strlen("/down/"));
   }
   else
   {
		softname_ = url.substr(url.find("/") + strlen("/"));
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
