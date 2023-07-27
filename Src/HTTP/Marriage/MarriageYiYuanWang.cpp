#include "StdAfx.h"
#include "MarriageYiYuanWang.h"

marriage::YiYuanWangMarriage::YiYuanWangMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "易缘网";	
}

marriage::YiYuanWangMarriage::~YiYuanWangMarriage()
{
	
}

int marriage::YiYuanWangMarriage::IsYiYuanWangMarriage(HttpSession *http_session)
{
	//网页注册
	if((http_session->m_Requestline.m_Host.Find("www.99yiyuan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index.php?c=passport")!=-1)&&
		(http_session->m_Requestline.m_Referer.Find("http://www.99yiyuan.com/passport/reg.html")!=-1)) 
	{
		return kPcWebRegister;
	}
	//网页登录
	if((http_session->m_Requestline.m_Host.Find("www.99yiyuan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index.php?c=passport")!=-1)&&
		(http_session->m_Requestline.m_Referer.Find("http://www.99yiyuan.com/index.php")!=-1)) 
	{
		return kPcWebLogin;
	}
	return 0;
}

int marriage::YiYuanWangMarriage::Process(const char *packet,int action)
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

	return 1;
}
//网页注册
int marriage::YiYuanWangMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	//手机号
	string phone = ms_->GetValueFromMapByKey("mobile",true);
	//密码
	string password = ms_->GetValueFromMapByKey("password",true);
	//用户名
	string username = ms_->GetValueFromMapByKey("username",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("gender",true)=="1"?"男":"女";
	//出生日期
	string birhday = ms_->GetValueFromMapByKey("ageyear",true) + "-" +
		             ms_->GetValueFromMapByKey("agemonth",true) + "-"+
					 ms_->GetValueFromMapByKey("ageday",true);
	if (email.empty())
	{
		output_text_ =
		    "oid=" +phone + "\r\n" +             //手机号
            "pass=" + password + "\r\n" +        //密码
			"user=" +username + "\r\n" +         //用户名
			"sex=" +sex + "\r\n" +               //性别
			"onick=" + birhday + "\r\n" +        //出生日期
 			"netmark=" + app_ + "\r\n" + 	     //来源
			"clientmsg=" + platformWeb + "\r\n" +//操作平台
			"state=注册\r\n";			         //状态
		WriteIndexFile();
	}
	else
	{
		output_text_ =
			"nick=" + email + "\r\n" +           //手机号
            "pass=" + password + "\r\n" +        //密码
			"user=" +username + "\r\n" +         //用户名
			"sex=" +sex + "\r\n" +               //性别
			"onick=" + birhday + "\r\n" +        //出生日期
			"netmark=" + app_ + "\r\n" + 	     //来源
			"clientmsg=" + platformWeb + "\r\n" +//操作平台
			"state=注册\r\n";			         //状态
		WriteIndexFile();
	}
	return 1;
}

//web登录
int marriage::YiYuanWangMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	string pwd = ms_->GetValueFromMapByKey("password",true);
	string loginname = ms_->GetValueFromMapByKey("loginname",true);
	if (mobile.empty())
	{
		output_text_ =
			"user=" + loginname + "\r\n" +         //用户名
			"pass=" + pwd + "\r\n" +               //密码
			"netmark=" + app_ + "\r\n" + 		   //来源
			"clientmsg=" + platformWeb + "\r\n" +  //操作平台
			"state=登录\r\n";			           //状态
       	WriteIndexFile();
	}
	else
	{
		output_text_ =
			"user=" + mobile + "\r\n" +           //用户名
			"pass=" + pwd + "\r\n" +               //密码
			"netmark=" + app_ + "\r\n" + 		   //来源
			"clientmsg=" + platformWeb + "\r\n" +  //操作平台
			"state=登录\r\n";			           //状态
       	WriteIndexFile();
	}
	return 1;
}
