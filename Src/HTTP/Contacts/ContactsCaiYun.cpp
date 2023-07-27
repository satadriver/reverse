
#include "StdAfx.h"

#include "ContactsCaiYun.h"

contacts::CaiYun::CaiYun(HttpSession *http_session, const string &pro_name) : Contacts(http_session,pro_name)
{
	
}

contacts::CaiYun::~CaiYun()
{
	
}

int contacts::CaiYun::IsCaiYun(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("a.cytxl.com.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/mcloud/jsonrpc_api.php") != -1 ))
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::CaiYun::Process(const char *packet,int action)
{
	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	return 0;
}

int contacts::CaiYun::AndroidUploadContacts(const char *packet)
{
	unsigned long zlen = http_session_->m_Requestline.m_Content_length;
	unsigned long len = zlen * 5;
	char *data = new char[len];
	memset(data,0,len);
	
	char *hd = strstr(packet,"\x1f\x8b\x08\x00\x00\x00");
	if (!hd)
	{
		delete [] data;
		return 0;
	}

	int ret = Compress::DecompressGZIP((unsigned char *)hd + 10,zlen,(unsigned char *)data,&len);

	if (ret == -1)
	{
		delete [] data;
		return 0;
	}
	
	
	string src_data(data);
	
	delete [] data;
	
	ms_->Replace(src_data,"\"},","&");
	ms_->Replace(src_data,"[","");
	ms_->Replace(src_data,"]","");
	ms_->Replace(src_data," ","");
	ms_->Replace(src_data,"\"","");
	ms_->Replace(src_data,"{","");
	ms_->Replace(src_data,"}}","&");
	
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
		
		
		name_ = ms_->GetValueFromMapByKey("name",true);
		phone_ = ms_->GetValueFromMapByKey("mobile");
		
	
		if (name_.empty() || phone_.empty())
		{
			continue;
		}
		
		ms_->Replace(phone_," ","");
		ms_->Replace(phone_,"+86","");
		
		output_text_ = 
			"nick=" + name_+ "\r\n" + 
			"user=" + phone_ + "\r\n";
		WriteIndexFile();
		retval = 1;
	}
	
	return retval;
}