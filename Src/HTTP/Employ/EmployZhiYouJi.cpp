#include "StdAfx.h"
#include "EmployZhiYouJi.h"

employ::ZhiYouJi::ZhiYouJi(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "职友集";	
}

employ::ZhiYouJi::~ZhiYouJi()
{
	
}

int employ::ZhiYouJi::IsZhiYouJi(HttpSession *http_session)
{
	//基本信息
	if((http_session->m_Requestline.m_Host.Find("www.ZhiYouJi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax.php?module=pub_post_auto_save&act=add")!=-1)) 
	{
		return kPcWebPaMain;
	}
	//修改基本信息 
	if((http_session->m_Requestline.m_Host.Find("www.jobui.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/process/")!=-1)) 
	{
		return kPcWebResume;
	}
	return 0;
}

int employ::ZhiYouJi::Process(const char *packet,int action)
{
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
	return 1;
}

//基本信息
int employ::ZhiYouJi::PcWebPaMain(const char *packet)
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
int employ::ZhiYouJi::PcWebResume(const char *packet)
{
	//判断 上传图片 还是 基本信息	
	//从referer 中提取数据
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
	if (referer.find("photo") == string::npos)
	{
		//精确提取数据  
		string src_data(packet);
		if (src_data.rfind("\r\n") != 0)
		{
			src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
		}
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');
		string formAction = ms_->GetValueFromMapByKey("formAction",true);
		//个人信息 姓名 地址
		if (!strcmp("user_profile_editInfo",formAction.c_str()))
		{
			string name = ms_->GetValueFromMapByKey("userName",true);
			//所在地区 (城市编码)
			string address = ms_->GetValueFromMapByKey("cityCode",true);
			output_text_ =
				"servername=" + name + "\r\n" + 	//姓名	        
				"nick="	 + 	 address + "\r\n" +     //地址		
				"netmark=" + app_ + "\r\n" + 	    //来源
				"state=修改基本信息\r\n";
			WriteIndexFile();    
		}
		//联系信息 
		if (!strcmp("user_profile_editContact",formAction.c_str()))
		{
			//手机号
			string mobile = ms_->GetValueFromMapByKey("phone",true);
			//邮箱
			string email = ms_->GetValueFromMapByKey("email",true);
			output_text_ =
				"oid=" + 	 mobile + "\r\n" + 	//手机号
				"mid=" + 	 email + "\r\n" + 	//邮箱	
				"netmark=" + app_ + "\r\n" + 	//来源
				"state=修改基本信息\r\n";
			WriteIndexFile();    
		}  
	}
	//上传头像
	if (referer.find("photo") != string::npos)
	{
		from_ = "职友集null";
		//文件部分
		string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
		BoundaryParser bp;
		bp.Parse(packet, http_session_->m_datalen, boundary); 
		BoundaryValue *bv = NULL;
		string key;
		key = bp.GetBoundaryKey("name=\"userPhoto\"");          
		bv = bp.GetBoundaryValue(key);	
		if (bv == NULL || key.empty())
		{
			return 0;
		}
		attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
		string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
		ConstructUploadAttachmentOuptputString();
		WriteIndexFile(real_file_name);
	}
	return 1;
}


