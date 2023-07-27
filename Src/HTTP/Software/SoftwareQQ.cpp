#include "StdAfx.h"
#include "SoftwareQQ.h"

software::QQ::QQ(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "��Ѷ��������";	
}

software::QQ::~QQ()
{
	
}

int software::QQ::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("s.pcmgr.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/tapi/web/searchcgi.php?type=search")!=-1)) 
	{
		return kWebSearch;
	}
	//���� (��������)
	if((http_session->m_Requestline.m_Host.Find("dlied6.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/invc/xfspeed/")!=-1)) 
	{
		return kWebDown;
	}
	//���� (��ͨ����)
	if((http_session->m_Requestline.m_Host.Find("dl.softmgr.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/original/")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::QQ::Process(const char *packet,int action)
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
int software::QQ::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //��������
   softname_ = ms_->GetValueFromMapByKey("keyword",true);
   if (softname_.empty())
   {
	   return 0;
   }
   output_text_ =				
	   "mid=" + 	  	softname_ + "\r\n" + 	//��������	
	   "netmark=" + 	app_ + "\r\n" + 		//��Դ 	
	   "clientmsg=" + 	platformWeb + "\r\n" + 	//����ƽ̨		
	   "state=����\r\n";						//״̬ 				
   WriteIndexFile();  
   return 0;
}
//����
int software::QQ::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //��������
   if (url.find("/invc/xfspeed/") != -1)
   {
	   softname_ = url.substr(url.rfind("/") + strlen("/"));
   }
   if (url.find("/original/") != -1)
   {
	   softname_ = url.substr(url.rfind("/") + strlen("/"));
   }
   //��������
   softurl_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);
   if (softname_.empty())
   {
	   return 0;
   }
   output_text_ =				
	   "mid=" + 	  	softname_ + "\r\n" + 	//��������	
	   "fileurl=" +     softurl_ + "\r\n" + 	//��������
	   "netmark=" + 	app_ + "\r\n" + 		//��Դ 	
	   "clientmsg=" + 	platformWeb + "\r\n" + 	//����ƽ̨		
	   "state=����\r\n";						//״̬ 
   WriteIndexFile();  
   return 0;
}