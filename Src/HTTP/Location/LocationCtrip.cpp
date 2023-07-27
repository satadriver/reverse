#include "StdAfx.h"
#include "LocationCtrip.h"

location::LocationCtrip::LocationCtrip(HttpSession *http_session, const string &pro_name):Location(http_session,pro_name)
{
	app_ = "携程";
}

location::LocationCtrip::~LocationCtrip()
{
	
}

int location::LocationCtrip::IsLocationCtrip(HttpSession *http_session)
{
	//位置信息
	if( ( http_session->m_Requestline.m_Host.Find("m.ctrip.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/restapi/soa2/10398/json/LBSLocateCity") != -1))	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::LocationCtrip::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::LocationCtrip::DeviceLocation(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	string countryname,townname;
	if(reader.parse(src_data,value))
	{	
		double lng,lat;
		CString strlng,strlat;
		lng = value["Longitude"].asDouble();
		lat = value["Latitude"].asDouble();	
		strlng.Format("%f",lng);
		strlat.Format("%f",lat);
		longitude_ = strlng.GetBuffer(0);
		latitude_ = strlat.GetBuffer(0);
	}	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputString();
	Record(app_);
	WriteIndexFile();
	return 1;
}