#include "StdAfx.h"
#include "WebSiteAdmin88.h"

website::Admin88::Admin88(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "Admin88网站流量计数器";	
}

website::Admin88::~Admin88()
{
	
}

int website::Admin88::IsAdmin88(HttpSession *http_session)
{
	//用户注册
	if((http_session->m_Requestline.m_Host.Find("www1.admin88.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg.asp")!=-1)) 
	{
		return kPcWebReg;
	}
	//用户登录
	if((http_session->m_Requestline.m_Host.Find("www1.admin88.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/check.asp")!=-1)) 
	{
		return kPcWebLogin;
	}
	//用户修改
	if((http_session->m_Requestline.m_Host.Find("www1.admin88.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/edit.asp")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int website::Admin88::Process(const char *packet,int action)
{
	if (action == kPcWebReg)
	{
		return PcWebReg(packet);
	}

	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}

	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}

//用户注册
int website::Admin88::PcWebReg(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string uname = ms_->GetValueFromMapByKey("user",true);
	//密码
	string pwd = ms_->GetValueFromMapByKey("password1",true);
	string pwd2 = ms_->GetValueFromMapByKey("password2",true);
	if (strcmp(pwd.c_str(),pwd2.c_str()))
	{
		return 0;
	}
	//邮箱
	string email = ms_->GetValueFromMapByKey("mail",true);
	//站点名称
	string sitename = ms_->GetValueFromMapByKey("sitename",true);
	//网址
	string siteurl = ms_->GetValueFromMapByKey("url",true);
	if (uname.empty() || pwd.empty() || email.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" + uname + "\r\n" +			//用户名
		"pass=" + pwd + "\r\n" + 			//密码
		"userid=" + sitename + "\r\n" +		//站点名称
		"servername=" + siteurl + "\r\n" + 	//网址
		"appname=" + email + "\r\n" +		//邮箱
		"webname=" + app_ + "\r\n" +		//来源
         "state=用户注册\r\n";				//状态 
		WriteIndexFile();    
	return 1;
}

//用户登陆
int website::Admin88::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("Name",true);
	string pwd = ms_->GetValueFromMapByKey("passwd",true);
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

//修改站点
int website::Admin88::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string uname = ms_->GetValueFromMapByKey("user",true);
	//密码
	string oldpasswd = ms_->GetValueFromMapByKey("oldpasswd",true);
	string pwd = ms_->GetValueFromMapByKey("passwd",true);
	string pwd2 = ms_->GetValueFromMapByKey("passwd2",true);
	if (pwd.empty() && pwd2.empty())
	{
		pwd = oldpasswd;
	}
	//邮箱
	string email = ms_->GetValueFromMapByKey("mail",true);
	//站点名称
	string sitename = ms_->GetValueFromMapByKey("sitename",true);
	//网址
	string siteurl = ms_->GetValueFromMapByKey("url",true);
	if (uname.empty() || pwd.empty() || email.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" +		uname + "\r\n" +	//用户名
		"pass=" +		pwd + "\r\n" + 		//密码
		"userid=" +		sitename + "\r\n" +	//站点名称
		"servername=" + siteurl + "\r\n" + 	//网址
		"appname=" +	email + "\r\n" +	//邮箱
		"webname=" +	app_ + "\r\n" +		//来源
		"state=修改站点\r\n";				//状态 
	WriteIndexFile();    
	return 1;
}
