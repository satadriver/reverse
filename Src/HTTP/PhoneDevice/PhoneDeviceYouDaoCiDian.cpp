
#include "StdAfx.h"

#include "PhoneDeviceYouDaoCiDian.h"

phonedevice::YouDaoCiDian::YouDaoCiDian(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "ÓÐµÀ´Êµä";
}

phonedevice::YouDaoCiDian::~YouDaoCiDian()
{
	
}

int phonedevice::YouDaoCiDian::IsYouDaoCiDian(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("dict.youdao.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/profile/text/get") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::YouDaoCiDian::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::YouDaoCiDian::UploadPhoneDevice(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	model_ = ms_->GetValueFromMapByKey("model",true);
	version_ = ms_->GetValueFromMapByKey("mid",true);
	imei_ = ms_->GetValueFromMapByKey("imei",true);
	os_ = GetOS();

	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

