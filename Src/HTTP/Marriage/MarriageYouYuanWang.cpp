#include "StdAfx.h"
#include "MarriageYouYuanWang.h"

marriage::YouYuanWangMarriage::YouYuanWangMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "有缘网";	
}

marriage::YouYuanWangMarriage::~YouYuanWangMarriage()
{
	
}

int marriage::YouYuanWangMarriage::IsYouYuanWangMarriage(HttpSession *http_session)
{
	//手机端登录
	if((http_session->m_Requestline.m_Host.Find("huluemt.youyuan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/login.gy")!=-1)) 
	{
		return kAndroidLogin;
	}
	return 0;
}

int marriage::YouYuanWangMarriage::Process(const char *packet,int action)
{
	//手机登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	return 1;
}

int marriage::YouYuanWangMarriage::AndroidLogin(const char* packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}
	string username = value["account"].asString();
	string pwd = value["password"].asString();
	if (username.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +				//用户名
		"pass=" + pwd + "\r\n" +					//密码
		"netmark=" + app_ + "\r\n" + 				//来源
		"clientmsg=" + platformAndroid + "\r\n" +	//操作平台
		"state=登录\r\n";							//状态
	WriteIndexFile();
	return 1;
}