
#include "StdAfx.h"

#include "MeiTuXiuXiu.h"

MeiTuXiuXiu::MeiTuXiuXiu()
{
	software_ = "ÃÀÍ¼ÐãÐã";
}

MeiTuXiuXiu::~MeiTuXiuXiu()
{
	
}

int MeiTuXiuXiu::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("data.mobile.meitu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("install.php") != -1 ) )	
	{
		return kUploadDeviceInfoM;
	}
	return 0;
}

int MeiTuXiuXiu::Process(const char *packet,int action)
{
	if (action == kUploadDeviceInfoM)
	{
		return UploadDeviceInfoM(packet);
	}
	return 0;
}


int MeiTuXiuXiu::UploadDeviceInfoM(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string version = ms_->GetValueFromMapByKey("ios");
	string model = ms_->GetValueFromMapByKey("itype");
	string imei = ms_->GetValueFromMapByKey("uid");
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

