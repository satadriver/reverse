
#include "StdAfx.h"

#include "Installed360ShouJiAnQuanWeiShi.h"

installed::ShouJiAnQuanWeiShi360::ShouJiAnQuanWeiShi360(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "360ÊÖ»úÎÀÊ¿";
}

installed::ShouJiAnQuanWeiShi360::~ShouJiAnQuanWeiShi360()
{
	
}

int installed::ShouJiAnQuanWeiShi360::IsShouJiAnQuanWeiShi360(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("openbox.mobilem.360.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/mintf/getAppsByPackNames") != -1 ) )	
	{
		return kSoftwareUpdate;
	}

	return 0;
}

int installed::ShouJiAnQuanWeiShi360::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}

	return 0;
}


int installed::ShouJiAnQuanWeiShi360::SoftwareUpdate(const char *packet)
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
			
			installed_ = item.substr(pos + 1);//strlen("|") -> 1
			installed_ = ms_->UTF8ToGBK(UrlDecode(installed_));
	
			
	//		WriteIndexFile();
			retval = 1;
		}

	

	}
	return retval;
}

