#include "StdAfx.h"
#include "LocationLvMaMaLvYou.h"

location::LvMaMaLvYou::LvMaMaLvYou(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "¿��������";
}

location::LvMaMaLvYou::~LvMaMaLvYou()
{
	
}

int location::LvMaMaLvYou::IsLvMaMaLvYou(HttpSession *http_session)
{
	//λ����Ϣ
	if( ( http_session->m_Requestline.m_Host.Find("api3g2.lvmama.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/router/rest.do?method=api.com.init.addDeviceToken") != -1))	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::LvMaMaLvYou::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::LvMaMaLvYou::DeviceLocation(const char *packet)
{

    //λ����Ϣ ���ݰ� �� uri �ж�����
	//string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));

	string src_data(packet);
	//boundary ֱ����ȡ
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;	 
	//����
	latitude_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"deviceToken\"\r\n\r\n",boundary,true);
	//γ��
	longitude_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"longitude\"\r\n\r\n",boundary,true);
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputString();
	Record(app_);
	WriteIndexFile();
	return 1;
}