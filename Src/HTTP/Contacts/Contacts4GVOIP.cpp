
#include "StdAfx.h"

#include "Contacts4GVOIP.h"

contacts::VOIP4G::VOIP4G(HttpSession *http_session, const string &pro_name) : Contacts(http_session,pro_name)
{
	
}

contacts::VOIP4G::~VOIP4G()
{
	
}

BOOL contacts::VOIP4G::IsVOIP4G(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("agw.3gwldh.com:2001")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/1.0/3g/contacts/backup")!=-1))
	{
		return TRUE;
	}
	return FALSE;
}

int contacts::VOIP4G::Process(HttpSession *pHttpSession,const char *packet)
{
	return 1;
}