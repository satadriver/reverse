
#include "StdAfx.h"

#include "PhoneDeviceAnZhuoShiChang.h"

phonedevice::AnZhuoShiChang::AnZhuoShiChang(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "安卓市场";
}

phonedevice::AnZhuoShiChang::~AnZhuoShiChang()
{
	
}

int phonedevice::AnZhuoShiChang::IsAnZhuoShiChang(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("appc.baidu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/appsrv") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::AnZhuoShiChang::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::AnZhuoShiChang::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	string url(http_session_->m_Requestline.m_URI.GetBuffer(0));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string pkname = ms_->GetValueBetweenBeginEnd(url,"pkname=","&",true);
	if (pkname == "com.baidu.appsearch")
	{
		app_ = "百度手机助手";
	}
	model_ = ms_->GetValueBetweenBeginEnd(src_data,"devinfo@",",",true);
	version_ = ms_->GetValueBetweenBeginEnd(src_data,"osver@",",",true);
	os_ = GetOS();
	if (version_.empty() || model_.empty() || os_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

