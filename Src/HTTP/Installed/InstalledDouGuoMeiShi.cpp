
#include "StdAfx.h"

#include "InstalledDouGuoMeiShi.h"

installed::DouGuoMeiShi::DouGuoMeiShi(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "豆果美食";
}

installed::DouGuoMeiShi::~DouGuoMeiShi()
{
	
}

int installed::DouGuoMeiShi::IsDouGuoMeiShi(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.douguo.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/app/installed") != -1 ) )	
	{
		return kSoftwareUpdate;
	}
	return 0;
}

int installed::DouGuoMeiShi::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	return 0;
}


int installed::DouGuoMeiShi::SoftwareUpdate(const char *packet)
{	

	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	string apps = ms_->GetValueFromMapByKey("apps",true);
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(apps,value))
	{
		return 0;
	}
	string appname;
	int size = value.size();
	for (int i = 0; i<size ; i++)
	{
		appname = value[i]["app_name"].asString();
		output_text_ =
			"webname=" + appname + "\r\n" +
			"type=手机\r\n" +
			"msg=" + app_ + "\r\n" +
			"state=安装\r\n";
			WriteIndexFile();
	}
	return 1;
}