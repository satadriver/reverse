#include "StdAfx.h"
#include  "SoftwarePconline.h"

software::Pconline::Pconline(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "太平洋下载";	
}

software::Pconline::~Pconline()
{
	
}

int software::Pconline::Is(HttpSession *http_session)
{
	//搜索
	if((http_session->m_Requestline.m_Host.Find("ks.pconline.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/download.shtml")!=-1)) 
	{
		return kWebSearch;
	}
	//下载 
	if(((http_session->m_Requestline.m_Host.Find(".xzn56.com")!=-1)||(http_session->m_Requestline.m_Host.Find(".xzm56.com")!=-1))  && 
		(http_session->m_Requestline.m_URI.Find("/pconline")!=-1)) 
	{
		return kWebDown;
	}
	//下载 
	if((http_session->m_Requestline.m_Host.Find(".pconline.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/filedown")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::Pconline::Process(const char *packet,int action)
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
int software::Pconline::WebSearch(const char *packet)
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
int software::Pconline::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //软件名称
   if (url.rfind("/") != -1)
   {
		softname_= url.substr(url.rfind("/")+1);
   }  
   //软件详情
   softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
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
