
#include "StdAfx.h"

#include "LocationMeiTuan.h"

location::MeiTuan::MeiTuan(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "美团";
}

location::MeiTuan::~MeiTuan()
{
	
}

int location::MeiTuan::IsMeiTuan(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("api.mobile.meituan.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/group/v1/city/latlng/") != -1 ) )	
	{
		return kDeviceLocation;
	}

	if( ( http_session->m_Requestline.m_Host.Find("api.meituan.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/group/v1/deal/topic/elaborate/city") != -1 ) )	
	{
		return kDeviceLocation2;
	}
	//更新 2017-02-27 提交订单时获取位置信息	
	if( ( http_session->m_Requestline.m_Host.Find("mreport.meituan.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/data/collect.json") != -1 ) )	
	{
		return kDevideLocationOrder;
	}
	return 0;
}

int location::MeiTuan::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}

	if (action == kDeviceLocation2)
	{
		return DeviceLocation2(packet);
	}
	//提交订单 时获取位置信息
	if (action == kDevideLocationOrder)
	{
		return DeviceLocationOrder(packet);
	}
	return 0;
}

int location::MeiTuan::DeviceLocation(const char *packet)
{
	string src_data(packet);
	string strtemp(packet + src_data.length() + 1);
	ms_->SplitStringToMap(strtemp,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("utm_content");
	longitude_ = strtemp.substr(0,strtemp.find("?"));
	latitude_ = src_data.substr(src_data.find("latlng/") + 7,src_data.length());
	
	if (imei_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}

int location::MeiTuan::DeviceLocation2(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	uri = ms_->UrlDecode(uri);
	ms_->SplitStringToMap(uri,'&','=');
	
	string latlng = ms_->GetValueFromMapByKey("latlng");
	string::size_type pos = latlng.find(",");
	if (pos == string::npos)
	{
		return 0;
	}

	latitude_ = latlng.substr(0,pos);
	longitude_ = latlng.substr(pos + 1);

	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}

//提交订单时 获取位置信息
int location::MeiTuan::DeviceLocationOrder(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("content");
	content = ms_->UrlDecode(content);
	double lng,lat;
	CString strlng,strlat;
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(content,value))
	{	
		imei_ = value["imsi"].asString();
		lng = value["lng"].asDouble();
		lat = value["lat"].asDouble();
		strlng.Format("%f",lng);
		strlat.Format("%f",lat);
		longitude_ = strlng.GetBuffer(0);
		latitude_ = strlat.GetBuffer(0);
		mac_ = value["mac"].asString();
	}	
	if (imei_.empty() || latitude_.empty() || longitude_.empty() || mac_.empty())
	{
		return 0;
	}	
	output_text_ = GetOutputString();

	Record(app_);
	WriteIndexFile();
	return 1;
}
