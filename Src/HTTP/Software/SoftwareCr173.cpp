#include "StdAfx.h"
#include  "SoftwareCr173.h"

software::Cr173::Cr173(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "�������԰";	
}

software::Cr173::~Cr173()
{
	
}

int software::Cr173::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("so.cr173.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/d/")!=-1)) 
	{
		return kWebSearch;
	}
	//���� (��������)
	if((http_session->m_Requestline.m_Host.Find(".xzqxzs.com")!=-1)  && 
		(http_session->m_Requestline.m_URI.Find("/download/")!=-1)) 
	{
		return kWebDown;
	}
	//���� (��ͨ����)
	if((http_session->m_Requestline.m_Host.Find(".mqego.com")!=-1) || 
		(http_session->m_Requestline.m_Host.Find(".fgtrj.com")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::Cr173::Process(const char *packet,int action)
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
int software::Cr173::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
 
   //�������
   softname_ = ms_->GetValueBetweenBeginEnd(url,"/search/d/","_pc_",true);
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
int software::Cr173::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //�������
   if (url.rfind("/") != -1)
   {
	   softname_ = url.substr(url.rfind("/")+1);
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
