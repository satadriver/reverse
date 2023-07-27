
#include "StdAfx.h"

#include "ChatingVoxer.h"

chating::Voxer::Voxer(HttpSession *http_session, const string &pro_name) : Chating(http_session,pro_name)
{
	
}

chating::Voxer::~Voxer()
{
	
}

int chating::Voxer::IsVoxer(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("api.yoz.io")  != -1 ) && 
		( http_session->m_Requestline.m_useragent.Find("com.rebelvox.voxer") != -1 ) )	
	{
		return kLogin;
	}

	if( ( http_session->m_Requestline.m_Host.Find("165.225.152.61")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("ping") != -1 ) )	
	{
		return kLogin;
	}

	if( ( http_session->m_Requestline.m_Host.Find("165.225.152.222")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("ping") != -1 ) )	
	{
		return kLogin;
	}
	return 0;
}

int chating::Voxer::Process(const char *packet,int action)
{
	if (action == kLogin)
	{
		return Login(packet);
	}
	return 0;
}

int chating::Voxer::Login(const char *packet)
{
	string src_data(packet);
// 	if (string::npos == src_data.find("device_id"))
// 	{
// 		return 0;
// 	}

	output_text_ =
		"state= иооъ(z)\r\n";
	WriteIndexFile();
	return 1;
}