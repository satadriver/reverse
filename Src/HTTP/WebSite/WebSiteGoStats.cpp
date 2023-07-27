#include "StdAfx.h"
#include "websitegostats.h"

website::GoStats::GoStats(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "gostats";	
}

website::GoStats::~GoStats()
{
	
}

int website::GoStats::IsGoStats(HttpSession *http_session)
{
	//用户注册
	if((http_session->m_Requestline.m_Host.Find("gostats.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/signup")!=-1)) 
	{
		return kPcWebReg;
	}
	//用户登录
	if((http_session->m_Requestline.m_Host.Find("gostats.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.xml")!=-1)) 
	{
		return kPcWebLogin;
	}
	//用户添加站点
	if((http_session->m_Requestline.m_Host.Find("gostats.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/add_site")!=-1)) 
	{
		return kPcWebAdd;
	}
	return 0;
}

int website::GoStats::Process(const char *packet,int action)
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
int website::GoStats::PcWebReg(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("username",true);
	string pwd = ms_->GetValueFromMapByKey("passwd1",true);
	string realname = ms_->GetValueFromMapByKey("realname",true);
	string mail = ms_->GetValueFromMapByKey("mail",true);
	string sitename = ms_->GetValueFromMapByKey("name",true);
	string siteurl = ms_->GetValueFromMapByKey("url",true);
	if (username.empty() || pwd.empty() || realname.empty() || mail.empty() || sitename.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" +		username + "\r\n" + //用户名
		"pass=" +		pwd + "\r\n" + 		//密码
		"msgtype=" +	realname + "\r\n"+	//真实姓名
		"userid=" +		sitename + "\r\n" +	//站点名称
		"servername=" + siteurl + "\r\n" +  //网址
		"appname=" +	mail + "\r\n" +		//邮箱
		"webname=" +	app_ + "\r\n" +		//来源
         "state=用户注册\r\n";				//状态 
		WriteIndexFile();    
	return 1;
}

//用户登陆
int website::GoStats::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("username",true);
	string pwd = ms_->GetValueFromMapByKey("password",true);
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
int website::GoStats::PcWebAdd(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string sitename = ms_->GetValueFromMapByKey("name",true);
	string siteurl = ms_->GetValueFromMapByKey("url",true);
	if (sitename.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"userid=" +		sitename + "\r\n" +	//站点名称
		"servername=" + siteurl + "\r\n" +  //网址
		"webname=" +	app_ + "\r\n" +		//来源
		"state=添加站点\r\n";				//状态 
		WriteIndexFile();    
	return 1;
}





