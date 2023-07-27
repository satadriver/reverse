
#include "StdAfx.h"

#include "PhoneInfoTaoBao.h"

phoneinfo::TaoBao::TaoBao(HttpSession *http_session, const string &pro_name) : PhoneInfo(http_session,pro_name)
{
	app_ = "ÌÔ±¦";
}

phoneinfo::TaoBao::~TaoBao()
{
	
}

int phoneinfo::TaoBao::IsTaoBao(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.m.taobao.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/rest/api3.do") != -1 ) )	
	{
		return kUploadPhoneInfo;
	}
	return 0;
}

int phoneinfo::TaoBao::Process(const char *packet,int action)
{
	if (action == kUploadPhoneInfo)
	{
		return UploadPhoneInfo(packet);
	}
	return 0;
}


int phoneinfo::TaoBao::UploadPhoneInfo(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->SplitStringToMap(src_data,'&','=');

	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
//	phoneinf = ms_->GetValueFromMapByKey("data");
//	version = ms_->GetValueBetweenBeginEnd(phoneinf,"devos%22%3A%22","%22%2C%22devinfo",true);
//	model = ms_->GetValueBetweenBeginEnd(phoneinf,"devinfo%22%3A%22","%22%2C%22utdid",true);
	
	if (imsi_.empty() || imei_.empty() /*|| version.empty() || model.empty()*/)
	{
		return 0;
	}
	WriteIndexFile();
	return 1;
}

