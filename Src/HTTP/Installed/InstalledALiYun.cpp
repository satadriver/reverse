
#include "StdAfx.h"

#include "InstalledALiYun.h"

installed::ALiYun::ALiYun(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "阿里云";
}

installed::ALiYun::~ALiYun()
{
	
}

int installed::ALiYun::IsALiYun(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("apps.aliyun.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("as.htm") != -1 ) )	
	{
		return kSoftwareUpdate;
	}
	return 0;
}

int installed::ALiYun::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	return 0;
}


int installed::ALiYun::SoftwareUpdate(const char *packet)
{	
	int retval = 0;
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);


	ms_->Replace(src_data," ","");
	ms_->Replace(src_data,"[","&");
	ms_->Replace(src_data,"]","&");
	ms_->Replace(src_data,"{","&");
	ms_->Replace(src_data,"}","&");
	ms_->Replace(src_data,"\"","");
	string item;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 

	while(1)
	{
		item_end_pos = src_data.find("&",item_beg_pos);
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
		if (!item.empty())
		{
			ms_->SplitStringToMap(item,',',':');

			installed_ = ms_->GetValueFromMapByKey("name",true);
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