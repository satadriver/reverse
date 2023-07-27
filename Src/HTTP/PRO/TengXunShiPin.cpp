
#include "StdAfx.h"

#include "TengXunShiPin.h"

TengXunShiPin::TengXunShiPin()
{
	software_ = "ÌÚÑ¶ÊÓÆµ";
}

TengXunShiPin::~TengXunShiPin()
{
	
}

int TengXunShiPin::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("sdkconfig.video.qq.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/getmfomat") != -1 ) )	
	{
		return kUploadDeviceInfoM;
	}
	return 0;
}

int TengXunShiPin::Process(const char *packet,int action)
{
	if (action == kUploadDeviceInfoM)
	{
		return UploadDeviceInfoM(packet);
	}
	return 0;
}


int TengXunShiPin::UploadDeviceInfoM(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');

	string version = ms_->GetValueFromMapByKey("sysver");
	string model = ms_->GetValueFromMapByKey("model");
	string imei = ms_->GetValueFromMapByKey("imei");
	string imsi = ms_->GetValueFromMapByKey("imsi");
	
	if (imei.empty() || version.empty() || model.empty())
	{
		return 0;
	}
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"imsi=" + imsi + "\r\n" +
		"webname=" + model + "\r\n" +
		"to=" + version + "\r\n" +
		"state=" + software_ + "\r\n";

	WriteIndexFile();
	return 1;
}

