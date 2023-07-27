#include "StdAfx.h"
#include  "SoftwareHuacolor.h"

software::Huacolor::Huacolor(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "�������԰";	
}

software::Huacolor::~Huacolor()
{
	
}

int software::Huacolor::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find(".huacolor.")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/e/search/index.php")!=-1)) 
	{
		return kWebSearch;
	}
	//���� (��ͨ����)
	if((http_session->m_Requestline.m_Host.Find(".huacolor.")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/down/")!=-1)) 
	{
		return kWebDown;
	}
	
	return 0;
}

int software::Huacolor::Process(const char *packet,int action)
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
int software::Huacolor::WebSearch(const char *packet)
{
  string src_data(packet);
   if (src_data.rfind("\r\n") != -1)
   {
	   src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
   }
   src_data = ms_->UrlDecode(src_data);
   ms_->SplitStringToMap(src_data,"&","=");
   //�������
   softname_= ms_->GetValueFromMapByKey("keyboard",true);
   output_text_ =				
	   "mid=" + 	  	softname_ + "\r\n" + 	//�������	
	   "netmark=" + 	app_ + "\r\n" + 		//��Դ 	
	   "clientmsg=" + 	platformWeb + "\r\n" + 	//����ƽ̨		
	   "state=����\r\n";						//״̬ 				
   WriteIndexFile();  
   return 0;
}
//����
int software::Huacolor::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   //�������
   if (url.find("/down/") != -1)
   {
	   softname_ = url.substr(url.rfind("/") + strlen("/"));
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
