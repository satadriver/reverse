
#include "StdAfx.h"
#include "TaxiYiHaoZhuanChe.h"


taxi::YiHaoZhuanChe::YiHaoZhuanChe(HttpSession *http_session, const string &pro_name) :  Taxi(http_session, pro_name)
{
}

taxi::YiHaoZhuanChe::~YiHaoZhuanChe()
{
	
}

int taxi::YiHaoZhuanChe::IsYiHaoZhuanChe(HttpSession *http_session)
{
	if(	(http_session->m_Requestline.m_URI.Find("/vip-d/driver/signInUp")!=-1))
	{
		return kZCDriverLogin;
	}
	return 0;
}

int taxi::YiHaoZhuanChe::Process(const char *packet,int action)
{
	
	if (action == kZCDriverLogin)
	{
		return ZCDriverLogin(packet);
	}
	return 0;
}

int taxi::YiHaoZhuanChe::ZCDriverLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	phone_ =  ms_->GetValueFromMapByKey("phone");
	imei_ = ms_->GetValueFromMapByKey("device_info");
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");

	if (phone_.empty() || imei_.empty() || longitude_.empty() || latitude_.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + phone_ + "\r\n" +
		"imei=" + imei_ + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=专车司机上线\r\n";

	WriteIndexFile();
	return 1;
}