
#include "StdAfx.h"
#include "PhoneDeviceJingDong.h"

phonedevice::PdJingDong::PdJingDong(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "京东";
}

phonedevice::PdJingDong::~PdJingDong()
{
	
}

int phonedevice::PdJingDong::IsPdJingDong(HttpSession *http_session)
{
	//手机设备信息 ()
	if( http_session->m_Requestline.m_Host.Find("api.m.jd.com")  != -1 && 
		http_session->m_Requestline.m_URI.Find("/client.action?functionId") != -1)	
	{
		return kUploadPhoneDevice;
	}
	//手机设备信息 (京东支付-添加新卡)
	if (http_session->m_Requestline.m_Host.Find("jdpaycert.jd.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/service/getCertificate") != -1)
	{
		return kUploadPhoneDeviceSec;
	}
	//手机设备信息 (京东支付-添加新卡)
	if (http_session->m_Requestline.m_Host.Find("ag.jd.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/rest/data/push") != -1)
	{
		return kUploadPhoneDeviceThi;
	}
	return 0;
}

int phonedevice::PdJingDong::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	if (action == kUploadPhoneDeviceSec)
	{
		return UploadPhoneDeviceSec(packet);
	}
	if (action == kUploadPhoneDeviceThi)
	{
		return UploadPhoneDeviceThi(packet);
	}
	return 0;
}


int phonedevice::PdJingDong::UploadPhoneDevice(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,"&","=");
	//
	string functionId = ms_->GetValueFromMapByKey("functionId");
	//有经纬度 搜索
    if (functionId == "search")
    { 
		//第一部分
		string uuid = ms_->GetValueFromMapByKey("uuid");
		if (uuid.find("-") != string::npos)
		{
			imei_ = uuid.substr(0,uuid.find("-"));
		}
		model_ = ms_->GetValueFromMapByKey("d_model");
		version_ = ms_->GetValueFromMapByKey("osVersion");
		os_ = ms_->GetValueFromMapByKey("client");
		//第二部分
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
        ms_->SplitStringToMap(src_data,"&","=");
		string body = ms_->GetValueFromMapByKey("body");
		//json提取数据
		Json::Value value;
		Json::Reader reader;
		if (!reader.parse(body,value))
		{
			return 0;
		}
		longitude_ = value["longitude"].asString();
		latitude_ = value["latitude"].asString();
		if (longitude_.empty() || latitude_.empty() || imei_.empty() || model_.empty() || version_.empty() || os_.empty())
		{
			return 0;
		}
		output_text_ = GetOutputText();	
		WriteIndexFile();
    }
	//没有经纬度
	if (functionId != "search")
	{
		string uuid = ms_->GetValueFromMapByKey("uuid");
		if (uuid.find("-") != string::npos)
		{
			imei_ = uuid.substr(0,uuid.find("-"));
		}
		model_ = ms_->GetValueFromMapByKey("d_model");
		version_ = ms_->GetValueFromMapByKey("osVersion");
		os_ = ms_->GetValueFromMapByKey("client");
		if (imei_.empty() || model_.empty() || version_.empty() || os_.empty())
		{
			return 0;
		}
		output_text_ = GetOutputText();	
		WriteIndexFile();
	}
	return 1;
}

int phonedevice::PdJingDong::UploadPhoneDeviceSec(const char *packet)
{
	string src_data(packet);
	string pos = "{\"sdkAppVersion\":";
	if (src_data.rfind(pos) != string::npos)
	{
		src_data = src_data.substr(src_data.rfind(pos));
	}
    //json提取数据
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(src_data,value))
	{
		return -1;
	}
    os_ = value["sdkClientName"].asString();
	imei_ = value["sdkDeviceId"].asString();
	imsi_ = value["sdkSimSerialNo"].asString();
	if (os_.empty() || imei_.empty() || imsi_.empty())
	{
		return -1;
	}
	output_text_ = GetOutputText();	
	WriteIndexFile();
	return 1;
}

int phonedevice::PdJingDong::UploadPhoneDeviceThi(const char *packet)
{
	string src_data(packet);
	string pos = "{\"data\":";
	if (src_data.rfind(pos) != string::npos)
	{
		src_data = src_data.substr(src_data.rfind(pos));
		ms_->Replace(src_data,"\\","");
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	}
    imei_ = ms_->GetValueBetweenBeginEnd(src_data,"deviceId\":\"","\"");
	imsi_ = ms_->GetValueBetweenBeginEnd(src_data,"imsi\":\"","\"");
	model_ = ms_->GetValueBetweenBeginEnd(src_data,"deviceName\":\"","\"");
    version_ = ms_->GetValueBetweenBeginEnd(src_data,"osVersion\":\"","\"");
	os_ = ms_->GetValueBetweenBeginEnd(src_data,"provider\":\"","\"");

	if (imei_.empty() || imsi_.empty())
	{
		return -1;
	}
	output_text_ = GetOutputText();	
	WriteIndexFile();
	return 1;
}