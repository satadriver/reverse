
#include "StdAfx.h"

#include "ContactsFengYunZhiBo.h"

contacts::FengYunZhiBo::FengYunZhiBo(HttpSession *http_session, const string &pro_name) : Contacts(http_session, pro_name)
{
	
}

contacts::FengYunZhiBo::~FengYunZhiBo()
{
	
}

int contacts::FengYunZhiBo::IsFengYunZhiBo(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("config.mobile.kukuplay.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("MobileConfig/keylog")!=-1))
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::FengYunZhiBo::Process(const char *packet,int action)
{
	
	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	
	return 0;
}

int contacts::FengYunZhiBo::AndroidUploadContacts(const char *packet)
{
	char *hd = strstr(packet,"contact=");
	if (!hd)
	{
		return 0;
	}
	
	int retval = 0;
	string src_data(hd + strlen("contact="));
	src_data = ms_->UrlDecode(src_data);

	ms_->Replace(src_data,"|","");
	
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
		
		string::size_type tmp = item.find(":");
		if (tmp != string::npos)
		{
			name_ = item.substr(0,tmp);
			name_ = ms_->UTF8ToGBK(name_);
			phone_ = item.substr(tmp + 1,item.size());
		}
		else
		{
			continue;
		}
		if (name_.empty() || phone_.empty())
		{
			continue;
		}
		output_text_ = 
			"nick=" + name_+ "\r\n" + 
			"user=" + phone_ + "\r\n" + 
			"state=∑Á‘∆÷±≤•\r\n";
		WriteIndexFile();
		retval = 1;
	}
	
	return retval;
}


