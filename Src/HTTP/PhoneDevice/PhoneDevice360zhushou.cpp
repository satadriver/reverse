
#include "StdAfx.h"

#include "PhoneDevice360zhushou.h"

phonedevice::SHOUJIZHUSHOU360::SHOUJIZHUSHOU360(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "360手机助手";
}

phonedevice::SHOUJIZHUSHOU360::~SHOUJIZHUSHOU360()
{
	
}

int phonedevice::SHOUJIZHUSHOU360::IsSHOUJIZHUSHOU360(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("md.openapi.360.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/baohe/list") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::SHOUJIZHUSHOU360::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::SHOUJIZHUSHOU360::UploadPhoneDevice(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
    //备用
	string cpu = ms_->GetValueFromMapByKey("cpu",true);
	string br = ms_->GetValueFromMapByKey("br",true);
	string ppi = ms_->GetValueFromMapByKey("ppi",true);

	model_ = ms_->GetValueFromMapByKey("md",true);
	version_ = ms_->GetValueFromMapByKey("os");
	os_ = GetOS();
	
	if ( version_.empty() || model_.empty() || os_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

