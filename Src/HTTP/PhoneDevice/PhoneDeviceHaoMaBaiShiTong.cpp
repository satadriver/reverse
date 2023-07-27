
#include "StdAfx.h"

#include "PhoneDeviceHaoMaBaiShiTong.h"

phonedevice::HaoMaBaiShiTong::HaoMaBaiShiTong(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "ºÅÂë°ÙÊÂÍ¨";
}

phonedevice::HaoMaBaiShiTong::~HaoMaBaiShiTong()
{
	
}

int phonedevice::HaoMaBaiShiTong::IsHaoMaBaiShiTong(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("if.pim.189.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/pim_rpc/") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::HaoMaBaiShiTong::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::HaoMaBaiShiTong::UploadPhoneDevice(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	url = ms_->UTF8ToGBK(UrlDecode(url));
	ms_->SplitStringToMap(url,"&","=");
	imei_ = ms_->GetValueFromMapByKey("meid",true);
	imsi_ = ms_->GetValueFromMapByKey("imsi",true);
	model_ = ms_->GetValueFromMapByKey("mobile_type",true);
	version_ = ms_->GetValueFromMapByKey("system_vresion",true);
	os_ = GetOS();
	if (imei_.empty() || imsi_.empty() || model_.empty() || version_.empty() || os_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

