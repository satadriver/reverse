
#include "StdAfx.h"

#include "LocationYeDuShi.h"

location::YeDuShi::YeDuShi(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Ò¹¶¼ÊÐ";
}

location::YeDuShi::~YeDuShi()
{
	
}

int location::YeDuShi::IsYeDuShi(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("apiand.yeds.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/API/User/Login") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::YeDuShi::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::YeDuShi::DeviceLocation(const char *packet)
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
			*p = '&';
		}
	}
	string src_data(http_session_->m_Requestline.m_URI);
	src_data = UrlDecode(src_data);
	string str = ms_->GetValueBetweenBeginEnd(src_data,"reqmsg={","}&",false);
	ms_->SplitStringToMap(str,',',':',"\"");
	string imei	= ms_->GetValueFromMapByKey("DeviceID");

	string tmp_data(packet);
	longitude_ = ms_->GetValueBetweenBeginEnd(tmp_data,"lng&&","&&",false);
	latitude_ = ms_->GetValueBetweenBeginEnd(tmp_data,"lat&&","&&",false);

	if (longitude_.empty() || latitude_.empty() || imei.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();

	Record(app_);
	WriteIndexFile();
	
	return 1;
}