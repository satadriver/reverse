
#include "StdAfx.h"

#include "SouGouZhuShou.h"

SouGouZhuShou::SouGouZhuShou()
{
	software_ = "搜狗助手";
}

SouGouZhuShou::~SouGouZhuShou()
{
	
}

int SouGouZhuShou::Is(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("mobile.zhushou.sogou.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/android/checkapp.html") != -1 ) )	
	{
		return kUploadInstalledSoftwareMA;
	}
	return 0;
}

int SouGouZhuShou::Process(const char *packet,int action)
{
	if (action == kUploadInstalledSoftwareMA)
	{
		return UploadInstalledSoftwareMA(packet);
	}
	return 0;
}


int SouGouZhuShou::UploadInstalledSoftwareMA(const char *packet)
{	
	int retval = 0;
	string src_data(packet);
	ms_->Replace(src_data,"},\"","&");
	ms_->Replace(src_data,"{\"","");
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
		
		string::size_type tmp_pos = item.find(":");
		if (tmp_pos != string::npos)
		{

			string installed = item.substr(0, tmp_pos);
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