#include "StdAfx.h"
#include  "SoftwareSina.h"

software::Sina::Sina(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "新浪下载";	
}

software::Sina::~Sina()
{
	
}

int software::Sina::Is(HttpSession *http_session)
{
	//搜索
	if((http_session->m_Requestline.m_Host.Find("down.tech.sina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/download/search.php")!=-1)) 
	{
		return kWebSearch;
	}
	//下载
	if((http_session->m_Requestline.m_Host.Find("tech.down.sina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("Expires")!=-1)) 
	{
		return kWebDown;
	}
	//浏览
	if((http_session->m_Requestline.m_Host.Find("sina.wrating.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/a.gif")!=-1)) 
	{
		return kWrating;
	}
	return 0;
}

int software::Sina::Process(const char *packet,int action)
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
	//浏览
	if (action == kWrating)
	{
		return Wrating(packet);
	}
	return 1;
}
//搜索
int software::Sina::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //软件名称
   softname_ = ms_->GetValueFromMapByKey("f_name",true);
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
int software::Sina::WebDown(const char *packet)
{ 
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   url = url.substr(0,url.rfind("?"));
   //软件名称
   softname_ = url.substr(url.rfind("/") + strlen("/"));
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
//浏览
int software::Sina::Wrating(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	if (url.find("?") != string::npos)
	{
		url = url.substr(url.find("?"));
	}
	url = ms_->UrlDecode(url);
    ms_->SplitStringToMap(url,"&","=");
	//软件名称
	softname_ = ms_->GetValueFromMapByKey("t",true);
	//软件详情
	softurl_ = ms_->GetValueFromMapByKey("b",true);
	if (softname_.empty())
	{
		return 0;
	}
	output_text_ = 
		"mid=" + 	  	softname_ + "\r\n" + 	//软件名称	
		"fileurl=" +     softurl_ + "\r\n" + 	//软件详情
		"netmark=" + 	app_ + "\r\n" + 		//来源 	
		"clientmsg=" + 	platformWeb + "\r\n" + 	//操作平台		
	    "state=浏览\r\n";		     			//状态 
	WriteIndexFile();
	return 0;
}