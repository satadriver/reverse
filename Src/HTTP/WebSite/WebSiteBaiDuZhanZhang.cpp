#include "StdAfx.h"
#include "WebSiteBaiDuZhanZhang.h"

website::BaiDuZhanZhang::BaiDuZhanZhang(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "�ٶ�վ��";	
}

website::BaiDuZhanZhang::~BaiDuZhanZhang()
{
	
}

int website::BaiDuZhanZhang::IsBaiDuZhanZhang(HttpSession *http_session)
{
	//�û����վ��
	if((http_session->m_Requestline.m_Host.Find("zhanzhang.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/site/add")!=-1)) 
	{
		return kPcWebAdd;
	}
	return 0;
}

int website::BaiDuZhanZhang::Process(const char *packet,int action)
{
	if (action == kPcWebAdd)
	{
		return PcWebAdd(packet);
	}
	return 1;
}

//�û����վ��
int website::BaiDuZhanZhang::PcWebAdd(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string siteurl = ms_->GetValueFromMapByKey("url",true);
	if (siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"servername=" + siteurl + "\r\n" +	//��ַ
		"webname=" +	app_ + "\r\n" +		//��Դ
		"state=���վ��\r\n";				//״̬ 
		WriteIndexFile();    
	return 0;
}
