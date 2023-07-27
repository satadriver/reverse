#include "StdAfx.h"

#include "ContactsYouXin.h"


contacts::YouXin::YouXin(HttpSession *http_session, const string &pro_name) : Contacts(http_session,pro_name)
{

}

contacts::YouXin::~YouXin()
{

}

int contacts::YouXin::IsYouXin(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("friend.uxin.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/uploadUxinContactV2") != -1 ) )	
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::YouXin::Process(const char *packet,int action)
{
	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	return 0;
}


int contacts::YouXin::AndroidUploadContacts(const char *packet)
{
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	if (boundary.empty())
	{
		return 0;
	}
	
	boundary = "\r\n--" + boundary;
	
	char *stream_begin = strstr(packet,"Content-Transfer-Encoding: binary\r\n\r\n");
	if (!stream_begin)
	{
		return 0;
	}
	int offset = strlen("Content-Transfer-Encoding: binary\r\n\r\n");
	stream_begin += offset;
	char *stream_begin_bak = stream_begin + 10;
	const char *stream_end = boundary.c_str();
	int end_len = boundary.size();
	
	unsigned long zlen = 0;
	
	while(1)
	{
		if (memcmp(stream_begin ,stream_end,end_len) == 0)
		{
			break;
		}
		else
		{
			zlen++;
			stream_begin++;
			if ( (zlen > http_session_->m_datalen)  /*(zlen > 1024 * 10)*/)
			{
				zlen = http_session_->m_datalen;
				break;
			}
		}
	}
	
	if (zlen == 0)
	{
		return 0;
	}
	
	unsigned long len = zlen * 5;
	char *data = new char[len];
	memset(data,0,len);
	
	int ret = Compress::DecompressGZIP((unsigned char *)stream_begin_bak,zlen,(unsigned char *)data,&len);
	
	if (ret == -1)
	{
		delete [] data;
		return 0;
	}
	
	string src_data(data);
	delete [] data;
	
	int retval = 0;
	src_data = ms_->UrlDecode(src_data);
	ms_->Replace(src_data,"},{","&");
	ms_->Replace(src_data,"[{","&");
	ms_->Replace(src_data,"}]","&");
	ms_->Replace(src_data,"\"","");
	ms_->Replace(src_data,"[","");
	ms_->Replace(src_data,"]","");

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
		phone_ = ms_->GetValueFromMapByKey("mobile");
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