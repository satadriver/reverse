
#include "StdAfx.h"

#include "PhoneDeviceXinLangWeiBo.h"

phonedevice::XinLangWeiBo::XinLangWeiBo(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "ÐÂÀËÎ¢²©";
}

phonedevice::XinLangWeiBo::~XinLangWeiBo()
{
	
}

int phonedevice::XinLangWeiBo::IsXinLangWeiBo(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("wbapp.mobile.sina.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/interface/h5/h5ad.php") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::XinLangWeiBo::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::XinLangWeiBo::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string devicinfo;
	devicinfo = ms_->GetValueFromMapByKey("ua");
	imei_ = ms_->GetValueFromMapByKey("imei");
	
	if (devicinfo.empty() || imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

