
#include "StdAfx.h"

#include "ZhuShou91.h"

ZhuShou91::ZhuShou91()
{
	software_ = "91助手";
}

ZhuShou91::~ZhuShou91()
{
	
}

int ZhuShou91::Is(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("v3.api.momo.im")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/contact/create_batch.json") != -1 ))
	{
		return kUploadContactsM;
	}
	if(  ( http_session->m_Requestline.m_Host.Find("appupdate.sj.91.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/service.ashx") != -1 ) )	
	{
		return kUploadInstalledSoftwareMA;
	}
	return 0;
}

int ZhuShou91::Process(const char *packet,int action)
{
	if (action == kUploadContactsM)
	{
		return UploadContactsM(packet);
	}

	if (action == kUploadInstalledSoftwareMA)
	{
		return UploadInstalledSoftwareMA(packet);
	}
	return 0;
}

int ZhuShou91::UploadContactsM(const char *packet)
{
	string src_data(packet);
	
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	
	int retval = 0;

	ms_->Replace(src_data,"\":{\"","&");
	ms_->Replace(src_data,"}]},","&");
	ms_->Replace(src_data,"}},","&");
	ms_->Replace(src_data,"\"","");

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
		
		ms_->Replace(item,"}","");
		ms_->Replace(item,"{","");
		ms_->SplitStringToMap(item,',',':');


		string given_name = ms_->GetValueFromMapByKey("given_name",true);
		string family_name = ms_->GetValueFromMapByKey("family_name",true);
		string middle_name = ms_->GetValueFromMapByKey("middle_name",true);
		string::size_type pos = middle_name.find(",");
		if (pos != string::npos)
		{
			middle_name = middle_name.substr(0, pos);
		}
		
		string name = family_name + middle_name + given_name;

		string tels = ms_->GetValueFromMapByKey("tels");

		ms_->Replace(tels,"[","");
		ms_->Replace(tels,"]","");
		ms_->SplitStringToMap(tels,',',':');
		string phone = ms_->GetValueFromMapByKey("value");

		ms_->Replace(phone," ","");
		ms_->Replace(phone,"+86","");
				
		if (name.empty() || phone.empty())
		{
			continue;
		}
		else
		{
			output_text_ = 
				"nick=" + name+ "\r\n" + 
				"user=" + phone + "\r\n";
			WriteIndexFile();
			retval = 1;
		}
	}
	
	return retval;
}

int ZhuShou91::UploadInstalledSoftwareMA(const char *packet)
{	
	
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');
	string imei(ms_->GetValueFromMapByKey("imei"));
	string imsi(ms_->GetValueFromMapByKey("imsi"));
	int retval = 0;
	
	char *data = strstr(packet,"installedSofts=");
	if (!data)
	{
		return 0;
	}
	string src_data(packet + strlen("installedSofts="));
	
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_data.find(";",item_beg_pos);
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
			item_beg_pos = item_end_pos + strlen(";");
		}
		
		string::size_type tmp_pos = item.find(",");
		if (tmp_pos != string::npos)
		{
			string installed = item.substr(0,tmp_pos);
		//	installed_ = GetAppName(installed_);
			if (installed.size() < 50)
			{
				output_text_ =
					"imei=" + imei + "\r\n" +
					"imsi=" + imsi + "\r\n" +
					"webname=" + installed + "\r\n" +
					"type=手机\r\n" +
					"msg=" + software_ + "\r\n" +
					"state=安装\r\n";
				WriteIndexFile();
				retval = 1;
			}
		}
		
	}
	return 1;
}