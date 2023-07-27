#include "StdAfx.h"
#include  "SoftwareSina.h"

software::Sina::Sina(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "��������";	
}

software::Sina::~Sina()
{
	
}

int software::Sina::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("down.tech.sina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/download/search.php")!=-1)) 
	{
		return kWebSearch;
	}
	//����
	if((http_session->m_Requestline.m_Host.Find("tech.down.sina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("Expires")!=-1)) 
	{
		return kWebDown;
	}
	//���
	if((http_session->m_Requestline.m_Host.Find("sina.wrating.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/a.gif")!=-1)) 
	{
		return kWrating;
	}
	return 0;
}

int software::Sina::Process(const char *packet,int action)
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
	//���
	if (action == kWrating)
	{
		return Wrating(packet);
	}
	return 1;
}
//����
int software::Sina::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //�������
   softname_ = ms_->GetValueFromMapByKey("f_name",true);
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
int software::Sina::WebDown(const char *packet)
{ 
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   url = url.substr(0,url.rfind("?"));
   //�������
   softname_ = url.substr(url.rfind("/") + strlen("/"));
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
//���
int software::Sina::Wrating(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	if (url.find("?") != string::npos)
	{
		url = url.substr(url.find("?"));
	}
	url = ms_->UrlDecode(url);
    ms_->SplitStringToMap(url,"&","=");
	//�������
	softname_ = ms_->GetValueFromMapByKey("t",true);
	//�������
	softurl_ = ms_->GetValueFromMapByKey("b",true);
	if (softname_.empty())
	{
		return 0;
	}
	output_text_ = 
		"mid=" + 	  	softname_ + "\r\n" + 	//�������	
		"fileurl=" +     softurl_ + "\r\n" + 	//�������
		"netmark=" + 	app_ + "\r\n" + 		//��Դ 	
		"clientmsg=" + 	platformWeb + "\r\n" + 	//����ƽ̨		
	    "state=���\r\n";		     			//״̬ 
	WriteIndexFile();
	return 0;
}