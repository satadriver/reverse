
#include "StdAfx.h"

#include "PhoneInfoTengXunShiPin.h"

phoneinfo::TengXunShiPin::TengXunShiPin(HttpSession *http_session, const string &pro_name) : PhoneInfo(http_session,pro_name)
{
	app_ = "ÌÚÑ¶ÊÓÆµ";
}

phoneinfo::TengXunShiPin::~TengXunShiPin()
{
	
}

int phoneinfo::TengXunShiPin::IsTengXunShiPin(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("sdkconfig.video.qq.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/getmfomat") != -1 ) )	
	{
		return kUploadPhoneInfo;
	}
	return 0;
}

int phoneinfo::TengXunShiPin::Process(const char *packet,int action)
{
	if (action == kUploadPhoneInfo)
	{
		return UploadPhoneInfo(packet);
	}
	return 0;
}


int phoneinfo::TengXunShiPin::UploadPhoneInfo(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');

//	version = ms_->GetValueFromMapByKey("sysver");
//	model = ms_->GetValueFromMapByKey("model");
	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	
	if (imei_.empty() || imsi_.empty() /*|| version.empty() || model.empty()*/)
	{
		return 0;
	}

	
	WriteIndexFile();
	return 1;
}

