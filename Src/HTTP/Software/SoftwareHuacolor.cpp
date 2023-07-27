#include "StdAfx.h"
#include  "SoftwareHuacolor.h"

software::Huacolor::Huacolor(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "华彩软件园";	
}

software::Huacolor::~Huacolor()
{
	
}

int software::Huacolor::Is(HttpSession *http_session)
{
	//搜索
	if((http_session->m_Requestline.m_Host.Find(".huacolor.")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/e/search/index.php")!=-1)) 
	{
		return kWebSearch;
	}
	//下载 (普通下载)
	if((http_session->m_Requestline.m_Host.Find(".huacolor.")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/down/")!=-1)) 
	{
		return kWebDown;
	}
	
	return 0;
}

int software::Huacolor::Process(const char *packet,int action)
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
int software::Huacolor::WebSearch(const char *packet)
{
  string src_data(packet);
   if (src_data.rfind("\r\n") != -1)
   {
	   src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
   }
   src_data = ms_->UrlDecode(src_data);
   ms_->SplitStringToMap(src_data,"&","=");
   //软件名称
   softname_= ms_->GetValueFromMapByKey("keyboard",true);
   output_text_ =				
	   "mid=" + 	  	softname_ + "\r\n" + 	//软件名称	
	   "netmark=" + 	app_ + "\r\n" + 		//来源 	
	   "clientmsg=" + 	platformWeb + "\r\n" + 	//操作平台		
	   "state=搜索\r\n";						//状态 				
   WriteIndexFile();  
   return 0;
}
//下载
int software::Huacolor::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //软件名称
   if (url.find("/down/") != -1)
   {
	   softname_ = url.substr(url.rfind("/") + strlen("/"));
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
