
#include "StdAfx.h"

#include "PhoneDeviceZhiFuBaoQianBao.h"

phonedevice::ZhiFuBaoQianBao::ZhiFuBaoQianBao(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "Ö§¸¶±¦Ç®°ü";
}

phonedevice::ZhiFuBaoQianBao::~ZhiFuBaoQianBao()
{
	
}

int phonedevice::ZhiFuBaoQianBao::IsZhiFuBaoQianBao(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("mobilecns.alipay.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/getMsgAndCfg.htm") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::ZhiFuBaoQianBao::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::ZhiFuBaoQianBao::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,',',':',"\"");

	model_ = ms_->GetValueFromMapByKey("model");
	os_ = ms_->GetValueFromMapByKey("osType");
	version_ = ms_->GetValueFromMapByKey("osVersion");
	string tmp = ms_->GetValueFromMapByKey("IMEI");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		imsi_ = tmp.substr(0,pos);
		imei_ = tmp.substr(pos + 1);
	}
	
	if (model_.empty() || os_.empty() || version_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

