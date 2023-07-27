
#include "StdAfx.h"

#include "PhoneInfoZhiFuBaoQianBao.h"

phoneinfo::ZhiFuBaoQianBao::ZhiFuBaoQianBao(HttpSession *http_session, const string &pro_name) : PhoneInfo(http_session,pro_name)
{
	app_ = "Ö§¸¶±¦Ç®°ü";
}

phoneinfo::ZhiFuBaoQianBao::~ZhiFuBaoQianBao()
{
	
}

int phoneinfo::ZhiFuBaoQianBao::IsZhiFuBaoQianBao(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("mobilecns.alipay.com:80")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/getMsgAndCfg.htm") != -1 ) )	
	{
		return kUploadPhoneInfo;
	}
	return 0;
}

int phoneinfo::ZhiFuBaoQianBao::Process(const char *packet,int action)
{
	if (action == kUploadPhoneInfo)
	{
		return UploadPhoneInfo(packet);
	}
	return 0;
}


int phoneinfo::ZhiFuBaoQianBao::UploadPhoneInfo(const char *packet)
{
	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,',',':',"\"");
//	string model,imeiimsi,imsi;
//	model = ms_->GetValueFromMapByKey("model");
	string imeiimsi = ms_->GetValueFromMapByKey("IMEI");
	imsi_ = imeiimsi.substr(0,imeiimsi.find("|"));
	imei_ = imeiimsi.substr(imeiimsi.find("|"));
	
	if (imsi_.empty() || imei_.empty() /*|| model.empty()*/)
	{
		return 0;
	}

	
	WriteIndexFile();
	return 1;
}

