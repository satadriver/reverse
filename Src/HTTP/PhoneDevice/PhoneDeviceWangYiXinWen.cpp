
#include "StdAfx.h"

#include "PhoneDeviceWangYiXinWen.h"

phonedevice::WangYiXinWen::WangYiXinWen(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "ÍøÒ×ÐÂÎÅ";
}

phonedevice::WangYiXinWen::~WangYiXinWen()
{
	
}

int phonedevice::WangYiXinWen::IsWangYiXinWen(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("m.analytics.126.net")!= -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/news/c") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::WangYiXinWen::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::WangYiXinWen::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,',',':',"\"");

	version_ = ms_->GetValueFromMapByKey("o");
	model_ = ms_->GetValueFromMapByKey("m",true);
	imei_ = ms_->GetValueFromMapByKey("u");
	
	os_ = GetOS();
	
	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
	
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

