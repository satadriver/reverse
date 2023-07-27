#include "StdAfx.h"
#include "EmployZhiTong.h"

employ::ZhiTong::ZhiTong(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "智通人才网";
}

employ::ZhiTong::~ZhiTong()
{
	
}

int employ::ZhiTong::IsZhiTong(HttpSession *http_session)
{
	//注册个人信息
	if((http_session->m_Requestline.m_Host.Find("www.job5156.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("per/new/step1/save")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//修改个人信息
	if((http_session->m_Requestline.m_Host.Find("my.job5156.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/per/resume")!=-1) )
	{
		return kPCWebModiy;
	}
	return 0;
}
int employ::ZhiTong::Process(const char *packet,int action)
{
	//注册个人信息
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//修改基本信息
	if (action==kPCWebModiy)
	{
		return PCWebModiy(packet);
	}
	return 1;
}

//注册个人信息
int employ::ZhiTong::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("userName",true);
	string sex = ms_->GetValueFromMapByKey("gender",true);
	if (sex == "1"){sex ="男";}
	else{sex = "女";}
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	string location = ms_->GetValueFromMapByKey("name_location",true);
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	string idtype = ms_->GetValueFromMapByKey("cardType",true);
	if (idtype == "1")
	{
		idtype = "身份证";
	}
	string cardNum = ms_->GetValueFromMapByKey("cardNum",true);
	output_text_=
		"servername=" + username + "\r\n" + 	//姓名	        
		"sex=" + sex + "\r\n" + 		//性别	
		"msg=" + birth	+ "\r\n" + 		//出生日期	
		"oid=" + mobile + "\r\n" + 		//手机号
		"imsi=" + idtype + "\r\n" + 	//证件类型	
		"appname=" + cardNum + "\r\n" + //证件号	
        "nick="	 + location + "\r\n" + 	//地址		
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=新建基本信息\r\n";  
	WriteIndexFile();
	return 1;
}


//修改基本信息
int employ::ZhiTong::PCWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string name = ms_->GetValueFromMapByKey("userName",true);
	string gender = ms_->GetValueFromMapByKey("gender",true);
	if (gender == "1")
	{
		gender = "男";
	}else
	{
		gender = "女";
	}
	string birthday = ms_->GetValueFromMapByKey("birthday",true);
	string location = ms_->GetValueFromMapByKey("name_location",true);
	string email = ms_->GetValueFromMapByKey("email",true);
	string cardType = ms_->GetValueFromMapByKey("cardType",true);
	if (cardType == "1")
	{
		cardType = "身份证";
	}
	string cardid = ms_->GetValueFromMapByKey("cardNum",true);
	output_text_=
		"servername=" + name + "\r\n" + //姓名	        
		"sex=" + gender + "\r\n" + 		//性别	
		"msg=" + birthday + "\r\n" + 	//出生日期
		"imsi=" + cardType + "\r\n" + 	//证件类型	
		"appname=" + cardid + "\r\n" + 	//证件号	
        "nick="	 + location + "\r\n" + 	//地址
		"mid=" + email + "\r\n" + 		//邮箱	
		"netmark=" + app_ + "\r\n" + 	//来源
		"state=修改基本信息\r\n";  
	WriteIndexFile();
	return 1;
}