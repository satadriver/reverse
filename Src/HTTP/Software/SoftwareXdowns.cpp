#include "StdAfx.h"
#include  "SoftwareXdowns.h"

software::Xdowns::Xdowns(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "��ɫ��������";	
}

software::Xdowns::~Xdowns()
{
	
}

int software::Xdowns::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("tag.xdowns.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/tag.asp")!=-1)) 
	{
		return kWebSearch;
	}
	if((http_session->m_Requestline.m_Host.Find("www.xdowns.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/so.asp")!=-1)) 
	{
		return kWebSearch;
	}
	//���� 
	if((http_session->m_Requestline.m_Referer.Find("http://www.xdowns.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find(".exe")!=-1)) 
	{
		return kWebDown;
	}
	if((http_session->m_Requestline.m_Referer.Find("http://www.xdowns.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find(".apk")!=-1)) 
	{
		return kWebDown;
	}
	if((http_session->m_Requestline.m_Referer.Find("http://www.xdowns.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find(".rar")!=-1)) 
	{
		return kWebDown;
	}
	if((http_session->m_Requestline.m_Referer.Find("http://www.xdowns.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find(".zip")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::Xdowns::Process(const char *packet,int action)
{
	//����
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
    //����
	if (action == kWebDown)
	{
		return WebDown(packet);
	}
	return 1;
}
//����
int software::Xdowns::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //��������
   softname_ = ms_->GetValueFromMapByKey("keyword",true);
   if (softname_.empty())
   {
	   return 0;
   }
   output_text_ =				
	   "mid=" + 	  	softname_ + "\r\n" + 	//��������	
	   "netmark=" + 	app_ + "\r\n" + 		//��Դ 	
	   "clientmsg=" + 	platformWeb + "\r\n" + 	//����ƽ̨		
	   "state=����\r\n";						//״̬ 				
   WriteIndexFile();  
   return 0;
}
//����
int software::Xdowns::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //��������
   softname_=url.substr(url.rfind("/")+1);
   //��������
   softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
   if (softname_.empty() || softurl_.empty())
   {
	   return 0;
   }
   output_text_ =				
	   "mid=" + 	  	softname_ + "\r\n" + 	//��������	
	   "fileurl=" +     softurl_ + "\r\n" + 	//��������
	   "netmark=" + 	app_ + "\r\n" + 		//��Դ 	
	   "clientmsg=" + 	platformWeb + "\r\n" + 	//����ƽ̨		
	   "state=����\r\n";						//״̬ 
   WriteIndexFile();  
	return 0;
}