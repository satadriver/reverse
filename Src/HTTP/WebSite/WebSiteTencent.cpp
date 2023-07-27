#include "StdAfx.h"
#include "WebSiteTencent.h"

website::Tencent::Tencent(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "腾讯分析";	
}

website::Tencent::~Tencent()
{
	
}

int website::Tencent::IsTencent(HttpSession *http_session)
{
	//添加站点
	if((http_session->m_Requestline.m_Host.Find("ta.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/bind/verify")!=-1)) 
	{
		return kPcWebAdd;
	}
	return 0;
}

int website::Tencent::Process(const char *packet,int action)
{
	//添加站点
	if (action == kPcWebAdd)
	{
		return PcWebAdd(packet);
	}
	return 1;
}

//用户添加站点
int website::Tencent::PcWebAdd(const char *packet)
{
	//cookie部分
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	string qq = ms_->GetValueFromMapByKey("ptui_loginuin",true);
	//data部分
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//站点名称
	string sitename = ms_->GetValueFromMapByKey("siteName",true);
	//网址
	string siteurl = ms_->GetValueFromMapByKey("siteUrl",true);
	if (qq.empty() || sitename.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =	
		"oid=" + qq + "\r\n" + 				//QQ
		"userid=" + sitename + "\r\n" +		//站点名称
		"servername=" + siteurl + "\r\n" + 	//网址
		"webname=" + app_ + "\r\n" +		//来源
		"state=添加站点\r\n";				//状态 
	WriteIndexFile();    
	return 1;
}
