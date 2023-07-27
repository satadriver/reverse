#include "StdAfx.h"

#include "ContactsSinaWeibo.h"


contacts::SinaWeiBo::SinaWeiBo(HttpSession *http_session, const string &pro_name) : Contacts(http_session,pro_name)
{

}

contacts::SinaWeiBo::~SinaWeiBo()
{

}

int contacts::SinaWeiBo::IsSinaWeiBo(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.weibo.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("contacts/upload_vcard") != -1 ) )	
	{
		return kAndroidUploadContacts;
	}
	return 0;
}

int contacts::SinaWeiBo::Process(const char *packet,int action)
{
	if (action == kAndroidUploadContacts)
	{
		return AndroidUploadContacts(packet);
	}
	return 0;
}


int contacts::SinaWeiBo::GetStreamLength(const char *stream,const char *stream_end)
{
	int end_len = strlen("stream_end");
	unsigned long zlen = 0;
	int stream_length = 0;
	int retval = 0;
	while(1)
	{
		if (memcmp(stream ,stream_end,end_len) == 0)
		{
			return stream_length;
		}
		else
		{
			zlen++;
			stream++;
			stream_length++;
			if (stream_length > 10240 * 10)
			{
				return 0;
			}
		}
	}
}

int contacts::SinaWeiBo::RemoveUselessData(const char *stream,int stream_length,char *stream_pure)
{
	int pure_length = 0;
	int temp_length = 0;;
	while(1)
	{
		if (temp_length == stream_length)
		{
			return pure_length;
		}
/*
		if ( memcmp(stream,"\x00",1) == 0)
		{
			stream++;
			pure_length--;
		}
		else*/ if (memcmp(stream,"\x1f\x8b\x08\x00\x00\x00\x00\x00\x00\x00",10) == 0)
		{
			stream += 10;
			pure_length -= 10;
		}
		else
		{
			memcpy(stream_pure,stream,1);
			temp_length++;
			pure_length++;
			stream++;
			stream_pure++;
		}
	}
}

int contacts::SinaWeiBo::AndroidUploadContacts(const char *packet)
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
	const char *stream_end = boundary.c_str();
	
	int offset = strlen("Content-Transfer-Encoding: binary\r\n\r\n");
	stream_begin += offset;
	char *stream_begin_bak = stream_begin;
	
	int stream_length = GetStreamLength(stream_begin,stream_end);
	
	
	char *stream = stream_begin;
	int retval = 0;
	int zlen_count = 0;
	while(1)
	{
		if (memcmp(stream,"\x1f\x8b\x08\x00\x00\x00\x00\x00\x00\x00",10) == 0)
		{
			int zlen = GetStreamLength(stream + 10,"\x1f\x8b\x08\x00\x00\x00\x00\x00\x00\x00");
			zlen_count = zlen_count + zlen;
			
			unsigned long len = zlen * 5;
			char *data = new char[len];
			
			memset(data,0,len);
			
			int ret = Compress::DecompressGZIP((unsigned char *)stream + 10,zlen,(unsigned char *)data,&len);
			if (ret == -1)
			{
				delete [] data;
				continue;
			}
			
			string src_data(data);
			delete [] data;
			
			
			src_data = ms_->UrlDecode(src_data);
			
			name_ = ms_->GetValueBetweenBeginEnd(src_data,"FN:","\n",true);
			phone_ = ms_->GetValueBetweenBeginEnd(src_data,"TYPE=CELL:","\n",true);
			if (name_.empty() || phone_.empty())
			{
				stream += zlen;
				continue;
			}
			ms_->Replace(phone_," 86","");
			
			output_text_ = 
				"nick=" + name_+ "\r\n" + 
				"user=" + phone_ + "\r\n";
			WriteIndexFile();
			
			retval = 1;
			
			
			stream += zlen;
			if (zlen_count >= stream_length)
			{
				return 1;
			}
		}
		else
		{
			stream++;
		}
	}
	
	return retval;
}