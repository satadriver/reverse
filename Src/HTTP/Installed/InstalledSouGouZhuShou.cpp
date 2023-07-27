
#include "StdAfx.h"

#include "InstalledSouGouZhuShou.h"

installed::SouGouZhuShou::SouGouZhuShou(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "搜狗助手";
}

installed::SouGouZhuShou::~SouGouZhuShou()
{
	
}

int installed::SouGouZhuShou::IsSouGouZhuShou(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("mobile.zhushou.sogou.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/android/checkapptotal.html") != -1 ) )	
	{
		return kSoftwareUpdate;
	}
	return 0;
}

int installed::SouGouZhuShou::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	return 0;
}


int installed::SouGouZhuShou::SoftwareUpdate(const char *packet)
{	
	//json 提取
    string src_data(packet);
	Json::Reader reader;
	Json::Value value;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}
	Json::Value::Members member = value.getMemberNames();
	for (Json::Value::Members::iterator iter = member.begin();iter!=member.end();++iter)
	{
		installed_ = GetAppName((*iter).c_str());
		output_text_ =
			"webname=" + installed_ + "\r\n" +
			"type=手机\r\n" +
			"msg=" + app_ + "\r\n" +
			"state=安装\r\n";
		WriteIndexFile();
	}
    return 0;
	/*
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

			installed_ = item.substr(0, tmp_pos);
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
	*/
}