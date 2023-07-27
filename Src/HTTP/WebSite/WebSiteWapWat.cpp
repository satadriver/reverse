#include "StdAfx.h"
#include "WebSiteWapWat.h"

website::WapWat::WapWat(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "瓦特网";	
}

website::WapWat::~WapWat()
{
	
}

int website::WapWat::IsWapWat(HttpSession *http_session)
{
	//用户注册
	if((http_session->m_Requestline.m_Host.Find("www.wapwat.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/Register.asp")!=-1)) 
	{
		return kPcWebReg;
	}
	//用户登录
	if((http_session->m_Requestline.m_Host.Find("www.wapwat.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/Login.asp")!=-1)) 
	{
		return kPcWebLogin;
	}
	//添加站点
	if((http_session->m_Requestline.m_Host.Find("www.wapwat.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/regsite.asp")!=-1)) 
	{
		return kPcWebAdd;
	}
	return 0;
}

int website::WapWat::Process(const char *packet,int action)
{
	//用户注册
	if (action == kPcWebReg)
	{
		return PcWebReg(packet);
	}
	//用户登录
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//添加站点
	if (action == kPcWebAdd)
	{
		return PcWebAdd(packet);
	}
	return 1;
}

//用户注册
int website::WapWat::PcWebReg(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string uname = ms_->GetValueFromMapByKey("UID",true);
	//密码
	string pwd = ms_->GetValueFromMapByKey("PWD1",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	//QQ
	string qq = ms_->GetValueFromMapByKey("qq",true);
	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	if (mobile.length() == 0)
	{
		mobile = "null";
	}
	if (uname.empty() || pwd.empty() || email.empty() || qq.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" + uname + "\r\n" +		//用户名
		"pass=" + pwd + "\r\n" + 		//密码
		"appname=" + email + "\r\n" +	//邮箱
		"oid=" + qq + "\r\n" + 			//QQ	
		"mid=" + mobile + "\r\n" + 		//手机号
		"webname=" + app_ + "\r\n" +	//来源
         "state=用户注册\r\n";			//状态 
		WriteIndexFile();    
	return 1;
}

//用户登陆
int website::WapWat::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string username = ms_->GetValueFromMapByKey("UID",true);
	//密码
	string pwd = ms_->GetValueFromMapByKey("PWD",true);
	if (username.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" + username + "\r\n" +	//用户名
		"pass=" + pwd + "\r\n" + 		//密码
		"webname=" + app_ + "\r\n" +	//来源
		"state=用户登录\r\n";			//状态 
	WriteIndexFile();    
	return 1;
}

//用户添加站点
int website::WapWat::PcWebAdd(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string sitename = ms_->GetValueFromMapByKey("SiteTitle",true);
	string siteurl = ms_->GetValueFromMapByKey("SiteUrl",true);
	if (sitename.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"userid=" +		sitename + "\r\n" +	//站点名称
		"servername=" + siteurl + "\r\n" + 	//网址
		"webname=" +	app_ + "\r\n" +		//来源
		"state=添加站点\r\n";				//状态 
		WriteIndexFile();    
	return 1;
}
