
#include "StdAfx.h"

#include "DianXin.h"

DianXin::DianXin()
{
	software_ = "µãÐÄ";
}

DianXin::~DianXin()
{
	
}

int DianXin::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("atv.dxsvr.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/scan") != -1 ) )	
	{
		return kUploadDeviceInfoM;
	}
	return 0;
}

int DianXin::Process(const char *packet,int action)
{
	if (action == kUploadDeviceInfoM)
	{
		return UploadDeviceInfoM(packet);
	}
	return 0;
}


int DianXin::UploadDeviceInfoM(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	
	ms_->SplitStringToMap(src_data,'&','=');

	string imei = ms_->GetValueFromMapByKey("ie");
	string imsi = ms_->GetValueFromMapByKey("is");
	string model = ms_->GetValueFromMapByKey("model");

	if (imei.empty() || model.empty())
	{
		return 0;
	}
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"imsi=" + imsi + "\r\n" +
		"webname=" + model + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}

