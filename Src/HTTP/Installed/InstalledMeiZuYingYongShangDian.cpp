
#include "StdAfx.h"

#include "InstalledMeiZuYingYongShangDian.h"

installed::MeiZuYingYongShangDian::MeiZuYingYongShangDian(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "魅族商店";
}

installed::MeiZuYingYongShangDian::~MeiZuYingYongShangDian()
{
	
}

int installed::MeiZuYingYongShangDian::IsMeiZuYingYongShangDian(HttpSession *http_session)
{
	/*	
		1 Host: u.meizu.com  POST /service/sys_upgrade/checkUpgrade.jsonp  2016-08-15更新
	*/
	if( ( http_session->m_Requestline.m_Host.Find("u.meizu.com")  != -1 ) &&
		( http_session->m_Requestline.m_URI.Find("/service/sys_upgrade/checkUpgrade.jsonp") != -1 ) )	
	{
		return kSoftwareUpdate;
	}
	//魅族商店 2
	/*	
		1 Host: api-app.meizu.com  GET /apps/public/history/white_list?  2016-08-15更新
	*/
	if( ( http_session->m_Requestline.m_Host.Find("api-app.meizu.com")  != -1 ) &&
		( http_session->m_Requestline.m_URI.Find("/apps/public/history/check_update") != -1 ) )	
	{
		return kSoftwareUpdate1;
	}
	return 0;
}

int installed::MeiZuYingYongShangDian::Process(const char *packet,int action)
{
	//魅族商店 1
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	//魅族商店 2
	if (action == kSoftwareUpdate1)
	{
		return SoftwareUpdate1(packet);
	}
	return 0;
}

//魅族商店 1
int installed::MeiZuYingYongShangDian::SoftwareUpdate(const char *packet)
{	
	int retval = 0;
	string src_data(packet);

	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//去除前面的 apps=
	string::size_type begin_pos = 0;
	string::size_type end_pos = 0;
	end_pos = src_data.find("apps=",begin_pos);
	if (end_pos != string::npos)
	{
		src_data = src_data.substr(begin_pos + strlen("apps="));
	}
	ms_->Replace(src_data,"[{","");
	ms_->Replace(src_data,"}]","");
	ms_->Replace(src_data,"},{","&");
	ms_->Replace(src_data,"\"","");

	string item;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_data.find("&",item_beg_pos);;
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
			item_beg_pos = item_end_pos + strlen("&");
		}

		ms_->SplitStringToMap(item,',',':');
		installed_ = ms_->GetValueFromMapByKey("pn");

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

//魅族商店 2
int installed::MeiZuYingYongShangDian::SoftwareUpdate1(const char *packet)
{	
	int retval = 0;
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));	

	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->GetValueFromMapByKey("apps");	  

	ms_->Replace(src_data,"[{","");
	ms_->Replace(src_data,"}]","&"); //--
	ms_->Replace(src_data,"},{","&");
	ms_->Replace(src_data,"\"","");
	string item;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_data.find("&",item_beg_pos);;
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
			item_beg_pos = item_end_pos + strlen("&");
		}

		ms_->SplitStringToMap(item,',',':');
		installed_ = ms_->GetValueFromMapByKey("package_name");	 

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