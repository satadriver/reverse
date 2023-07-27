
#include "StdAfx.h"

#include "PhoneDeviceAiQiYi.h"

phonedevice::AiQiYi::AiQiYi(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "°®ÆæÒÕ";
}

phonedevice::AiQiYi::~AiQiYi()
{
	
}

int phonedevice::AiQiYi::IsAiQiYi(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("iface.iqiyi.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/initLogin") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::AiQiYi::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::AiQiYi::UploadPhoneDevice(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');

	model_ = ms_->GetValueFromMapByKey("ua",true);
	imei_ = ms_->GetValueFromMapByKey("id");
	version_ = ms_->GetValueFromMapByKey("os");
	os_ = GetOS();
	

	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

