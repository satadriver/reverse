#include "StdAfx.h"
#include "WebSiteChinaz.h"

website::Chinaz::Chinaz(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "վ��֮��";	
}

website::Chinaz::~Chinaz()
{
	
}

int website::Chinaz::IsChinaz(HttpSession *http_session)
{
	//��ѯվ��
	if((http_session->m_Requestline.m_Host.Find("seo.chinaz.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajaxseo.aspx")!=-1)) 
	{
		return kPcWebSearch;
	}
	return 0;
}

int website::Chinaz::Process(const char *packet,int action)
{
	//��ѯվ��
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	return 1;
}

//��ѯվ��
int website::Chinaz::PcWebSearch(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string siteurl = ms_->GetValueFromMapByKey("host",true);
	if (siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"servername=" + siteurl + "\r\n" + 	//��ַ
		"webname=" +	app_ + "\r\n" +		//��Դ
		"state=��ѯվ��\r\n";				//״̬ 
		WriteIndexFile();    
	return 0;
}
