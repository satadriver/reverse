
#include "StdAfx.h"

#include "LocationLingShengDuoDuo.h"

location::LingShengDuoDuo::LingShengDuoDuo(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÁåÉù¶à¶à";
}

location::LingShengDuoDuo::~LingShengDuoDuo()
{
	
}

int location::LingShengDuoDuo::IsLingShengDuoDuo(HttpSession *http_session)
{
	if(( http_session->m_Requestline.m_Host.Find("r.domob.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/a/") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::LingShengDuoDuo::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::LingShengDuoDuo::DeviceLocation(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string position = ms_->GetValueFromMapByKey("d[coord]");

	string::size_type pos = position.find(",");
	if (pos != string::npos)
	{
		longitude_ = position.substr(pos + 1);
		latitude_ = position.substr(0, pos);
	}
	
	imei_ = ms_->GetValueFromMapByKey("idv");
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputString();
	
	WriteIndexFile();
	return 1;
}