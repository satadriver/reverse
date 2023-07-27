#include "StdAfx.h"
#include "HotelPodinns.h"

hotel::HotelPodinns::HotelPodinns(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "布丁酒店";
}

hotel::HotelPodinns::~HotelPodinns()
{

}

hotel::HotelPodinns::IsHotelPodinns(HttpSession *http_session)
{
	//web注册
	if( ( http_session->m_Requestline.m_Host.Find("www.podinns.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/Account/RegPost") != -1 ))	
	{
		return kWebRegister;
	}
	//web登录
	if( ( http_session->m_Requestline.m_Host.Find("www.podinns.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/Account/LogOn") != -1 ))	
	{
		return kWebLogin;
	}
	//web国内酒店预订
	if( ( http_session->m_Requestline.m_Host.Find("www.podinns.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/HotelBook/EnterBookInfo") != -1))	
	{
		return kWebNewOrder;
	}
	//常用姓名
	if (http_session->m_Requestline.m_Host.Find("www.podinns.com") != 1 &&
		http_session->m_Requestline.m_URI.Find("/Member/MyCommonUserEdit") != -1)
	{
		return kWebCommonUser;
	}
	//android登录
	if (http_session->m_Requestline.m_Host.Find("smart.zhotels.cn:8081") != 1 &&
		http_session->m_Requestline.m_URI.Find("/app/login") != -1)
	{
		return kAndroidLogin;
	}
	//android定位
	if (http_session->m_Requestline.m_Host.Find("115.236.19.4:9008") != 1 &&
		http_session->m_Requestline.m_URI.Find("/login/main") != -1)
	{
		return kAndroidLocation;
	}
	return 0;
}

