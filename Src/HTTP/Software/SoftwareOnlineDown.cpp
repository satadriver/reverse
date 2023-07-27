#include "StdAfx.h"
#include  "SoftwareOnlineDown.h"

software::OnlineDown::OnlineDown(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "�������԰";	
}

software::OnlineDown::~OnlineDown()
{
	
}

int software::OnlineDown::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("search.onlinedown.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search_list.php")!=-1)) 
	{
		return kWebSearch;
	}
	//���� (��������)
	if(((http_session->m_Requestline.m_Host.Find("dl.apxlx.com")!=-1) || (http_session->m_Requestline.m_Host.Find("cl2.cjsdxz.com") != -1)) && 
		(http_session->m_Requestline.m_URI.Find("/download/")!=-1)) 
	{
		return kWebDown;
	}
	//���� (��ͨ����)
	if((http_session->m_Requestline.m_Host.Find(".onlinedown.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/down/")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::OnlineDown::Process(const char *packet,int action)
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
int software::OnlineDown::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //�������
   softname_ = ms_->GetValueFromMapByKey("searchname",true);
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
int software::OnlineDown::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //�������
   if (url.find("/download/") != -1)
   {
	   softname_ = url.substr(url.find("/download/") + strlen("/download/"));
   }
   if (url.find("/down/") != -1)
   {
	   softname_ = url.substr(url.find("/down/") + strlen("/down/"));
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
