
#include "StdAfx.h"

#include "Installed91ZhuShou.h"

installed::ZhuShou91::ZhuShou91(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "91助手";
}

installed::ZhuShou91::~ZhuShou91()
{
	
}

int installed::ZhuShou91::IsZhuShou91(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("appupdate.sj.91.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/service.ashx") != -1 ) )	
	{
		return kSoftwareUpdate;
	}
	return 0;
}

int installed::ZhuShou91::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	return 0;
}


int installed::ZhuShou91::SoftwareUpdate(const char *packet)
{	

	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');
	string imei(ms_->GetValueFromMapByKey("imei"));
	string imsi(ms_->GetValueFromMapByKey("imsi"));
	int retval = 0;

	char *data = strstr(packet,"installedSofts=");
	if (!data)
	{
		return 0;
	}
	string src_data(packet + strlen("installedSofts="));

	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_data.find(";",item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = src_data.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = src_data.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + strlen(";");
		}
		
		string::size_type tmp_pos = item.find(",");
		if (tmp_pos != string::npos)
		{
			installed_ = item.substr(0,tmp_pos);
			installed_ = GetAppName(installed_);
			if (installed_.size() < 50)
			{
				output_text_ =
					"imei=" + imei + "\r\n" +
					"nick=" + imsi + "\r\n" +
					"webname=" + installed_ + "\r\n" +
					"type=手机\r\n" +
					"msg=" + app_ + "\r\n" +
					"state=安装\r\n";
				WriteIndexFile();
				retval = 1;
			}
		}
			
	}
	return 1;
}