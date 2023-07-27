
#include "StdAfx.h"

#include "PhoneDeviceDaZhongDianPing.h"

phonedevice::DaZhongDianPing::DaZhongDianPing(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "´óÖÚµãÆÀ";
}

phonedevice::DaZhongDianPing::~DaZhongDianPing()
{
	
}

int phonedevice::DaZhongDianPing::IsDaZhongDianPing(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("m.api.dianping.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/site") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::DaZhongDianPing::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::DaZhongDianPing::UploadPhoneDevice(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');

	//os = ms_->GetValueFromMapByKey("Platform");
	version_ = ms_->GetValueFromMapByKey("os");
	model_ = ms_->GetValueFromMapByKey("model",true);
	imei_ = ms_->GetValueFromMapByKey("deviceid");
	os_ = GetOS();

	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

