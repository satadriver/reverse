
#include "StdAfx.h"

#include "RealNameQiTian.h"

realname::QiTian::QiTian(HttpSession *http_session, const string &pro_name) : RealName(http_session,pro_name)
{
	app_ = "ÆßÌì¾Æµê";
}

realname::QiTian::~QiTian()
{
	
}

int realname::QiTian::IsQiTian(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("www.plateno.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/booking/booking2") != -1 ) )	
	{
		return krealname;
	}
	return 0;
}

int realname::QiTian::Process(const char *packet,int action)
{
	if (action == krealname)
	{
		return RealNameUpdate(packet);
	}
	return 0;
}


int realname::QiTian::RealNameUpdate(const char *packet)
{	
	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=',"\"");
	name_ = ms_->GetValueFromMapByKey("bookingGuestName",true);
	phone_ = ms_->GetValueFromMapByKey("bookingMobilePhone");
	if (name_.empty() || phone_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}