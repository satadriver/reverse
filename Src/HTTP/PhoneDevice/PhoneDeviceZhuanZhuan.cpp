
#include "StdAfx.h"
#include "PhoneDeviceZhuanZhuan.h"

phonedevice::PdZhuanZhuan::PdZhuanZhuan(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "转转";
}

phonedevice::PdZhuanZhuan::~PdZhuanZhuan()
{
	
}

int phonedevice::PdZhuanZhuan::IsPdZhuanZhuan(HttpSession *http_session)
{
	//手机设备信息 1
	if( http_session->m_Requestline.m_Host.Find("api.lego.wireless.58.com")  != -1 && 
		http_session->m_Requestline.m_URI.Find("/v1/blood") != -1)	
	{
		return kUploadPhoneDevice;
	}
	//手机设备信息 1
	if (http_session->m_Requestline.m_Host.Find("api.lego.wireless.58.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/v1/coke") != -1)
	{
		return kUploadPhoneDevice;
	}
	//手机设备信息 2
	if (http_session->m_Requestline.m_Host.Find("zzpush.58.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/settokens") != -1)
	{
		return kUploadPhoneDeviceSec;
	}
	//附近查看宝贝位置
	if (http_session->m_Requestline.m_Host.Find("apis.map.qq.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/tools/poimarker") != -1)
	{
		return kUploadPhoneLocation;
	}
	return 0;
}

int phonedevice::PdZhuanZhuan::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	if (action == kUploadPhoneDeviceSec)
	{
		return UploadPhoneDeviceSec(packet);
	}
	if (action == kUploadPhoneLocation)
	{
		return UploadPhoneLocation(packet);
	}
	return 0;
}


int phonedevice::PdZhuanZhuan::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
    string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
    boundary = "\r\n--" + boundary;
    longitude_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"lego_lon\"\r\n\r\n",boundary,true);
    latitude_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"lego_lat\"\r\n\r\n",boundary,true);
    version_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"lego_osv\"\r\n\r\n",boundary,true); 
	os_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"lego_os\"\r\n\r\n",boundary,true);
    model_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"lego_ua\"\r\n\r\n",boundary,true);
    imei_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"lego_devid\"\r\n\r\n",boundary,true);
	if (longitude_.empty() || latitude_.empty() || os_.empty() || version_.empty() || model_.empty() || imei_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

int phonedevice::PdZhuanZhuan::UploadPhoneDeviceSec(const char *packet)
{
	string src_data(packet);
	int pos = src_data.rfind("{\"appid");
	if (pos != -1)
	{
		src_data = src_data.substr(pos);
	}
	//json提取数据
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(src_data,value))
	{
		return -1;
	}
    string tokens = value["tokens"].asString();
    
	Json::Value valuetoken;
	Json::Reader readertoken;
	if (!readertoken.parse(tokens,valuetoken))
	{
		return -1;
	}
	version_ = valuetoken["osVersion"].asString();
	os_ = GetOS();
	model_ = valuetoken["model"].asString();

	if (os_.empty() || version_.empty() || model_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

int phonedevice::PdZhuanZhuan::UploadPhoneLocation(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,"&","=");
	string marker = ms_->GetValueFromMapByKey("marker",true);
	//经度
	latitude_ = ms_->GetValueBetweenBeginEnd(marker,"coord:",",");
	longitude_ = marker.substr(marker.find(",") + strlen(","));
	if (longitude_.empty() || latitude_.empty())
	{
		return -1;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 0;
}
