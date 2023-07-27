
#include "StdAfx.h"

#include "LocationYouJia.h"

location::YouJia::YouJia(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "сп╪с";
}

location::YouJia::~YouJia()
{
	
}

int location::YouJia::IsYouJia(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.youja.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/v1/friendsearch/geo") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::YouJia::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::YouJia::DeviceLocation(const char *packet)
{
	char *p = (char *)packet;
	if (!p)
	{
		return 0;
	}
	int len = http_session_->m_Requestline.m_nseq - http_session_->m_Requestline.m_Ssequence;
	int i = 0;
	for (;i <= len ; i++,p++)
	{
		if ( 0 == *p)
		{
			*p = '/';
		}
	}
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string position = ms_->GetValueFromMapByKey("latlng");
	longitude_ = position.substr(position.find("/") + 1,position.length());
	latitude_ = position.substr(0,position.find("/"));
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();

	Record(app_);
	WriteIndexFile();
	return 1;
}