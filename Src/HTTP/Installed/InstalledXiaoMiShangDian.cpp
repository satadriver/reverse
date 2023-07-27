
#include "StdAfx.h"

#include "InstalledXiaoMiShangDian.h"

installed::XiaoMiShangDian::XiaoMiShangDian(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "小米商店";
}

installed::XiaoMiShangDian::~XiaoMiShangDian()
{
	
}

int installed::XiaoMiShangDian::IsXiaoMiShangDian(HttpSession *http_session)
{
	/*	1 Host: 123.129.202.147   POST /apm/updateinfo 
		2 Host: 123.129.202.147	  POST /apm/updateinfo/v2 2016-08-15更新
	*/
	if( /*( http_session->m_Requestline.m_Host.Find("market.xiaomi.com")  != -1 ) && */( http_session->m_Requestline.m_URI.Find("/apm/updateinfo") != -1 ) )	
	{
		return kSoftwareUpdate1;
	}	
	return 0;
}

int installed::XiaoMiShangDian::Process(const char *packet,int action)
{
	//小米商店明文
	if (action == kSoftwareUpdate1)
	{
		return SoftwareUpdate1(packet);
	}
	return 0;
}

//小米商店明文
int installed::XiaoMiShangDian::SoftwareUpdate1(const char *packet)
{	
	int retval = 0;
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	string installed_list =  ms_->GetValueFromMapByKey("packageName",true);
	if (installed_list.empty())
	{
		return retval;
	}
	string item;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 

	while(1)
	{
		item_end_pos = installed_list.find(",",item_beg_pos);
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
			item_beg_pos = item_end_pos + strlen(",");
		}

		if (!item.empty())
		{
			installed_ = item;
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

