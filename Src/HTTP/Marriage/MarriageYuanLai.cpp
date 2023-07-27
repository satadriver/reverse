#include "StdAfx.h"
#include "MarriageYuanLai.h"

marriage::YuanLaiMarriage::YuanLaiMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "缘来网";	
}

marriage::YuanLaiMarriage::~YuanLaiMarriage()
{
	
}

int marriage::YuanLaiMarriage::IsYuanLaiMarriage(HttpSession *http_session)
{
	//网页登录
	if((http_session->m_Requestline.m_Host.Find("www.yuanlai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login/user-login.do")!=-1)) 
	{
		return kPcWebLogin;
	}
	//网页实名认证
	if((http_session->m_Requestline.m_Host.Find("www.yuanlai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/profile/add-gzt-user-info.do")!=-1)) 
	{
		return kPcWebCheckName;
	}
	return 0;
}

int marriage::YuanLaiMarriage::Process(const char *packet,int action)
{
	//网页登录
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//网页实名认证
	if (action == kPcWebCheckName)
	{
		return PcWebCheckName(packet);
	}
	return 1;
}
//网页登录
int marriage::YuanLaiMarriage::PcWebLogin(const char *packet)
{
    //数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string user = ms_->GetValueFromMapByKey("user",true);
	//密码
	string password = ms_->GetValueFromMapByKey("password");	
	output_text_ =
		"user=" + user + "\r\n" +				//20	[13]	//用户名 
		"pass=" +  password + "\r\n" + 			//21	[14]	//密码 	
		"netmark=" + app_ + "\r\n" + 		    //59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +   //62	[126]	//操作平台
		"state=登录\r\n";			            //64	[19]	//状态
	WriteIndexFile();
	return 1;
}

//网页实名认证
int marriage::YuanLaiMarriage::PcWebCheckName(const char *packet)
{
	//从cookie 中提取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//用户名
	string userId = ms_->GetValueFromMapByKey("userId");
    //数据部分
	string src_data(packet);
	if (src_data.find("idNumber=") != string::npos)
	{
		src_data = src_data.substr(src_data.find("idNumber="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("name",true);
	//证件类型
	string idType = "身份证";
	//证件号
	string idNumber = ms_->GetValueFromMapByKey("idNumber");
	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobile");
	
	output_text_ =
		"userid=" +		userId + "\r\n" + 			//28    [92]    //用户ID
		"mid=" +		name + "\r\n" + 			//56	[121]	//姓名
		"servername=" + idType + "\r\n" + 			//49	[116]   //证件类型      
		"appname=" +	idNumber + "\r\n" + 		//50	[115]   //证件号
		"oid=" +		mobile + "\r\n" + 			//54	[119]	//手机号 
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//操作平台
		"state=实名认证\r\n";						//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
