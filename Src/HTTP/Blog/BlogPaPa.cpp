
#include "StdAfx.h"

#include "BlogPaPa.h"

blog::PaPa::PaPa(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::PaPa::~PaPa()
{
	
}

int blog::PaPa::IsPaPa(HttpSession *http_session)
{
	if( (http_session->m_Requestline.m_Host.Find("api.papa.me") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/api/v1/post") != -1))	
	{
		return kNewTopic;
	}
	return 0;
}

int blog::PaPa::Process(const char *packet,int action)
{
	if (action == kNewTopic)
	{
		return NewTopic(packet);
	}
	return 0;
}

int blog::PaPa::NewTopic(const char *packet)
{
	string src_data = GetPostData_tmp(packet);

	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string mac = ms_->GetValueFromMapByKey("Device-MAC");

	ms_->SplitStringToMap(src_data,'&','=');
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	content_ = ms_->GetValueFromMapByKey("attachMessage",true);

	if (mac.empty() || longitude_.empty() || latitude_.empty() || content_.empty())
	{
		return 0;
	}

	output_text_ =
		"devicemac=" + mac + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"msgtype=·¢±í\r\n";
	WriteIndexFile();
	return 1;
}