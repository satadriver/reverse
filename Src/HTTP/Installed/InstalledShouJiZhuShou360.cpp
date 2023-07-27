
#include "StdAfx.h"

#include "InstalledShouJiZhuShou360.h"

installed::ShouJiZhuShou360::ShouJiZhuShou360(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "360手机助手";
}

installed::ShouJiZhuShou360::~ShouJiZhuShou360()
{
	
}

int installed::ShouJiZhuShou360::IsShouJiZhuShou360(HttpSession *http_session)
{
	/*	
		1 Host: openbox.mobilem.360.cn  POST /index/getTidySoftInfoByIds? 2016-08-15更新
	*/
	if( ( http_session->m_Requestline.m_Host.Find("openbox.mobilem.360.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/index/getTidySoftInfoByIds") != -1 ) )	
	{
		return kUploadInstalled1;
	}
	/*	
		1 Host: intf1.zsall.mobilem.360.cn  POST /intf/getUninstallSoft3 2016-08-15更新
	*/
	if( ( http_session->m_Requestline.m_Host.Find("zsall.mobilem.360.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/intf/getUninstallSoft") != -1 ) )	
	{
	//	return kUploadInstalled2;
	}

	if( /*( http_session->m_Requestline.m_Host.Find("gc.mobilem.360.cn")  != -1 ) && */
		( http_session->m_Requestline.m_URI.Find("/xgamebox/floatFilter") != -1 ) )	
	{
		return kUploadInstalled3;
	}

	/*
	if( ( http_session->m_Requestline.m_Host.Find("gc.mobilem.360.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/xgamebox/floatFilter") != -1 ) )	
	{
		return kUploadInstalled4;
	}*/
	return 0;
}

int installed::ShouJiZhuShou360::Process(const char *packet,int action)
{
	if (action == kUploadInstalled1)
	{
		return UploadInstalled1(packet);
	}

	if (action == kUploadInstalled2)
	{
		return UploadInstalled2(packet);
	}

	if (action == kUploadInstalled3)
	{
		return UploadInstalled3(packet);
	}
	return 0;
}


int installed::ShouJiZhuShou360::UploadInstalled1(const char *packet)
{
	int retval = 0;
	string applist(packet);
	//去除前面的 pnames=
	string::size_type begin_pos = 0;
	string::size_type end_pos = 0;
	end_pos = applist.find("pnames=",begin_pos);
	if (end_pos != string::npos)
	{
		applist = applist.substr(begin_pos + strlen("pnames="));
	}
    
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	string item;
	while(1)
	{
		item_end_pos = applist.find(",",item_beg_pos);
		
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = applist.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = applist.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + strlen(",");
		}
		
		installed_ = GetAppName(item);
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
	return retval;
}

int installed::ShouJiZhuShou360::UploadInstalled2(const char *packet)
{
	int retval = 0; 
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//去除前面的 packages=
	string::size_type begin_pos = 0;
	string::size_type end_pos = 0;
	end_pos = src_data.find("packages=",begin_pos);
	if (end_pos != string::npos)
	{
		src_data = src_data.substr(begin_pos + strlen("packages="));
	}
    //替换字符
	ms_->Replace(src_data,"[{","");
	ms_->Replace(src_data,"}]","");
	ms_->Replace(src_data,"},{","&");
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
			item_beg_pos = item_end_pos + strlen("&");
		}		
		ms_->SplitStringToMap(item,',',':');
		installed_ = ms_->GetValueFromMapByKey("pname");
		installed_ = GetAppName(item);
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
	return retval;
}

int installed::ShouJiZhuShou360::UploadInstalled3(const char *packet)
{
	int retval = 0;
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	string item;
	while(1)
	{
		item_end_pos = src_data.find(",",item_beg_pos);
		
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
		
		ms_->SplitStringToMap(item,',',':',"\"");
		installed_ = ms_->GetValueFromMapByKey("pkgname");
		installed_ = GetAppName(item);
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
	return retval;
}