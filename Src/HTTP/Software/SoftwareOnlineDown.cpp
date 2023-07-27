#include "StdAfx.h"
#include  "SoftwareOnlineDown.h"

software::OnlineDown::OnlineDown(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "华军软件园";	
}

software::OnlineDown::~OnlineDown()
{
	
}

int software::OnlineDown::Is(HttpSession *http_session)
{
	//搜索
	if((http_session->m_Requestline.m_Host.Find("search.onlinedown.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search_list.php")!=-1)) 
	{
		return kWebSearch;
	}
	//下载 (高速下载)
	if(((http_session->m_Requestline.m_Host.Find("dl.apxlx.com")!=-1) || (http_session->m_Requestline.m_Host.Find("cl2.cjsdxz.com") != -1)) && 
		(http_session->m_Requestline.m_URI.Find("/download/")!=-1)) 
	{
		return kWebDown;
	}
	//下载 (普通下载)
	if((http_session->m_Requestline.m_Host.Find(".onlinedown.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/down/")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::OnlineDown::Process(const char *packet,int action)
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
int software::OnlineDown::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //软件名称
   softname_ = ms_->GetValueFromMapByKey("searchname",true);
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
int software::OnlineDown::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //软件名称
   if (url.find("/download/") != -1)
   {
	   softname_ = url.substr(url.find("/download/") + strlen("/download/"));
   }
   if (url.find("/down/") != -1)
   {
	   softname_ = url.substr(url.find("/down/") + strlen("/down/"));
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
