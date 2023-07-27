#include "StdAfx.h"
#include "website51la.h"

website::LA51::LA51(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "51la";	
}

website::LA51::~LA51()
{
	
}

int website::LA51::IsLA51(HttpSession *http_session)
{
	//�û�ע��
	if((http_session->m_Requestline.m_Host.Find("www.51.la")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg.asp")!=-1)) 
	{
		return kPcWebReg;
	}
	//�û���¼
	if((http_session->m_Requestline.m_Host.Find("www.51.la")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.asp")!=-1)) 
	{
		return kPcWebLogin;
	}
	//�û����վ��
	if((http_session->m_Requestline.m_Host.Find("www.51.la")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/newsite.asp")!=-1)) 
	{
		return kPcWebAdd;
	}
	//�û��޸�
	if((http_session->m_Requestline.m_Host.Find("www.jobui.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/process/")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int website::LA51::Process(const char *packet,int action)
{
	if (action == kPcWebReg)
	{
		return PcWebReg(packet);
	}

	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}

	if (action == kPcWebAdd)
	{
		return PcWebAdd(packet);
	}
	return 1;
}

//�û�ע��
int website::LA51::PcWebReg(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string uname = ms_->GetValueFromMapByKey("uname",true);
	string pwd = ms_->GetValueFromMapByKey("pass1",true);
	string email = ms_->GetValueFromMapByKey("email",true);
	string sitename = ms_->GetValueFromMapByKey("sitename",true);
	string siteurl = ms_->GetValueFromMapByKey("siteurl",true);
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
         "state=�û�ע��\r\n";				//״̬ 
		WriteIndexFile();    
	return 1;
}

//�û���½
int website::LA51::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("uname",true);
	string pwd = ms_->GetValueFromMapByKey("upass",true);
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
int website::LA51::PcWebAdd(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string sitename = ms_->GetValueFromMapByKey("sitename",true);
	string siteurl = ms_->GetValueFromMapByKey("siteurl",true);
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
