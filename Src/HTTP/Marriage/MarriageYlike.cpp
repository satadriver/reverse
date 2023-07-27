#include "StdAfx.h"
#include "MarriageYlike.h"

marriage::YlikeMarriage::YlikeMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "缘来客";	
}

marriage::YlikeMarriage::~YlikeMarriage()
{
	
}

int marriage::YlikeMarriage::IsYlikeMarriage(HttpSession *http_session)
{
	//注册(用户名 密码)	
	if((http_session->m_Requestline.m_Host.Find("www.ylike.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ReglSave.do")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("www.ylike.com/Reg_Base.do") != -1)) 
	{
		return kWebRegister;
	}	
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.ylike.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ReglSave.do")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("www.ylike.com/UserLogin.do") != -1)) 
	{
		return kWebLogin;
	}	
	//web修改资料
	if((http_session->m_Requestline.m_Host.Find("www.ylike.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/save.do")!=-1)) 
	{
		return kWebModify;
	}	
	return 0;
}

int marriage::YlikeMarriage::Process(const char *packet,int action)
{
	//注册
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//登录
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//web修改资料
	if (action == kWebModify)
	{
		return WebModify(packet);
	}
	return 1;
}
//注册
int marriage::YlikeMarriage::WebRegister(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.find("choseType=") != -1)
	{
		src_data = src_data.substr(src_data.find("choseType="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
    //用户动作
	string action = ms_->GetValueFromMapByKey("action");
	//用户名
	string username = ms_->GetValueFromMapByKey("User_Name",true);
	//密码
	string password = ms_->GetValueFromMapByKey("User_Password");
	string passwordconfirm = ms_->GetValueFromMapByKey("User_Password2");
	if (password != passwordconfirm)
	{
		return 0;
	}
	//昵称
	string nickname = ms_->GetValueFromMapByKey("Name",true); 
	//性别
	string sex = ms_->GetValueFromMapByKey("Sex",true);
	//出生日期
	string birthday = ms_->GetValueFromMapByKey("Birthday");
	//QQ
	string QQ = ms_->GetValueFromMapByKey("QQ");
	//邮箱
	string email = ms_->GetValueFromMapByKey("Mail",true);
	//地址
	string address = ms_->GetValueFromMapByKey("Canton",true) +
					 ms_->GetValueFromMapByKey("City",true) +
					 ms_->GetValueFromMapByKey("Town",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	
	output_text_ =
		"user=" + 		username + "\r\n" +		//20	[13]	//用户名 
		"pass=" + 		password + "\r\n" + 	//21	[14]	//密码 
		"msg=" + 	  	nickname + "\r\n" + 	//63	[128] 	//昵称 
		"sex=" + 	  	sex + "\r\n" + 			//60	[125]	//性别
		"onick=" + 	  	birthday + "\r\n" + 	//55	[120]	//出生日期
		"clientip=" + 	QQ + "\r\n" + 			//68	[147] 	//QQ
		"nick="	 + 	  	email + "\r\n" + 		//58	[123] 	//邮箱
		"islogin=" + 	address + "\r\n" + 		//53	[118]   //地址
		"netmark=" +	app_ + "\r\n" + 		//59	[124]	//来源
		"clientmsg=" +	platformWeb + "\r\n" +	//62	[126]	//操作平台
		"state=注册\r\n";						//64	[19]	//状态
		WriteIndexFile();
	return 1;
}
//登录
int marriage::YlikeMarriage::WebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.find("User_Name=") != -1)
	{
		src_data = src_data.substr(src_data.find("User_Name="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
    //用户动作
	string action = ms_->GetValueFromMapByKey("action");
	//用户名
	string username = ms_->GetValueFromMapByKey("User_Name",true);
	//密码
	string password = ms_->GetValueFromMapByKey("User_Password");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +			//20	[13]	//用户名 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//密码 	
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//来源
		"clientmsg=" +	platformWeb + "\r\n" +		//62	[126]	//操作平台
		"state=登录\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//web修改资料
int marriage::YlikeMarriage::WebModify(const char *packet)
{
	//从cookie中获取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	if (cookie.find("Sky=") != string::npos)
	{
		cookie = cookie.substr(cookie.find("Sky=") + strlen("Sky="));
	}
	ms_->SplitStringToMap(cookie,'&','=');
	//用户ID
	string userid = ms_->GetValueFromMapByKey("User_ID");
	//性别 
	string sex = ms_->GetValueFromMapByKey("Sex");
	//数据部分
	string src_data(packet);
	if (src_data.find("Canton=") != -1)
	{
		src_data = src_data.substr(src_data.find("Canton="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//出生日期
	string birthday = ms_->GetValueFromMapByKey("Birthday");
	//手机号
	string mobile = ms_->GetValueFromMapByKey("tel");
	//QQ
	string qq = ms_->GetValueFromMapByKey("QQ");
	//微信
	string weixin = ms_->GetValueFromMapByKey("MSN");
	//邮箱
	string email = ms_->GetValueFromMapByKey("Mail",true);
	//地址
	string address = ms_->GetValueFromMapByKey("Canton",true) +
					 ms_->GetValueFromMapByKey("City",true) +
					 ms_->GetValueFromMapByKey("Town",true);
	output_text_ =
		"userid=" +		userid + "\r\n" + 		//28    [92]    //用户ID
		"sex=" + 	  	sex + "\r\n" +			//60	[125]	//性别
		"onick=" + 	  	birthday + "\r\n" + 	//55	[120]	//出生日期
		"oid=" + 	  	mobile + "\r\n" + 		//54	[119]	//手机号 
		"clientip=" + 	qq + "\r\n" + 			//68	[147] 	//QQ
		"wifissid=" + 	weixin + "\r\n" + 		//69	[148] 	//微信
		"nick="	 + 		email + "\r\n" + 		//58	[123] 	//邮箱
		"islogin=" +	address + "\r\n" + 		//53	[118]   //地址
		"netmark=" +	app_ + "\r\n" + 		//59	[124]	//来源
		"clientmsg=" +	platformWeb + "\r\n" +	//62	[126]	//操作平台
		"state=修改个人信息\r\n";				//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
