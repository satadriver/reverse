#include "StdAfx.h"
#include "InstalledZhongXingShangDian.h"

installed::ZhongXingShangDian::ZhongXingShangDian(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "����Ӧ���̵�";
}

installed::ZhongXingShangDian::~ZhongXingShangDian()
{
	
}

int installed::ZhongXingShangDian::IsZhongXingShangDian(HttpSession *http_session)
{
	if (http_session->m_Requestline.m_Host.Find("zxapp.ztems.com") != -1 && 
		http_session->m_Requestline.m_URI.Find("/AppUpgradeServer/api/update") != -1)  
	{
		return kSoftwareUpdate;
	} 
	return 0;
}

int installed::ZhongXingShangDian::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	return 0;
}

int installed::ZhongXingShangDian::SoftwareUpdate(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		for (int j = 0;j <value.size();j++)
		{
			Json::Value arrayObj = value[j];
			installed_ = arrayObj["pkgName"].asString();
			installed_ = GetAppName(installed_);
			output_text_ =
				"webname=" + installed_ + "\r\n" +
				"type=�ֻ�\r\n" +
				"msg=" + app_ + "\r\n" +
				"state=��װ\r\n";
				WriteIndexFile();      
		}
	}
	return 0;
}

