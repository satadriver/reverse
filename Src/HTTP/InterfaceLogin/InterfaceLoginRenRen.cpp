#include "StdAfx.h"
#include "InterfaceLoginRenRen.h"

interfacelogin::InterfaceLoginRenRen::InterfaceLoginRenRen(HttpSession *http_session, const string &pro_name): InterfaceLogin(http_session,pro_name)
{

}

interfacelogin::InterfaceLoginRenRen::~InterfaceLoginRenRen()
{

}

interfacelogin::InterfaceLoginRenRen::IsInterfaceLoginRenRen(HttpSession *http_session)
{
	
	if( ( http_session->m_Requestline.m_Host.Find("graph.renren.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/oauth/grant") != -1 ))	
	{
		return kAndroidInterfaceRenRen;
	}
	return 0;
}

interfacelogin::InterfaceLoginRenRen::Process(const char *packet,int action)
{
	if (action == kAndroidInterfaceRenRen)
	{
		return AndroidInterfaceRenRen(packet);
	}
	return 0;
}

interfacelogin::InterfaceLoginRenRen::AndroidInterfaceRenRen(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
    ms_->SplitStringToMap(src_data,"&","=");
	string username = ms_->GetValueFromMapByKey("username");
	string password = ms_->GetValueFromMapByKey("password");
	string redirect = ms_->GetValueFromMapByKey("redirect_uri");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +	//�û���
		"pass=" + 		password + "\r\n" + //����	
		"islogin=" + 	redirect + "\r\n" + //Ӧ������
		"state=��������������Ȩ��¼\r\n";	//״̬
	WriteIndexFile();
	return 0;
}
