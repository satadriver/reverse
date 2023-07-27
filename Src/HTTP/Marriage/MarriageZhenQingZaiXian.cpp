#include "StdAfx.h"
#include "MarriageZhenQingZaiXian.h"

marriage::ZhenQingZaiXianMarriage::ZhenQingZaiXianMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "真情在线";	
}

marriage::ZhenQingZaiXianMarriage::~ZhenQingZaiXianMarriage()
{
	
}

int marriage::ZhenQingZaiXianMarriage::IsZhenQingZaiXianMarriage(HttpSession *http_session)
{
	//网页注册
	if((http_session->m_Requestline.m_Host.Find("www.lol99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg/index.php")!=-1)) 
	{
		return kPcWebRegister;
	}
	//网页登录
	if((http_session->m_Requestline.m_Host.Find("www.lol99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login/login.php")!=-1)) 
	{
		return kPcWebLogin;
	}
	//网页修改资料
	if((http_session->m_Requestline.m_Host.Find("www.lol99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/edit.php")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int marriage::ZhenQingZaiXianMarriage::Process(const char *packet,int action)
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

	//网页资料修改
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}
//网页注册
int marriage::ZhenQingZaiXianMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//昵称
	string nick = ms_->GetValueFromMapByKey("name",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("gender",true)=="0"?"男":"女";
	//出生日期
	string birthday = ms_->GetValueFromMapByKey("birth_year",true) + "-" +
		              ms_->GetValueFromMapByKey("birth_month",true) + "-" + 
					  ms_->GetValueFromMapByKey("birth_day",true);
	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	//真实姓名
	string truename = ms_->GetValueFromMapByKey("truename",true);
	//身份证
	string idcard = ms_->GetValueFromMapByKey("sfcard",true);
	output_text_ =
		"msg=" +nick + "\r\n" +              //昵称
		"sex=" +sex + "\r\n" +               //性别
		"onick=" + birthday + "\r\n" +        //出生日期
		"oid=" +mobile + "\r\n" +            //手机号
		"nick=" +email + "\r\n" +            //邮箱
		"mid=" +truename + "\r\n" +          //真实姓名
		"servername=" +"身份证" + "\r\n" +   //证件类型
		"appname=" +idcard + "\r\n" +        //身份证号
 		"netmark=" + app_ + "\r\n" + 	     //来源
		"clientmsg=" + platformWeb + "\r\n" +//操作平台
		"state=注册\r\n";			         //状态
	WriteIndexFile();
	return 1;
}

//web登录
int marriage::ZhenQingZaiXianMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string username = ms_->GetValueFromMapByKey("mobile",true);
	string pwd = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +          //用户名
		"pass=" + pwd + "\r\n" +               //密码
		"netmark=" + app_ + "\r\n" + 		   //来源
		"clientmsg=" + platformWeb + "\r\n" +  //操作平台
		"state=登录\r\n";			           //状态
	WriteIndexFile();
	return 1;
}

//详细资料个人信息
int marriage::ZhenQingZaiXianMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//姓名
	string name = ms_->GetValueFromMapByKey("truename",true);
	//email
	string email = ms_->GetValueFromMapByKey("email",true);
	//地址
	string address = ms_->GetValueFromMapByKey("address",true);
	output_text_ =
		"mid=" +name + "\r\n" +                 //姓名
		"nick=" + email + "\r\n" +              //邮箱
		"islogin=" +address + "\r\n" +          //地址
		"netmark=" + app_ + "\r\n" + 		    //来源
		"clientmsg=" + platformWeb + "\r\n" +   //操作平台
		"state=修改个人信息\r\n";			    //状态
	WriteIndexFile();
	return 1;
}

