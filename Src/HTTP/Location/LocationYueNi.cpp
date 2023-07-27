
#include "StdAfx.h"

#include "LocationYueNi.h"

location::YueNi::YueNi(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Ô¼Äã";
}

location::YueNi::~YueNi()
{
	
}

int location::YueNi::IsYueNi(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("open.iyueni.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/index.php") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::YueNi::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::YueNi::DeviceLocation(const char *packet)
{
	string src_data(packet);
	src_data = UrlDecode(src_data);
	string str = ms_->GetValueBetweenBeginEnd(src_data,"params\":{","}}&",false);
	ms_->SplitStringToMap(str,',',':',"\"");
	string imei	= ms_->GetValueFromMapByKey("imei");
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	string model = ms_->GetValueFromMapByKey("model",true);
	string user = ms_->GetValueFromMapByKey("phonenumber");
	
	if (longitude_.empty() || latitude_.empty() || imei.empty() || user.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}