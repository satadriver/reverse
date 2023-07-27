#include "StdAfx.h"
#include  "SoftwareDownxia.h"

software::Downxia::Downxia(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "�������԰";	
}

software::Downxia::~Downxia()
{
	
}

int software::Downxia::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find(".downxia.")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search.php")!=-1)) 
	{
		return kWebSearch;
	}
	//���� exe
	if(http_session->m_Requestline.m_Host.Find(".url.") != -1 &&
		http_session->m_Requestline.m_URI.Find(".exe") != -1 &&
		http_session->m_Requestline.m_Referer.Find("http://www.downxia.com") != -1)
	{
		return kWebDown;
	}
	//���� rar
	if (http_session->m_Requestline.m_Host.Find(".downxia.com") != -1 &&
		http_session->m_Requestline.m_URI.Find(".rar") != -1 &&
		http_session->m_Requestline.m_Referer.Find("http://www.downxia.com") != -1)
	{
		return kWebDown;
	}
	//���� apk
	if(http_session->m_Requestline.m_Host.Find("download.downxia.com") != -1 && 
	   http_session->m_Requestline.m_URI.Find(".apk") != -1 &&
	   http_session->m_Requestline.m_Referer.Find("http://www.downxia.com") != -1) 
	{
		return kWebDown;
	}
	
	return 0;
}

int software::Downxia::Process(const char *packet,int action)
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
int software::Downxia::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //�������
   softname_ = ms_->GetValueFromMapByKey("q",true);
   if (softname_.empty())
   {
	   return 0;
   }
   output_text_ =				
	   "mid=" + 	  	softname_ + "\r\n" + 	//�������	
	   "netmark=" + 	app_ + "\r\n" + 		//��Դ 	
	   "clientmsg=" + 	platformWeb + "\r\n" + 	//����ƽ̨		
	   "state=����\r\n";						//״̬ 				
   WriteIndexFile();  
   return 0;
}
//����
int software::Downxia::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //�������
   if (url.rfind("/") != -1)
   {
	   softname_ = url.substr(url.rfind("/") + strlen("/"));
   }
   //�������
   softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
   if (softname_.empty() || softurl_.empty())
   {
	   return 0;
   }
   output_text_ =				
	   "mid=" + 	  	softname_ + "\r\n" + 	//�������	
	   "fileurl=" +     softurl_ + "\r\n" + 	//�������
	   "netmark=" + 	app_ + "\r\n" + 		//��Դ 	
	   "clientmsg=" + 	platformWeb + "\r\n" + 	//����ƽ̨		
	   "state=����\r\n";						//״̬ 
   WriteIndexFile();  
	return 0;
}
