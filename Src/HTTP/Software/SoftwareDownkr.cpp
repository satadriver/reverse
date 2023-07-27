#include "StdAfx.h"
#include  "SoftwareDownkr.h"

software::DownKr::DownKr(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "�������԰";	
}

software::DownKr::~DownKr()
{
	
}

int software::DownKr::Is(HttpSession *http_session)
{
	//����
	if(http_session->m_Requestline.m_Host.Find("s.downkr.com")!=-1 && 
	  (http_session->m_Requestline.m_URI.Find("/cse/search")!=-1 || 
	   http_session->m_Requestline.m_URI.Find("/api/customsearch/click")!=-1)) 
	{
		return kWebSearch;
	}
	//���� 
	if(http_session->m_Requestline.m_Host.Find(".url.")!=-1 && 
	   http_session->m_Requestline.m_URI.Find("/down/")!=-1 &&
	   http_session->m_Requestline.m_Referer.Find("http://www.downkr.com/soft/") != -1) 
	{
		return kWebDown;
	}
	//���� 
	if(http_session->m_Requestline.m_Host.Find(".downkr.com")!=-1 && 
	   http_session->m_Requestline.m_URI.Find("/soft/")!=-1 &&
	   http_session->m_Requestline.m_Referer.Find("http://www.downkr.com/soft/") != -1) 
	{
		return kWebDown;
	}
	return 0;
}

int software::DownKr::Process(const char *packet,int action)
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
int software::DownKr::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   if (url.find("?") != -1)
   {
	   url = url.substr(url.find("?") + strlen("?"));
   }
   url = ms_->UrlDecode(url);
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
int software::DownKr::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //�������
   if (url.find("/down/") != -1)
   {
	   softname_ = url.substr(url.find("/down/") + strlen("/down/"));
   }
   //�������
   if (url.find("/soft/") != -1)
   {
	   softname_ = url.substr(url.find("/soft/") + strlen("/soft/"));
   }
   //�������
   softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
   if (softname_.empty())
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
