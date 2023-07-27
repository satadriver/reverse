
#include "StdAfx.h"

#include "LocationMoJiTianQi.h"

location::MoJiTianQi::MoJiTianQi(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Ä«¼£ÌìÆø";
}

location::MoJiTianQi::~MoJiTianQi()
{
	
}

int location::MoJiTianQi::IsMoJiTianQi(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("albs.moji001.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/weather/GetCDNWeatherByLocation?") != -1 ))	
	{
		return kDeviceLocationA;
	}
	if( ( http_session->m_Requestline.m_Host.Find("lbs.moji.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/location/json/query") != -1 ))	
	{
		return kDeviceLocationI;
	}
	return 0;
}


int location::MoJiTianQi::Process(const char *packet,int action)
{
	if (action == kDeviceLocationA)
	{
		return DeviceLocationA(packet);
	}
	if (action == kDeviceLocationI)
	{
		return DeviceLocationI(packet);
	}
	return 0;
}

int location::MoJiTianQi::DeviceLocationA(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','='); 
	
	imei_ = ms_->GetValueFromMapByKey("IMEI");
	longitude_ = ms_->GetValueFromMapByKey("cdma_lng");
	latitude_ = ms_->GetValueFromMapByKey("cdma_lat");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputString() + 
		"platform=Android\r\n";
	
	Record(app_);
	WriteIndexFile();
	return 1;

}

int location::MoJiTianQi::DeviceLocationI(const char *packet)
{
	
	string src_data = GetPostData_tmp(packet);
	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	ms_->Replace(src_data," ","");
	ms_->Replace(src_data,"\r","");
	ms_->Replace(src_data,"\n","");

	ms_->SplitStringToMap(src_data,',',':');

	imei_ = ms_->GetValueFromMapByKey("identifier");
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");

	if (latitude_.empty() || longitude_.empty())
	{
			return 0;
	}
		
	output_text_ = GetOutputString() + 
		"platform=iOS\r\n";
	Record(app_);
	WriteIndexFile();
	return 1;
	
}
