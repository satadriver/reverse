
#include "StdAfx.h"

#include "PhoneDeviceDouGuoMeiShi.h"

phonedevice::DouGuoMeiShi::DouGuoMeiShi(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "¶¹¹ûÃÀÊ³";
}

phonedevice::DouGuoMeiShi::~DouGuoMeiShi()
{
	
}

int phonedevice::DouGuoMeiShi::IsDouGuoMeiShi(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("api.douguo.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/app/active/") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::DouGuoMeiShi::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::DouGuoMeiShi::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");

	model_ = ms_->GetValueFromMapByKey("device",true);
	imei_ = ms_->GetValueFromMapByKey("imei",true);
	version_ = ms_->GetValueFromMapByKey("sdk",true);
	os_ = "Android";

	if (model_.empty() || version_.empty() || imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

