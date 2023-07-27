
#include "StdAfx.h"

#include "PhoneInfoTianMiao.h"

phoneinfo::TianMiao::TianMiao(HttpSession *http_session, const string &pro_name) : PhoneInfo(http_session,pro_name)
{
	app_ = "ÌìÃ¨";
}

phoneinfo::TianMiao::~TianMiao()
{
	
}

int phoneinfo::TianMiao::IsTianMiao(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("ya.tmall.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/rest.do") != -1 ) )	
	{
		return kUploadPhoneInfo;
	}
	return 0;
}

int phoneinfo::TianMiao::Process(const char *packet,int action)
{
	if (action == kUploadPhoneInfo)
	{
		return UploadPhoneInfo(packet);
	}
	return 0;
}


int phoneinfo::TianMiao::UploadPhoneInfo(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');

	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
//	version = ms_->GetValueBetweenBeginEnd(phoneinf,"Version%22%3A%22","%22%2C%22modle",true);
//	phoneinf = ms_->GetValueFromMapByKey("data");
//	model = ms_->GetValueBetweenBeginEnd(phoneinf,"modle%22%3A%22","%22%2C%22channel",true);
	
	if (imsi_.empty() || imei_.empty()/* || version.empty() || model.empty()*/)
	{
		return 0;
	}

	
	WriteIndexFile();
	return 1;
}

