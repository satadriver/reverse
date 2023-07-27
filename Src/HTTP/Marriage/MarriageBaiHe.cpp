#include "StdAfx.h"
#include "MarriageBaiHe.h"

marriage::BaiHeMarriage::BaiHeMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "百合网";	
}

marriage::BaiHeMarriage::~BaiHeMarriage()
{
	
}

int marriage::BaiHeMarriage::IsBaiHeMarriage(HttpSession *http_session)
{
	//网页登录
	if((http_session->m_Requestline.m_Host.Find("my.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login/gotoLogin")!=-1)) 
	{
		return kPcLogin;
	}
	//网页修改资料	--处理包问题	
	if((http_session->m_Requestline.m_Host.Find("my.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Userinfo/subBasicInfo")!=-1)) 
	{
		//return kPcWebModiy;
	}
	//网页实名认证
	if((http_session->m_Requestline.m_Host.Find("app.iddun.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/baihe2015web/3")!=-1)) 
	{
		return kPcRealNameCheck;
	}
	//安卓登录
	if((http_session->m_Requestline.m_Host.Find("plus.app.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register/login")!=-1)) 
	{
		return kAndroidLogin;
	}
	//安卓修改资料
	if((http_session->m_Requestline.m_Host.Find("plus.app.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/editUserInfo")!=-1)) 
	{
		return kAndroidModify;
	}
	//安卓实名认证	
	if((http_session->m_Requestline.m_Host.Find("118.145.4.82:3010")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/baihe2015app/3")!=-1)) 
	{
		return kAndroidNameCheck;
	}
	//安卓学历认证
	if((http_session->m_Requestline.m_Host.Find("plus.app.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/credit/orangeAuthGetEdu")!=-1)) 
	{
		return kAndroidEduCheck;
	}
	//安卓金融认证
	if((http_session->m_Requestline.m_Host.Find("wallet.jr.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wallet/api/outer/account/openGetSmsCode")!=-1)) 
	{
		return kAndroidFinCheck;
	}
	//安卓定位
	if((http_session->m_Requestline.m_Host.Find("plus.app.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/searchNearby")!=-1)) 
	{
		return kAndroidLocation;
	}
	return 0;
}

int marriage::BaiHeMarriage::Process(const char *packet,int action)
{
	//网页登录
	if (action == kPcLogin)
	{
		return PcWebLogin(packet);
	}
	//网页修改基本信息
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	//网页实名认证
	if (action == kPcRealNameCheck)
	{
		return PcRealNameCheck(packet);
	}
	//安卓登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//安卓修改资料
	if (action == kAndroidModify)
	{
		return AndroidModify(packet);
	}
	//安卓实名认证	
	if (action == kAndroidNameCheck)
	{
		return AndroidNameCheck(packet);
	}
	//安卓学历认证
	if (action == kAndroidEduCheck)
	{
		return AndroidEduCheck(packet);
	}
	//安卓金融认证
	if (action == kAndroidFinCheck)
	{
		return AndroidFinCheck(packet);
	}
	//安卓定位
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	return 1;
}

//网页登陆
int marriage::BaiHeMarriage::PcWebLogin(const char *packet)
{
	//从cookie 中提取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//用户ID
	string userId = ms_->GetValueFromMapByKey("userID");
    //数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string username = ms_->GetValueFromMapByKey("txtLoginEMail",true);
	//密码
	string password = ms_->GetValueFromMapByKey("txtLoginPwd");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"userid=" + userId + "\r\n" + 			//28    [92]    //用户ID
		"user=" + username + "\r\n" +			//20	[13]	//用户名 
		"pass=" + password + "\r\n" + 			//21	[14]	//密码 	
		"netmark=" + app_ + "\r\n" + 		    //59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +   //62	[126]	//操作平台
		"state=登录\r\n";						//64	[19]	//状态
	WriteIndexFile();
	return 1;
}

//网页修改基本信息
int marriage::BaiHeMarriage::PcWebModiy(const char *packet)
{
	//从cookie 中提取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//用户ID
	string userId = ms_->GetValueFromMapByKey("userID");
	//从uri 中提取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	//昵称
    string nickName = ms_->GetValueFromMapByKey("nickname",true);
	//出生日期
	string birthdate = ms_->GetValueFromMapByKey("birthday");
	//电话号码(备用)
	string tel = ms_->GetValueFromMapByKey("phoneContact");
	//微信号
	string weiXinContact = ms_->GetValueFromMapByKey("weiXinContact");
	//qq
	string qqContact = ms_->GetValueFromMapByKey("qqContact");

// 	if (qqContact.empty())
// 	{
// 		qqContact = "未填写";
// 	}
	output_text_ =
		"userid=" + userId + "\r\n" + 			//28    [92]    //用户ID
		"msg=" +	nickName + "\r\n" + 		//65	[104]   //昵称
		"onick=" +	birthdate + "\r\n" + 		//55	[120]	//出生日期
		"clientip=" + qqContact + "\r\n" + 		//68	[147] 	//QQ
		"wifissid=" + weiXinContact + "\r\n" + 	//69	[148] 	//微信
		"netmark=" + app_ + "\r\n" + 		    //59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +   //62	[126]	//操作平台
		"state=修改个人信息\r\n";			    //64	[19]	//状态
	WriteIndexFile();
	return 1;
}

//网页实名认证
int marriage::BaiHeMarriage::PcRealNameCheck(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.rfind("real_name=") != 0)
	{
		src_data = src_data.substr(src_data.rfind("real_name="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string real_name = ms_->GetValueFromMapByKey("real_name",true);
	//证件类型
	string idType = "身份证";
	//身份证号 
	string identity_card = ms_->GetValueFromMapByKey("identity_card");
	//手机号
	string user_mob = ms_->GetValueFromMapByKey("user_mob");
    
	output_text_ =
		"mid=" + real_name + "\r\n" + 			//56	[121]	//姓名	
		"servername=" + idType + "\r\n" + 		//49	[116]   //证件类型      
		"appname=" + identity_card + "\r\n" + 	//50	[115]   //证件号
		"oid=" + user_mob + "\r\n" + 			//54	[119]	//手机号 
		"netmark=" + app_ + "\r\n" + 		    //59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +   //62	[126]	//操作平台
		"state=实名认证\r\n";					//64	[19]	//状态
	WriteIndexFile();
	return 1;
}

//安卓登录
int marriage::BaiHeMarriage::AndroidLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//用户名
	string username = ms_->GetValueBetweenBeginEnd(src_data,"\"mobile\":\"","\"");
	//密码
	string password = ms_->GetValueBetweenBeginEnd(src_data,"\"password\":\"","\"");
    //IMEI
	string imei = ms_->GetValueBetweenBeginEnd(src_data,"\"lastLoginIMEI\":\"","\"");
	//MAC
	string mac = ms_->GetValueBetweenBeginEnd(src_data,"\"lastLoginMAC\":\"","\"");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +			//用户名 
		"pass=" + 		password + "\r\n" + 		//密码 
		"netmark=" +	app_ + "\r\n" + 			//来源
		"clientmsg=" +	platformAndroid + "\r\n" +	//操作平台
		"state=登录\r\n";							//状态
	WriteIndexFile();
	return 0;
}
//安卓修改资料
int marriage::BaiHeMarriage::AndroidModify(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	//用户ID 
	string userID = ms_->GetValueBetweenBeginEnd(src_data,"\"userID\":\"","\"",true);
	//昵称
	string nickname = ms_->GetValueBetweenBeginEnd(src_data,"\"nickname\":\"","\"",true);
	if (userID.empty() || nickname.empty())
	{
		return 0;
	}
	output_text_ =
		"userid=" +     userID + "\r\n" + 			//用户ID
		"msg=" + 	  	nickname + "\r\n" +			//昵称 
		"netmark=" +	app_ + "\r\n" + 			//来源
		"clientmsg=" + platformAndroid + "\r\n" +	//操作平台
		"state=修改基本信息\r\n";					//状态
	WriteIndexFile();
	return 0;
}
//安卓实名认证	
int marriage::BaiHeMarriage::AndroidNameCheck(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.rfind("real_name=") != 0)
	{
		src_data = src_data.substr(src_data.rfind("real_name="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string real_name = ms_->GetValueFromMapByKey("real_name",true);
	//证件类型
	string idType = "身份证";
	//身份证号 
	string identity_card = ms_->GetValueFromMapByKey("identity_card");
	//手机号
	string user_mob = ms_->GetValueFromMapByKey("user_mob");
    
	output_text_ =
		"mid=" + real_name + "\r\n" + 				//56	[121]	//姓名	
		"servername=" + idType + "\r\n" + 			//49	[116]   //证件类型      
		"appname=" + identity_card + "\r\n" + 		//50	[115]   //证件号
		"oid=" + user_mob + "\r\n" + 				//54	[119]	//手机号 
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//来源
		"clientmsg=" + platformAndroid + "\r\n" +   //62	[126]	//操作平台
		"state=实名认证\r\n";						//64	[19]	//状态
	WriteIndexFile();
	return 1;
}

//安卓学历认证
int marriage::BaiHeMarriage::AndroidEduCheck(const char *packet)
{
    //数据部分
	string src_data(packet);
	//用户ID 
	string userID = ms_->GetValueBetweenBeginEnd(src_data,"\"userID\":\"","\",\"name",true);
	//姓名 	 
	string name = ms_->GetValueBetweenBeginEnd(src_data,"\"name\":\"","\",\"idno",true);
	//证件类型
	string idType = "身份证";
	//身份证号 
	string idno = ms_->GetValueBetweenBeginEnd(src_data,"idno\":\"","\",\"mobile",true);
	//手机号
	string mobile = ms_->GetValueBetweenBeginEnd(src_data,"mobile\":\"","\",\"smsCode",true);
	output_text_ =
		"userid=" + userID + "\r\n" + 				//28    [92]    //用户ID
		"mid=" + 	name + "\r\n" + 				//56	[121]	//姓名
		"servername=" + idType + "\r\n" + 			//49	[116]   //证件类型      
		"appname=" + idno + "\r\n" + 				//50	[115]   //证件号
		"oid=" + mobile + "\r\n" + 					//54	[119]	//手机号 
		"netmark=" +app_ + "\r\n" + 				//59	[124]	//来源
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126]	//操作平台
		"state=学历认证\r\n";						//64	[19]	//状态
	WriteIndexFile();
	return 1;
}

//安卓金融认证
int marriage::BaiHeMarriage::AndroidFinCheck(const char *packet)
{
	//从cookie 中提取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//用户ID
	string userId = ms_->GetValueFromMapByKey("userID");
    //数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名 	 
	string username = ms_->GetValueFromMapByKey("cust_name",true);
	//证件类型
	string idType = "身份证";
	//身份证号 
	string idcard = ms_->GetValueFromMapByKey("id_code",true);
	//开户银行 
	string bankname = ms_->GetValueFromMapByKey("bank_name",true);
	//银行卡号 
	string bandcard = ms_->GetValueFromMapByKey("acct_id",true);
	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobile_phone");

	output_text_ =
		"userid=" +		userId + "\r\n" + 			//28    [92]    //用户ID
		"mid=" + 	  	username + "\r\n" + 		//56	[121]	//姓名
		"servername=" + idType + "\r\n" + 			//49	[116]   //证件类型      
		"appname=" + 	idcard + "\r\n" + 			//50	[115]   //证件号
		"fileurl=" + 	bankname + "\r\n" + 		//66	[144]   //开户银行
		"clientmac=" + 	bandcard + "\r\n" + 		//67	[146] 	//银行卡号
		"oid=" + 	  	mobile + "\r\n" + 			//54	[119]	//手机号 
		"netmark=" +	app_ + "\r\n" + 		    //59	[124]	//来源
		"clientmsg=" +	platformAndroid + "\r\n" +  //62	[126]	//操作平台
		"state=金融认证\r\n";						//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
int marriage::BaiHeMarriage::AndroidLocation(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	//用户ID
	string userid = ms_->GetValueBetweenBeginEnd(src_data,"\"userID\":\"","\"");
	//经度
	string longtitude = ms_->GetValueBetweenBeginEnd(src_data,"\"longitude\":\"","\"");
	//纬度
	string latitude = ms_->GetValueBetweenBeginEnd(src_data,"\"latitude\":\"","\"");
	if (userid.empty() || longtitude.empty() || latitude.empty())
	{
		return 0;
	}
	output_text_ = 
		"userid=" +		userid + "\r\n" + 			//用户ID
		"from=" + 	  	longtitude + "\r\n" +		//经度  
		"to=" + 	  	latitude + "\r\n" + 		//纬度
		"netmark=" +	app_ + "\r\n" + 			//来源
		"clientmsg=" +	platformAndroid + "\r\n" +	//操作平台
		"state=定位\r\n";	                        //状态
	WriteIndexFile();								
	return 0;
}