#include "StdAfx.h"
#include  "SoftwareHao123.h"

software::Hao123::Hao123(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "hao123�������";	
}

software::Hao123::~Hao123()
{
	
}

int software::Hao123::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("soft.hao123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/s.php")!=-1)) 
	{
		return kWebSearch;
	}
	//����
	if((http_session->m_Requestline.m_Host.Find("softdown1.hao123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hao123-soft-online-bcs/")!=-1)) 
	{
		return kWebDown;
	}
	//���� (������ַ)
	if((http_session->m_Requestline.m_Host.Find("www.hao123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("h.gif")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("title") != -1)) 
	{
		return kWebDownAddress;
	}
	//���� (������ַ 2)
	if((http_session->m_Requestline.m_Host.Find("hcl.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("h.gif")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("title") != -1)) 
	{
		return kWebDownAddress;
	}
	return 0;
}

int software::Hao123::Process(const char *packet,int action)
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
	//���� (������ַ)
	if (action == kWebDownAddress)
	{
		return WebDownAddress(packet);
	}
	return 1;
}
//����
int software::Hao123::WebSearch(const char *packet)
{
   string src_data(packet);
   if (src_data.rfind("\r\n") != -1)
   {
	   src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
   }
   src_data = ms_->UrlDecode(src_data);
   ms_->SplitStringToMap(src_data,"&","=");
   //�������
   softname_ = ms_->GetValueFromMapByKey("key",true);
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
int software::Hao123::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //�������
   if (url.find("/soft/") != -1)
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
//���� (������ַ)
int software::Hao123::WebDownAddress(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	if (url.find("?") != string::npos)
	{
		url = url.substr(url.find("?"));
	}
	url = ms_->UrlDecode(url);
    ms_->SplitStringToMap(url,"&","=");
	//�������
	softname_ = ms_->GetValueFromMapByKey("title",true);
	//���ص�ַ
	softdownurl_ = ms_->GetValueFromMapByKey("objurl",true);
	//�������
	softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	softurl_ = ms_->UrlDecode(softurl_);

	if (softname_.empty())
	{
		return 0;
	}
	output_text_ = 
		"mid=" + 	  	softname_ + "\r\n" + 	//�������	
		"fileurl=" +    softurl_ + "\r\n" + 	//�������
		"clientip=" + 	softdownurl_ + "\r\n" + //���ص�ַ
		"netmark=" + 	app_ + "\r\n" + 		//��Դ 	
		"clientmsg=" + 	platformWeb + "\r\n" + 	//����ƽ̨		
		"state=����\r\n";		     			//״̬ 
	WriteIndexFile();
	return 0;
}
