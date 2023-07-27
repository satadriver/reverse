#include "StdAfx.h"
#include  "SoftwarePconline.h"

software::Pconline::Pconline(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "̫ƽ������";	
}

software::Pconline::~Pconline()
{
	
}

int software::Pconline::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("ks.pconline.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/download.shtml")!=-1)) 
	{
		return kWebSearch;
	}
	//���� 
	if(((http_session->m_Requestline.m_Host.Find(".xzn56.com")!=-1)||(http_session->m_Requestline.m_Host.Find(".xzm56.com")!=-1))  && 
		(http_session->m_Requestline.m_URI.Find("/pconline")!=-1)) 
	{
		return kWebDown;
	}
	//���� 
	if((http_session->m_Requestline.m_Host.Find(".pconline.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/filedown")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::Pconline::Process(const char *packet,int action)
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
int software::Pconline::WebSearch(const char *packet)
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
int software::Pconline::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //�������
   if (url.rfind("/") != -1)
   {
		softname_= url.substr(url.rfind("/")+1);
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
