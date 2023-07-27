#include "StdAfx.h"
#include  "SoftwareXiaZaiBa.h"

software::XiaZaiBa::XiaZaiBa(HttpSession *http_session, const string &pro_name):Software(http_session,pro_name)
{
	app_ = "���ذ����վ";	
}

software::XiaZaiBa::~XiaZaiBa()
{
	
}

int software::XiaZaiBa::Is(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("zhannei.baidu.com")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/api/customsearch/")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("http://zhannei.baidu.com/")!=-1))
	{
		return kWebSearch;
	}
	if((http_session->m_Requestline.m_Host.Find("zhannei.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cse/search")!=-1)&&
		(http_session->m_Requestline.m_Referer.Find("http://www.xiazaiba.com/")!=-1))
	{
		return kWebSearch;
	}
	if((http_session->m_Requestline.m_Host.Find("zhannei.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/static/html")!=-1)&&
		(http_session->m_Requestline.m_Referer.Find("http://zhannei.baidu.com/")!=-1))
	{
		return kWebSearch;
	}
	//���� 
	if((http_session->m_Requestline.m_Host.Find("khit.cn")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/xiazai/")!=-1))
	{
		return kWebDown;
	}
	//���� (��ͨ����)
	if((http_session->m_Requestline.m_Host.Find(".cnzz.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/stat.htm")!=-1)&&
		(http_session->m_Requestline.m_Referer.Find("http://www.xiazaiba.com/html/")!=-1)) 
	{
		return kWebDown;
	}
	return 0;
}

int software::XiaZaiBa::Process(const char *packet,int action)
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
int software::XiaZaiBa::WebSearch(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UrlDecode(url);
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //�������
   softname_=ms_->GetValueFromMapByKey("q",true); 
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
int software::XiaZaiBa::WebDown(const char *packet)
{
   string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
   url = ms_->UTF8ToGBK(UrlDecode(url));
   ms_->Replace(url,"?","&");
   ms_->SplitStringToMap(url,"&","=");
   //�������
   if (url.find("/stat.htm")!=-1)
   {
	   softname_=ms_->GetValueFromMapByKey("t",true);
   }
   if (url.find("/xiazai/")!=-1)
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
