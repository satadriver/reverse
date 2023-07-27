#include "StdAfx.h"
#include "PhoneDeviceMiaoPai.h"

phonedevice::PdMiaoPai::PdMiaoPai(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "秒拍";
}

phonedevice::PdMiaoPai::~PdMiaoPai()
{
	
}

int phonedevice::PdMiaoPai::IsPdMiaoPai(HttpSession *http_session)
{
	//手机设备信息
	if( http_session->m_Requestline.m_Host.Find("stat.ztems.com")  != -1 && 
		(http_session->m_Requestline.m_URI.Find("/m/ver.json") != -1 ||
		http_session->m_Requestline.m_URI.Find("/m/v4_remind.json") != -1 ||
		http_session->m_Requestline.m_URI.Find("/m/v4_feed.json") != -1 ||
		http_session->m_Requestline.m_URI.Find("/m/index_cate.json") != -1 ||
		http_session->m_Requestline.m_URI.Find("/m/hotword.json") != -1 ||
		http_session->m_Requestline.m_URI.Find("/m/citywide") != -1 ||
		http_session->m_Requestline.m_URI.Find("/m/cate2_channel.json") != -1 ||
		http_session->m_Requestline.m_URI.Find("/m/ascent") != -1 ||
		http_session->m_Requestline.m_URI.Find("/m/ad.json") != -1) ||
		http_session->m_Requestline.m_URI.Find("1/config/global.json"))	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::PdMiaoPai::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::PdMiaoPai::UploadPhoneDevice(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
    uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->SplitStringToMap(uri,"&","=");
	//
	imei_ = ms_->GetValueFromMapByKey("imei");
	model_ = ms_->GetValueFromMapByKey("model");
	os_ = ms_->GetValueFromMapByKey("os");
	version_ = ms_->GetValueFromMapByKey("version");

	if (imei_.empty() || os_.empty() || version_.empty() || model_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

