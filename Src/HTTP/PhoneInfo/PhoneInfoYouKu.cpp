
#include "StdAfx.h"

#include "PhoneInfoYouKu.h"

phoneinfo::YouKu::YouKu(HttpSession *http_session, const string &pro_name) : PhoneInfo(http_session,pro_name)
{
	app_ = "се©А";
}

phoneinfo::YouKu::~YouKu()
{
	
}

int phoneinfo::YouKu::IsYouKu(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.mobile.youku.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/openapi-wireless/initial") != -1 ) )	
	{
		return kUploadPhoneInfo;
	}
	return 0;
}

int phoneinfo::YouKu::Process(const char *packet,int action)
{
	if (action == kUploadPhoneInfo)
	{
		return UploadPhoneInfo(packet);
	}
	return 0;
}


int phoneinfo::YouKu::UploadPhoneInfo(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
//	string os,version,model,imsi;
//	os= ms_->GetValueFromMapByKey("os");
//	version = ms_->GetValueFromMapByKey("os_ver");
//	model = ms_->GetValueFromMapByKey("btype");
	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	
	if (/*os.empty() || */imsi_.empty() || imei_.empty() /*|| version.empty() || model.empty()*/)
	{
		return 0;
	}

	
	WriteIndexFile();
	return 1;
}

