#include "StdAfx.h"
#include "MarriageTaoNan.h"

marriage::TaoNanMarriage::TaoNanMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "淘男网";	
}

marriage::TaoNanMarriage::~TaoNanMarriage()
{
	
}

int marriage::TaoNanMarriage::IsTaoNanMarriage(HttpSession *http_session)
{
	//web淘男网 (修改资料 登录 QQ注册)
	if((http_session->m_Requestline.m_Host.Find("www.taonanw.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index.php")!=-1)) 
	{
		return kWebTaoNan;
	}
	return 0;
}

int marriage::TaoNanMarriage::Process(const char *packet,int action)
{
	//web淘男网 (修改资料 登录 QQ注册)
	if (action == kWebTaoNan)
	{
		return WebTaoNan(packet);
	}
	return 1;
}

//web淘男网 (修改资料 登录 QQ注册)
int marriage::TaoNanMarriage::WebTaoNan(const char *packet)
{
    //数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//动作
	string action = ms_->GetValueFromMapByKey("action",true);
	//login_with_qq (使用QQ登录时，重置的登录邮箱 密码)
	if (action == "restore" || action == "simple_reg")
	{
		//邮箱
		string email = ms_->GetValueFromMapByKey("email");
		//密码 
		string password = ms_->GetValueFromMapByKey("password");
		string confirm_password = ms_->GetValueFromMapByKey("confirm_password");
		if (password != confirm_password)
		{
			return 0;
		}
		//昵称 
		string nickName = ms_->GetValueFromMapByKey("netname",true);
		//出生日期
		string birthDate = ms_->GetValueFromMapByKey("birthday_year") + "-" +
						   ms_->GetValueFromMapByKey("birthday_month") + "-" +
						   ms_->GetValueFromMapByKey("birthday_day");
		output_text_ =
			"nick="	 + 	  	email + "\r\n" + 			//58	[123] 	//邮箱
			"pass=" + 		password + "\r\n" + 		//21	[14]	//密码 	
			"msg=" + 	 	nickName + "\r\n" + 		//65	[104]   //昵称
			"netmark=" +	app_ + "\r\n" + 			//59	[124]	//来源
			"clientmsg=" +	platformWeb + "\r\n" +		//62	[126]	//操作平台
			"state=新建个人信息\r\n";					//64	[19]	//状态
		WriteIndexFile();
	}
	//login (普通登录)
	if (action == "do_login")
	{
		//用户名
		string username = ms_->GetValueFromMapByKey("username",true);
		//密码
		string password = ms_->GetValueFromMapByKey("password");
		if (username.empty() || password.empty())
		{
			return 0;
		}
		output_text_ =
			"user=" + username + "\r\n" +				//20	[13]	//用户名 
			"pass=" + password + "\r\n" + 				//21	[14]	//密码 	
			"netmark=" + app_ + "\r\n" + 				//59	[124]	//来源
			"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//操作平台
			"state=登录\r\n";							//64	[19]	//状态
		WriteIndexFile();
	}
	//profile_edit (个人资料修改)
	if (action == "profile_edit")
	{
		//用户ID
		string userId = ms_->GetValueFromMapByKey("usr_id");
		//昵称 
		string nickname = ms_->GetValueFromMapByKey("netname",true);
		//出生日期 
		string birthdate = ms_->GetValueFromMapByKey("birthday");
		//手机号
		string phonenumber = ms_->GetValueFromMapByKey("phone_number"); 
		//QQ
		string qqContact = ms_->GetValueFromMapByKey("qq");
		//WeiChat
		string WeiChat = ms_->GetValueFromMapByKey("weixin");

		output_text_ =
			"userid=" +		userId + "\r\n" + 			//28    [92]    //用户ID
			"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //昵称
			"onick=" + 	  	birthdate + "\r\n" + 		//55	[120]	//出生日期 
			"oid=" + 	  	phonenumber + "\r\n" + 		//54	[119]	//手机号 
			"clientip=" + 	qqContact + "\r\n" + 		//68	[147] 	//QQ
			"wifissid=" + 	WeiChat + "\r\n" + 			//69	[148] 	//微信
			"netmark=" + app_ + "\r\n" + 				//59	[124]	//来源
			"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//操作平台
			"state=修改个人信息\r\n";					//64	[19]	//状态
		WriteIndexFile();
	}
	return 1;
}
