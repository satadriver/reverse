#include "StdAfx.h"
#include  "SoftwareGreentea.h"

software::Greentea::Greentea(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "�̲����԰";	
}

software::Greentea::~Greentea()
{
	
}

int software::Greentea::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("www.33lc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index.php")!=-1)) 
	{
		return kWebSearch;
	}
	//����
	if((http_session->m_Requestline.m_URI.Find("/small/") != -1 || (http_session->m_Requestline.m_URI.Find("/360mse/") != -1) ) && 
		(http_session->m_Requestline.m_Referer.Find("www.33lc.com/android")!=-1 || http_session->m_Requestline.m_Referer.Find("www.33lc.com/soft")!=-1))	
	{
		return kWebDown;
	}
	return 0;
}

int software::Greentea::Process(const char *packet,int action)
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
int software::Greentea::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //�������
   softname_ = ms_->GetValueFromMapByKey("keywords",true);
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
int software::Greentea::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //�������
   if (url.rfind("/") != -1)
   {
	   softname_=url.substr(url.rfind('/') + sizeof('/'));
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
