
#include "StdAfx.h"

#include "PhoneDeviceBaiDuShouJiWeiShi.h"

phonedevice::BaiDuShouJiWeiShi::BaiDuShouJiWeiShi(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "百度手机卫士";
}

phonedevice::BaiDuShouJiWeiShi::~BaiDuShouJiWeiShi()
{
	
}

int phonedevice::BaiDuShouJiWeiShi::IsBaiDuShouJiWeiShi(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("rc.dxsvr.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/get") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::BaiDuShouJiWeiShi::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::BaiDuShouJiWeiShi::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string devicinfo;
	devicinfo = ms_->GetValueFromMapByKey("ua");
	imei_ = ms_->GetValueFromMapByKey("imei");
	
	if (devicinfo.empty() || imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

