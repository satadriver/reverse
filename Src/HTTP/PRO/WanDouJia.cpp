
#include "StdAfx.h"

#include "WanDouJia.h"

WanDouJia::WanDouJia()
{
	software_ = "豌豆荚";
}

WanDouJia::~WanDouJia()
{
	
}

int WanDouJia::Is(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.wandoujia.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/v2/update") != -1 ) )	
	{
		return kUploadInstalledSoftwareMA;
	}
	return 0;
}

int WanDouJia::Process(const char *packet,int action)
{
	if (action == kUploadInstalledSoftwareMA)
	{
		return UploadInstalledSoftwareMA(packet);
	}
	return 0;
}


int WanDouJia::UploadInstalledSoftwareMA(const char *packet)
{	
	unsigned long zlen = http_session_->m_datalen;
	unsigned long len = zlen * 5;
	
	char *data = new char[len];
	if (-1 == Compress::DecompressGZIP((unsigned char *)(packet + 10),zlen,(unsigned char *)data,&len))
	{
		return 0;
	}

	int retval = 0;
	string src_data(data);
	ms_->Replace(src_data,"[{","&");
	ms_->Replace(src_data,"}]","&");
	ms_->Replace(src_data,"},{","&");
	ms_->Replace(src_data,"\"","");
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_data.find("&",item_beg_pos);
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
			item_beg_pos = item_end_pos + strlen("&");
		}

		ms_->SplitStringToMap(item,',',':');
		string installed = ms_->GetValueFromMapByKey("p");
		if (!installed.empty())
		{
	//		installed_ = GetAppName(installed_);
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