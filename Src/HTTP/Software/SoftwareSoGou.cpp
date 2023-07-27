#include "StdAfx.h"
#include  "SoftwareSoGou.h"

software::SoGou::SoGou(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "搜狗下载";	
}

software::SoGou::~SoGou()
{
	
}

int software::SoGou::Is(HttpSession *http_session)
{
	//搜索
	if((http_session->m_Requestline.m_Host.Find("xiazai.sogou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search")!=-1)) 
	{
		return kWebSearch;
	}
	//下载 
	if((http_session->m_Requestline.m_Host.Find("xiazai.sogou.com")!=-1)&& 
		(http_session->m_Requestline.m_URI.Find("/comm/redir")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::SoGou::Process(const char *packet,int action)
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
int software::SoGou::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //软件名称
   softname_ = ms_->GetValueFromMapByKey("query",true);
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
int software::SoGou::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //软件名称
   softname_ = ms_->GetValueFromMapByKey("filename",true);
   if (softname_.empty())
   {
	   return 0;
   }
   //软件详情
   softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
   softurl_= ms_->UTF8ToGBK(UrlDecode(softurl_));
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
