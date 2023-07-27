
#include "StdAfx.h"

#include "InstalledkuaiYong.h"

installed::KuaiYong::KuaiYong(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "快用";
}

installed::KuaiYong::~KuaiYong()
{
	
}

int installed::KuaiYong::IsKuaiYong(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("iphonetwo.kuaiyong.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/i/i.php") != -1 ) )	
	{
		return kSoftwareUpdate;
	}
	return 0;
}

int installed::KuaiYong::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	return 0;
}


int installed::KuaiYong::SoftwareUpdate(const char *packet)
{	
	int retval = 0;
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string installed_list =  ms_->GetValueFromMapByKey("userApp",true);
	
	if (installed_list.empty())
	{
		return retval;
	}
	
	ms_->Replace(installed_list,"[{","");
	ms_->Replace(installed_list,"]}","");
	ms_->Replace(installed_list,"},{","&");
	ms_->Replace(installed_list,"\"","");


	string item;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = installed_list.find("&",item_beg_pos);
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = installed_list.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = installed_list.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + strlen("&");
		}

		ms_->SplitStringToMap(item,',',':');
		installed_ = ms_->GetValueFromMapByKey("appId");
		if (!installed_.empty())
		{
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