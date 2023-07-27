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
	//�������� 1
	if((http_session->m_Requestline.m_Host.Find("restapi.amap.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3/geocode/regeo")!=-1))
	{
		return kAndroidLocation;
	}
	//�������� 2
	if((http_session->m_Requestline.m_Host.Find("api.map.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/sdkproxy/v2/lbs_androidsdk/geocoder/v2")!=-1))
	{
		return kAndroidLocation2;
	}
	//����
	if((http_session->m_Requestline.m_Host.Find("restapi.amap.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3/place/text")!=-1))
	{
		return kAndroidPlaceSearch;
	}
	//ԤԼ
	if((http_session->m_Requestline.m_Host.Find("restapi.amap.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3/direction/walking")!=-1))
	{
		return kAndroidOrder;
	}
	return 0;
}

int taxi::Mobike::Process(const char *packet,int action)
{
	//�������� 1
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	//�������� 2
	if (action == kAndroidLocation2)
	{
		return AndroidLocation2(packet);
	}
	//����
	if (action == kAndroidPlaceSearch)
	{
		return AndroidPlaceSearch(packet);
	}
	//ԤԼ
	if (action == kAndroidOrder)
	{
		return AndroidOrder(packet);
	}
	return 0;
}

//�������� 1
int taxi::Mobike::AndroidLocation(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("output=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("output="));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//λ����Ϣ
	string location = ms_->GetValueFromMapByKey("location");
	//����
	string longitude = location.substr(0,location.find(","));
	//γ��
	string latitude = location.substr(location.rfind(",") + 1);
	if (longitude.empty() || latitude.empty())
	{
		return 0;
	}
	//��������
	output_text_ = 
		"from=" +	location + "\r\n" +
		"to=" +		src_data + "\r\n" +
		"state=�������\r\n";

	//output_text_ = 
	//	"from=" +	longitude + "\r\n" +
	//	"to=" +		latitude + "\r\n" +
	//	"state=�������\r\n";
	WriteIndexFile();
	return 1;
}

//�������� 2
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
		"state=�������\r\n";
	WriteIndexFile();
	return 1;
}
//����
int taxi::Mobike::AndroidPlaceSearch(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("output=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("output="));
	}
	ms_->SplitStringToMap(src_data,'&','=');

	//����(��׼��ַ��)
	string city = ms_->GetValueFromMapByKey("city");
	//��ַ
	string address = ms_->GetValueFromMapByKey("keywords",true);
	city += address;
	if (city.empty())
	{
		return 0;
	}
	output_text_ = 
		"contact=" + city + "\r\n" +
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}
//ԤԼ
int taxi::Mobike::AndroidOrder(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("key=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("key="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//��ʼλ��
	string origin = ms_->GetValueFromMapByKey("origin");
	//Ŀ��λ��
	string destination = ms_->GetValueFromMapByKey("destination");
	if (origin.empty() || destination.empty())
	{
		return 0;
	}
	output_text_ = 
		"webname=" + origin + "\r\n" + 		//74	[101] //��ǰλ��
		"contact=" + destination + "\r\n" +	//75    [??]  //Ŀ��λ��
		"state=ԤԼ\r\n";
	WriteIndexFile();
	return 1;
}
