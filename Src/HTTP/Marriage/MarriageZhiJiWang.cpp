#include "StdAfx.h"
#include "MarriageZhiJiWang.h"

marriage::ZhiJiMarriage::ZhiJiMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "知己网";	
}

marriage::ZhiJiMarriage::~ZhiJiMarriage()
{
	
}

int marriage::ZhiJiMarriage::IsZhiJiMarriage(HttpSession *http_session)
{
	//网页注册
	if((http_session->m_Requestline.m_Host.Find("www.zhiji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg2008/reg_newg_submit.asp")!=-1)) 
	{
		return kPcWebRegister;
	}
	//网页登录
	if((http_session->m_Requestline.m_Host.Find("zhanghu.zhiji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login_submit.asp")!=-1)) 
	{
		return kPcWebLogin;
	}
	//网页修改资料
	if((http_session->m_Requestline.m_Host.Find("zhanghu.zhiji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/members/templete/ajax_register_change_submit.asp")!=-1)) 
	{
		return kPcWebModiy;
	}
	//手机登录
	if((http_session->m_Requestline.m_Host.Find("app.zhiji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/zhiji-mobile-web/login/loginios")!=-1)) 
	{
		return kAndroidLogin;
	}
	//手机修改
	if((http_session->m_Requestline.m_Host.Find("app.zhiji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/zhiji-mobile-web/memberdata/xiugai")!=-1)) 
	{
		return kAndroidModiy;
	}
	return 0;
}

int marriage::ZhiJiMarriage::Process(const char *packet,int action)
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
	//手机登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	if (action == kAndroidModiy)
	{
		return AndroidModiy(packet);
	}
	return 1;
}
//pc注册
int marriage::ZhiJiMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//昵称
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	nickname = ms_->UnicodeToGBKPrefix_u(nickname);
	//通讯类型
	string contactType = "QQ";
	//QQ
	string qq = ms_->GetValueFromMapByKey("QQ",true);
	//手机
	string phone = ms_->GetValueFromMapByKey("phone",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("sex",true);
	if(sex == "1"){sex = "男";}
	else{sex = "女";}
	//出生日期
	string year = ms_->GetValueFromMapByKey("ldy_age",true);
	//地址
	string address = ms_->GetValueFromMapByKey("province_name",true)+
		             ms_->GetValueFromMapByKey("city_name",true);
	address = ms_->UnicodeToGBKPrefix_u(address);
	output_text_ =
		"msg=" + 	 nickname + "\r\n" + 		//昵称 
		"msgtype=" + contactType + "\r\n" +		//通讯类型
		"subject=" + 	  	qq + "\r\n" + 		//通讯号
		"sex=" +sex + "\r\n" +					//性别
		"onick=" + year + "\r\n" +				//出生日期
		"oid=" + phone + "\r\n" +				//手机号
		"islogin=" + address + "\r\n" +			//地址
		"netmark=" + app_ + "\r\n" + 			//来源
		"clientmsg=" + platformWeb + "\r\n" +	//操作平台
		"state=注册\r\n";						//状态
	WriteIndexFile();
	return 1;
}

