
#include "StdAfx.h"

#include "PhoneInfoPPTV.h"

phoneinfo::PPTV::PPTV(HttpSession *http_session, const string &pro_name) : PhoneInfo(http_session,pro_name)
{
	app_ = "PPTV";
}

phoneinfo::PPTV::~PPTV()
{
	
}

int phoneinfo::PPTV::IsPPTV(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("de.as.pptv.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/ikandelivery/ipadad") != -1 ) )	
	{
		return kUploadPhoneInfo;
	}
	return 0;
}

int phoneinfo::PPTV::Process(const char *packet,int action)
{
	if (action == kUploadPhoneInfo)
	{
		return UploadPhoneInfo(packet);
	}
	return 0;
}


int phoneinfo::PPTV::UploadPhoneInfo(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	
	ms_->SplitStringToMap(src_data,'&','=');

	string os = ms_->GetValueFromMapByKey("os");
	string version = ms_->GetValueFromMapByKey("osv");

	imei_ = ms_->GetValueFromMapByKey("did");
	imsi_ = ms_->GetValueFromMapByKey("carrier");
	
	if (imsi_.empty() || imei_.empty() || version.empty() /*|| model.empty()*/)
	{
		return 0;
	}
	WriteIndexFile();
	return 1;
}

