#include "StdAfx.h"
#include  "SoftwareWmzhe.h"

software::Wmzhe::Wmzhe(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "��������";	
}

software::Wmzhe::~Wmzhe()
{
	
}

int software::Wmzhe::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("s.wmzhe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cse/search")!=-1)) 
	{
		return kWebSearch;
	}
	//���� exe
	if(http_session->m_Requestline.m_Host.Find(".url.") != -1 &&
		http_session->m_Requestline.m_URI.Find(".exe") != -1 &&
		http_session->m_Requestline.m_Referer.Find("www.wmzhe.com/soft") != -1) 
	{
		return kWebDown;
	}
	//���� zip
	if (http_session->m_Requestline.m_Host.Find(".wmzhe.com") != -1 &&
		http_session->m_Requestline.m_URI.Find(".zip") != -1 &&
		http_session->m_Requestline.m_Referer.Find("www.wmzhe.com/soft") != -1)
	{
		return kWebDown;
	}
	//android ����
	if( http_session->m_Requestline.m_Host.Find(".wmzhe.com")!=-1 && 
		http_session->m_Requestline.m_URI.Find(".apk")!=-1 &&
		http_session->m_Requestline.m_Referer.Find("www.wmzhe.com/android/") != -1) 
	{
		return kWebDown;
	}
	return 0;
}

int software::Wmzhe::Process(const char *packet,int action)
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
int software::Wmzhe::WebSearch(const char *packet)
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
int software::Wmzhe::WebDown(const char *packet)
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