hotel::HotelPodinns::Process(const char *packet,int action)
{
	//web注册
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//web登录
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//web国内酒店预订
	if (action == kWebNewOrder)
	{
		return WebNewOrder(packet);
	}
	//web常用姓名
	if (action == kWebCommonUser)
	{
		return WebCommonUser(packet);
	}
	//android登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android定位
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	return 0;
}
//web注册
hotel::HotelPodinns::WebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("Name") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("Name"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("Name",true);
	//联系手机 
	string mobile = ms_->GetValueFromMapByKey("Mobile");
	//密码 
	string password = ms_->GetValueFromMapByKey("Password2");	
	string confirmpassword = ms_->GetValueFromMapByKey("ConfirmPassword");
    if (password != confirmpassword)
    {
		return -1;
    }
	//身份证号
	string idcard = ms_->GetValueFromMapByKey("Sfz");
	if (idcard.empty())
	{
		idcard = "未填写";
	}
	//邮箱
	string email = ms_->GetValueFromMapByKey("Email",true);
	if (email.empty())
	{
		email = "未填写";
	}
	if (name.empty() || mobile.empty() || password.empty())
	{
		return -1;
	}
	output_text_ =
		"wifissid=" + 	name + "\r\n" + 		//姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"pass=" + 		password + "\r\n" +		//密码
		"msg=" + 	  	idcard + "\r\n" + 		//身份证号
		"oid=" + 	  	email + "\r\n" + 		//联系邮箱
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=注册\r\n";						//状态
	WriteIndexFile();
	return 1;
}
//web登录
hotel::HotelPodinns::WebLogin(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.rfind("isTravel") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("isTravel"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
    string username = ms_->GetValueFromMapByKey("UserName",true);
	string password = ms_->GetValueFromMapByKey("Password");

	if (username.empty() || password.empty())
	{
		return -1;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +	    //用户名
		"pass=" + 		password + "\r\n" +		//密码
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=登录\r\n";						//状态
	WriteIndexFile();
	return 1;
}

//web国内酒店预订
hotel::HotelPodinns::WebNewOrder(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.rfind("InName") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("InName"));
	}
	ms_->SplitStringToMap(src_data,'&','=');

	//住客姓名 
	string name = ms_->GetValueFromMapByKey("InName",true);
    //身份证号
	string idcard = ms_->GetValueFromMapByKey("Sfzh");
	//联系手机 
	string mobile = ms_->GetValueFromMapByKey("LinkMobile");
	//联系邮箱 
	string email = ms_->GetValueFromMapByKey("LinkEmail",true);

	if (name.empty() || idcard.empty() || mobile.empty() || email.empty())
	{
		return -1;
	}
	output_text_ =
		"subject=" + 	name + "\r\n" + 		//住客姓名
		"msg=" + 	  	idcard + "\r\n" + 		//身份证号
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"oid=" + 	  	email + "\r\n" + 		//联系邮箱
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=国内酒店预订\r\n";				//状态
	WriteIndexFile();
	return 1;
}
//web常用姓名
hotel::HotelPodinns::WebCommonUser(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("MCU_ID") != -1)
	{
		src_data = src_data.substr(src_data.rfind("MCU_ID"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//判断 新增 修改
	string mcu_id = ms_->GetValueFromMapByKey("MCU_ID");
	//姓名
	string username = ms_->GetValueFromMapByKey("MCU_USERNAME",true);
	//身份证号
	string idcard = ms_->GetValueFromMapByKey("MCU_LINKUSER");
	//联系手机
	string mobile = ms_->GetValueFromMapByKey("MCU_MOBILE");
	//联系邮箱
	string email = ms_->GetValueFromMapByKey("MCU_EMAIL",true);
	if ("0" == mcu_id)
	{
		//新增
		output_text_ =
			"wifissid=" + 	username + "\r\n" + 	//姓名
			"msg=" + 	  	idcard + "\r\n" + 		//身份证号
			"islogin=" + 	mobile + "\r\n" + 		//联系手机
			"oid=" + 	  	email + "\r\n" + 		//联系邮箱
			"netmark=" +	app_ + "\r\n" + 		//来源
			"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=新增常用姓名\r\n";					//状态
	}
	else if ("0" != mcu_id)
	{
		//修改
		output_text_ =
			"wifissid=" + 	username + "\r\n" + 	//姓名
			"msg=" + 	  	idcard + "\r\n" + 		//身份证号
			"islogin=" + 	mobile + "\r\n" + 		//联系手机
			"oid=" + 	  	email + "\r\n" + 		//联系邮箱
			"netmark=" +	app_ + "\r\n" + 		//来源
			"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=修改常用姓名\r\n";	
	}
	WriteIndexFile();
   return 0;
}

//android登录
hotel::HotelPodinns::AndroidLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("phone") != -1)
	{
		src_data = src_data.substr(src_data.rfind("phone"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//联系手机
	string mobile = ms_->GetValueFromMapByKey("phone");
	if (mobile.empty())
	{
		return -1;
	}
	//修改
	output_text_ =
		"islogin=" + 	mobile + "\r\n" + 			//联系手机
		"netmark=" +	app_ + "\r\n" + 			//来源
		"clientmsg=" +  platformAndroid + "\r\n" +  //操作平台
		"state=登录\r\n";	
	WriteIndexFile();
	return 0;
}

//android定位
hotel::HotelPodinns::AndroidLocation(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("{\"memberId") != -1)
	{
		src_data = src_data.substr(src_data.rfind("{\"memberId"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));

	AfxMessageBox(src_data.c_str());

	string username,nickname,longitude,latitude;
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		username = value["username"].asString();
		nickname = value["nickname"].asString();
		longitude = value["longitude"].asString();
		latitude = value["latitude"].asString();
	}
	if (username.empty() || longitude.empty() || latitude.empty())
	{
		return -1;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +			//用户名
		"from=" + 	  	longitude + "\r\n" + 		//经度 
		"to=" + 	  	latitude + "\r\n" + 		//纬度
		"netmark=" +	app_ + "\r\n" + 			//来源
		"clientmsg=" +  platformAndroid + "\r\n" +  //操作平台
		"state=定位-动态圈\r\n";	
	WriteIndexFile();
	return 0;
}