#include "StdAfx.h"
#include "TaxiYiDaoYongChe.h"


taxi::YiDaoYongChe::YiDaoYongChe(HttpSession *http_session, const string &pro_name) :  Taxi(http_session, pro_name)
{
}

taxi::YiDaoYongChe::~YiDaoYongChe()
{
	
}

int taxi::YiDaoYongChe::IsYiDaoYongChe(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("driver-api.yongche.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Driver/VerifyCooperaStatus")!=-1))
	{
		return kDriverLogin;
	}
	return 0;
}

int taxi::YiDaoYongChe::Process(const char *packet,int action)
{

	if (action == kDriverLogin)
	{
		return DriverLogin(packet);
	}
	return 0;
}
int taxi::YiDaoYongChe::DriverLogin(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');

	string carno = ms_->GetValueFromMapByKey("vehicle_number");
	phone_ =  ms_->GetValueFromMapByKey("cellphone");
	imei_ = ms_->GetValueFromMapByKey("imei");

	if (carno.empty() || phone_.empty() ||  imei_.empty())
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