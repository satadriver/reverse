#include "StdAfx.h"
#include "MarriageTongChengYueHui.h"

marriage::TongChengMarriage::TongChengMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "同城约会";	
}

marriage::TongChengMarriage::~TongChengMarriage()
{
	
}

int marriage::TongChengMarriage::IsTongChengMarriage(HttpSession *http_session)
{
	//网页注册
	if((http_session->m_Requestline.m_Host.Find("yuehui.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register3.do")!=-1)) 
	{
		return kPcWebRegister;
	}
	//网页修改资料
	if((http_session->m_Requestline.m_Host.Find("yuehui.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myinfobasic.do")!=-1)) 
	{
		return kPcWebModiy;
	}
	//Android注册
	if((http_session->m_Requestline.m_Host.Find("yuehui.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/app/appregister2.do")!=-1)) 
	{
		return kAndroidRegister;
	}
	//Android修改
	if((http_session->m_Requestline.m_Host.Find("yuehui.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/app/updateuserbase.do")!=-1)) 
	{
		return kAndroidModiy;
	}
	return 0;
}

int marriage::TongChengMarriage::Process(const char *packet,int action)
{
	//网页注册
	if (action == kPcWebRegister)
	{
		return PcWebRegister(packet);
	}
	//网页修改个人资料
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	//Android注册
	if (action == kAndroidRegister)
	{
		return AndroidRegister(packet);
	}
	//Android修改
	if (action == kAndroidModiy)
	{
		return AndroidModiy(packet);
	}
	return 1;
}
//pc注册
int marriage::TongChengMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//账号
	string account = ms_->GetValueFromMapByKey("account",true);
	//昵称
	string nick = ms_->GetValueFromMapByKey("nick",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("sex",true);
	if (sex == "1")
	{
		sex = "男";
	}
	else
	{
		sex = "女";
	}
	//出生日期
	string birthday = ms_->GetValueFromMapByKey("byear",true)+"-"+
					  ms_->GetValueFromMapByKey("bmonth",true)+"-"+
					  ms_->GetValueFromMapByKey("bday",true)+"-";
	output_text_ =
	    "user=" + account + "\r\n" +         //用户名
	    "msg=" + nick + "\r\n" +             //昵称
		"sex=" +sex + "\r\n" +               //性别
		"onick=" + birthday + "\r\n" +       //出生日期
		"netmark=" + app_ + "\r\n" + 	     //来源
		"clientmsg=" + platformWeb + "\r\n" +//操作平台
		"state=注册\r\n";			         //状态
	WriteIndexFile();
	return 1;
}
//详细资料个人信息
int marriage::TongChengMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//用户
	string account = ms_->GetValueFromMapByKey("account",true);
	//昵称
	string nick = ms_->GetValueFromMapByKey("nick",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("sex",true)=="1"?"男":"女";
	//出生日期
	string birthday = ms_->GetValueFromMapByKey("byear",true)+"-"+
		              ms_->GetValueFromMapByKey("bmonth",true)+"-"+
					  ms_->GetValueFromMapByKey("bday",true)+"-";
	output_text_ =
	    "user=" + account + "\r\n" +          //用户名
	    "msg=" + nick + "\r\n" +             //昵称
		"sex=" +sex + "\r\n" +               //性别
		"onick=" + birthday + "\r\n" +       //出生日期
		"netmark=" + app_ + "\r\n" + 	     //来源
		"clientmsg=" + platformWeb + "\r\n" +//操作平台
		"state=修改个人信息\r\n";			 //状态
	WriteIndexFile();
	return 1;
}
//Android注册
int marriage::TongChengMarriage::AndroidRegister(const char* packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//昵称
	string nick = ms_->GetValueFromMapByKey("nick",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("sex",true)=="1"?"男":"女";
	//出生日期
	string birthday = ms_->GetValueFromMapByKey("byear",true)+"-"+
		              ms_->GetValueFromMapByKey("bmonth",true)+"-"+
					  ms_->GetValueFromMapByKey("bday",true);
	//用户名
	string account = ms_->GetValueFromMapByKey("account",true);
	output_text_ =
		"user=" + account + "\r\n" +          //用户名
		"msg=" + nick + "\r\n" +             //昵称
		"sex=" +sex + "\r\n" +               //性别
		"onick=" + birthday + "\r\n" +       //出生日期
		"netmark=" + app_ + "\r\n" + 	     //来源
		"clientmsg=" + platformAndroid + "\r\n" +//操作平台
		"state=注册\r\n";			         //状态
	WriteIndexFile();
	return 1;
}
//Android修改
int marriage::TongChengMarriage::AndroidModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//昵称
	string nick = ms_->GetValueFromMapByKey("nick",true);
	//账户
	string account = ms_->GetValueFromMapByKey("account",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("sex",true)=="1"?"男":"女";
	//出生日期
	string birhtday = ms_->GetValueFromMapByKey("byear",true)+"-"+
		              ms_->GetValueFromMapByKey("bmonth",true)+"-"+
					  ms_->GetValueFromMapByKey("bday",true);
	output_text_ =
		"user=" + account + "\r\n" +          //用户名
		"msg=" + nick + "\r\n" +             //昵称
		"sex=" +sex + "\r\n" +               //性别
		"onick=" + birhtday + "\r\n" +       //日期
		"netmark=" + app_ + "\r\n" + 	     //来源
		"clientmsg=" + platformAndroid + "\r\n" +//操作平台
		"state=修改个人信息\r\n";			         //状态
	WriteIndexFile();
	return 1;
}

