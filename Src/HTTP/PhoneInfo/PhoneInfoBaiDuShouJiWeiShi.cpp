
#include "StdAfx.h"

#include "PhoneInfoBaiDuShouJiWeiShi.h"

phoneinfo::BaiDuShouJiWeiShi::BaiDuShouJiWeiShi(HttpSession *http_session, const string &pro_name) : PhoneInfo(http_session,pro_name)
{
	app_ = "百度手机卫士";
}

phoneinfo::BaiDuShouJiWeiShi::~BaiDuShouJiWeiShi()
{
	
}

int phoneinfo::BaiDuShouJiWeiShi::IsBaiDuShouJiWeiShi(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("atv.dxsvr.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/scan") != -1 ) )	
	{
		return kUploadPhoneInfo;
	}
	return 0;
}

int phoneinfo::BaiDuShouJiWeiShi::Process(const char *packet,int action)
{
	if (action == kUploadPhoneInfo)
	{
		return UploadPhoneInfo(packet);
	}
	return 0;
}


int phoneinfo::BaiDuShouJiWeiShi::UploadPhoneInfo(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	
	ms_->SplitStringToMap(src_data,'&','=');

	imei_ = ms_->GetValueFromMapByKey("ie");
	imsi_ = ms_->GetValueFromMapByKey("is");
//	model = ms_->GetValueFromMapByKey("model");

	if (imsi_.empty() || imei_.empty() /*|| model_.empty()*/)
	{
		return 0;
	}
	output_text_ = 
//		"model=" + model + "\r\n" +
		"imei=" + imei_ + "\r\n" +
		"imsi=" + imsi_ + "\r\n" +
		"state=" + app_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}

