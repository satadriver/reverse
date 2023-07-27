
#include "StdAfx.h"

#include "ContactsYYBackup.h"

contacts::YYBackup::YYBackup(HttpSession *http_session, const string &pro_name) : Contacts(http_session, pro_name)
{
	
}

contacts::YYBackup::~YYBackup()
{
	
}

int contacts::YYBackup::IsYYBackup(HttpSession *http_session)
{

	if((http_session->m_Requestline.m_Host.Find("yy1.bjshinesky.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v1/backup_contact.php")!=-1))
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::YYBackup::Process(const char *packet,int action)
{

	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	
	return 0;
}

int contacts::YYBackup::AndroidUploadContacts(const char *packet)
{
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "--" +boundary;
	string temp_data(packet);

	int filebegin = temp_data.find("multipart/form-data") + strlen("multipart/form-data");
	int fileend = temp_data.find(boundary,filebegin);
	string strtemp = temp_data.substr(filebegin,fileend);
	
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0;

	int retval = 0;
	while(1)
	{
		item_end_pos = strtemp.find("END:VCARD",item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = strtemp.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = strtemp.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + strlen("END:VCARD");
		}
		ms_->Replace(item,"\r\n","&");
		ms_->SplitStringToMap(item,'&',':');
		
		string name = ms_->GetValueFromMapByKey("FN",true);
		string phone = ms_->GetValueFromMapByKey("TEL;CELL");
		
		if (name.empty() || phone.empty())
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


