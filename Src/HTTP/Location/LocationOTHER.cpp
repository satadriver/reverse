
#include "StdAfx.h"

#include "LocationOTHER.h"

location::Other::Other(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÖÜÄ©È¥ÄÄ";
}

location::Other::~Other()
{
	
}

int location::Other::IsOther(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("www.wanzhoumo.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/wanzhoumo?UUID=") != -1 ) )	
	{
		return kchangba;
	}
	return 0;
}

int location::Other::Process(const char *packet,int action)
{
	if (action == kchangba)
	{
		return changba(packet);
	}
	return 0;
}

int location::Other::changba(const char *packet)
{
return 1;
}