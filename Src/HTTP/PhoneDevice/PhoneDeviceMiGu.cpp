
#include "StdAfx.h"
#include "PhoneDeviceMiGu.h"

phonedevice::PdMiGu::PdMiGu(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "�乾����";
}

phonedevice::PdMiGu::~PdMiGu()
{
	
}

int phonedevice::PdMiGu::IsPdMiGu(HttpSession *http_session)
{
	//�ֻ��豸��Ϣ (action �ɳ��ȼ�)
	if( http_session->m_Requestline.m_Host.Find("a.mll.migu.cn")  != -1 && 
		http_session->m_Requestline.m_URI.Find("/app/v2/controller/level/level.shtml") != -1)	
	{
		return kUploadPhoneDevice;
	}
	//�ֻ��豸��Ϣ (action ����)
	if( http_session->m_Requestline.m_Host.Find("zazhi.migu.cn")  != -1 && 
		http_session->m_Requestline.m_URI.Find("/soe/uniaccess") != -1)	
	{
		return kUploadPhoneDeviceListen;
	}
	return 0;
}

int phonedevice::PdMiGu::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	if (action == kUploadPhoneDeviceListen)
	{
		return UploadPhoneDeviceListen(packet);
	}
	return 0;
}

//�ɳ��ȼ�
int phonedevice::PdMiGu::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
 
	ms_->SplitStringToMap(src_data,"\r\n",":");

    imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
    version_ = ms_->GetValueFromMapByKey("os");
	model_ = ms_->GetValueFromMapByKey("brand");
    //phonenum
	string msisdn = ms_->GetValueFromMapByKey("msisdn");

	if (imei_.empty() || imsi_.empty() || version_.empty() || model_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

//����
int phonedevice::PdMiGu::UploadPhoneDeviceListen(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,"\r\n",":");
	imei_ = ms_->GetValueFromMapByKey("IMEI");
	imsi_ = ms_->GetValueFromMapByKey("IMSI");
	if (imei_.length() > 19 || imei_.length() <=0 )
	{
		return 0;
	}
    version_ = ms_->GetValueFromMapByKey("os");
	model_ = ms_->GetValueFromMapByKey("brand");
    //λ����Ϣ
	string locationdata = ms_->GetValueFromMapByKey("location-data");
	longitude_ = locationdata.substr(0,locationdata.find(","));
	latitude_ = locationdata.substr(locationdata.find(","));
	if (imei_.empty() || imsi_.empty() || version_.empty() || model_.empty() || longitude_.empty() || latitude_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}