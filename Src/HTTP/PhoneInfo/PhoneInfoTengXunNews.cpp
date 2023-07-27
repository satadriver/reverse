
#include "StdAfx.h"

#include "PhoneInfoTengXunNews.h"

phoneinfo::TengXunNews::TengXunNews(HttpSession *http_session, const string &pro_name) : PhoneInfo(http_session,pro_name)
{
	app_ = "ÌÚÑ¶ÐÂÎÅ";
}

phoneinfo::TengXunNews::~TengXunNews()
{
	
}

int phoneinfo::TengXunNews::IsTengXunNews(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("r.inews.qq.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/getQQNewsRemoteConfigAndroid") != -1 ) )	
	{
		return kUploadPhoneInfo;
	}
	return 0;
}

int phoneinfo::TengXunNews::Process(const char *packet,int action)
{
	if (action == kUploadPhoneInfo)
	{
		return UploadPhoneInfo(packet);
	}
	return 0;
}


int phoneinfo::TengXunNews::UploadPhoneInfo(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');

	imei_ = ms_->GetValueFromMapByKey("devid");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	
	if (imei_.empty() || imsi_.empty())
	{
		return 0;
	}

	WriteIndexFile();
	return 1;
}

