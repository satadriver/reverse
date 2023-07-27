
#include "StdAfx.h"

#include "PhoneDeviceFengHuangXinWen.h"

phonedevice::FengHuangXinWen::FengHuangXinWen(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "·ï»ËÐÂÎÅ";
}

phonedevice::FengHuangXinWen::~FengHuangXinWen()
{
	
}

int phonedevice::FengHuangXinWen::IsFengHuangXinWen(HttpSession *http_session)
{
	if(	(http_session->m_Requestline.m_Host.Find("stadig.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/appsta.js?")!=-1))
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::FengHuangXinWen::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::FengHuangXinWen::UploadPhoneDevice(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(uri,'&','=');

	model_ = ms_->GetValueFromMapByKey("ua");
	imei_ = ms_->GetValueFromMapByKey("userkey");
	string os = ms_->GetValueFromMapByKey("mos");
	string::size_type pos = os.find("_");
	if (pos != string::npos)
	{
		os_ = os.substr(0, pos);
		version_ = os.substr(pos + 1);
	}
	
	if (model_.empty() || imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

