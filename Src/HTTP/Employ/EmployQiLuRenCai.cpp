#include "StdAfx.h"
#include "EmployQiLuRenCai.h"

employ::QiLuRenCai::QiLuRenCai(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "齐鲁人才网";	
}

employ::QiLuRenCai::~QiLuRenCai()
{
	
}

int employ::QiLuRenCai::IsQiLuRenCai(HttpSession *http_session)
{
	//注册(邮箱注册)
	if((http_session->m_Requestline.m_Host.Find("www.qlrc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Personal/sys/Register")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//注册(手机号注册)
	if((http_session->m_Requestline.m_Host.Find("www.qlrc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Personal/sys/MobileRegister")!=-1) )
	{
		return kPcWebRegisterPhone;
	}
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.qlrc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/personal/sys/login")!=-1) )
	{
		return kPcWebLogin;
	}
	//基本信息
	if((http_session->m_Requestline.m_Host.Find("www.qlrc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Personal/Account/SavePaMain")!=-1) )
	{
		return kPcWebPaMain;
	}
	//上传头像	
	if((http_session->m_Requestline.m_Host.Find("down.51rc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ImagesManager/PaPhotoProc2.asp")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	//上传简历 
	if((http_session->m_Requestline.m_Host.Find("down.51rc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ImagesManager/AttachmentQuickProcNew.asp")!=-1) )
	{
		return kPcWebPaResume;
	}
	return 0;
}

int employ::QiLuRenCai::Process(const char *packet,int action)
{
	//注册(邮箱注册)
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//注册(手机号注册)
	if (action == kPcWebRegisterPhone)
	{
		return PcWebRegisterPhone(packet);
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
	//上传头像
	if (action == kPcWebPaPhoto)
	{
		return PcWebPaPhoto(packet);
	}
	//上传简历
	if (action == kPcWebPaResume)
	{
		return PcWebPaResume(packet);
	}
	return 1;
}

//注册(邮箱注册)
int employ::QiLuRenCai::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//邮箱
    string txtEmail = ms_->GetValueFromMapByKey("txtEmail",true);
	//密码
	string txtPassword = ms_->GetValueFromMapByKey("txtPassword",true);
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

//注册(手机号注册)
int employ::QiLuRenCai::PcWebRegisterPhone(const char *packet)
{
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//手机号
    string txtMobile = ms_->GetValueFromMapByKey("txtMobile",true);
	//密码
	string txtMobilePassword = ms_->GetValueFromMapByKey("txtMobilePassword",true);
	if (txtMobile.empty() || txtMobilePassword.empty())
	{
		return 0;
	}
	output_text_ =
		"oid=" + txtMobile + "\r\n" + 			//手机号		
		"pass=" + txtMobilePassword + "\r\n" + 	//密码
		"netmark=" + app_ + "\r\n" + 			//来源
		"state=注册\r\n";
	WriteIndexFile();
	return 1;
}

//登录
int employ::QiLuRenCai::PcWebLogin(const char *packet)
{
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//账号
    string UserName = ms_->GetValueFromMapByKey("UserName",true);
	//密码
	string Password = ms_->GetValueFromMapByKey("Password",true);
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
int employ::QiLuRenCai::PcWebPaMain(const char *packet)
{
	//从cookie 中获得数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');	
	string PaLoginID = ms_->GetValueFromMapByKey("PaLoginID",true);
	//http://www.qlrc.com/personal/cv/stepOne/26718570
	//http://www.qlrc.com/Personal/cv/Resume/26718465
	//从referer 中获得数据
	BOOL falg;
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
    if (referer.find("stepOne") != string::npos)
    {
		falg = TRUE;
    }
	if (referer.find("Resume") != string::npos)
	{
		falg = FALSE;
	}
	//数据部分
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("Name",true);
	//性别
	string gender = ms_->GetValueFromMapByKey("Gender",true);
	if (gender == "0")
	{
		gender = "男";
	}
	if (gender == "1")
	{
		gender = "女";
	}
	//出生年月		//199101
	string birth = ms_->GetValueFromMapByKey("BirthDay",true);
	//现居住地		//3208
	string address = ms_->GetValueFromMapByKey("LivePlace",true);
	//户口所在地	//320802
	//address_ = ms_->GetValueFromMapByKey("AccountPlace",true);
	//成长地		//3208
	//string GrowPlace = ms_->GetValueFromMapByKey("GrowPlace",true);
	//手机号码
	string mobile = ms_->GetValueFromMapByKey("Mobile",true);
	//电子邮箱
	string email = ms_->GetValueFromMapByKey("Email",true);
	if (email.empty())
	{
		email = "null";
	}
	if (falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"nick="	 + 	  	address + "\r\n" + 	//地址		
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"mid=" + 	  	email + "\r\n" + 	//邮箱		
			"netmark=" +	app_ + "\r\n" + 	//来源
			"state=修改基本信息\r\n";
	}
	if (!falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"nick="	 + 	  	address + "\r\n" + 	//地址		
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"mid=" + 	  	email + "\r\n" + 	//邮箱	
			"netmark=" +	app_ + "\r\n" + 	//来源
			"state=修改基本信息\r\n";
	}
	WriteIndexFile();    
	return 1;
}

//上传头像
int employ::QiLuRenCai::PcWebPaPhoto(const char *packet)
{	
	//从uri中提取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string userTemp = ms_->GetValueFromMapByKey("un", true);
	from_ = userTemp.substr(0,userTemp.find("_"));

	//文件部分
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"upfile\"");          
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

//上传简历
int employ::QiLuRenCai::PcWebPaResume(const char *packet)
{	
	//从uri中提取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	from_ = ms_->GetValueFromMapByKey("paMainID", true);
	
	//文件部分
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"upfile\"");          
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
