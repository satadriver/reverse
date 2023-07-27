#include "StdAfx.h"
#include "WebSiteTencent.h"

website::Tencent::Tencent(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "��Ѷ����";	
}

website::Tencent::~Tencent()
{
	
}

int website::Tencent::IsTencent(HttpSession *http_session)
{
	//���վ��
	if((http_session->m_Requestline.m_Host.Find("ta.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/bind/verify")!=-1)) 
	{
		return kPcWebAdd;
	}
	return 0;
}

int website::Tencent::Process(const char *packet,int action)
{
	//���վ��
	if (action == kPcWebAdd)
	{
		return PcWebAdd(packet);
	}
	return 1;
}

//�û����վ��
int website::Tencent::PcWebAdd(const char *packet)
{
	//cookie����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	string qq = ms_->GetValueFromMapByKey("ptui_loginuin",true);
	//data����
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//վ������
	string sitename = ms_->GetValueFromMapByKey("siteName",true);
	//��ַ
	string siteurl = ms_->GetValueFromMapByKey("siteUrl",true);
	if (qq.empty() || sitename.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =	
		"oid=" + qq + "\r\n" + 				//QQ
		"userid=" + sitename + "\r\n" +		//վ������
		"servername=" + siteurl + "\r\n" + 	//��ַ
		"webname=" + app_ + "\r\n" +		//��Դ
		"state=���վ��\r\n";				//״̬ 
	WriteIndexFile();    
	return 1;
}
