
#include "StdAfx.h"

#include "PaPa.h"

PaPa::PaPa()
{
	
}

PaPa::~PaPa()
{
	
}

int PaPa::Is(HttpSession *http_session)
{
	if( (http_session->m_Requestline.m_Host.Find("api.papa.me") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/api/v1/post") != -1))	
	{
		return kNewTopic;
	}
	return 0;
}

int PaPa::Process(const char *packet,int action)
{
	if (action == kNewTopic)
	{
		return NewTopic(packet);
	}
	return 0;
}

int PaPa::NewTopic(const char *packet)
{
	string src_data = GetPostData_tmp(packet);

	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string mac = ms_->GetValueFromMapByKey("Device-MAC");

	ms_->SplitStringToMap(src_data,'&','=');

	string content = ms_->GetValueFromMapByKey("attachMessage",true);

	if (mac.empty() ||  content.empty())
	{
		return 0;
	}

	output_text_ =
		"devicemac=" + mac + "\r\n" +
		"msg=" + content + "\r\n" +
		"msgtype=·¢±í\r\n";
	WriteIndexFile();
	return 1;
}