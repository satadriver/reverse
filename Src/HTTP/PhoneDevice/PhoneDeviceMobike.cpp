
#include "StdAfx.h"

#include "PhoneDeviceMobike.h"

phonedevice::Mobike::Mobike(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "摩拜单车";
}

phonedevice::Mobike::~Mobike()
{
	
}

int phonedevice::Mobike::IsMobike(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("182.92.0.214:80")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/1187161213178428/kefuchannelapp32172/devices") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::Mobike::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}
//设备信息
int phonedevice::Mobike::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{	
		version_ = value["version"].asString();
		model_ = value["model"].asString();
		imei_ = value["imei"].asString();
		os_ = GetOS();
		double lng,lat;
		CString strLng,strLat;
		lng = value["loc.lng"].asDouble();
		lat = value["loc.lat"].asDouble();
		strLng.Format("%f",lng);
		strLat.Format("%f",lat);
		longitude_ = strLng.GetBuffer(0);
		latitude_ = strLat.GetBuffer(0);
	}
	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty() || longitude_.empty() || latitude_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}

