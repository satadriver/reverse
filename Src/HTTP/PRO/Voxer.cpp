
#include "StdAfx.h"

#include "Voxer.h"

Voxer::Voxer()
{
	
}

Voxer::~Voxer()
{
	
}

int Voxer::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("api.yoz.io")  != -1 ) && 
		( http_session->m_Requestline.m_useragent.Find("com.rebelvox.voxer") != -1 ) )	
	{
		return kLoginM;
	}
	return 0;
}

int Voxer::Process(const char *packet,int action)
{
	if (action == kLoginM)
	{
		return LoginM(packet);
	}
	return 0;
}

int Voxer::LoginM(const char *packet)
{
	string src_data(packet);
// 	if (string::npos == src_data.find("device_id"))
// 	{
// 		return 0;
// 	}

	output_text_ =
		"state= …œœﬂ(z)\r\n";
	WriteIndexFile();
	return 1;
}