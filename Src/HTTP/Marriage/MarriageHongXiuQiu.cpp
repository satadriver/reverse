#include "StdAfx.h"
#include "MarriageHongXiuQiu.h"

marriage::HongXiuQiuMarriage::HongXiuQiuMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "红绣球";	
}

marriage::HongXiuQiuMarriage::~HongXiuQiuMarriage()
{
	
}

int marriage::HongXiuQiuMarriage::IsHongXiuQiuMarriage(HttpSession *http_session)
{
	//网页注册
	if((http_session->m_Requestline.m_Host.Find("www.xiuqiu99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register.aspx")!=-1)) 
	{
		return kPcWebRegister;
	}
	//网页登录
	if((http_session->m_Requestline.m_Host.Find("www.xiuqiu99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.aspx")!=-1)) 
	{
		return kPcWebLogin;
	}
	//网页修改资料
	if((http_session->m_Requestline.m_Host.Find("www.xiuqiu99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/setProfile.aspx")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int marriage::HongXiuQiuMarriage::Process(const char *packet,int action)
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
int marriage::HongXiuQiuMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//手机
	string tbphone = ms_->GetValueFromMapByKey("tbPhone",true);
	//登录号
	string username = ms_->GetValueFromMapByKey("tbEmail",true);
	//昵称
	string nick = ms_->GetValueFromMapByKey("tbNicheng",true);
	//密码
	string password = ms_->GetValueFromMapByKey("tbPwd",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("rblSex",true);

	output_text_ =
		"user=" +   username + "\r\n" +           //用户名 
        "pass=" +   password + "\r\n" +           //密码  
		"msg=" +    nick + "\r\n" +               //昵称
		"oid=" +     tbphone + "\r\n" +           //手机号 
		"sex=" +     sex + "\r\n" +               //性别 
		"netmark=" + app_ + "\r\n" + 	          //来源
		"clientmsg=" + platformWeb + "\r\n" +     //操作平台
		"state=注册\r\n";			              //状态
	WriteIndexFile();
	return 1;
}

//web登录
int marriage::HongXiuQiuMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}	
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("tbEmail",true);
	string pwd = ms_->GetValueFromMapByKey("tbPwd");
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
int marriage::HongXiuQiuMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//真实姓名
	string truename = ms_->GetValueFromMapByKey("tbTrueName",true);
	//身份证
	string idcard = ms_->GetValueFromMapByKey("tbSFZ",true);
	//手机号
	string phone = ms_->GetValueFromMapByKey("tbTel",true);
	//QQ
	string QQ = ms_->GetValueFromMapByKey("tbQQ",true);
	//地址
	string address = ms_->GetValueFromMapByKey("tbAddr",true);

	output_text_ =
		"mid=" + truename + "\r\n" +                //姓名	
        "servername=" + "身份证"  + "\r\n" +       //证件类型     
		"appname=" +     idcard + "\r\n" +         //证件号
		"oid=" +     phone + "\r\n" +              //手机号 
		"msgtype=" +    "QQ" + "\r\n" +            //通讯类型
        "subject=" +     QQ + "\r\n" +             //通讯号
		"islogin=" +     address + "\r\n" +        //地址
		"netmark=" + app_ + "\r\n" + 		       //来源
		"clientmsg=" + platformWeb + "\r\n" +	   //操作平台
		"state=修改个人信息\r\n";			       //状态
	WriteIndexFile();
	return 1;
}

