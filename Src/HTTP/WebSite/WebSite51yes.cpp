#include "StdAfx.h"
#include "website51yes.h"

website::YES51::YES51(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "51yes";	
}

website::YES51::~YES51()
{
	
}

int website::YES51::IsYES51(HttpSession *http_session)
{
	//用户注册
	if((http_session->m_Requestline.m_Host.Find("countt.51yes.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg-2.aspx")!=-1)) 
	{
		return kPcWebReg;
	}
	//用户登录
	if((http_session->m_Requestline.m_Host.Find("countt.51yes.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.aspx")!=-1)) 
	{
		return kPcWebLogin;
	}
	return 0;
}

int website::YES51::Process(const char *packet,int action)
{
	if (action == kPcWebReg)
	{
		return PcWebReg(packet);
	}

	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}

	return 1;
}

//用户注册
int website::YES51::PcWebReg(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	ms_->SplitStringToMap(src_data,'&','=');
	string uname = ms_->GetValueFromMapByKey("txtusername",true);
	string pwd = ms_->GetValueFromMapByKey("txtpassword",true);
	string webname = ms_->GetValueFromMapByKey("txtwebname",true);
	string weburl = ms_->GetValueFromMapByKey("txtaddress",true);
	string email = ms_->GetValueFromMapByKey("txtemail",true);
	if (uname.empty() || pwd.empty() || webname.empty() || weburl.empty() || email.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" +		uname + "\r\n" +  //用户名
		"pass=" +		pwd + "\r\n" + 	  //密码
		"userid=" +		webname + "\r\n" +//站点名称
		"servername=" + weburl + "\r\n" + //网址
		"appname=" +	email + "\r\n" +  //邮箱
		"webname=" +	app_ + "\r\n" +   //来源
         "state=用户注册\r\n";            //状态 
		WriteIndexFile();    
	return 1;
}

//用户登陆
int website::YES51::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("LoginName",true);
	string pwd = ms_->GetValueFromMapByKey("LoginPassword",true);
	if (username.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" + username + "\r\n" + //用户名
		"pass=" + pwd + "\r\n" + 	  //密码
		"webname=" + app_ + "\r\n" +  //来源
		"state=用户登录\r\n";         //状态 
		WriteIndexFile();    
	return 1;
}
