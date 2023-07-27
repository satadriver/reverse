
#include "StdAfx.h"
#include "TaxiDiDiZhuanChe.h"


taxi::DiDiZhuanChe::DiDiZhuanChe(HttpSession *http_session, const string &pro_name) :  Taxi(http_session, pro_name)
{
}

taxi::DiDiZhuanChe::~DiDiZhuanChe()
{
	
}

int taxi::DiDiZhuanChe::IsDiDiZhuanChe(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("api.udache.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/gulfstream/api/v1/driver/dLogin")!=-1))
	{
		return kZCDriverLogin;
	}
	return 0;
}

int taxi::DiDiZhuanChe::Process(const char *packet,int action)
{
	

	if (action == kZCDriverLogin)
	{
		return ZCDriverLogin(packet);
	}
	return 0;
}


int taxi::DiDiZhuanChe::ZCDriverLogin(const char *packet)
{
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	phone_ = ms_->GetValueFromMapByKey("phone");
	imei_ = ms_->GetValueFromMapByKey("imei");

	if (phone_.empty() || imei_.empty())
	{
		return 0;
	}

	output_text_ = 
		"user=" + phone_ + "\r\n" +
		"imei=" + imei_ + "\r\n" +
		"state=专车司机上线\r\n";
	WriteIndexFile();
	return 1;
}