//web登录
int marriage::ZhiJiMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("login_name",true);
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
int marriage::ZhiJiMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	email = ms_->UnicodeToGBKPrefix_u(email);
	//手机号
	string phone = ms_->GetValueFromMapByKey("mobileno",true);
	phone = ms_->UnicodeToGBKPrefix_u(phone);
	//QQ
	string oicq = ms_->GetValueFromMapByKey("oicq",true);
    oicq = ms_->UnicodeToGBKPrefix_u(oicq);
	//微信
	string msn = ms_->GetValueFromMapByKey("msn",true);
	msn = ms_->UnicodeToGBKPrefix_u(msn);
	string oicqid = "qq/微信";
	oicq = oicq + "/" +msn;
	//出生日期
	string birth = ms_->GetValueFromMapByKey("birth_year",true)+"年"+
		           ms_->GetValueFromMapByKey("birth_mon",true)+"月"+
				   ms_->GetValueFromMapByKey("birth_day",true)+"日";
	birth = ms_->UnicodeToGBKPrefix_u(birth);
	//地址
	string province = ms_->GetValueFromMapByKey("register_province_name",true);
	string city	 =   ms_->GetValueFromMapByKey("register_city_name",true);
	province = ms_->UnicodeToGBKPrefix_u(province);
	city = ms_->UnicodeToGBKPrefix_u(city);
	string address = province + city;
	ms_->Replace(address,"%u","");
	address = ms_->UnicodeToGBK(address);
	//性别
	string sex = ms_->GetValueFromMapByKey("sex",true);
	sex = ms_->UnicodeToGBKPrefix_u(sex);
	if (sex == "1"){sex = "男";}
	else{sex = "女";}
	output_text_ =
		"msgtype=" + oicqid + "\r\n" +		//通讯类型
		"subject=" +  oicq + "\r\n" +       //通讯号
		"nick="  +     email + "\r\n" +		//邮箱
		"sex=" +sex + "\r\n" +              //性别
		"onick=" + birth + "\r\n" +         //出生日期
		"oid=" + phone + "\r\n" +           //手机号
		"islogin=" + address + "\r\n" +     //地址
		"netmark=" + app_ + "\r\n" + 	    //来源
		"clientmsg=" + platformWeb + "\r\n" +//操作平台
		"state=修改个人信息\r\n";
 	WriteIndexFile();
	return 1;
}
//手机登录
int marriage::ZhiJiMarriage::AndroidLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string lon = ms_->GetValueFromMapByKey("lon",true);
	string lat = ms_->GetValueFromMapByKey("lat",true);
	string login_name = ms_->GetValueFromMapByKey("login_name",true);
	login_name = ms_->UnicodeToGBKPrefix_u(login_name);
	string password = ms_->GetValueFromMapByKey("password",true);
	output_text_ =
		"user=" +   login_name + "\r\n" +			//用户名 
		"pass=" +   password  + "\r\n" +			//密码  
		"from=" +     lon + "\r\n" +				//经度  
        "to=" +     lat + "\r\n" +					//纬度
		"netmark=" + app_ + "\r\n" + 				//来源
		"clientmsg=" + platformAndroid + "\r\n" +	//操作平台
		"state=登录\r\n";
	WriteIndexFile();
	return 1;

}
//手机修改
int marriage::ZhiJiMarriage::AndroidModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//登录名
	string login_name = ms_->GetValueFromMapByKey("login_name",true);
	login_name = ms_->UnicodeToGBKPrefix_u(login_name);
	//地区
	string province = ms_->GetValueFromMapByKey("province_name",true);
	string city = ms_->GetValueFromMapByKey("city_name",true);
	province = ms_->UnicodeToGBKPrefix_u(province);
	city = ms_->UnicodeToGBKPrefix_u(city);
	string address = province+city;
	//出生日期
	string birth = ms_->GetValueFromMapByKey("birth_year",true)+"-"+
		           ms_->GetValueFromMapByKey("birth_mon",true)+"-"+
				   ms_->GetValueFromMapByKey("birth_day",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("sex_data",true);
	if (sex=="0")
	{
		sex = "男";
	}
	else
	{
		sex = "女";
	}
	output_text_=
		"msg=" + 	 login_name + "\r\n" + 		//昵称 
		"islogin=" + address + "\r\n" +			//地址
		"sex=" +sex + "\r\n" +					//性别
		"onick=" + birth + "\r\n" +				//出生日期
		"netmark=" + app_ + "\r\n" + 	        //来源
		"clientmsg=" + platformAndroid + "\r\n" +//操作平台
		"state=修改个人信息\r\n";
	WriteIndexFile();
	return 1;
}


