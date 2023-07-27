
#include "StdAfx.h"

#include "ShouJiWeiShi360.h"

ShouJiWeiShi360::ShouJiWeiShi360()
{
	software_ = "360ÊÖ»úÎÀÊ¿";
}

ShouJiWeiShi360::~ShouJiWeiShi360()
{
	
}

int ShouJiWeiShi360::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("openbox.mobilem.360.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/mintf/getAppsByPackNames") != -1 ) )	
	{
		return kUploadInstalledSoftwareMA;
	}

	return 0;
}

int ShouJiWeiShi360::Process(const char *packet,int action)
{
	if (action == kUploadInstalledSoftwareMA)
	{
		return UploadInstalledSoftwareMA(packet);
	}

	return 0;
}


int ShouJiWeiShi360::UploadInstalledSoftwareMA(const char *packet)
{
	unsigned long zlen = http_session_->m_datalen;
	unsigned long len = zlen * 5;
	
	char *data = new char[len * 5];
	if (-1 == Compress::DecompressGZIP((unsigned char *)(packet + 10),zlen,(unsigned char *)data,&len))
	{
		delete [] data;
		return 0;
	}
	
	string src_data(data);

	delete [] data;

	int retval = 0;
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

		string::size_type pos = item.rfind("|");
		if (pos != string::npos)
		{
			
			string installed = item.substr(pos + 1);//strlen("|") -> 1
			installed = ms_->UTF8ToGBK(UrlDecode(installed));
	
			
	//		WriteIndexFile();
			retval = 1;
		}

	

	}
	return retval;
}

