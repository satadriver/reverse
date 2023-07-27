#include "StdAfx.h"
#include  "SoftwareZol.h"

software::Zol::Zol(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "Zol����";	
}

software::Zol::~Zol()
{
	
}

int software::Zol::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("xiazai.zol.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search")!=-1)) 
	{
		return kWebSearch;
	}
	//���� (���Ÿ�������)&&��ͨ����
	if((http_session->m_Requestline.m_Host.Find(".zol.com.cn")!=-1) && 
		(http_session->m_Requestline.m_Referer.Find("http://xiazai.zol.com.cn/detail/")!=-1)) 
	{
		return kWebDown;
	}
	//Android
	if((http_session->m_Requestline.m_Host.Find(".zol.com.cn")!=-1) && 
		(http_session->m_Requestline.m_Referer.Find("http://sj.zol.com.cn")!=-1)&& 
		(http_session->m_Requestline.m_Host.Find("down")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::Zol::Process(const char *packet,int action)
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
int software::Zol::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //�������
   softname_ = ms_->GetValueFromMapByKey("wd",true);
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
int software::Zol::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //�������
   if (url.find("/")!=-1)
   {
	   softname_=url.substr(url.rfind("/")+1);
   }
   if (softname_.empty())
   {
	   return 0;
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
