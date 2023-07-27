
#include "StdAfx.h"

#include "Contacts91ZhuShou.h"

contacts::ZhuShou91::ZhuShou91(HttpSession *http_session, const string &pro_name) : Contacts(http_session,pro_name)
{
	
}

contacts::ZhuShou91::~ZhuShou91()
{
	
}

int contacts::ZhuShou91::IsZhuShou91(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("v3.api.momo.im")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/contact/create_batch.json") != -1 ))
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::ZhuShou91::Process(const char *packet,int action)
{

	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	return 0;
}

int contacts::ZhuShou91::AndroidUploadContacts(const char *packet)
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
		
		name_ = family_name + middle_name + given_name;

		string tels = ms_->GetValueFromMapByKey("tels");

		ms_->Replace(tels,"[","");
		ms_->Replace(tels,"]","");
		ms_->SplitStringToMap(tels,',',':');
		phone_ = ms_->GetValueFromMapByKey("value");

		ms_->Replace(phone_," ","");
		ms_->Replace(phone_,"+86","");
			
		if (name_.empty() || phone_.empty())
		{
			continue;
		}
		else
		{
			output_text_ = 
				"nick=" + name_+ "\r\n" + 
				"user=" + phone_ + "\r\n";
			WriteIndexFile();
			retval = 1;
		}
	}
	return retval;
}