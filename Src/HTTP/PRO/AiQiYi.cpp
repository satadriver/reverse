
#include "StdAfx.h"

#include "AiQiYi.h"

AiQiYi::AiQiYi()
{
	software_ = "°®ÆæÒÕ";
}

AiQiYi::~AiQiYi()
{
	
}

int AiQiYi::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("iface.iqiyi.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/initLogin") != -1 ) )	
	{
		return kUploadDeviceInfoM;
	}
	return 0;
}

int AiQiYi::Process(const char *packet,int action)
{
	if (action == kUploadDeviceInfoM)
	{
		return UploadDeviceInfoM(packet);
	}
	return 0;
}

int AiQiYi::UploadDeviceInfoM(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');

	string model = ms_->GetValueFromMapByKey("ua",true);
	string imei = ms_->GetValueFromMapByKey("id");
	string version = ms_->GetValueFromMapByKey("os");
	DeviceInfo device_info;
	string os = device_info.GetOS(model,version);
	

	if (imei.empty() || version.empty() || model.empty() || os.empty())
	{
		return 0;
	}

	output_text_ = 
		"imei=" + imei + "\r\n" +
		"webname=" + model + "\r\n" +
		"from=" + os + "\r\n" +
		"to=" + version + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}

