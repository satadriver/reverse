
#include "StdAfx.h"

#include "PhoneDeviceBiLin.h"
#include "../BoundaryParser.h"
phonedevice::BiLin::BiLin(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "╠хаз";
}

phonedevice::BiLin::~BiLin()
{
	
}

int phonedevice::BiLin::IsBiLin(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("crash-reporting.yy.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/dau/reporting") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::BiLin::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::BiLin::UploadPhoneDevice(const char *packet)
{

	string src_data(packet);
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary);
	
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"data\"");
	
	bv = bp.GetBoundaryValue(key);
	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string content_ = ms_->GetValueBetweenBeginEnd(src_data,key,boundary,true);	
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(content_,value))
	{
		return 0;
	}
	imei_ = value["imei"].asString();
	model_ = value["from"].asString();
	os_ = "Android";
	
	if ( imei_.empty() || model_.empty() || os_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

