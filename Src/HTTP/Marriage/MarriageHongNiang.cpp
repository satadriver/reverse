#include "StdAfx.h"
#include "MarriageHongNiang.h"

marriage::HongNiangMarriage::HongNiangMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "中国红娘网";	
}

marriage::HongNiangMarriage::~HongNiangMarriage()
{
	
}

int marriage::HongNiangMarriage::IsHongNiangMarriage(HttpSession *http_session)
{
	//注册
	if((http_session->m_Requestline.m_Host.Find("www.hongniang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/do_register_new1")!=-1)) 
	{
		return kWebRegister;
	}	
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.hongniang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/do_login")!=-1)) 
	{
		return kWebLogin;
	}	
	//web修改资料(注册信息)
	if((http_session->m_Requestline.m_Host.Find("www.hongniang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/save_percenter")!=-1)) 
	{
		return kWebModify;
	}
	//web修改资料(详细信息)
	if((http_session->m_Requestline.m_Host.Find("www.hongniang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/save_detail")!=-1)) 
	{
		return kWebModifyM;
	}
	return 0;
}

int marriage::HongNiangMarriage::Process(const char *packet,int action)
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
	//web修改资料注册信息
	if (action == kWebModify)
	{
		return WebModify(packet);
	}
	//web修改资料详细资料
	if (action == kWebModifyM)
	{
		return WebModifyM(packet);
	}
	return 1;
}
//注册
int marriage::HongNiangMarriage::WebRegister(const char *packet)
{
    //数据部分
	string src_data(packet);
	if (src_data.find("sex=") != string::npos)
	{
		src_data = src_data.substr(src_data.find("sex="));
	}
	ms_->SplitStringToMap(src_data,'&','=');

	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//密码
	string password = ms_->GetValueFromMapByKey("password");
	string re_password = ms_->GetValueFromMapByKey("re_password");
	if (password != re_password)
	{
		return 0;
	}
	//昵称
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//出生日期
	string birthdate = ms_->GetValueFromMapByKey("year") + "-" +
					   ms_->GetValueFromMapByKey("month") + "-" +
					   ms_->GetValueFromMapByKey("day");
	
	output_text_ =
		"oid=" + 	  	mobile + "\r\n" + 			//54	[119]	//手机号 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//密码
		"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //昵称
		"onick=" + 	  	birthdate + "\r\n" + 		//55	[120]	//出生日期 
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//操作平台
		"state=注册\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//登录
int marriage::HongNiangMarriage::WebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("username=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("username="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string username = ms_->GetValueFromMapByKey("username",true);
	//密码
	string password = ms_->GetValueFromMapByKey("password");
	
	output_text_ =
		"user=" + 		username + "\r\n" +			//20	[13]	//用户名 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//密码 	
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//操作平台
		"state=登录\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//web修改资料详细信息
int marriage::HongNiangMarriage::WebModifyM(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("realname", true);
	if (name.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=" +		name + "\r\n" + 		 //姓名
		"netmark=" + app_ + "\r\n" + 			//来源
		"clientmsg=" + platformWeb + "\r\n" +	//操作平台
		"state=修改详细信息\r\n";				//状态
	WriteIndexFile();
	return 1;
}
//web修改资料注册信息
int marriage::HongNiangMarriage::WebModify(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//手机号
	string email = ms_->GetValueFromMapByKey("mobile",true);
	if (nickname.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + 	 	nickname + "\r\n" + 	//65	[104]   //昵称
		"mobile="	 + 	  	email + "\r\n" + 	//手机号
		"netmark=" + app_ + "\r\n" + 			//59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126]	//操作平台
		"state=修改注册信息\r\n";				//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
