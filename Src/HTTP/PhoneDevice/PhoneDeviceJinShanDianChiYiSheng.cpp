
#include "StdAfx.h"

#include "PhoneDeviceJinShanDianYiSheng.h"

phonedevice::JinShanDianChiYiSheng::JinShanDianChiYiSheng(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "金山电池医生";
}

phonedevice::JinShanDianChiYiSheng::~JinShanDianChiYiSheng()
{
	
}

int phonedevice::JinShanDianChiYiSheng::IsJinShanDianChiYiSheng(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("dcys.ijinshan.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("imei") != -1 ) )	
	{
		return kUploadPhoneDevice1;
	}
	/*
	if((http_session->m_Requestline.m_Host.Find("dcys.ijinshan.com")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("imei")!=-1))
	{
		return kUploadPhoneDevice2;
	}
	*/
	return 0;
}

int phonedevice::JinShanDianChiYiSheng::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice1)
	{
		return UploadPhoneDevice1(packet);
	}

	if (action == kUploadPhoneDevice2)
	{
		return UploadPhoneDevice2(packet);
	}
	return 0;
}


int phonedevice::JinShanDianChiYiSheng::UploadPhoneDevice1(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');


	version_ = ms_->GetValueFromMapByKey("andver");
	model_ = ms_->GetValueFromMapByKey("mode");
	imei_ = ms_->GetValueFromMapByKey("imei");
	
	if (src_data.find("androidid=") != string::npos)
	{
		os_ = "Android";
	}

	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
		return 0;
	}
		
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

int phonedevice::JinShanDianChiYiSheng::UploadPhoneDevice2(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');
	
	
	version_ = ms_->GetValueFromMapByKey("andver");
	model_ = ms_->GetValueFromMapByKey("mode");
	imei_ = ms_->GetValueFromMapByKey("imei");
	
	os_ = GetOS();
	
	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}
