#include "StdAfx.h"
#include "WebSiteWapWat.h"

website::WapWat::WapWat(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "������";	
}

website::WapWat::~WapWat()
{
	
}

int website::WapWat::IsWapWat(HttpSession *http_session)
{
	//�û�ע��
	if((http_session->m_Requestline.m_Host.Find("www.wapwat.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/Register.asp")!=-1)) 
	{
		return kPcWebReg;
	}
	//�û���¼
	if((http_session->m_Requestline.m_Host.Find("www.wapwat.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/Login.asp")!=-1)) 
	{
		return kPcWebLogin;
	}
	//���վ��
	if((http_session->m_Requestline.m_Host.Find("www.wapwat.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/regsite.asp")!=-1)) 
	{
		return kPcWebAdd;
	}
	return 0;
}

int website::WapWat::Process(const char *packet,int action)
{
	//�û�ע��
	if (action == kPcWebReg)
	{
		return PcWebReg(packet);
	}
	//�û���¼
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//���վ��
	if (action == kPcWebAdd)
	{
		return PcWebAdd(packet);
	}
	return 1;
}

//�û�ע��
int website::WapWat::PcWebReg(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string uname = ms_->GetValueFromMapByKey("UID",true);
	//����
	string pwd = ms_->GetValueFromMapByKey("PWD1",true);
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	//QQ
	string qq = ms_->GetValueFromMapByKey("qq",true);
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	if (mobile.length() == 0)
	{
		mobile = "null";
	}
	if (uname.empty() || pwd.empty() || email.empty() || qq.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" + uname + "\r\n" +		//�û���
		"pass=" + pwd + "\r\n" + 		//����
		"appname=" + email + "\r\n" +	//����
		"oid=" + qq + "\r\n" + 			//QQ	
		"mid=" + mobile + "\r\n" + 		//�ֻ���
		"webname=" + app_ + "\r\n" +	//��Դ
         "state=�û�ע��\r\n";			//״̬ 
		WriteIndexFile();    
	return 1;
}

//�û���½
int website::WapWat::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string username = ms_->GetValueFromMapByKey("UID",true);
	//����
	string pwd = ms_->GetValueFromMapByKey("PWD",true);
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

//�û����վ��
int website::WapWat::PcWebAdd(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string sitename = ms_->GetValueFromMapByKey("SiteTitle",true);
	string siteurl = ms_->GetValueFromMapByKey("SiteUrl",true);
	if (sitename.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"userid=" +		sitename + "\r\n" +	//վ������
		"servername=" + siteurl + "\r\n" + 	//��ַ
		"webname=" +	app_ + "\r\n" +		//��Դ
		"state=���վ��\r\n";				//״̬ 
		WriteIndexFile();    
	return 1;
}
