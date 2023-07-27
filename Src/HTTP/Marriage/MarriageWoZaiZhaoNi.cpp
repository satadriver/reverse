#include "StdAfx.h"
#include "MarriageWoZaiZhaoNi.h"

marriage::WoZaiZhaoNiMarriage::WoZaiZhaoNiMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "我在找你网";	
}

marriage::WoZaiZhaoNiMarriage::~WoZaiZhaoNiMarriage()
{
	
}

int marriage::WoZaiZhaoNiMarriage::IsWoZaiZhaoNiMarriage(HttpSession *http_session)
{
	//注册	
	if((http_session->m_Requestline.m_Host.Find("www.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/user/register/")!=-1)) 
	{
		return kWebRegister;
	}	
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/user/login/")!=-1)) 
	{
		return kWebLogin;
	}	
	//web修改资料
	if((http_session->m_Requestline.m_Host.Find("www.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/user/upMember")!=-1)) 
	{
		return kWebModify;
	}	
	//android注册
	if((http_session->m_Requestline.m_Host.Find("mi.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v150/index/register/")!=-1)) 
	{
		return kAndroidRegister;
	}
	//android登录
	if((http_session->m_Requestline.m_Host.Find("mi.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v160/index/mlogin/")!=-1)) 
	{
		return kAndroidLogin;
	}	
	//android修改资料
	if((http_session->m_Requestline.m_Host.Find("mi.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v150/user/update/")!=-1)) 
	{
		return kAndroidModify;
	}	
	//android定位
	if((http_session->m_Requestline.m_Host.Find("mi.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/updatecoor/")!=-1)) 
	{
		return kAndroidLocation;
	}
	return 0;
}

int marriage::WoZaiZhaoNiMarriage::Process(const char *packet,int action)
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
	//android注册
	if (action == kAndroidRegister)
	{
		return AndroidRegister(packet);
	}
	//android登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android修改资料
	if (action == kAndroidModify)
	{
		return AndroidModify(packet);
	}
	//android定位
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	return 1;
}
//注册
int marriage::WoZaiZhaoNiMarriage::WebRegister(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.rfind("nickname=") != string::npos )
	{
		src_data = src_data.substr(src_data.rfind("nickname="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//密码
	string passwd = ms_->GetValueFromMapByKey("passwd");
	//性别
	string sex = ms_->GetValueFromMapByKey("sex")=="1"?"男":"女";
	if (nickname.empty() || mobile.empty() || passwd.empty() )
	{
		return 0;
	}	
	output_text_ =
		"msg=" + 	 nickname + "\r\n" + 		//65	[104]   //昵称
		"oid=" + 	 mobile + "\r\n" + 			//54	[119]	//手机号 
		"pass=" + 	 passwd + "\r\n" + 			//21	[14]	//密码 	
		"sex=" + 	 sex + "\r\n" + 			//60	[125]	//性别 
		"netmark=" + app_ + "\r\n" + 			//59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126]	//操作平台
	"state=注册\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//登录
int marriage::WoZaiZhaoNiMarriage::WebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("mobile=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("mobile="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//密码
	string password = ms_->GetValueFromMapByKey("passwd");
	
	output_text_ =
		"oid=" + 	  	mobile + "\r\n" + 			//54	[119]	//手机号 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//密码 	
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//操作平台
		"state=登录\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//web修改资料
int marriage::WoZaiZhaoNiMarriage::WebModify(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.find("nickname=") != -1)
	{
		src_data = src_data.substr(src_data.find("nickname="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//类型
	string input = ms_->GetValueFromMapByKey("input",true);
	if (input != "nickname" || nickname.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + 	 	nickname + "\r\n" + 	//65	[104]   //昵称
		"netmark=" + app_ + "\r\n" + 			//59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126]	//操作平台
		"state=修改个人信息\r\n";				//64	[19]	//状态
	WriteIndexFile();
	return 1;
}

//android注册
int marriage::WoZaiZhaoNiMarriage::AndroidRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("vocation=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("vocation="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//密码
	string passwd = ms_->GetValueFromMapByKey("passwd");
	//性别
	string sex = ms_->GetValueFromMapByKey("sex")=="1" ? "男":"女";
	//经度
	string longitude = ms_->GetValueFromMapByKey("lng");
	//纬度
	string latitude = ms_->GetValueFromMapByKey("lat");
	if (nickname.empty() || mobile.empty() || passwd.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //昵称
		"oid=" + 	  	mobile + "\r\n" + 			//54	[119]	//手机号 
		"pass=" + 		passwd + "\r\n" + 			//21	[14]	//密码 	
		"sex=" + 	  	sex + "\r\n" + 				//60	[125]	//性别 
		"from=" + 	  	longitude + "\r\n" + 		//37	[41]  	//经度  
		"to=" + 	  	latitude + "\r\n" + 		//38	[42]    //纬度
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//来源
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126]	//操作平台
		"state=注册\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//android登录
int marriage::WoZaiZhaoNiMarriage::AndroidLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("sign=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("sign="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//密码
	string password = ms_->GetValueFromMapByKey("passwd");
	if (mobile.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"oid=" + 	  	mobile + "\r\n" + 			//54	[119]	//手机号 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//密码 	
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//来源
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126]	//操作平台
		"state=登录\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//android修改资料
int marriage::WoZaiZhaoNiMarriage::AndroidModify(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.find("value=") != -1)
	{
		src_data = src_data.substr(src_data.find("value="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	string nickname = ms_->GetValueFromMapByKey("value",true);
	//类型
	string input = ms_->GetValueFromMapByKey("field",true);
	if (input != "nickname" || nickname.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //昵称
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//来源
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126]	//操作平台
		"state=修改个人信息\r\n";					//64	[19]	//状态
	WriteIndexFile();
	return 1;
}

//android定位
int marriage::WoZaiZhaoNiMarriage::AndroidLocation(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("lat=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("lat="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//经度
	string longitude = ms_->GetValueFromMapByKey("lng");
	//纬度
	string latitude = ms_->GetValueFromMapByKey("lat");
    if (longitude.empty() || latitude.empty())
    {
		return 0;
    }	
	output_text_ =
		"from=" + 		longitude + "\r\n" + 		//37	[41]  	//经度  
		"to=" + 		latitude + "\r\n" + 		//38	[42]    //纬度
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//来源
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126]	//操作平台
		"state=定位\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}


