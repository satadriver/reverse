
#include "StdAfx.h"

#include "ZhiFuBaoQianBao.h"

ZhiFuBaoQianBao::ZhiFuBaoQianBao()
{
	software_ = "Ö§¸¶±¦Ç®°ü";
}

ZhiFuBaoQianBao::~ZhiFuBaoQianBao()
{
	
}

int ZhiFuBaoQianBao::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("mobilecns.alipay.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/getMsgAndCfg.htm") != -1 ) )	
	{
		return kUploadDeviceInfoM;
	}
	return 0;
}

int ZhiFuBaoQianBao::Process(const char *packet,int action)
{
	if (action == kUploadDeviceInfoM)
	{
		return UploadDeviceInfoM(packet);
	}
	return 0;
}


int ZhiFuBaoQianBao::UploadDeviceInfoM(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,',',':',"\"");

	string model = ms_->GetValueFromMapByKey("model");
	string os = ms_->GetValueFromMapByKey("osType");
	string version = ms_->GetValueFromMapByKey("osVersion");
	string tmp = ms_->GetValueFromMapByKey("IMEI");
	string imei;
	string imsi;
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		imsi = tmp.substr(0,pos);
		imei = tmp.substr(pos + 1);
	}
	
	if (model.empty() || os.empty() || version.empty())
	{
		return 0;
	}
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"imsi=" + imsi + "\r\n" +
		"webname=" + model + "\r\n" +
		"from=" + os + "\r\n" +
		"to=" + version + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}

