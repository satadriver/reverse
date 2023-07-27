#include "StdAfx.h"
#include "EmployRuoLin.h"

employ::RuoLin::RuoLin(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "若邻网";
}

employ::RuoLin::~RuoLin()
{
	
}

int employ::RuoLin::IsRuoLin(HttpSession *http_session)
{
	//注册(邮箱注册)
	if((http_session->m_Requestline.m_Host.Find("www.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/passport/register/registerEmail")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/passport/login")!=-1) )
	{
		return kPcWebLogin;
	}
	//基本信息
	if((http_session->m_Requestline.m_Host.Find("www.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/simple/basicSaveAjax")!=-1) ) 
	{
		return kPcWebPaMain;
	}
	//修改基本信息	 
	if((http_session->m_Requestline.m_Host.Find("www.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/basic/saveAjax")!=-1) ) 
	{
		return kPcWebResume;
	}
	//上传头像	
	if((http_session->m_Requestline.m_Host.Find("upload.qiniu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	//android登录
	if((http_session->m_Requestline.m_Host.Find("api5.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Passport/login")!=-1) )
	{
		return kAndroidLogin;
	}
	//android修改基本信息
	if((http_session->m_Requestline.m_Host.Find("m.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/info/save")!=-1) ) 
	{
		return kAndroidResume;
	}
	return 0;
}

int employ::RuoLin::Process(const char *packet,int action)
{
	//邮箱注册
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//登录
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//基本信息
	if (action == kPcWebPaMain)
	{
		return PcWebPaMain(packet);
	}
	//修改基本信息
	if (action == kPcWebResume)
	{
		return PcWebResume(packet);
	}
	//上传头像
	if (action == kPcWebPaPhoto)
	{
		return PcWebPaPhoto(packet);
	}
	//android登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android修改基本信息
	if (action == kAndroidResume)
	{
		return AndroidResume(packet);
	}
	return 1;
}

//注册(邮箱注册)
int employ::RuoLin::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//邮箱
    string txtEmail = ms_->GetValueFromMapByKey("email",true);
	//密码
	string txtPassword = ms_->GetValueFromMapByKey("password",true);
	if (txtEmail.empty() || txtPassword.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=" + txtEmail + "\r\n" + 		//邮箱		
		"pass=" + 	txtPassword + "\r\n" + 	//密码
		"netmark=" + app_ + "\r\n" + 		//来源
		"state=注册\r\n";
	WriteIndexFile();
	return 1;
}

//登录
int employ::RuoLin::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//账号
    string UserName = ms_->GetValueFromMapByKey("username",true);
	//密码
	string Password = ms_->GetValueFromMapByKey("password",true);
	if (UserName.empty() || Password.empty())
	{
		return 0;
	}
	output_text_ =
		"user="  + UserName + "\r\n" + 	//账号		
		"pass=" + Password + "\r\n" + 	//密码
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}


//基本信息
int employ::RuoLin::PcWebPaMain(const char *packet)
{
	//数据部分
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("data[0][user_name]",true);
	//现居住地		//3208
	string address = ms_->GetValueFromMapByKey("data[0][living_city_code]",true);
	//手机号码
	string mobile = ms_->GetValueFromMapByKey("data[0][mobile]",true);
	output_text_ =
		"servername=" + name + "\r\n" + 	//姓名	       
		"nick="	 + 	  	address + "\r\n" + 	//地址	
		"oid=" + 	  	mobile + "\r\n" + 	//手机号
		"netmark=" +	app_ + "\r\n" + 	//来源
		"state=新建基本信息\r\n";
	WriteIndexFile();    
	return 1;
}

//修改基本信息
int employ::RuoLin::PcWebResume(const char *packet)
{
	//数据部分
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("user_name",true);
	//性别
	string gender = ms_->GetValueFromMapByKey("sex",true);
	if (gender == "M")
	{
		gender = "男";
	}
	if (gender == "F")
	{
		gender = "女";
	}
	//出生年月		//199101
	string birth = ms_->GetValueFromMapByKey("birth_year",true) + "-" + 
				ms_->GetValueFromMapByKey("birth_mon",true) + "-" + 
				ms_->GetValueFromMapByKey("birth_day",true);
	//现居住地		//3208
	string address = ms_->GetValueFromMapByKey("living_city_code",true);
	//户口所在地	//320802
	//address_ = ms_->GetValueFromMapByKey("birth_city_code",true);
	//成长地		//3208
	//string GrowPlace = ms_->GetValueFromMapByKey("GrowPlace",true);
	output_text_ =
		"servername=" + name + "\r\n" + 	//姓名	       
		"sex=" + 	  	gender + "\r\n" + 	//性别		
		"msg=" + 	  	birth + "\r\n" + 	//出生日期	
		"nick="	 + 	  	address + "\r\n" + 	//地址	
		"netmark=" +	app_ + "\r\n" + 	//来源
		"state=修改基本信息\r\n";
	WriteIndexFile();    
	return 1;
}

//上传头像
int employ::RuoLin::PcWebPaPhoto(const char *packet)
{	
	from_ = "若邻网null";
	//文件部分
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"file\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}

//android登录
int employ::RuoLin::AndroidLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//账号
    string UserName = ms_->GetValueFromMapByKey("username",true);
	//密码
	string Password = ms_->GetValueFromMapByKey("password",true);
	if (UserName.empty() || Password.empty())
	{
		return 0;
	}
	output_text_ =
		"user="  + UserName + "\r\n" + 	//账号		
		"pass=" + Password + "\r\n" + 	//密码
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}

//android修改基本信息
int employ::RuoLin::AndroidResume(const char *packet)
{
	//数据部分
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("user_name",true);
	//性别
	string gender = ms_->GetValueFromMapByKey("sex",true);
	if (gender == "M")
	{
		gender = "男";
	}
	if (gender == "F")
	{
		gender = "女";
	}
	//出生年月		//199101
	string birth = ms_->GetValueFromMapByKey("birth",true);
	//现居住地		//3208
	string address = ms_->GetValueFromMapByKey("living_city_code",true);
	//户口所在地	//320802
	//address_ = ms_->GetValueFromMapByKey("birth_city_code",true);
	//成长地		//3208
	//string GrowPlace = ms_->GetValueFromMapByKey("GrowPlace",true);
	output_text_ =
		"servername=" + name + "\r\n" + 	//姓名	       
		"sex=" + 	  	gender + "\r\n" + 	//性别		
		"msg=" + 	  	birth + "\r\n" + 	//出生日期	
		"nick="	 + 	  	address + "\r\n" + 	//地址	
		"netmark=" +	app_ + "\r\n" + 	//来源
		"state=修改基本信息\r\n";
	WriteIndexFile();    
	return 1;
}
