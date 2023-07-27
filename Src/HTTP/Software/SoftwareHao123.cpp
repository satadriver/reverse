#include "StdAfx.h"
#include  "SoftwareHao123.h"

software::Hao123::Hao123(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "hao123软件下载";	
}

software::Hao123::~Hao123()
{
	
}

int software::Hao123::Is(HttpSession *http_session)
{
	//搜索
	if((http_session->m_Requestline.m_Host.Find("soft.hao123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/s.php")!=-1)) 
	{
		return kWebSearch;
	}
	//下载
	if((http_session->m_Requestline.m_Host.Find("softdown1.hao123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hao123-soft-online-bcs/")!=-1)) 
	{
		return kWebDown;
	}
	//下载 (包含地址)
	if((http_session->m_Requestline.m_Host.Find("www.hao123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("h.gif")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("title") != -1)) 
	{
		return kWebDownAddress;
	}
	//下载 (包含地址 2)
	if((http_session->m_Requestline.m_Host.Find("hcl.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("h.gif")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("title") != -1)) 
	{
		return kWebDownAddress;
	}
	return 0;
}

int software::Hao123::Process(const char *packet,int action)
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
	//下载 (包含地址)
	if (action == kWebDownAddress)
	{
		return WebDownAddress(packet);
	}
	return 1;
}
//搜索
int software::Hao123::WebSearch(const char *packet)
{
   string src_data(packet);
   if (src_data.rfind("\r\n") != -1)
   {
	   src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
   }
   src_data = ms_->UrlDecode(src_data);
   ms_->SplitStringToMap(src_data,"&","=");
   //软件名称
   softname_ = ms_->GetValueFromMapByKey("key",true);
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
int software::Hao123::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //软件名称
   if (url.find("/soft/") != -1)
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
//下载 (包含地址)
int software::Hao123::WebDownAddress(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	if (url.find("?") != string::npos)
	{
		url = url.substr(url.find("?"));
	}
	url = ms_->UrlDecode(url);
    ms_->SplitStringToMap(url,"&","=");
	//软件名称
	softname_ = ms_->GetValueFromMapByKey("title",true);
	//下载地址
	softdownurl_ = ms_->GetValueFromMapByKey("objurl",true);
	//软件详情
	softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	softurl_ = ms_->UrlDecode(softurl_);

	if (softname_.empty())
	{
		return 0;
	}
	output_text_ = 
		"mid=" + 	  	softname_ + "\r\n" + 	//软件名称	
		"fileurl=" +    softurl_ + "\r\n" + 	//软件详情
		"clientip=" + 	softdownurl_ + "\r\n" + //下载地址
		"netmark=" + 	app_ + "\r\n" + 		//来源 	
		"clientmsg=" + 	platformWeb + "\r\n" + 	//操作平台		
		"state=下载\r\n";		     			//状态 
	WriteIndexFile();
	return 0;
}
