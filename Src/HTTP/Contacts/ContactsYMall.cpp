#include "StdAfx.h"

#include "ContactsYMall.h"


contacts::YMall::YMall(HttpSession *http_session, const string &pro_name) : Contacts(http_session,pro_name)
{

}

contacts::YMall::~YMall()
{

}

int contacts::YMall::IsYMall(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("m.ymall.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/contacter/import") != -1 ) )	
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::YMall::Process(const char *packet,int action)
{
	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	return 0;
}

int contacts::YMall::AndroidUploadContacts(const char *packet)
{
	char *hd = strstr(packet,"data=");
	if (!hd)
	{
		return 0;
	}
	
	int retval = 0;
	
	string src_data(hd + strlen("data="));
	
	src_data = ms_->UrlDecode(src_data);

	ms_->Replace(src_data,"{\"home_phone","&home_phone");
	ms_->Replace(src_data,"},","");
	ms_->Replace(src_data,"\"","");
	
	
	
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
			item_beg_pos = item_end_pos + 1;
		}
		
		ms_->SplitStringToMap(item,'&',':');
		
		string first_name = ms_->GetValueFromMapByKey("first_name",true);
		string last_name = ms_->GetValueFromMapByKey("last_name",true);
		
		name_ = last_name + first_name ;
		phone_ = ms_->GetValueFromMapByKey("home_phone");
		if (name_.empty() || phone_.empty())
		{
			continue;
		}
	
		if (phone_[0] = ',')
		{
			phone_[0] = ' ';
		}

		ms_->Replace(phone_,"+86","");
		ms_->Replace(phone_,"-","");
		ms_->Replace(phone_," ","");

		
		output_text_ = 
			"nick=" + name_+ "\r\n" + 
			"user=" + phone_ + "\r\n";
		WriteIndexFile();
		retval = 1;
	}
	return retval;
}