
#include "StdAfx.h"

#include "ContactsXiMaLaYa.h"

contacts::XiMaLaYa::XiMaLaYa(HttpSession *http_session, const string &pro_name) : Contacts(http_session,pro_name)
{

}

contacts::XiMaLaYa::~XiMaLaYa()
{

}

int contacts::XiMaLaYa::IsXiMaLaYa(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("mobile.ximalaya.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/mobile/friendship/uploadphone") != -1 ))	
	{
		return kUploadContacts;
	}
	return 0;
}

int contacts::XiMaLaYa::Process(const char *packet,int action)
{
	if (action == kUploadContacts)
	{
		return UploadContacts(packet);
	}
	return 0;
}

int contacts::XiMaLaYa::UploadContacts(const char *packet)
{

	char *hdaz = strstr(packet,"linkmanList=");	//°²×¿
	char *hdi = strstr(packet,"linkmanlist=");	//iOS

	if ((!hdaz) &&(!hdi))
	{
		return 0;
	}
	string src_data;
	int retval = 0;
	if (hdaz)
	{
	    src_data = hdaz + strlen("linkmanList=");
	}
	if (hdi)
	{
		src_data = hdi + strlen("linkmanlist=");
	}
	
	
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_data.find(",",item_beg_pos);
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
			item_beg_pos = item_end_pos + strlen(",");
		}
		
		string::size_type temp_pos = item.find("-");
		if (temp_pos != string::npos)
		{
			name_ = item.substr(0,temp_pos);
			name_ = ms_->UTF8ToGBK(ms_->UrlDecode(name_));
			phone_ = item.substr(temp_pos + 1);
			
			if (name_.empty() || phone_.empty())
			{
				continue;
			}
			
			int b = true;
			
			for (int i = 0; i < phone_.size(); ++i)
			{
				if ( !(('0' <= phone_[i] ) && (phone_[i] <= '9')))
				{
					b = false;
				}
			}
			
			if (b)
			{
				ms_->Replace(phone_,"+86","");
				output_text_ = 
					"nick=" + name_+ "\r\n" + 
					"user=" + phone_ + "\r\n";
				WriteIndexFile();
				retval = 1;
			}
			else
			{
				continue;
			}
		}
		else
		{
			continue;
		}
	}
	
	return retval;
}