#include "StdAfx.h"
#include "MarriageLoveInHere.h"

marriage::LoveInHereMarriage::LoveInHereMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "爱在这儿";	
}

marriage::LoveInHereMarriage::~LoveInHereMarriage()
{
	
}

int marriage::LoveInHereMarriage::IsLoveInHereMarriage(HttpSession *http_session)
{
	//注册	
	if((http_session->m_Requestline.m_Host.Find("www.loveinhere.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/memberData_appe.aspx")!=-1)) 
	{
		return kWebRegister;
	}	
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.loveinhere.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/login_check.aspx")!=-1)) 
	{
		return kWebLogin;
	}	
	//web修改资料
	if((http_session->m_Requestline.m_Host.Find("www.loveinhere.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/memberData_adm_modi.aspx")!=-1)) 
	{
		return kWebModify;
	}	
	return 0;
}

int marriage::LoveInHereMarriage::Process(const char *packet,int action)
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
	return 1;
}
//注册
int marriage::LoveInHereMarriage::WebRegister(const char *packet)
{
	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户ID
	string userid = ms_->GetValueFromMapByKey("memberid");
	//密码
	string password = ms_->GetValueFromMapByKey("pass");
	string passconfirm = ms_->GetValueFromMapByKey("pass_confirm");
	if (password != passconfirm)
	{
		return 0;
	}
	//昵称
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//姓名
	string name = ms_->GetValueFromMapByKey("name",true); 
	//性别
	string sex = ms_->GetValueFromMapByKey("sex")=="1"?"男":"女";
	//出生日期
	string birthday = ms_->GetValueFromMapByKey("y12") +
					  ms_->GetValueFromMapByKey("y3") +
					  ms_->GetValueFromMapByKey("y4") + "-" +
					  ms_->GetValueFromMapByKey("m") + "-" +
					  ms_->GetValueFromMapByKey("d");
	//证件类型
	string idtype = "身份证";
	//身份证号
	string personID = ms_->GetValueFromMapByKey("personID");
	if (personID.empty())
	{
		personID = "未填写";
	}
	//地址
	string address = ms_->GetValueFromMapByKey("address",true);
	//手机号
	string mobile = ms_->GetValueFromMapByKey("tel_mobile");
	//通讯类型
	string IMkind = "QQ/Skype/微信";
	//通讯号
	string IMnumber = ms_->GetValueFromMapByKey("IM_number");
	if (userid.empty() || name.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"userid=" +		userid + "\r\n" + 		//28    [92]    //用户ID
		"pass=" + 		password + "\r\n" + 	//21	[14]	//密码 
		"msg=" + 	  	nickname + "\r\n" + 	//63	[128] 	//昵称 
		"mid=" + 	  	name + "\r\n" + 		//56	[121]	//姓名
		"sex=" + 	  	sex + "\r\n" + 			//60	[125]	//性别
		"onick=" + 	  	birthday + "\r\n" + 	//55	[120]	//出生日期
		"islogin=" + 	address + "\r\n" + 		//53	[118]   //地址
		"servername=" + idtype + "\r\n" + 		//49	[116]   //证件类型      
		"appname=" + 	personID + "\r\n" + 	//50	[115]   //证件号 
		"oid=" + 	  	mobile + "\r\n" + 		//54	[119]	//手机号 
		"msgtype=" + 	IMkind + "\r\n" +		//51	[127]	//通讯类型
		"subject=" + 	IMnumber + "\r\n" + 	//52	[46]    //通讯号
		"netmark=" +	app_ + "\r\n" + 		//59	[124]	//来源
		"clientmsg=" +	platformWeb + "\r\n" +	//62	[126]	//操作平台
		"state=注册\r\n";						//64	[19]	//状态
		WriteIndexFile();
	return 1;
}
//登录
int marriage::LoveInHereMarriage::WebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("LoginKind=") != -1)
	{
		src_data = src_data.substr(src_data.rfind("LoginKind="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//用户ID
	string memberid = ms_->GetValueFromMapByKey("memberid");
	if (memberid.empty())
	{
		return 0;
	}
	output_text_ =
		"userid=" + 	memberid + "\r\n" + 		//28    [92]    //用户ID
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//来源
		"clientmsg=" +	platformWeb + "\r\n" +		//62	[126]	//操作平台
		"state=登录\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//web修改资料
int marriage::LoveInHereMarriage::WebModify(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户ID
	string userid = ms_->GetValueFromMapByKey("memberid");
	//昵称
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//姓名
	string name = ms_->GetValueFromMapByKey("name",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("sex")=1?"男":"女";
	//出生日期
	string birthday = ms_->GetValueFromMapByKey("y12") + 
		              ms_->GetValueFromMapByKey("y3") +
					  ms_->GetValueFromMapByKey("y4") + "-" +
					  ms_->GetValueFromMapByKey("m") + "-" +
					  ms_->GetValueFromMapByKey("d");
	//证件类型
	string idtype = "身份证";
 	//身份证号
	string personID = ms_->GetValueFromMapByKey("personID");
	if (personID.empty())
	{
		personID = "未填写";
	}
	//地址
	string address = ms_->GetValueFromMapByKey("address",true);
	//手机号
	string mobile = ms_->GetValueFromMapByKey("tel_mobile");
	//通讯类型
	string IMkind = "QQ/Skype/微信";
	//通讯号
	string IMnumber = ms_->GetValueFromMapByKey("IM_number");

	output_text_ =
		"userid=" +		userid + "\r\n" + 		//28    [92]    //用户ID
		"msg=" + 	  	nickname + "\r\n" + 	//63	[128] 	//昵称 
		"mid=" + 	  	name + "\r\n" + 		//56	[121]	//姓名
		"sex=" + 	  	sex + "\r\n" + 			//60	[125]	//性别
		"onick=" + 	  	birthday + "\r\n" + 	//55	[120]	//出生日期
		"servername=" + idtype + "\r\n" + 		//49	[116]   //证件类型      
		"appname=" + 	personID + "\r\n" + 	//50	[115]   //证件号 
		"islogin=" + 	address + "\r\n" + 		//53	[118]   //地址
		"oid=" + 	  	mobile + "\r\n" + 		//54	[119]	//手机号 
		"msgtype=" + 	IMkind + "\r\n" +		//51	[127]	//通讯类型
		"subject=" + 	IMnumber + "\r\n" + 	//52	[46]    //通讯号
		"netmark=" +	app_ + "\r\n" + 		//59	[124]	//来源
		"clientmsg=" +	platformWeb + "\r\n" +	//62	[126]	//操作平台
		"state=修改个人信息\r\n";				//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
