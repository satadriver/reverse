#include "StdAfx.h"
#include "EmployZhongHuaYingCai.h"
#include "../BoundaryParser.h"

employ::ZhongHuanYingCai::ZhongHuanYingCai(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "中华英才网";	
}

employ::ZhongHuanYingCai::~ZhongHuanYingCai()
{
	
}
//获取证件类型
string employ::ZhongHuanYingCai::GetIdType(string idType)
{
	if (idType=="1"){	idType = "身份证";}
	if (idType=="2"){	idType = "护照";}
	if (idType=="3"){	idType = "军官证";}
	if (idType=="7"){	idType = "香港身份证";}
	if (idType=="8"){	idType = "澳门身份证";}
	if (idType=="5"){	idType = "港澳通行证";}
	if (idType=="6"){	idType = "台胞证";}
	if (idType=="4"){	idType = "其他";}
	return idType;
}
int employ::ZhongHuanYingCai::IsZhongHuanYingCai(HttpSession *http_session)
{
	//个人信息
	if((http_session->m_Requestline.m_Host.Find("my.chinahr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cv/ajax/createCvFirst")!=-1) )
	{
		return kPcWebInfo;
	}
	//修改个人信息
	if((http_session->m_Requestline.m_Host.Find("my.chinahr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/saveBasic")!=-1) )
	{
		return kPCWebModiy;
	}
	return 0;
}

int employ::ZhongHuanYingCai::Process(const char *packet,int action)
{
	//个人信息
	if (action == kPcWebInfo)
	{
		return PcWebInfo(packet);
	}
	if (action==kPCWebModiy)
	{
		return PCWebModiy(packet);
	}
	return 1;
}

//个人信息
int employ::ZhongHuanYingCai::PcWebInfo(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("basicInfo.name",true);
	//性别
	string gender = ms_->GetValueFromMapByKey("basicInfo.gender",true);
	if (gender == "1"){gender = "男";}
	if (gender == "2"){gender = "女";}
	//手机号
	string mobile = ms_->GetValueFromMapByKey("basicInfo.mobile",true);
	//生日
	string birth = ms_->GetValueFromMapByKey("birth",true);
	//现居住地
	string address = ms_->GetValueFromMapByKey("basicInfo.living.fullpath",true);
	//参见工作时间(备用)
	string workTime = ms_->GetValueFromMapByKey("workTime",true);
	output_text_=
		"servername=" + name + "\r\n" + 	//姓名	       
		"sex=" + 	  	gender + "\r\n" + 	//性别		
		"msg=" + 	  	birth + "\r\n" + 	//出生日期	
		"nick="	 + 	  	address + "\r\n" + 	//地址	
		"oid=" + 	  	mobile + "\r\n" + 	//手机号
		"netmark=" +	app_ + "\r\n" + 	//来源
		"state=新建基本信息\r\n";
	WriteIndexFile();
	return 1;
}
//修改个人信息
int employ::ZhongHuanYingCai::PCWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("basicInfo.name",true);
	//性别
	string gender = ms_->GetValueFromMapByKey("basicInfo.gender",true);
	if (gender == "1")
	{
		gender = "男";
	}
	if (gender == "2")
	{
		gender = "女";
	}
	//生日
	string birth = ms_->GetValueFromMapByKey("birth",true);
	//现居住地
	string address = ms_->GetValueFromMapByKey("basicInfo.living.fullpath",true);
	//手机
	string mobile = ms_->GetValueFromMapByKey("basicInfo.mobile",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("basicInfo.email",true);
	if (name.empty() || gender.empty() || birth.empty() || mobile.empty() || email.empty())
	{
		return 0;
	}
	output_text_=
		"servername=" + name + "\r\n" + 	//姓名	       
		"sex=" + 	  	gender + "\r\n" + 	//性别		
		"msg=" + 	  	birth + "\r\n" + 	//出生日期	
		"nick="	 + 	  	address + "\r\n" + 	//地址	
		"oid=" + 	  	mobile + "\r\n" + 	//手机号
		"mid=" + 	  	email + "\r\n" + 	//邮箱	
		"netmark=" +	app_ + "\r\n" + 	//来源
		"state=修改基本信息\r\n";
	WriteIndexFile();
	return 1;
}