#include "StdAfx.h"
#include "WebSiteAdmin88.h"

website::Admin88::Admin88(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "Admin88��վ����������";	
}

website::Admin88::~Admin88()
{
	
}

int website::Admin88::IsAdmin88(HttpSession *http_session)
{
	//�û�ע��
	if((http_session->m_Requestline.m_Host.Find("www1.admin88.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg.asp")!=-1)) 
	{
		return kPcWebReg;
	}
	//�û���¼
	if((http_session->m_Requestline.m_Host.Find("www1.admin88.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/check.asp")!=-1)) 
	{
		return kPcWebLogin;
	}
	//�û��޸�
	if((http_session->m_Requestline.m_Host.Find("www1.admin88.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/edit.asp")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int website::Admin88::Process(const char *packet,int action)
{
	if (action == kPcWebReg)
	{
		return PcWebReg(packet);
	}

	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}

	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}

//�û�ע��
int website::Admin88::PcWebReg(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string uname = ms_->GetValueFromMapByKey("user",true);
	//����
	string pwd = ms_->GetValueFromMapByKey("password1",true);
	string pwd2 = ms_->GetValueFromMapByKey("password2",true);
	if (strcmp(pwd.c_str(),pwd2.c_str()))
	{
		return 0;
	}
	//����
	string email = ms_->GetValueFromMapByKey("mail",true);
	//վ������
	string sitename = ms_->GetValueFromMapByKey("sitename",true);
	//��ַ
	string siteurl = ms_->GetValueFromMapByKey("url",true);
	if (uname.empty() || pwd.empty() || email.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" + uname + "\r\n" +			//�û���
		"pass=" + pwd + "\r\n" + 			//����
		"userid=" + sitename + "\r\n" +		//վ������
		"servername=" + siteurl + "\r\n" + 	//��ַ
		"appname=" + email + "\r\n" +		//����
		"webname=" + app_ + "\r\n" +		//��Դ
         "state=�û�ע��\r\n";				//״̬ 
		WriteIndexFile();    
	return 1;
}

//�û���½
int website::Admin88::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("Name",true);
	string pwd = ms_->GetValueFromMapByKey("passwd",true);
	if (username.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" + username + "\r\n" +	//�û���
		"pass=" + pwd + "\r\n" + 		//����
		"webname=" + app_ + "\r\n" +	//��Դ
		"state=�û���¼\r\n";			//״̬ 
		WriteIndexFile();    
	return 1;
}

//�޸�վ��
int website::Admin88::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string uname = ms_->GetValueFromMapByKey("user",true);
	//����
	string oldpasswd = ms_->GetValueFromMapByKey("oldpasswd",true);
	string pwd = ms_->GetValueFromMapByKey("passwd",true);
	string pwd2 = ms_->GetValueFromMapByKey("passwd2",true);
	if (pwd.empty() && pwd2.empty())
	{
		pwd = oldpasswd;
	}
	//����
	string email = ms_->GetValueFromMapByKey("mail",true);
	//վ������
	string sitename = ms_->GetValueFromMapByKey("sitename",true);
	//��ַ
	string siteurl = ms_->GetValueFromMapByKey("url",true);
	if (uname.empty() || pwd.empty() || email.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" +		uname + "\r\n" +	//�û���
		"pass=" +		pwd + "\r\n" + 		//����
		"userid=" +		sitename + "\r\n" +	//վ������
		"servername=" + siteurl + "\r\n" + 	//��ַ
		"appname=" +	email + "\r\n" +	//����
		"webname=" +	app_ + "\r\n" +		//��Դ
		"state=�޸�վ��\r\n";				//״̬ 
	WriteIndexFile();    
	return 1;
}
