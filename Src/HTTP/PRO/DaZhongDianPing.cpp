
#include "StdAfx.h"

#include "DaZhongDianPing.h"

DaZhongDianPing::DaZhongDianPing()
{
	software_ = "´óÖÚµãÆÀ";
}

DaZhongDianPing::~DaZhongDianPing()
{
	
}

int DaZhongDianPing::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("m.api.dianping.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/site") != -1 ) )	
	{
		return kUploadDeviceInfoM;
	}
	return 0;
}

int DaZhongDianPing::Process(const char *packet,int action)
{
	if (action == kUploadDeviceInfoM)
	{
		return UploadDeviceInfoM(packet);
	}
	return 0;
}


int DaZhongDianPing::UploadDeviceInfoM(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');


	string version = ms_->GetValueFromMapByKey("os");
	string model = ms_->GetValueFromMapByKey("model",true);
	string imei = ms_->GetValueFromMapByKey("deviceid");
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

