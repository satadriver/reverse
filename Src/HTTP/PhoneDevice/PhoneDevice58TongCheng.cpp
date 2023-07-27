
#include "StdAfx.h"
#include "PhoneDevice58TongCheng.h"

phonedevice::Pd58TongCheng::Pd58TongCheng(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "58ͬ��";
}

phonedevice::Pd58TongCheng::~Pd58TongCheng()
{
	
}

int phonedevice::Pd58TongCheng::IsPd58TongCheng(HttpSession *http_session)
{
	//�״ΰ�װ�� 
	if( http_session->m_Requestline.m_Host.Find("app.58.com")  != -1 && 
		http_session->m_Requestline.m_URI.Find("/api/log/api/log/trackaction") != -1)	
	{
		return kUploadPhoneDevice;
	}
	//�л�����
	if( http_session->m_Requestline.m_Host.Find("app.58.com")  != -1 && 
	http_session->m_Requestline.m_URI.Find("/api/log/api/info/city/") != -1)	
	{
		return kUploadPhoneDeviceSec;
	}
	//�޸������ ��������
	if( http_session->m_Requestline.m_Host.Find("jianli.58.com")  != -1 && 
		http_session->m_Requestline.m_URI.Find("jlprotect/isResumeProtect") != -1)	
	{
		//return kUploadPhoneDeviceTre;
	}
	return 0;
}

int phonedevice::Pd58TongCheng::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	if (action == kUploadPhoneDeviceSec)
	{
		return UploadPhoneDeviceSec(packet);
	}
	return 0;
}

//��Ӧ��
int phonedevice::Pd58TongCheng::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	//boundary ֱ����ȡ
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;	 
	//����
	longitude_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"lon\"\r\n",boundary,true);
	longitude_ = longitude_.substr(longitude_.find("\r\n\r\n") + strlen("\r\n\r\n"));
	//γ��
	latitude_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"lat\"\r\n",boundary,true);
	latitude_ = latitude_.substr(latitude_.find("\r\n\r\n") + strlen("\r\n\r\n"));
	//��װϵͳ
	os_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"platform\"\r\n",boundary,true);
	os_ = os_.substr(os_.find("\r\n\r\n") + strlen("\r\n\r\n"));
    //ϵͳ�汾
	version_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"osv\"\r\n",boundary,true);
	version_ = version_.substr(version_.find("\r\n\r\n") + strlen("\r\n\r\n"));
	//IMEI
	imei_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"imei\"\r\n",boundary,true);
	imei_ = imei_.substr(imei_.find("\r\n\r\n") + strlen("\r\n\r\n"));
	//�ֻ��ͺ�
	model_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"ua\"\r\n",boundary,true);
	model_ = model_.substr(model_.find("\r\n\r\n") + strlen("\r\n\r\n"));
  
    if (longitude_.empty() || latitude_.empty() || os_.empty() || version_.empty() || imei_.empty() || model_.empty())
    {
		return 0;
    }
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

//�л�����
int phonedevice::Pd58TongCheng::UploadPhoneDeviceSec(const char *packet)
{
	string src_data(packet);
	if (src_data.find("official") != -1)
	{
		src_data = src_data.substr(src_data.find("official"));
	}
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"\r\n",":");
	//����
	longitude_ = ms_->GetValueFromMapByKey("lon");
	//γ��
	latitude_ = ms_->GetValueFromMapByKey("lat");
	//��װϵͳ
	os_ = ms_->GetValueFromMapByKey("os");
	//ϵͳ�汾
	version_ = ms_->GetValueFromMapByKey("osv");
	//IMEI
	imei_ = ms_->GetValueFromMapByKey("imei");
	//�ֻ��ͺ�
	model_ = ms_->GetValueFromMapByKey("ua");
    if (longitude_.empty() || latitude_.empty() || os_.empty() || version_.empty() || imei_.empty() || model_.empty())
    {
		return 0;
    }
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}
