#include "StdAfx.h"
#include "MarriageHuaTian.h"

marriage::HuaTianMarriage::HuaTianMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "网易花田";	
}

marriage::HuaTianMarriage::~HuaTianMarriage()
{
	
}

int marriage::HuaTianMarriage::IsHuaTianMarriage(HttpSession *http_session)
{
	//web修改资料
	if((http_session->m_Requestline.m_Host.Find("love.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/updateUser")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/api/user/updateUser") ==-1)) 
	{
		return kWebModify;
	}
	//android修改资料
	if((http_session->m_Requestline.m_Host.Find("love.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/user/updateNew")!=-1)) 
	{
		return kAndroidModify;
	}
	//android实名认证
	if((http_session->m_Requestline.m_Host.Find("love.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wap/user/verifyName")!=-1)) 
	{
		return kAndroidRealName;
	}	
	//android定位 1 
	if((http_session->m_Requestline.m_Host.Find("love.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/home/allNearby")!=-1)) 
	{
		return kAndroidLocation;
	}	
	//android定位 2	
	if((http_session->m_Requestline.m_Host.Find("love.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/location/accessLocation")!=-1)) 
	{
		return kAndroidLocation2;
	}	
	return 0;
}

int marriage::HuaTianMarriage::Process(const char *packet,int action)
{
	//web修改资料
	if (action == kWebModify)
	{
		return WebModify(packet);
	}
	//android修改资料
	if (action == kAndroidModify)
	{
		return AndroidModify(packet);
	}
	//android实名认证
	if (action == kAndroidRealName)
	{
		return AndroidRealName(packet);
	}	
	//android定位 1
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	//android定位 2
	if (action == kAndroidLocation2)
	{
		return AndroidLocation2(packet);
	}
	return 1;
}

//web修改资料
int marriage::HuaTianMarriage::WebModify(const char *packet)
{
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0);
	ms_->SplitStringToMap(cookie,';','=');
	string P_INFO = ms_->GetValueFromMapByKey("P_INFO",true);
	string usrName;
	if (P_INFO.find("|") != string::npos)
	{
		usrName = P_INFO.substr(0,P_INFO.find("|"));
	}
    //数据部分
	string src_data(packet);
	if (src_data.rfind("\r\n") != -1)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	string nickname = ms_->GetValueFromMapByKey("nickName",true);
	//出生日期
	string birthdate = ms_->GetValueFromMapByKey("birthday");
	
	output_text_ =
		"user=" + 		usrName + "\r\n" +			//20	[13]	//用户名 
		"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //昵称
		"onick=" + 	  	birthdate + "\r\n" + 		//55	[120]	//出生日期 
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//来源
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//操作平台
		"state=修改基本信息\r\n";					//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//android修改资料
int marriage::HuaTianMarriage::AndroidModify(const char *packet)
{
    //数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	string nickname = ms_->GetValueFromMapByKey("nickName",true);
	//出生日期
	string birthdate = ms_->GetValueFromMapByKey("birthday");
	
	output_text_ =
		"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //昵称
		"onick=" + 	  	birthdate + "\r\n" + 		//55	[120]	//出生日期 
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//来源
		"clientmsg=" + platformAndroid + "\r\n" +   //62	[126]	//操作平台
		"state=修改基本信息\r\n";					//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//android实名认证
int marriage::HuaTianMarriage::AndroidRealName(const char *packet)
{
    //数据部分
	string src_data(packet);
	if (src_data.find("realName=") != string::npos)
	{
		src_data = src_data.substr(src_data.find("realName="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("realName",true);
	//证件类型
	string idType = "身份证";
	//证件号
	string idNumber = ms_->GetValueFromMapByKey("idCard");
	
	output_text_ =
		"mid=" +		name + "\r\n" + 				//56	[121]	//姓名
		"servername=" + idType + "\r\n" + 				//49	[116]   //证件类型      
		"appname=" +	idNumber + "\r\n" + 			//50	[115]   //证件号
		"netmark=" + app_ + "\r\n" + 					//59	[124]	//来源
		"clientmsg=" + platformAndroid + "\r\n" +		//62	[126]	//操作平台
		"state=实名认证\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}

//android定位
int marriage::HuaTianMarriage::AndroidLocation(const char *packet)
{
    //数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');	
	//经度
	string longitude = ms_->GetValueFromMapByKey("longitude",true);
	//纬度
	string latitude = ms_->GetValueFromMapByKey("latitude");
	if (longitude.empty() || latitude.empty())
	{
		return 0;
	}
	output_text_ =
		"from=" + 	 longitude + "\r\n" + 			//37	[41]  	//经度  
		"to=" + 	 latitude + "\r\n" + 			//38	[42]    //纬度	
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//来源
		"clientmsg=" + platformAndroid + "\r\n" +   //62	[126]	//操作平台
		"state=定位\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
//android定位 2
int marriage::HuaTianMarriage::AndroidLocation2(const char *packet)
{
    //数据部分
	string src_data(packet);
	if (src_data.find("latitude=") != string::npos)
	{
		src_data = src_data.substr(src_data.find("latitude="));
	}
	ms_->SplitStringToMap(src_data,'&','=');	
	//经度
	string longitude = ms_->GetValueFromMapByKey("longitude",true);
	//纬度
	string latitude = ms_->GetValueFromMapByKey("latitude");
	if (longitude.empty() || latitude.empty())
	{
		return 0;
	}
	output_text_ =
		"from=" + 	 longitude + "\r\n" + 			//37	[41]  	//经度  
		"to=" + 	 latitude + "\r\n" + 			//38	[42]    //纬度	
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//来源
		"clientmsg=" + platformAndroid + "\r\n" +   //62	[126]	//操作平台
		"state=定位\r\n";							//64	[19]	//状态
	WriteIndexFile();
	return 1;
}
