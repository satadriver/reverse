#include "StdAfx.h"
#include "MarriageJiaYuan.h"

marriage::JiaYuanMarriage::JiaYuanMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "世纪佳缘网";	
}

marriage::JiaYuanMarriage::~JiaYuanMarriage()
{
	
}

int marriage::JiaYuanMarriage::IsJiaYuanMarriage(HttpSession *http_session)
{
	//网页修改资料	
	if((http_session->m_Requestline.m_Host.Find("www.jiayuan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/usercp/profile_postdo_new.php")!=-1)) 
	{
		return kPcWebModiy;
	}
	//安卓手机注册
	if((http_session->m_Requestline.m_Host.Find("api.jiayuan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg/register_basic.php")!=-1)) 
	{
		return kAndroidRegiste;
	}
	return 0;
}

int marriage::JiaYuanMarriage::Process(const char *packet,int action)
{
	//网页修改基本信息
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	//安卓手机注册
	if (action == kAndroidRegiste)
	{
		return AndroidRegiste(packet);
	}
	return 1;
}

//网页修改基本信息
int marriage::JiaYuanMarriage::PcWebModiy(const char *packet)
{
	//从cookie 中提取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//用户名
	string userName = ms_->GetValueFromMapByKey("save_jy_login_name",true);
	//基本信息 
	string proFile = ms_->GetValueFromMapByKey("PROFILE",true);
	//用户ID
	int pos1 = proFile.find(":");
	int pos2 = proFile.find(":",pos1 + 1);
	string userId = proFile.substr(0,pos1);
	//昵称
    string nickName = proFile.substr(pos1,pos2 - pos1); 
    //数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("true_name",true);
	//证件类型
	string cardType = "身份证";
	//身份证号
	string idCard = ms_->GetValueFromMapByKey("id_card");
	//通讯类型
	string connectType = "QQ";
	//qq号
	string qq = ms_->GetValueFromMapByKey("qq");
	//地址
	string address = ms_->GetValueFromMapByKey("address",true);
	//邮编 (备用)
    string postcode = ms_->GetValueFromMapByKey("postcode");

	output_text_ =
		"user=" + userName + "\r\n" +			//20	[13]	//用户名 
		"userid=" + userId + "\r\n" + 			//28    [92]    //用户ID
		"msg=" + 	nickName + "\r\n" + 		//65	[104]   //昵称
		"mid=" + 	name + "\r\n" + 			//56	[121]	//姓名
		"servername=" + cardType + "\r\n" + 	//49	[116]   //证件类型      
		"appname=" + idCard + "\r\n" + 			//50	[115]   //证件号
		"msgtype=" + connectType + "\r\n" +		//51	[127]	//通讯类型
		"subject=" + qq + "\r\n" + 				//52	[46]    //通讯号
		"islogin=" + address + "\r\n" + 		//53	[118]   //地址
		"netmark=" + app_ + "\r\n" + 		    //59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +   //62	[126]	//操作平台
		"state=修改个人信息\r\n";			    //64	[19]	//状态
	WriteIndexFile();
	return 1;
}

//安卓手机注册
int marriage::JiaYuanMarriage::AndroidRegiste(const char *packet)
{
    //数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//密码 
	string password = ms_->GetValueFromMapByKey("password");
	string chkpass = ms_->GetValueFromMapByKey("chkpass");
	if (strcmp(password.c_str(),chkpass.c_str()) != 0)
	{
		return 0;
	}
	//昵称 
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//性别 
	string sex = ms_->GetValueFromMapByKey("sex");
	if (sex == "m")
	{
		sex = "男";
	}
	else 
	{
		sex = "女";
	}
	//出生日期
	string birthdate = ms_->GetValueFromMapByKey("year") + "-" +
					   ms_->GetValueFromMapByKey("month") + "-" +
					   ms_->GetValueFromMapByKey("day");
	if (mobile.empty())
	{
		return 0;
	}
	output_text_ =
		"oid=" +	mobile + "\r\n" + 				//54	[119]	//手机号 
		"pass=" +	password + "\r\n" + 			//21	[14]	//密码 
		"msg=" +	nickname + "\r\n" + 			//65	[104]   //昵称
		"sex=" +	sex + "\r\n" + 					//60	[125]	//性别 
		"onick=" +	birthdate + "\r\n" + 			//55	[120]	//出生日期 
		"netmark=" +	app_ + "\r\n" + 		    //59	[124]	//来源
		"clientmsg=" +	platformAndroid + "\r\n" +  //62	[126]	//操作平台
		"state=注册\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}

