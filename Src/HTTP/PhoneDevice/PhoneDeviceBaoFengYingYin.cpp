
#include "StdAfx.h"

#include "PhoneDeviceBaoFengYingYin.h"

phonedevice::BaoFengYingYin::BaoFengYingYin(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "±©·çÓ°Òô";
}

phonedevice::BaoFengYingYin::~BaoFengYingYin()
{
	
}

int phonedevice::BaoFengYingYin::IsBaoFengYingYin(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("wx.houyi.baofeng.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/Consultation/web.php") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::BaoFengYingYin::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::BaoFengYingYin::UploadPhoneDevice(const char *packet)
{
	char *p = (char *)packet;
	if (!p)
	{
		return 0;
	}
	int len = http_session_->m_Requestline.m_nseq - http_session_->m_Requestline.m_Ssequence;
	int i = 0;
	for (;i <= len ; i++,p++)
	{
		if ( 0 == *p)
		{
			*p = '&';
		}
	}
	string src_data(packet);
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');

	os_ = ms_->GetValueFromMapByKey("os");
	version_ = ms_->GetValueFromMapByKey("osver");
	model_ = ms_->GetValueFromMapByKey("jx");
	imei_ = ms_->GetValueFromMapByKey("imei");
	
	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

