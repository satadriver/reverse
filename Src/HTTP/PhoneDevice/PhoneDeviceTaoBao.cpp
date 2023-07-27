
#include "StdAfx.h"

#include "PhoneDeviceTaoBao.h"

phonedevice::TaoBao::TaoBao(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "ÌÔ±¦";
}

phonedevice::TaoBao::~TaoBao()
{
	
}

int phonedevice::TaoBao::IsTaoBao(HttpSession *http_session)
{
	/*
	if(  ( http_session->m_Requestline.m_Host.Find("api.m.taobao.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/rest/api3.do") != -1 ) )	
	{
	//	return kUploadPhoneDevice;
	}*/
	return 0;
}

int phonedevice::TaoBao::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::TaoBao::UploadPhoneDevice(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->SplitStringToMap(src_data,'&','=');

	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	string tmp = ms_->GetValueFromMapByKey("data");
//	version_ = ms_->GetValueBetweenBeginEnd(phoneinf,"devos%22%3A%22","%22%2C%22devinfo",true);
//	model_ = ms_->GetValueBetweenBeginEnd(phoneinf,"devinfo%22%3A%22","%22%2C%22utdid",true);
	
	if (imei_.empty() || version_.empty() || model_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

