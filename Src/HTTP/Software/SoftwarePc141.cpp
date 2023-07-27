#include "StdAfx.h"
#include  "SoftwarePc141.h"

software::Pc141::Pc141(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "PC�������վ";	
}

software::Pc141::~Pc141()
{
	
}

int software::Pc141::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("www.pc141.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index.php")!=-1)) 
	{
		return kWebSearch;
	}
	//����
	if((http_session->m_Requestline.m_Host.Find(".url.xaskm.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/down/")!=-1)) 
	{
		return kWebDown;
	}
	if((http_session->m_Requestline.m_Host.Find("down.pc141.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/down/")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::Pc141::Process(const char *packet,int action)
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
int software::Pc141::WebSearch(const char *packet)
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
int software::Pc141::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //�������
   if (url.find("/down/")!=-1)
   {
		softname_=url.substr(url.rfind("/")+1);
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
