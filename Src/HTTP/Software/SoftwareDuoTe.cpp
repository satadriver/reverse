#include "StdAfx.h"
#include  "SoftwareDuoTe.h"

software::DuoTe::DuoTe(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "2345�����ȫ(ԭ�������վ)";	
}

software::DuoTe::~DuoTe()
{
	
}

int software::DuoTe::Is(HttpSession *http_session)
{
	//���� pc���
	if(http_session->m_Requestline.m_Host.Find("so.duote.com")!=-1 && 
		http_session->m_Requestline.m_URI.Find("/cse/search")!=-1 ) 
	{
		return kWebSearch;
	}
	//���� ��׿apk	
	if(http_session->m_Requestline.m_Host.Find("www.duote.com")!=-1 && 
		http_session->m_Requestline.m_URI.Find("/searchPhone.php")!=-1 ) 
	{
		return kWebSearch;
	}
	//���� 
	if(http_session->m_Requestline.m_Referer.Find("www.duote.com")!=-1 && 
		http_session->m_Requestline.m_URI.Find(".exe")!=-1) 
	{
		return kWebDown;
	}
	//���� apk
	if(http_session->m_Requestline.m_Host.Find("app.2345.cn")!=-1 && 
	   http_session->m_Requestline.m_URI.Find(".apk")!=-1) 
	{
		return kApkDown;
	}
	return 0;
}

int software::DuoTe::Process(const char *packet,int action)
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
	//���� apk
	if (action == kApkDown)
	{
		return ApkDown(packet);
	}
	return 1;
}
//����
int software::DuoTe::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   ms_->Replace(url,"?","&");
   url = ms_->UrlDecode(url);
   ms_->SplitStringToMap(url,"&","=");

   if (url.find("/cse/search") != -1)
   {
	   //�������
		softname_ = ms_->GetValueFromMapByKey("q",true);
   }
   else if (url.find("/searchPhone.php") != -1)
   {
	   //�������
		softname_ = ms_->GetValueFromMapByKey("so",true);
   }
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
int software::DuoTe::WebDown(const char *packet)
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
//���� apk
int software::DuoTe::ApkDown(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	url = ms_->UrlDecode(url);
	//�������
	if (url.rfind("/") != -1)
	{
		softname_ = url.substr(url.rfind("/") + strlen("/"));
	}
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
