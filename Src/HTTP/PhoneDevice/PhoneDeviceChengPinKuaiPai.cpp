
#include "StdAfx.h"

#include "PhoneDeviceChengPinKuaiPai.h"

phonedevice::ChengPinKuaiPai::ChengPinKuaiPai(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "³ÉÆ·¿ìÅÄ";
}

phonedevice::ChengPinKuaiPai::~ChengPinKuaiPai()
{
	
}

int phonedevice::ChengPinKuaiPai::IsChengPinKuaiPai(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("guansir.kuaipai.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/support/api/user/subscriber/save.do") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::ChengPinKuaiPai::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::ChengPinKuaiPai::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");

	model_ = ms_->GetValueFromMapByKey("mobileModel",true);
	os_ = ms_->GetValueFromMapByKey("mobileOs",true);
	imei_ = ms_->GetValueFromMapByKey("imei",true);
	imsi_ = ms_->GetValueFromMapByKey("imsi",true);
	longitude_ = ms_->GetValueFromMapByKey("latitude",true);
	latitude_ = ms_->GetValueFromMapByKey("longitude",true);


	string location = ms_->GetValueFromMapByKey("location",true);


	if (model_.empty() || os_.empty() || imei_.empty() || imsi_.empty() || longitude_.empty() || latitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

