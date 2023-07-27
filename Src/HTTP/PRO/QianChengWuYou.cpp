
#include "StdAfx.h"

#include "QianChengWuYou.h"

QianChengWuYou::QianChengWuYou()
{
	software_ = "前程无忧";
}

QianChengWuYou::~QianChengWuYou()
{
	
}

int QianChengWuYou::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("api.51job.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/util/get_location.php") != -1 ) )	
	{
		return kDeviceGPSLocationM;
	}

	if( ( http_session->m_Requestline.m_Host.Find("my.51job.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/cv/in/Resume/WizardAction.php") != -1 ) )	
	{
		return kCreateResumeCW;
	}
	return 0;
}

int QianChengWuYou::Process(const char *packet,int action)
{
	if (action == kDeviceGPSLocationM)
	{
		return DeviceGPSLocationM(packet);
	}

	if (action == kCreateResumeCW)
	{
		return CreateResumeCW(packet);
	}
	return 0;
}


int QianChengWuYou::DeviceGPSLocationM(const char *packet)
{
	string src_data = GetPostData_tmp(packet);

	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	
	string longitude = ms_->GetValueFromMapByKey("baiduloc_longitude");
	string latitude = ms_->GetValueFromMapByKey("baiduloc_latitude");
	
	if (latitude.empty() || longitude.empty())
	{
		return 0;
	}

	output_text_ = 
		"from=" + longitude + "\r\n" +
		"to=" + latitude + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}

int QianChengWuYou::CreateResumeCW(const char *packet)
{	
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');
	string name = ms_->GetValueFromMapByKey("Name",true);
	string phone = ms_->GetValueFromMapByKey("Mobile");
	string gender = ms_->GetValueFromMapByKey("Gender");
	string id = ms_->GetValueFromMapByKey("IDNumber");
	
	if (name.empty() || phone.empty() || gender.empty() )
	{
		return 0;
	}
	
	if (gender == "0")
	{
		gender = "男";
	}
	if (gender == "1")
	{
		gender = "女";
	}
	
	output_text_ = 
		"user=" + name + "\r\n" +
		"from=" + gender + "\r\n" +
		"to=" + id + "\r\n" +
		"webname=" + phone + "\r\n" +
		"state=" + software_ + "\r\n";
	WriteIndexFile();
	return 1;
}