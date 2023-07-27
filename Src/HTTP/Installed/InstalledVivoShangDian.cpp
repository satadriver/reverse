#include "StdAfx.h"
#include "InstalledVivoShangDian.h"

installed::VivoShangDian::VivoShangDian(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "vivo应用商店";
}

installed::VivoShangDian::~VivoShangDian()
{
	
}

int installed::VivoShangDian::IsVivoShangDian(HttpSession *http_session)
{
	//检查软件更新
	if( ( http_session->m_Requestline.m_Host.Find("update.appstore.vivo.com.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/port/packages_update/") != -1 ) )	
	{
		return kSoftwareUpdate;
	}
	return 0;
}

int installed::VivoShangDian::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	return 0;
}


int installed::VivoShangDian::SoftwareUpdate(const char *packet)
{
	string src_dataPre(packet);
	ms_->SplitStringToMap(src_dataPre,'&','=');
	//获取包
    string src_data;
	src_data= ms_->GetValueFromMapByKey("packages",true);
	int retval = 0;

	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_data.find(",",item_beg_pos);
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
			item_beg_pos = item_end_pos + strlen(",");
		}
		
		string::size_type tmp_pos = item.find("|");
		if (tmp_pos != string::npos)
		{

			installed_ = item.substr(0, tmp_pos);
			installed_ = GetAppName(installed_);
			if (installed_.size() < 50)
			{
				output_text_ =
					"webname=" + installed_ + "\r\n" +
					"type=手机\r\n" +
					"msg=" + app_ + "\r\n" +
					"state=安装\r\n";
				WriteIndexFile();
				retval = 1;
			}
		}
	}
	return retval;
}