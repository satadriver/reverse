
#include "StdAfx.h"

#include "LocationBaiDuWangYeDingWei.h"

location::BaiDuWangYeDingWei::BaiDuWangYeDingWei(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "百度网页定位";
}

location::BaiDuWangYeDingWei::~BaiDuWangYeDingWei()
{
	
}

int location::BaiDuWangYeDingWei::IsBaiDuWangYeDingWei(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("aladdin.wapmap.baidu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/plugin") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::BaiDuWangYeDingWei::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::BaiDuWangYeDingWei::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string position = ms_->GetValueFromMapByKey("xyr");

	
	string::size_type pos2 = position.rfind("_");

	if (pos2 != string::npos)
	{
		position = position.substr(0,pos2);
		string::size_type pos1 = position.find("_");
		if (pos1 != string::npos)
		{
			longitude_ = position.substr(0, pos1);
			latitude_ = position.substr(pos1 + 1);
		}
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