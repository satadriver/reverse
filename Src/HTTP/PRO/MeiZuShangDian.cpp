
#include "StdAfx.h"

#include "MeiZuShangDian.h"

MeiZuShangDian::MeiZuShangDian()
{
	software_ = "魅族商店";
}

MeiZuShangDian::~MeiZuShangDian()
{
	
}

int MeiZuShangDian::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("mystore.meizu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/service/api/checkAppUpdate.jsonp") != -1 ) )	
	{
		return kUploadInstalledSoftwareMA;
	}
	return 0;
}

int MeiZuShangDian::Process(const char *packet,int action)
{
	if (action == kUploadInstalledSoftwareMA)
	{
		return UploadInstalledSoftwareMA(packet);
	}
	return 0;
}


int MeiZuShangDian::UploadInstalledSoftwareMA(const char *packet)
{	
	int retval = 0;
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
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
		string installed = ms_->GetValueFromMapByKey("an");
		if (!installed.empty())
		{
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