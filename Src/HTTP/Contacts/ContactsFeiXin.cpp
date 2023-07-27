
#include "StdAfx.h"

#include "ContactsFeiXin.h"


contacts::FeiXin::FeiXin(HttpSession *http_session, const string &pro_name) : Contacts(http_session,pro_name)
{

}

contacts::FeiXin::~FeiXin()
{

}

int contacts::FeiXin::IsFeiXin(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("221.176.30.216")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/ciservice/services/mobileservice/syncaddressbook") != -1 ) )	
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::FeiXin::Process(const char *packet,int action)
{
	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	return 0;
}


int contacts::FeiXin::AndroidUploadContacts(const char *packet)
{
	char *zdata = strstr(packet,"\x1f\x8b\x08");
	if (!zdata)
	{
		return 0;
	}
	
	zdata = zdata + 10;
	unsigned zlen = http_session_->m_Requestline.m_Content_length;
	unsigned long len = zlen * 5;
	char *data = new char[len];
	memset(data,0,len);
	
	int ret = Compress::DecompressGZIP((unsigned char *)zdata,zlen,(unsigned char *)data,&len);
	
	if (ret == -1)
	{
		delete [] data;
		return 0;
	}
	
	string src_data(data);
	delete [] data;
	
	int retval = 0;
	src_data = ms_->UTF8ToGBK(src_data);
 	ms_->Replace(src_data,"},{","&");
 	ms_->Replace(src_data,"[{","&");
 	ms_->Replace(src_data,"}]","&");
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

		//½«´í¾Í´í
		name_ = ms_->GetValueFromMapByKey("m",true);
		phone_ = ms_->GetValueFromMapByKey("n",true);
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