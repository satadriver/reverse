
#include "StdAfx.h"

#include "XiaoMiShangDian.h"

XiaoMiShangDian::XiaoMiShangDian()
{
	software_ = "小米商店";
}

XiaoMiShangDian::~XiaoMiShangDian()
{
	
}

int XiaoMiShangDian::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("market.xiaomi.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/apm/updateinfo") != -1 ) )	
	{
		return kUploadInstalledSoftwareMA;
	}
	return 0;
}

int XiaoMiShangDian::Process(const char *packet,int action)
{
	if (action == kUploadInstalledSoftwareMA)
	{
		return UploadInstalledSoftwareMA(packet);
	}
	return 0;
}


int XiaoMiShangDian::UploadInstalledSoftwareMA(const char *packet)
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
			string installed = item;
//			installed_ = GetAppName(installed_);
			if (installed.size() < 50)
			{
				output_text_ =
					"webname=" + installed + "\r\n" +
					"type=手机\r\n" +
					"msg=" + software_ + "\r\n" +
					"state=安装\r\n";
				WriteIndexFile();
				retval = 1;
			}

		}

			
		
	}
	return retval;
}