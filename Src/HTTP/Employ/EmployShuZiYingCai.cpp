#include "StdAfx.h"
#include "EmployShuZiYingCai.h"

employ::ShuZiYingCai::ShuZiYingCai(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "数字英才";	
}

employ::ShuZiYingCai::~ShuZiYingCai()
{
	
}

string employ::ShuZiYingCai::GetIdType(string idType)
{
	if (idType.length()==0){ idType="null";}
	if (idType=="0"){ idType="null";}
	if (idType=="1"){ idType="中国大陆身份证";}
	if (idType=="2"){ idType="军官证";}
	if (idType=="4"){ idType="护照";}
	if (idType=="3"){ idType="其他";}
	return idType;
}

int employ::ShuZiYingCai::IsShuZiYingCai(HttpSession *http_session)
{
	//注册(用户名注册)
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/register.do")!=-1) )
	{
		return kPcWebRegisterUser;
	}
	//登录
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/login.do")!=-1) )
	{
		return kPcWebLogin;
	}
	//基本信息(标准型简历)
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/job/ajax.do?op=baseInfo")!=-1) )
	{
		return kPcWebPaMain;
	}
	//基本信息(简述型简历)
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/job/ajax.do?op=saveSimpleResumeBaseInfo")!=-1) )
	{
		return kPcWebPaSimple;
	}
	//基本信息(微型简历)
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/savePrivateResume.do")!=-1) )
	{
		return kPcWebPaPrivate;
	}
	//上传简历(附件简历)
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/affixAdd.do")!=-1) )
	{
		return kPcWebPaResume;
	}
	//上传头像	
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/newUploadPhoto.do")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	//android注册(用户名注册)
	if((http_session->m_Requestline.m_Host.Find("open.01hr.com:8084")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jobmap/register.do")!=-1) )
	{
		return kAndroidRegisterUser;
	}
	//android登录
	if((http_session->m_Requestline.m_Host.Find("open.01hr.com:8084")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jobmap/login.do")!=-1) )
	{
		return kAndroidLogin;
	}
	return 0;
}

int employ::ShuZiYingCai::Process(const char *packet,int action)
{
	//注册(用户名注册)
	if (action == kPcWebRegisterUser)
	{
		return PcWebRegisterUser(packet);
	}
	//登录
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//基本信息(标准型简历)
	if (action == kPcWebPaMain)
	{
		return PcWebPaMain(packet);
	}
	//基本信息(简述型简历)
	if (action == kPcWebPaSimple)
	{
		return PcWebPaSimple(packet);
	}
	//基本信息(微型简历)
	if (action == kPcWebPaPrivate)
	{
		return PcWebPaPrivate(packet);
	}
	//上传简历(附件简历)
	if (action == kPcWebPaResume)
	{
		return PcWebPaResume(packet);
	}
	//上传头像
	if (action == kPcWebPaPhoto)
	{
		return PcWebPaPhoto(packet);
	}
	//android注册(用户名注册)
	if (action == kAndroidRegisterUser)
	{
		return AndroidRegisterUser(packet);
	}
	//android登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	return 1;
}

//注册(用户名注册)
int employ::ShuZiYingCai::PcWebRegisterUser(const char *packet)
{
	//精确提取数据  
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
    string login = ms_->GetValueFromMapByKey("login",true);
	//密码
	string password = ms_->GetValueFromMapByKey("password",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	if (login.empty() || password.empty() || email.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 	login + "\r\n" +	//账号	
		"pass=" + 	password + "\r\n" + //密码	
		"mid=" + 	email + "\r\n" + 	//邮箱	
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=注册\r\n";
	WriteIndexFile();
	return 1;
}

//登录
int employ::ShuZiYingCai::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//账号
    string UserName = ms_->GetValueFromMapByKey("login",true);
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


//基本信息(标准型简历)
int employ::ShuZiYingCai::PcWebPaMain(const char *packet)
{
	//从referer 中获得数据
	BOOL falg;
	//修改	http://my.01hr.com/resume/modify.do?id=283996651
	//新增	http://my.01hr.com/resume/modify.do?from=reg
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
    if (referer.find("from=") != string::npos)	
    {
		falg = TRUE;
    }
	if (referer.find("id=") != string::npos)	
	{
		falg = FALSE;
	}
	//数据部分
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("name",true);
	//性别
	string gender = ms_->GetValueFromMapByKey("gender",true);
	if (gender == "1")
	{
		gender = "男";
	}
	if (gender == "2")
	{
		gender = "女";
	}
	//出生年月		//199101
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	//现居住地		//3208
	string address = ms_->GetValueFromMapByKey("livingPlaceId",true);
	//手机号码
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//证件类型
	string idTypeTemp = ms_->GetValueFromMapByKey("idType",true);
    string idType = GetIdType(idTypeTemp);
	//证件号
	string idCard = ms_->GetValueFromMapByKey("idCard",true);
	if (idCard.length() == 0)
	{
		idCard = "null";
	}
	if (falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"nick="	 + 	  	address + "\r\n" + 	//地址		
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"imsi=" + 	 	idType + "\r\n" + 	//证件类型
			"appname=" + 	 idCard + "\r\n" + 	//证件号
			"netmark=" +	app_ + "\r\n" + 	//来源
			"state=新增基本信息\r\n";
	}
	if (!falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"nick="	 + 	  	address + "\r\n" + 	//地址		
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"imsi=" + 	 	idType + "\r\n" + 	//证件类型
			"appname=" + 	 idCard + "\r\n" + 	//证件号
			"netmark=" +	app_ + "\r\n" + 	//来源
			"state=修改基本信息\r\n";
	}
	WriteIndexFile();    
	return 1;
}


//基本信息(简述型简历)
int employ::ShuZiYingCai::PcWebPaSimple(const char *packet)
{
	//从referer 中获得数据
	BOOL falg = FALSE;
	//修改	http://my.01hr.com/resume/simpleResumePreModify.do?id=30279
	//新增	http://my.01hr.com/resume/modify.do?from=reg
// 	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
//     if (referer.find("from=") != string::npos)	
//     {
// 		falg = TRUE;
//     }
// 	if (referer.find("id=") != string::npos)	
// 	{
// 		falg = FALSE;
// 	}
	//数据部分
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("baseInfoName",true);
	//性别
	string gender = ms_->GetValueFromMapByKey("gender",true);
	if (gender == "1")
	{
		gender = "男";
	}
	if (gender == "2")
	{
		gender = "女";
	}
	//出生年月		//199101
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	//现居住地		//3208
	string address = ms_->GetValueFromMapByKey("livingPlaceId",true);
	//手机号码
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//证件类型
	string idTypeTemp = ms_->GetValueFromMapByKey("idType",true);
    string idType = GetIdType(idTypeTemp);
	//证件号
	string idCard = ms_->GetValueFromMapByKey("idCard",true);
	if (idCard.length() == 0)
	{
		idCard = "null";
	}
	if (falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"nick="	 + 	  	address + "\r\n" + 	//地址		
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"imsi=" + 	 	idType + "\r\n" + 	//证件类型
			"appname=" + 	 idCard + "\r\n" + 	//证件号
			"netmark=" +	app_ + "\r\n" + 	//来源
			"state=新增基本信息\r\n";
	}
	if (!falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"nick="	 + 	  	address + "\r\n" + 	//地址		
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"imsi=" + 	 	idType + "\r\n" + 	//证件类型
			"appname=" + 	 idCard + "\r\n" + 	//证件号
			"netmark=" +	app_ + "\r\n" + 	//来源
			"state=修改基本信息\r\n";
	}
	WriteIndexFile();    
	return 1;
}


//基本信息(微型简历)
int employ::ShuZiYingCai::PcWebPaPrivate(const char *packet)
{
	//从referer 中获得数据
	BOOL falg = FALSE;
	//修改	http://my.01hr.com/resume/editPrivateResume.do
	//新增	http://my.01hr.com/resume/modify.do?from=reg
// 	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
//     if (referer.find("from=") != string::npos)	
//     {
// 		falg = TRUE;
//     }
// 	if (referer.find("id=") != string::npos)	
// 	{
// 		falg = FALSE;
// 	}
	//数据部分
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("name",true);
	//性别
	string gender = ms_->GetValueFromMapByKey("gender",true);
	if (gender == "1")
	{
		gender = "男";
	}
	if (gender == "2")
	{
		gender = "女";
	}
	//出生年月		//199101
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	//现居住地		//3208
	string address = ms_->GetValueFromMapByKey("livingPlace",true);
	//手机号码
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//证件类型
	string idTypeTemp = ms_->GetValueFromMapByKey("idType",true);
    string idType = GetIdType(idTypeTemp);
	//证件号
	string idCard = ms_->GetValueFromMapByKey("idCard",true);
	if (idCard.length() == 0)
	{
		idCard = "null";
	}
	if (falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"nick="	 + 	  	address + "\r\n" + 	//地址		
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"imsi=" + 	 	idType + "\r\n" + 	//证件类型
			"appname=" + 	idCard + "\r\n" + 	//证件号
			"netmark=" +	app_ + "\r\n" + 	//来源
			"state=新增基本信息\r\n";
	}
	if (!falg)
	{
		string 
			output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"nick="	 + 	  	address + "\r\n" + 	//地址		
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"imsi=" + 	 	idType + "\r\n" + 	//证件类型
			"appname=" + 	idCard + "\r\n" + 	//证件号
			"netmark=" +	app_ + "\r\n" + 	//来源
			"state=修改基本信息\r\n";
	}
	WriteIndexFile();    
	return 1;
}

//上传简历(附件简历)
int employ::ShuZiYingCai::PcWebPaResume(const char *packet)
{	
	from_ = "数字英才null";
	
	//文件部分
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"resourcefile\"");          
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

//上传头像
int employ::ShuZiYingCai::PcWebPaPhoto(const char *packet)
{	
	from_ = "数字英才null";
	//文件部分
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"photoFile\"");          
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

//android注册(用户名注册)
int employ::ShuZiYingCai::AndroidRegisterUser(const char *packet)
{
	//精确提取数据  
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
    string login = ms_->GetValueFromMapByKey("login",true);
	//密码
	string password = ms_->GetValueFromMapByKey("password",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	if (login.empty() || password.empty() || email.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 	login + "\r\n" +	//账号	
		"pass=" + 	password + "\r\n" + //密码	
		"mid=" + 	email + "\r\n" + 	//邮箱	
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=注册\r\n";
	WriteIndexFile();
	return 1;
}

//android登录
int employ::ShuZiYingCai::AndroidLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//账号
    string UserName = ms_->GetValueFromMapByKey("login",true);
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
