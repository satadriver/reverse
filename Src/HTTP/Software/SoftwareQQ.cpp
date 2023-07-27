#include "StdAfx.h"
#include "SoftwareQQ.h"

software::QQ::QQ(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "腾讯软件中心";	
}

software::QQ::~QQ()
{
	
}

int software::QQ::Is(HttpSession *http_session)
{
	//搜索
	if((http_session->m_Requestline.m_Host.Find("s.pcmgr.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/tapi/web/searchcgi.php?type=search")!=-1)) 
	{
		return kWebSearch;
	}
	//下载 (高速下载)
	if((http_session->m_Requestline.m_Host.Find("dlied6.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/invc/xfspeed/")!=-1)) 
	{
		return kWebDown;
	}
	//下载 (普通下载)
	if((http_session->m_Requestline.m_Host.Find("dl.softmgr.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/original/")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::QQ::Process(const char *packet,int action)
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
int software::QQ::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //软件名称
   softname_ = ms_->GetValueFromMapByKey("keyword",true);
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
int software::QQ::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //软件名称
   if (url.find("/invc/xfspeed/") != -1)
   {
	   softname_ = url.substr(url.rfind("/") + strlen("/"));
   }
   if (url.find("/original/") != -1)
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
