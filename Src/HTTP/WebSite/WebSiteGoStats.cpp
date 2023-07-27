#include "StdAfx.h"
#include "websitegostats.h"

website::GoStats::GoStats(HttpSession *http_session, const string &pro_name) : WebSite(http_session,pro_name)
{
	app_ = "gostats";	
}

website::GoStats::~GoStats()
{
	
}

int website::GoStats::IsGoStats(HttpSession *http_session)
{
	//�û�ע��
	if((http_session->m_Requestline.m_Host.Find("gostats.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/signup")!=-1)) 
	{
		return kPcWebReg;
	}
	//�û���¼
	if((http_session->m_Requestline.m_Host.Find("gostats.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.xml")!=-1)) 
	{
		return kPcWebLogin;
	}
	//�û����վ��
	if((http_session->m_Requestline.m_Host.Find("gostats.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/add_site")!=-1)) 
	{
		return kPcWebAdd;
	}
	return 0;
}

int website::GoStats::Process(const char *packet,int action)
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
int website::GoStats::PcWebReg(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("username",true);
	string pwd = ms_->GetValueFromMapByKey("passwd1",true);
	string realname = ms_->GetValueFromMapByKey("realname",true);
	string mail = ms_->GetValueFromMapByKey("mail",true);
	string sitename = ms_->GetValueFromMapByKey("name",true);
	string siteurl = ms_->GetValueFromMapByKey("url",true);
	if (username.empty() || pwd.empty() || realname.empty() || mail.empty() || sitename.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"user=" +		username + "\r\n" + //�û���
		"pass=" +		pwd + "\r\n" + 		//����
		"msgtype=" +	realname + "\r\n"+	//��ʵ����
		"userid=" +		sitename + "\r\n" +	//վ������
		"servername=" + siteurl + "\r\n" +  //��ַ
		"appname=" +	mail + "\r\n" +		//����
		"webname=" +	app_ + "\r\n" +		//��Դ
         "state=�û�ע��\r\n";				//״̬ 
		WriteIndexFile();    
	return 1;
}

//�û���½
int website::GoStats::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("username",true);
	string pwd = ms_->GetValueFromMapByKey("password",true);
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
int website::GoStats::PcWebAdd(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string sitename = ms_->GetValueFromMapByKey("name",true);
	string siteurl = ms_->GetValueFromMapByKey("url",true);
	if (sitename.empty() || siteurl.empty())
	{
		return 0;
	}
	output_text_ =				
		"userid=" +		sitename + "\r\n" +	//վ������
		"servername=" + siteurl + "\r\n" +  //��ַ
		"webname=" +	app_ + "\r\n" +		//��Դ
		"state=���վ��\r\n";				//״̬ 
		WriteIndexFile();    
	return 1;
}





