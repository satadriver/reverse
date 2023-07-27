#include "StdAfx.h"
#include "EmployKanZhunWang.h"

employ::KanZhunWang::KanZhunWang(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "��׼��";
}

employ::KanZhunWang::~KanZhunWang()
{
	
}

int employ::KanZhunWang::IsKanZhunWang(HttpSession *http_session)
{
	//ע��(����ע��)
	if((http_session->m_Requestline.m_Host.Find("www.kanzhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register/doregister")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.kanzhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.json")!=-1) )
	{
		return kPcWebLogin;
	}
	//�޸ĸ�����Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.kanzhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/update/basic.json")!=-1) )
	{
		return kPCWebModiy;
	}
	return 0;
}

int employ::KanZhunWang::Process(const char *packet,int action)
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
	if (action==kPCWebModiy)
	{
		return PCWebModiy(packet);
	}
	return 1;
}

//ע��(����ע��)
int employ::KanZhunWang::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	string username = ms_->GetValueFromMapByKey("username",true);
	//����
	string email = ms_->GetValueFromMapByKey("account",true);
	//����
	string pass = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || email.empty() || pass.empty())
	{
		return 0;
	}

	output_text_ =
		"user=" + 	username + "\r\n" +	//�˺�		
		"mid=" + 	email + "\r\n" + 	//����		 	
		"pass=" + 	pass + "\r\n" + 	//����	
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=ע��\r\n";
	WriteIndexFile();
	return 1;
}

//��¼
int employ::KanZhunWang::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�˺�
    string UserName = ms_->GetValueFromMapByKey("account",true);
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
int employ::KanZhunWang::PCWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("name",true);
	//�Ա�
	string gender = ms_->GetValueFromMapByKey("gender",true);
	if (gender=="1")
	{
		gender = "��";
	}
	if (gender =="2")
	{
		gender = "Ů";
	}
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	//��������
	string birth = ms_->GetValueFromMapByKey("birthdayYear",true) + "-" + 
					ms_->GetValueFromMapByKey("birthdayMonth",true) + "-" + 
					ms_->GetValueFromMapByKey("birthdayDay",true);

	if (name.empty() || gender.empty() || mobile.empty() || email.empty() || birth.empty())
	{
		return 0;
	}
	output_text_=
		"servername=" + name + "\r\n" + 	//����	        
		"sex=" + 	  	gender + "\r\n" + 	//�Ա�	
		"msg=" + 	  	birth + "\r\n" + 	//��������	
		"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���	
		"mid=" + 	  	email + "\r\n" + 	//����	
		"netmark=" + app_ + "\r\n" + 		//��Դ
		"state=�޸Ļ�����Ϣ\r\n";  
	WriteIndexFile();
	return 1;
}