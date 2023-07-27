
#include "StdAfx.h"

#include "PhoneDeviceYingYongBao.h"

phonedevice::YingYongBao::YingYongBao(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "Ó¦ÓÃ±¦";
}

phonedevice::YingYongBao::~YingYongBao()
{
	
}

int phonedevice::YingYongBao::IsYingYongBao(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("isdspeed.qq.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/cgi-bin/r.cgi") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::YingYongBao::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::YingYongBao::UploadPhoneDevice(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	imei_ = ms_->GetValueBetweenBeginEnd(src_data,"imei=",";",true);
	os_ = "Android";
	
	if ( imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

