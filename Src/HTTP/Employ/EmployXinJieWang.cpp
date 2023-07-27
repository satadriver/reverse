#include "StdAfx.h"
#include "EmployXinJieWang.h"

employ::XinJieWang::XinJieWang(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "н����";
}

employ::XinJieWang::~XinJieWang()
{
	
}

int employ::XinJieWang::IsXinJieWang(HttpSession *http_session)
{
	//ע��(����ע��)
	if((http_session->m_Requestline.m_Host.Find("jn.wep123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/register_email.aspx")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//ע��(�û���)
	if((http_session->m_Requestline.m_Host.Find("jn.wep123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/register_user.aspx")!=-1) )
	{
		return kPcWebRegisteruser;
	}
	//��¼
	if((http_session->m_Requestline.m_Host.Find("jn.wep123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/person/login.aspx")!=-1) )
	{
		return kPcWebLogin;
	}
	//�½�������Ϣ
	if((http_session->m_Requestline.m_Host.Find("jn.wep123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/Register_two")!=-1) )
	{
		return kPcWebRegisterInfo;
	}
	//�޸Ļ�����Ϣ
	if((http_session->m_Requestline.m_Host.Find("jn.wep123.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Resume/Res_Resume_Basic.aspx")!=-1) )
	{
		return kPcWebModiy;
	}
	return 0;
}

int employ::XinJieWang::Process(const char *packet,int action)
{
	//ע��(����ע��)
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//ע��(�û���)
	if (action ==kPcWebRegisteruser)
	{
		return PcWebRegisteruser(packet);
	}
	//��¼
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//�½�������Ϣ
	if (action == kPcWebRegisterInfo)
	{
		return PcWebRegisterInfo(packet);
	}
	//�޸Ļ�����Ϣ
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}

//ע��(����ע��)
int employ::XinJieWang::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�˺�
	string email = ms_->GetValueFromMapByKey("txtEmail",true);
	//�ֻ���
	string phone = ms_->GetValueFromMapByKey("txtMobile",true);
	//����
	string pass = ms_->GetValueFromMapByKey("txtPassword",true);
	if (email.empty() || phone.empty() || pass.empty())
	{
		return 0;
	}
	output_text_ =
		"oid=" + phone + "\r\n" + 	//�ֻ���		
		"mid=" + email + "\r\n" + 	//����	
		"pass=" + pass + "\r\n" + 	//����	
		"netmark=" + app_ + "\r\n" + //��Դ
		"state=ע��\r\n";
	WriteIndexFile();
	return 1;
}

//�û���(ע��)
int employ::XinJieWang::PcWebRegisteruser(const char *packet)
{
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string username = ms_->GetValueFromMapByKey("txtUsername",true);
	//����
	string pwd = ms_->GetValueFromMapByKey("txtPassword",true);
	//����
	string email = ms_->GetValueFromMapByKey("txtEmail",true);
	//�ֻ���
	string phone = ms_->GetValueFromMapByKey("txtMobile",true);
	if (username.empty() || pwd.empty() || email.empty() || phone.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +	//�˺�	
		"pass=" + pwd + "\r\n" + 		//����		
		"oid=" + phone + "\r\n" + 		//�ֻ���	
		"mid=" + email + "\r\n" + 		//����	
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=ע��\r\n";
	WriteIndexFile();
	return 1;
}

//��¼
int employ::XinJieWang::PcWebLogin(const char *packet)
{
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�˺�
    string UserName = ms_->GetValueFromMapByKey("txt_Username",true);
	//����
	string Password = ms_->GetValueFromMapByKey("txt_Password",true);
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

//�½�������Ϣ
int employ::XinJieWang::PcWebRegisterInfo(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string username = ms_->GetValueFromMapByKey("txt_Realname",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("rbn_Sex",true);
	if (sex == "True")
	{
		sex = "��";
	}else
	{
		sex = "Ů";
	}
	//��������
	string birth = ms_->GetValueFromMapByKey("txt_BirthDate",true);
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("txt_Mobile",true);
	//����
	string email = ms_->GetValueFromMapByKey("hid_Email",true);
	output_text_ =
		"servername=" + username + "\r\n" + //����	       
		"sex=" + sex + "\r\n" + 		//�Ա�	
		"msg=" + birth + "\r\n" + 		//��������	
		"oid=" + mobile + "\r\n" + 		//�ֻ���		
		"mid=" + email + "\r\n" + 		//����	
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=�½�������Ϣ\r\n";
	WriteIndexFile();
	return 1;
}

//�޸ĸ�����Ϣ
int employ::XinJieWang::PcWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string username = ms_->GetValueFromMapByKey("txt_Realname",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("rbn_Sex",true);
	if (sex == "False")
	{
		sex = "Ů";
	}
	else
	{
		sex = "��";
	}
	//����
	string birth = ms_->GetValueFromMapByKey("txt_BirthDate",true);
	//֤������
	string shw_IdType = ms_->GetValueFromMapByKey("shw_IdType",true);
	//֤����
	string idcode = ms_->GetValueFromMapByKey("txt_IdCode",true);
	//�ֻ��� 
	string mobile = ms_->GetValueFromMapByKey("txt_Mobile",true);
	if (username.empty() || sex.empty() || mobile.empty() || birth.empty())
	{
		return 0;
	}
	if (idcode.empty())
	{
		idcode = "��";
	}
	output_text_=
		"servername=" + username + "\r\n" + //����	       
		"sex=" + sex + "\r\n" + 		//�Ա�	
		"msg=" + birth + "\r\n" + 		//��������	
		"oid=" + mobile + "\r\n" + 		//�ֻ���
		"imsi=" + shw_IdType + "\r\n" + //֤������	
		"appname=" + idcode + "\r\n" + 	//֤����
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=�޸Ļ�����Ϣ\r\n";
	WriteIndexFile();
	return 1;
}