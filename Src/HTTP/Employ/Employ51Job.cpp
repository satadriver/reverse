#include "StdAfx.h"
#include "Employ51Job.h"

employ::QianChengWuYou::QianChengWuYou(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "前程无忧";
}

employ::QianChengWuYou::~QianChengWuYou()
{
	
}

string employ::QianChengWuYou::GetIdType(string idType)
{
	if (idType=="0"){ idType="身份证";}
	if (idType=="1"){ idType="护照";}
	if (idType=="2"){ idType="军人证";}
	if (idType=="4"){ idType="香港身份证";}
	if (idType=="3"){ idType="其他";}
	return idType;
}

int employ::QianChengWuYou::IsQianChengWuYou(HttpSession *http_session)
{
	//注册个人信息
	if((http_session->m_Requestline.m_Host.Find("i.51job.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/ajax/tri_resume.php")!=-1) )
	{
		return kPcWebInfo;
	}
	//修改个人信息
	if((http_session->m_Requestline.m_Host.Find("i.51job.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/ajax/base_detail.php?action=save")!=-1) )
	{
		return kPCWebModiy;
	}
	//登录
	if (http_session->m_Requestline.m_Host.Find("login.51job.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/ajax/login.php") != -1)
	{
		return kPcLogin;
	}
	//上传头像	
	if((http_session->m_Requestline.m_Host.Find("i.51job.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/ajax/avatar.php")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	return 0;
}

int employ::QianChengWuYou::Process(const char *packet,int action)
{
	//个人信息
	if (action == kPcWebInfo)
	{
		return PcWebInfo(packet);
	}
	//修改个人信息
	if (action==kPCWebModiy)
	{
		return PCWebModiy(packet);
	}
	//登录
	if (action == kPcLogin)
	{
		return PcLogin(packet);
	}
	//上传头像
	if (action == kPcWebPaPhoto)
	{
		return PcWebPaPhoto(packet);
	}
	return 1;
}

//个人信息
int employ::QianChengWuYou::PcWebInfo(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("cname",true);
	//性别
	string gender = ms_->GetValueFromMapByKey("sex",true);
	if (gender == "0")
	{
		gender = "男";
	}
	else
	{
		gender ="女";
	}
	//出生日期
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobilephone",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	if (name.empty() || birth.empty() || mobile.empty() || email.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + name + "\r\n" + 	//姓名	       
		"sex=" + 	gender + "\r\n" + 	//性别	
		"msg=" + 	birth + "\r\n" + 	//出生日期	
		"oid=" + 	mobile + "\r\n" + 	//手机号	
		"mid=" + 	email + "\r\n" + 	//邮箱
		"netmark=" + app_ + "\r\n" + 	//来源	
		"state=新建基本信息\r\n";
	WriteIndexFile();
	return 1;
}

int employ::QianChengWuYou::PCWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string name = ms_->GetValueFromMapByKey("cname",true);
	//性别
	string gender = ms_->GetValueFromMapByKey("sex",true);
	if (gender == "0"){gender = "男";}
	else{ gender ="女";}
	//出生日期
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	//手机
	string mobile = ms_->GetValueFromMapByKey("mobilephone",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	if (name.empty() || mobile.empty())
	{
		return 0;
	}
	output_text_=
		"servername=" + name + "\r\n" + //姓名	       
		"sex=" + 	gender + "\r\n" + 	//性别	
		"msg=" + 	birth + "\r\n" + 	//出生日期	
		"oid=" + 	mobile + "\r\n" + 	//手机号	
		"mid=" + 	email + "\r\n" + 	//邮箱
		"netmark=" + app_ + "\r\n" + 	//来源	
		"state=修改基本信息\r\n";    
	WriteIndexFile();
	return 1;
}

//登录
int employ::QianChengWuYou::PcLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,"&","=");
	//用户名
	string username = ms_->GetValueFromMapByKey("loginname",true);
	//密码
	string password = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_=
		"user=" +    username + "\r\n" + //账号		
		"pass=" + 	 password + "\r\n" + //密码
		"netmark=" + app_ + "\r\n" + 	 //来源	
		"state=登录\r\n";    
	WriteIndexFile();
	return 0;
}

//上传头像
int employ::QianChengWuYou::PcWebPaPhoto(const char *packet)
{	
	//cname=东方喻晓&|
	//从cookie 中获得数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	from_ = ms_->GetValueBetweenBeginEnd(cookie,"cname=","&|",true);
	//文件部分
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"avatar\"");          
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
