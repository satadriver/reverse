
#include "StdAfx.h"

#include "MoJiTianQi.h"

MoJiTianQi::MoJiTianQi()
{
	software_ = "Ä«¼£ÌìÆø";
}

MoJiTianQi::~MoJiTianQi()
{
	
}

int MoJiTianQi::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("albs.moji001.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/weather/GetCDNWeatherByLocation?") != -1 ))	
	{
		return kGPSLocationMA;
	}

	if( ( http_session->m_Requestline.m_Host.Find("lbs.moji.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/location/json/query") != -1 ))	
	{
		return kGPSLocationMI;
	}

	if( ( http_session->m_Requestline.m_Host.Find("register.moji001.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/weather/RegisterUser") != -1 ) )	
	{
		return kUploadDeviceInfoMA;
	}
	return 0;
}


int MoJiTianQi::Process(const char *packet,int action)
{
	if (action == kGPSLocationMA)
	{
		return GPSLocationMA(packet);
	}

	if (action == kGPSLocationMI)
	{
		return GPSLocationMI(packet);
	}

	if (action == kUploadDeviceInfoMA)
	{
		return UploadDeviceInfoMA(packet);
	}
	return 0;
}

int MoJiTianQi::GPSLocationMA(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','='); 
	
	string imei = ms_->GetValueFromMapByKey("IMEI");
	string longitude = ms_->GetValueFromMapByKey("cdma_lng");
	string latitude = ms_->GetValueFromMapByKey("cdma_lat");
	
	if (latitude.empty() || longitude.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"oid=" + imei + "\r\n" +
		"from=" + longitude + "\r\n" +
		"to=" + latitude + "\r\n" +
		"state=" + software_ + "\r\n" + 
		"platform=Android\r\n";
	
	WriteIndexFile();
	return 1;

}

int MoJiTianQi::GPSLocationMI(const char *packet)
{
	string src_data = GetPostData_tmp(packet);
	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	ms_->Replace(src_data," ","");
	ms_->Replace(src_data,"\r","");
	ms_->Replace(src_data,"\n","");

	ms_->SplitStringToMap(src_data,',',':');

	string imei = ms_->GetValueFromMapByKey("identifier");
	string longitude = ms_->GetValueFromMapByKey("longitude");
	string latitude = ms_->GetValueFromMapByKey("latitude");

	if (latitude.empty() || longitude.empty())
	{
		return 0;
	}
		
	output_text_ =
		"oid=" + imei + "\r\n" +
		"from=" + longitude + "\r\n" +
		"to=" + latitude + "\r\n" +
		"state=" + software_ + "\r\n" + 
		"platform=iOS\r\n";
	WriteIndexFile();
	return 1;
	
}

int MoJiTianQi::UploadDeviceInfoMA(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string os = ms_->GetValueFromMapByKey("Platform");
	string version = ms_->GetValueFromMapByKey("SdkVer");
	string model = ms_->GetValueFromMapByKey("Model",true);
	string imei = ms_->GetValueFromMapByKey("IMEI");
	
	
	if (imei.empty() || version.empty() || model.empty() || os.empty())
	{
		
		return 0;
	}
	
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"webname=" + model + "\r\n" +
		"from=" + os + "\r\n" +
		"to=" + version + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	
	return 1;
}
