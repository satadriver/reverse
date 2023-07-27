#include "StdAfx.h"
#include  "SoftwareCr173.h"

software::Cr173::Cr173(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "西西软件园";	
}

software::Cr173::~Cr173()
{
	
}

int software::Cr173::Is(HttpSession *http_session)
{
	//搜索
	if((http_session->m_Requestline.m_Host.Find("so.cr173.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/d/")!=-1)) 
	{
		return kWebSearch;
	}
	//下载 (高速下载)
	if((http_session->m_Requestline.m_Host.Find(".xzqxzs.com")!=-1)  && 
		(http_session->m_Requestline.m_URI.Find("/download/")!=-1)) 
	{
		return kWebDown;
	}
	//下载 (普通下载)
	if((http_session->m_Requestline.m_Host.Find(".mqego.com")!=-1) || 
		(http_session->m_Requestline.m_Host.Find(".fgtrj.com")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::Cr173::Process(const char *packet,int action)
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
int software::Cr173::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
 
   //软件名称
   softname_ = ms_->GetValueBetweenBeginEnd(url,"/search/d/","_pc_",true);
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
int software::Cr173::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //软件名称
   if (url.rfind("/") != -1)
   {
	   softname_ = url.substr(url.rfind("/")+1);
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
