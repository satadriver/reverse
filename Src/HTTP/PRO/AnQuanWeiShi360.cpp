
#include "StdAfx.h"

#include "AnQuanWeiShi360.h"

AnQuanWeiShi360::AnQuanWeiShi360()
{
	software_ = "360安全卫士";
}

AnQuanWeiShi360::~AnQuanWeiShi360()
{
	
}

int AnQuanWeiShi360::ClearVar()
{
	return 0;
}

int AnQuanWeiShi360::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("q.soft.360.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/get_polls.php") != -1 ) )	
	{
		return kUploadInstalledSoftwareC;
	}
	return 0;
}

int AnQuanWeiShi360::Process(const char *packet,int action)
{
	if (action == kUploadInstalledSoftwareC)
	{
		return UploadInstalledSoftwareC(packet);
	}

	return 0;
}

int AnQuanWeiShi360::UploadInstalledSoftwareC(const char *packet)
{
	int retval = 0;
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	string::size_type pos = uri.find("?mid=");
	if (pos == string::npos)
	{
		return 0;
	}
	string mid = uri.substr(pos + 5/*== strlen("?mid=")*/);	
	char *data = strstr(packet,"pd=");
	if (!data)
	{
		return 0;
	}
	string src_data(data);
	ms_->SplitStringToMap(src_data,'&','=');
	string software_list = ms_->GetValueFromMapByKey("ks",true);
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	string item;
	while(1)
	{
		item_end_pos = software_list.find("|||",item_beg_pos);

		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = software_list.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = software_list.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + strlen("|||");
		}

		ms_->Replace(item,"+"," ");
		string installed = item;
		output_text_ =
			"webname=" + installed + "\r\n" +
			"type=电脑\r\n" +
			"msg=" + software_ + "\r\n" +
			"mid=" + mid + "\r\n" +
			"from=360安全卫士\r\n" +
			"state=安装\r\n";
		WriteIndexFile();	
		retval = 1;
	}
	return retval;
}
