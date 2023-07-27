#include "StdAfx.h"
#include "TaxiMobike.h"

taxi::Mobike::Mobike(HttpSession *http_session, const string &pro_name) :  Taxi(http_session, pro_name)
{
}

taxi::Mobike::~Mobike()
{
	
}

int taxi::Mobike::IsMobike(HttpSession *http_session)
{
	//附近单车 1
	if((http_session->m_Requestline.m_Host.Find("restapi.amap.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3/geocode/regeo")!=-1))
	{
		return kAndroidLocation;
	}
	//附近单车 2
	if((http_session->m_Requestline.m_Host.Find("api.map.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/sdkproxy/v2/lbs_androidsdk/geocoder/v2")!=-1))
	{
		return kAndroidLocation2;
	}
	//搜索
	if((http_session->m_Requestline.m_Host.Find("restapi.amap.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3/place/text")!=-1))
	{
		return kAndroidPlaceSearch;
	}
	//预约
	if((http_session->m_Requestline.m_Host.Find("restapi.amap.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3/direction/walking")!=-1))
	{
		return kAndroidOrder;
	}
	return 0;
}

int taxi::Mobike::Process(const char *packet,int action)
{
	//附近单车 1
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	//附近单车 2
	if (action == kAndroidLocation2)
	{
		return AndroidLocation2(packet);
	}
	//搜素
	if (action == kAndroidPlaceSearch)
	{
		return AndroidPlaceSearch(packet);
	}
	//预约
	if (action == kAndroidOrder)
	{
		return AndroidOrder(packet);
	}
	return 0;
}

//附近单车 1
int taxi::Mobike::AndroidLocation(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("output=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("output="));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//位置信息
	string location = ms_->GetValueFromMapByKey("location");
	//经度
	string longitude = location.substr(0,location.find(","));
	//纬度
	string latitude = location.substr(location.rfind(",") + 1);
	if (longitude.empty() || latitude.empty())
	{
		return 0;
	}
	//测试数据
	output_text_ = 
		"from=" +	location + "\r\n" +
		"to=" +		src_data + "\r\n" +
		"state=距离最近\r\n";

	//output_text_ = 
	//	"from=" +	longitude + "\r\n" +
	//	"to=" +		latitude + "\r\n" +
	//	"state=距离最近\r\n";
	WriteIndexFile();
	return 1;
}

//附近单车 2
int taxi::Mobike::AndroidLocation2(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	uri = ms_->UrlDecode(uri);
	ms_->SplitStringToMap(uri,"&","=");
	string location = ms_->GetValueFromMapByKey("location");
	longitude_ = location.substr(location.rfind(",") + strlen(","));
	latitude_ = location.substr(0,location.find(","));

	if (longitude_.empty() || latitude_.empty())
	{
		return 0;
	}
	output_text_ = 
		"from=" +	longitude_ + "\r\n" +
		"to=" +		latitude_ + "\r\n" +
		"state=距离最近\r\n";
	WriteIndexFile();
	return 1;
}
//搜素
int taxi::Mobike::AndroidPlaceSearch(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("output=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("output="));
	}
	ms_->SplitStringToMap(src_data,'&','=');

	//城市(标准地址码)
	string city = ms_->GetValueFromMapByKey("city");
	//地址
	string address = ms_->GetValueFromMapByKey("keywords",true);
	city += address;
	if (city.empty())
	{
		return 0;
	}
	output_text_ = 
		"contact=" + city + "\r\n" +
		"state=搜索\r\n";
	WriteIndexFile();
	return 1;
}
//预约
int taxi::Mobike::AndroidOrder(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("key=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("key="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//起始位置
	string origin = ms_->GetValueFromMapByKey("origin");
	//目的位置
	string destination = ms_->GetValueFromMapByKey("destination");
	if (origin.empty() || destination.empty())
	{
		return 0;
	}
	output_text_ = 
		"webname=" + origin + "\r\n" + 		//74	[101] //当前位置
		"contact=" + destination + "\r\n" +	//75    [??]  //目的位置
		"state=预约\r\n";
	WriteIndexFile();
	return 1;
}
