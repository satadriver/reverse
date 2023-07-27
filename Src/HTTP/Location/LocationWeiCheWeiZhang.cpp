
#include "StdAfx.h"

#include "LocationWeiCheWeiZhang.h"

location::WeiCheWeiZhang::WeiCheWeiZhang(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Î¢³µÎ¥ÕÂ²éÑ¯";
}

location::WeiCheWeiZhang::~WeiCheWeiZhang()
{
	
}

int location::WeiCheWeiZhang::IsWeiCheWeiZhang(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("martin.frontend_log")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("log.buding.cn") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::WeiCheWeiZhang::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::WeiCheWeiZhang::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,',',':');
	
	string imei = ms_->GetValueFromMapByKey("imei");
	string imsi = ms_->GetValueFromMapByKey("imsi");
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	
	if (imei.empty() ||imsi.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();

	Record(app_);
	WriteIndexFile();
	return 1;
}