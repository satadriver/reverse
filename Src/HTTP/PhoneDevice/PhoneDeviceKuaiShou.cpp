
#include "StdAfx.h"
#include "PhoneDeviceKuaiShou.h"

phonedevice::PdKuaiShou::PdKuaiShou(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "快手";
}

phonedevice::PdKuaiShou::~PdKuaiShou()
{
	
}

int phonedevice::PdKuaiShou::IsPdKuaiShou(HttpSession *http_session)
{
	//手机设备信息
	if( (http_session->m_Requestline.m_Host.Find("api.gifshow.com")  != -1 ||
		http_session->m_Requestline.m_Host.Find("124.243.249.4")  != -1 || 
		http_session->m_Requestline.m_Host.Find("upload.gifshow.com")  != -1) && 
		http_session->m_Requestline.m_URI.Find("/rest/") != -1)	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::PdKuaiShou::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::PdKuaiShou::UploadPhoneDevice(const char *packet)
{
    
	string uri = http_session_->m_Requestline.m_URL.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->SplitStringToMap(uri,"&","=");
	//
    model_ = ms_->GetValueFromMapByKey("mod");
    version_ = ms_->GetValueFromMapByKey("sys");
	os_ = ms_->GetValueFromMapByKey("sys");
	longitude_ = ms_->GetValueFromMapByKey("lon");
	latitude_ = ms_->GetValueFromMapByKey("lat");

	if (os_.empty() || version_.empty() || model_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

