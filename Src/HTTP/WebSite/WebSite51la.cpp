#include "StdAfx.h"
#include "website51la.h"

website::LA51::LA51(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "51la";	
}

website::LA51::~LA51()
{
	
}

int website::LA51::IsLA51(HttpSession *http_session)
{
	//用户注册
	if((http_session->m_Requestline.m_Host.Find("www.51.la")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg.asp")!=-1)) 
	{
		return kPcWebReg;
	}
	//用户登录
	if((http_session->m_Requestline.m_Host.Find("www.51.la")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.asp")!=-1)) 
	{
		return kPcWebLogin;
	}
	//用户添加站点
	if((http_session->m_Requestline.m_Host.Find("www.51.la")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/newsite.asp")!=-1)) 
	{
		return kPcWebAdd;
	}
	//用户修改
	if((http_session->m_Requestline.m_Host.Find("www.jobui.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/process/")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int website::LA51::Process(const char *packet,int action)
{
	if (action == kPcWebReg)
	{
		return PcWebReg(packet);
	}

	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}

	if (action == kPcWebAdd)
	{
		return PcWebAdd(packet);
	}
	return 1;
}

//用户注册
int website::LA51::PcWebReg(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string uname = ms_->GetValueFromMapByKey("uname",true);
	string pwd = ms_->GetValueFromMapByKey("pass1",true);
	string email = ms_->GetValueFromMapByKey("email",true);
	string sitename = ms_->GetValueFromMapByKey("sitename",true);
	string siteurl = ms_->GetValueFromMapByKey("siteurl",true);
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
         "state=用户注册\r\n";				//状态 
		WriteIndexFile();    
	return 1;
}

//用户登陆
int website::LA51::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("uname",true);
	string pwd = ms_->GetValueFromMapByKey("upass",true);
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
int website::LA51::PcWebAdd(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string sitename = ms_->GetValueFromMapByKey("sitename",true);
	string siteurl = ms_->GetValueFromMapByKey("siteurl",true);
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
