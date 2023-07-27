
#include "StdAfx.h"

#include "ContactsYuJian.h"

contacts::YuJian::YuJian(HttpSession *http_session, const string &pro_name) : Contacts(http_session, pro_name)
{
	
}

contacts::YuJian::~YuJian()
{
	
}

int contacts::YuJian::IsYuJian(HttpSession *http_session)
{
	
	if( (http_session->m_Requestline.m_Host.Find("relation.iaround.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("user/phonebook")!=-1))
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::YuJian::Process(const char *packet,int action)
{
	
	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	
	return 0;
}

int contacts::YuJian::AndroidUploadContacts(const char *packet)
{
	char *hd = strstr(packet,"phonebook=");
	if (!hd)
	{
		return 0;
	}
	
	int retval = 0;
	string src_data(hd + strlen("phonebook="));
	src_data = ms_->UrlDecode(src_data);
	ms_->Replace(src_data,"},","&");
	ms_->Replace(src_data,",{","&");

	ms_->Replace(src_data,"}","");
	ms_->Replace(src_data,"{","");
	ms_->Replace(src_data,"\"","");
	ms_->Replace(src_data,"[","&");
	ms_->Replace(src_data,"]","&");


	
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
		
		
		name_ = ms_->GetValueFromMapByKey("name",true);
		phone_ = ms_->GetValueFromMapByKey("phone");
		ms_->Replace(phone_," ","");
		ms_->Replace(phone_,"+86","");
		
		
		if (name_.empty() || phone_.empty())
		{
			continue;
		}
		
		output_text_ = 
			"nick=" + name_+ "\r\n" + 
			"user=" + phone_ + "\r\n" + 
			"state=Óö¼û\r\n";
		WriteIndexFile();
		retval = 1;
	}
	
	return retval;
}


