
#include "StdAfx.h"

#include "ContactsBaiSiBuDeJie.h"

contacts::BaiSiBuDeJie::BaiSiBuDeJie(HttpSession *http_session, const string &pro_name) : Contacts(http_session, pro_name)
{
	
}

contacts::BaiSiBuDeJie::~BaiSiBuDeJie()
{
	
}

int contacts::BaiSiBuDeJie::IsBaiSiBuDeJie(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("api.budejie.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("api/api_open.php")!=-1))
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::BaiSiBuDeJie::Process(const char *packet,int action)
{
	
	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	return 0;
}

int contacts::BaiSiBuDeJie::AndroidUploadContacts(const char *packet)
{
	char *hd = strstr(packet,"list=");
	if (!hd)
	{
		return 0;
	}
	
	int retval = 0;
	string src_data(hd + strlen("list="));
	src_data = ms_->UrlDecode(src_data);

	ms_->Replace(src_data,"[{","");
	ms_->Replace(src_data,"}]","");
	ms_->Replace(src_data,"},{","&");
	ms_->Replace(src_data,"},","&");
	ms_->Replace(src_data,",{","&");
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
		name_ = ms_->GetValueFromMapByKey("b",true);
		phone_ = ms_->GetValueFromMapByKey("c");
		ms_->Replace(phone_," ","");
		ms_->Replace(phone_,"+86","");
	
		if (name_.empty() || phone_.empty())
		{
			continue;
		}
		
		output_text_ = 
			"nick=" + name_+ "\r\n" + 
			"user=" + phone_ + "\r\n" + 
			"state=百思不得姐\r\n";
		WriteIndexFile();
		retval = 1;
	}	
	return retval;
}


