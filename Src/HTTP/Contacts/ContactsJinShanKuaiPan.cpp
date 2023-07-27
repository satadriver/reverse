
#include "StdAfx.h"

#include "ContactsJinShanKuaiPan.h"

contacts::JinShanKuaiPan::JinShanKuaiPan(HttpSession *http_session, const string &pro_name) : Contacts(http_session,pro_name)
{
	
}

contacts::JinShanKuaiPan::~JinShanKuaiPan()
{
	
}

int contacts::JinShanKuaiPan::IsJinShanKuaiPan(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("kcloud.kuaipan.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/kcloud/open/AndroidUploadContacts?") != -1 ))	
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::JinShanKuaiPan::Process(const char *packet,int action)
{
	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	return 0;
}


int contacts::JinShanKuaiPan::AndroidUploadContacts(const char *packet)
{
	string src_data(packet);
	
	ms_->Replace(src_data,"{","");
	ms_->Replace(src_data,"}","");
	ms_->Replace(src_data,"/","");
	ms_->Replace(src_data,"\\","");
	ms_->Replace(src_data,"\"","");
	ms_->Replace(src_data,"detail","&");
	
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	
	int retval = 0;
	
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
		name_ = ms_->GetValueFromMapByKey("display_name",true);
		phone_ = ms_->GetValueFromMapByKey("number");
		ms_->Replace(phone_," ","");
		ms_->Replace(phone_,"+86","");	
		if (name_.empty() || phone_.empty())
		{
			continue;
		}	
		output_text_ = 
			"nick=" + name_+ "\r\n" + 
			"user=" + phone_ + "\r\n";
		WriteIndexFile();
		retval = 1;
	}	
	return retval;
}