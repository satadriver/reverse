#include "StdAfx.h"
#include  "SoftwarePc6.h"

software::Pc6::Pc6(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "PC6下载站";	
}

software::Pc6::~Pc6()
{
	
}

int software::Pc6::Is(HttpSession *http_session)
{
	//搜索
	if((http_session->m_Requestline.m_Host.Find("s.pc6.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cse/search")!=-1)||
		(http_session->m_Requestline.m_URI.Find("/suggest")!=-1)) 
	{
		return kWebSearch;
	}
	//高速下载 
	if((http_session->m_Requestline.m_Host.Find(".gxjsxq.com")!=-1)&& 
		(http_session->m_Requestline.m_URI.Find("/download/")!=-1)) 
	{
		return kWebDown;
	}
	//普通下载 
	if((http_session->m_Requestline.m_Host.Find("stat-api.20hn.cn")!=-1)&& 
		(http_session->m_Requestline.m_URI.Find("/jf.gif")!=-1)) 
	{
		return kWebDown1;
	}
	//安卓下载
	if((http_session->m_Requestline.m_Host.Find(".pc6.com")!=-1) && (http_session->m_Requestline.m_URI.Find(".apk") != -1) &&
		(http_session->m_Requestline.m_Referer.Find("http://www.pc6.com/az/")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::Pc6::Process(const char *packet,int action)
{
	//搜索
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
    //高速下载
	if (action == kWebDown)
	{
		return WebDown(packet);
	}
	//普通下载
	if (action == kWebDown1)
	{
		return WebDown1(packet);
	}
	return 1;
}
//搜索
int software::Pc6::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //软件名称
   if (url.find("/cse/search")!=-1)
   {
	   softname_ = ms_->GetValueFromMapByKey("q",true);
   }
   if (url.find("/suggest")!=-1)
   {
	   softname_=ms_->GetValueFromMapByKey("k",true);
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
//普通下载
int software::Pc6::WebDown1(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	url = ms_->UTF8ToGBK(UrlDecode(url));
	ms_->Replace(url,"?","&");
	ms_->SplitStringToMap(url,"&","=");
	//软件名称
	softname_=ms_->GetValueFromMapByKey("title",true);	
	if (softname_.empty())
	{
		return 0;
	}
	//软件详情
	softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	softurl_= ms_->UTF8ToGBK(UrlDecode(softurl_));
	if (softname_.empty() || softurl_.empty())
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
//高速下载&&安卓下载
int software::Pc6::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //软件名称
	softname_=url.substr(url.rfind("/")+1);
   if (softname_.empty())
   {
	   return 0;
   }
   //软件详情
   softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
   softurl_= ms_->UTF8ToGBK(UrlDecode(softurl_));
   if (softname_.empty() || softurl_.empty())
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
