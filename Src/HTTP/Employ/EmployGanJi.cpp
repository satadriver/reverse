#include "StdAfx.h"
#include "EmployGanJi.h"

employ::GanJi::GanJi(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "赶集网";	
}

employ::GanJi::~GanJi()
{
	
}

int employ::GanJi::IsGanJi(HttpSession *http_session)
{
	//注册(邮箱注册)
	if((http_session->m_Requestline.m_Host.Find("www.ganji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/register.php?method=phone_reg")!=-1) )
	{
		return kPcWebRegisterPhone;
	}
	//基本信息
	if((http_session->m_Requestline.m_Host.Find("www.ganji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax.php?module=pub_post_auto_save&act=add")!=-1) ) 
	{
		return kPcWebPaMain;
	}
	//修改基本信息	 
	if((http_session->m_Requestline.m_Host.Find("www.ganji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pub/pub.php?")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("act=update&method=submit")!=-1)) 
	{
		return kPcWebResume;
	}
	//上传头像	
	if((http_session->m_Requestline.m_Host.Find("upload.ganji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/upload.php")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	return 0;
}

int employ::GanJi::Process(const char *packet,int action)
{
	//邮箱注册
	if (action == kPcWebRegisterPhone)
	{
		return PcWebRegisterPhone(packet);
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
	return 1;
}

//注册(手机号注册)
int employ::GanJi::PcWebRegisterPhone(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string username = ms_->GetValueFromMapByKey("username",true);
	//手机号
    string phone = ms_->GetValueFromMapByKey("phone",true);
	//密码
	string phone_password = ms_->GetValueFromMapByKey("phone_password",true);
	//确认密码
	string phone_password2 = ms_->GetValueFromMapByKey("phone_password2",true);
	if (phone_password != phone_password2)
	{
		return 0;
	}
	if (phone.empty() || phone_password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +		    //账号	
		"oid=" + phone + "\r\n" + 				//手机号	
		"pass=" + 	phone_password + "\r\n" + 	//密码
		"netmark=" + app_ + "\r\n" + 			//来源
		"state=注册\r\n";
	WriteIndexFile();
	return 1;
}

//基本信息
int employ::GanJi::PcWebPaMain(const char *packet)
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
	//需要的信息
	string post_info = ms_->GetValueFromMapByKey("post_info",true);
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	string name,birth,mobile,email;
	if (reader.parse(post_info,value))
	{	
		//姓名
		name = value["person"].asString();
		//出生日期
		birth = value["birthdate_year"].asString() + "-" + value["birthdate_month"].asString();
		//手机号码
		mobile = value["phone"].asString();
		//邮箱
		email = value["email"].asString();
		if (email.empty())
		{
			email = "null";
		}
	}
	output_text_ =
		"servername=" + name + "\r\n" + 	//姓名	              
		"msg=" + 	  	birth + "\r\n" + 	//出生日期	
		"oid=" + 	  	mobile + "\r\n" + 	//手机号	
		"mid=" + 	  	email + "\r\n" + 	//邮箱	
		"netmark=" + app_ + "\r\n" + 		//来源
		"state=新建基本信息\r\n";
	WriteIndexFile();    
	return 1;
}

//修改基本信息
int employ::GanJi::PcWebResume(const char *packet)
{
	//数据部分
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//boundary 直接提取
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;	 
	//姓名
	string name = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"person\"\r\n\r\n",boundary,true);
	//手机号
	string mobile = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"phone\"\r\n\r\n",boundary,true);
	//出生日期
	string birth = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"birthdate_year\"\r\n\r\n",boundary,true) + "-" + 
				   ms_-> GetValueBetweenBeginEnd(src_data,"name=\"birthdate_month\"\r\n\r\n",boundary,true);
	//邮箱
	string email = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"email\"\r\n\r\n",boundary,true);

	output_text_ =
		"servername=" + name + "\r\n" + 	//姓名	        
		"msg=" + 	  	birth + "\r\n" + 	//出生日期
		"oid=" + 	  	mobile + "\r\n" + 	//手机号	
		"mid=" + 	  	email + "\r\n" + 	//邮箱
		"netmark=" +	app_ + "\r\n" + 	//来源
		"state=修改基本信息\r\n";
	WriteIndexFile();    
	return 1;
}

//上传头像
int employ::GanJi::PcWebPaPhoto(const char *packet)
{	
	from_ = "赶集网null";
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
