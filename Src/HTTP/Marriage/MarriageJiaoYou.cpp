#include "StdAfx.h"
#include "MarriageJiaoYou.h"

marriage::JiaoYouMarriage::JiaoYouMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "中国交友网";	
}

marriage::JiaoYouMarriage::~JiaoYouMarriage()
{
	
}

int marriage::JiaoYouMarriage::IsJiaoYouMarriage(HttpSession *http_session)
{
	//注册(用户名 密码)		m_Requestline.m_Action==HTTP_POST
	if((http_session->m_Requestline.m_Host.Find("www.jiaoyou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register.php?step=1")!=-1)) 
	{
		return kWebRegister;
	}	
	//注册(姓名 电话 地址 通讯号)
	if((http_session->m_Requestline.m_Host.Find("www.jiaoyou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register.php?step=6")!=-1)) 
	{
		return kWebRegister;
	}
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.jiaoyou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.php?act=login")!=-1)) 
	{
		return kWebLogin;
	}	
	//web修改资料(个人信息)
	if((http_session->m_Requestline.m_Host.Find("www.hongniang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/ajax_edit_contact")!=-1)) 
	{
		return kWebModify;
	}	
	return 0;
}

int marriage::JiaoYouMarriage::Process(const char *packet,int action)
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
int marriage::JiaoYouMarriage::WebRegister(const char *packet)
{
	//判断 uri
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	//用户名 密码
	if (uri.find("/register.php?step=1") != -1)
	{
		//数据部分
		string src_data(packet);
		if (src_data.find("user_name=") != string::npos)
		{
			src_data = src_data.substr(src_data.find("user_name="));
		}
		ms_->SplitStringToMap(src_data,'&','=');
		//用户名
		string username = ms_->GetValueFromMapByKey("user_name",true);
		//密码
		string password = ms_->GetValueFromMapByKey("userpass");
		string password2 = ms_->GetValueFromMapByKey("userpass2");
		if (password != password2)
		{
			return 0;
		}
		if (username.empty() || password.empty())
		{
			return 0;
		}
		output_text_ =
			"user=" + 		username + "\r\n" +			//20	[13]	//用户名 
			"pass=" + 		password + "\r\n" + 		//21	[14]	//密码
			"netmark=" +	app_ + "\r\n" + 			//59	[124]	//来源
			"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//操作平台
			"state=注册\r\n";							//64	[19]	//状态
		WriteIndexFile();
	}
	//注册 (姓名 电话 地址 通讯账号)
	if (uri.find("/register.php?step=6") != -1)
	{
		//数据部分
		string src_data(packet);
		if (src_data.rfind("real_name=") != string::npos )
		{
			src_data = src_data.substr(src_data.rfind("real_name="));
		}
		ms_->SplitStringToMap(src_data,'&','=');
		//姓名
		string realName = ms_->GetValueFromMapByKey("real_name",true);
		//电话
		string phone = ms_->GetValueFromMapByKey("p_tel");
		//地址
		string address = ms_->GetValueFromMapByKey("p_address",true);
		//通讯类型
		string contactType = "QQ/MSN/Yahoo通";
		//通讯账号
		string contact = ms_->GetValueFromMapByKey("p_contact",true);
		if (realName.empty())
		{
			return 0;
		}
		
		output_text_ =
			"mid=" + 	  	realName + "\r\n" + 		//56	[121]	//姓名	
			"oid=" + 	  	phone + "\r\n" + 			//54	[119]	//手机号 
			"msgtype=" + 	contactType + "\r\n" +		//51	[127]	//通讯类型
			"subject=" + 	contact + "\r\n" + 			//52	[46]    //通讯号
			"netmark=" +	app_ + "\r\n" + 			//59	[124]	//来源
			"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//操作平台
			"state=注册\r\n";							//64	[19]	//状态
		WriteIndexFile();
	}
	return 1;
}
//登录
int marriage::JiaoYouMarriage::WebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("user_name=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("user_name="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string username = ms_->GetValueFromMapByKey("user_name",true);
	//密码
	string password = ms_->GetValueFromMapByKey("userpass");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +			//20	[13]	//用户名 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//密码 	
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//操作平台
		"state=登录\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//web修改资料
int marriage::JiaoYouMarriage::WebModify(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	//联系方式
	if (uri.find("/user/ajax_edit_contact") != -1)
	{
		//数据部分
		string src_data(packet);
		if (src_data.find("mid=") != -1)
		{
			src_data = src_data.substr(src_data.find("mid="));
		}
		ms_->SplitStringToMap(src_data,'&','=');
		//用户ID
		string userId = ms_->GetValueFromMapByKey("mid");
		//QQ
		string QQ = ms_->GetValueFromMapByKey("qq");
		//微信
		string weixin = ms_->GetValueFromMapByKey("weixin");
		//邮箱
		string email = ms_->GetValueFromMapByKey("email");
		
		output_text_ =
			"userid=" +		userId + "\r\n" + 		//28    [92]    //用户ID
			"clientip=" + 	QQ + "\r\n" + 			//68	[147] 	//QQ
			"wifissid=" + 	weixin + "\r\n" + 		//69	[148] 	//微信
			"nick="	 + 	  	email + "\r\n" + 		//58	[123] 	//邮箱
			"netmark=" + app_ + "\r\n" + 			//59	[124]	//来源
			"clientmsg=" + platformWeb + "\r\n" +	//62	[126]	//操作平台
			"state=修改个人信息\r\n";				//64	[19]	//状态
		WriteIndexFile();
	}
	//个人信息
	if (uri.find("/user/ajax_edit_personal") != -1)
	{
		//数据部分
		string src_data(packet);
		if (src_data.find("mid=") != -1)
		{
			src_data = src_data.substr(src_data.find("mid="));
		}
		ms_->SplitStringToMap(src_data,'&','=');
		//用户ID
		string userId = ms_->GetValueFromMapByKey("mid");
		//昵称
		string nickname = ms_->GetValueFromMapByKey("nickname",true);
		//邮箱
		string email = ms_->GetValueFromMapByKey("email",true);
		
		output_text_ =
			"userid=" +		userId + "\r\n" + 		//28    [92]    //用户ID
			"msg=" + 	 	nickname + "\r\n" + 	//65	[104]   //昵称
			"nick="	 + 	  	email + "\r\n" + 		//58	[123] 	//邮箱
			"netmark=" + app_ + "\r\n" + 			//59	[124]	//来源
			"clientmsg=" + platformWeb + "\r\n" +	//62	[126]	//操作平台
			"state=修改个人信息\r\n";				//64	[19]	//状态
		WriteIndexFile();
	}
	return 1;
}
