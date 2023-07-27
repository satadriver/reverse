
#include "StdAfx.h"

#include "ContactsDouGuo.h"

contacts::DouGuo::DouGuo(HttpSession *http_session, const string &pro_name) : Contacts(http_session, pro_name)
{
	
}

contacts::DouGuo::~DouGuo()
{
	
}

int contacts::DouGuo::IsDouGuo(HttpSession *http_session)
{

	if((http_session->m_Requestline.m_Host.Find("api.douguo.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/uploadcontacts")!=-1))
	{
		return kAndroidUploadContacts;
	}

	if((http_session->m_Requestline.m_Host.Find("api.douguo.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/setaddressbook")!=-1))
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::DouGuo::Process(const char *packet,int action)
{

	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	
	return 0;
}

int contacts::DouGuo::AndroidUploadContacts(const char *packet)
{
	char *hd = strstr(packet,"addressbook=");
	if (!hd)
	{
		return 0;
	}
	
	int retval = 0;
	string src_data(hd + strlen("addressbook="));
	src_data = ms_->UrlDecode(src_data);
	
	ms_->Replace(src_data,"[","&");
	ms_->Replace(src_data,"]","&");
	ms_->Replace(src_data,"{","&");
	ms_->Replace(src_data,"}","&");
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
		
		
		name_ = ms_->GetValueFromMapByKey("nick",true);
		phone_ = ms_->GetValueFromMapByKey("mobile");
		ms_->Replace(phone_," ","");
		ms_->Replace(phone_,"+86","");
		
		
		if (name_.empty() || phone_.empty())
		{
			continue;
		}
		
		output_text_ = 
			"nick=" + name_+ "\r\n" + 
			"user=" + phone_ + "\r\n" + 
			"state=¶¹¹û\r\n";
		WriteIndexFile();
		retval = 1;
	}
	
	return retval;
}


