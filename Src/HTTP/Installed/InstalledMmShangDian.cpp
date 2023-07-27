
#include "StdAfx.h"

#include "InstalledMmShangDian.h"

installed::MMShangDian::MMShangDian(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "mm商店";
}

installed::MMShangDian::~MMShangDian()
{
	
}

int installed::MMShangDian::IsMMShangDian(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("120.197.233.102")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/t.do?requestid=app_upchecked_interface") != -1 ) )	
	{
		return kSoftwareUpdate;
	}
	return 0;
}

int installed::MMShangDian::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	return 0;
}


int installed::MMShangDian::SoftwareUpdate(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	Json::Value vale;
	Json::Reader reader;
	if (!reader.parse(src_data,vale))
	{
		return 0;
	}
	Json::Value apps;
	apps = vale["apps"];
	int size = apps.size();
	for (int i = 0;i<size;i++)
	{
		string appid = apps[i]["id"].asString();
		string appname = apps[i]["name"].asString();
		if (appid.empty() || appname.empty())
		{
			continue;
		}
		output_text_ =
			"webname=" + appname + "\r\n" +
			"type=手机\r\n" +
			"msg=" + app_ + "\r\n" +
			"state=安装\r\n";
		WriteIndexFile();
	}
	return 1;
}