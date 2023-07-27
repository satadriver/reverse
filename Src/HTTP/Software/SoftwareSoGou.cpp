#include "StdAfx.h"
#include  "SoftwareSoGou.h"

software::SoGou::SoGou(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "�ѹ�����";	
}

software::SoGou::~SoGou()
{
	
}

int software::SoGou::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("xiazai.sogou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search")!=-1)) 
	{
		return kWebSearch;
	}
	//���� 
	if((http_session->m_Requestline.m_Host.Find("xiazai.sogou.com")!=-1)&& 
		(http_session->m_Requestline.m_URI.Find("/comm/redir")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::SoGou::Process(const char *packet,int action)
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
int software::SoGou::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //�������
   softname_ = ms_->GetValueFromMapByKey("query",true);
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
int software::SoGou::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //�������
   softname_ = ms_->GetValueFromMapByKey("filename",true);
   if (softname_.empty())
   {
	   return 0;
   }
   //�������
   softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
   softurl_= ms_->UTF8ToGBK(UrlDecode(softurl_));
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
