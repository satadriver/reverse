
#include "StdAfx.h"

#include "ChatingYiXin.h"

map<string, YiXinFileBuffList *> chating::YiXin::file_buffer_ = map<string, YiXinFileBuffList *>();

chating::YiXin::YiXin(HttpSession *http_session, const string &pro_name) : Chating(http_session,pro_name)
{
	
}

chating::YiXin::~YiXin()
{
	
}

int chating::YiXin::IsYiXin(HttpSession *http_session)
{
	if ((http_session->m_Requestline.m_Host.Find("api.dd.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/api/queryareacode") != -1) )
	{
		return kLogin;
	}

	if( (http_session->m_Requestline.m_URI.Find("/nos/cacheUpload") != -1) &&
		(http_session->m_Requestline.m_URI.Find("uid=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("partsize") != -1))	
	{
		return kSendVoiceMessageM;
	}


	if( (http_session->m_Requestline.m_URI.Find("/nos/cacheUpload") != -1) &&
		(http_session->m_Requestline.m_URI.Find("resourceType=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("sid=") != -1) && 
		(http_session->m_Requestline.m_URI.Find("offset=") != -1) && 
		(http_session->m_Requestline.m_URI.Find("partsize") == -1))
	{
		return kCacheUploadFile;
	}
	if ((http_session->m_Requestline.m_Host.Find("qq.com") != -1) )
	{
		return kReassembleFileBuff;
	}
	
	return 0;
}

int chating::YiXin::Process(const char *packet,int action)
{
	if (action == kLogin)
	{
		Login(packet);
	}
	if (action == kCacheUploadFile)
	{
		CacheUploadFile(packet);
	}

	if (action == kSendVoiceMessageM)
	{
		return SendVoiceMessageM(packet);
	}

	if (action == kUploadPic)
	{
		return UploadPic(packet);
	}

	if (action == kUploadVoiceMI)
	{
		return UploadVoiceMI(packet);
	}

	if (action == kReassembleFileBuff)
	{
		ReassembleFileBuff();
	}
	return 0;
}

int chating::YiXin::Login(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = UrlDecode(uri);
	ms_->Replace(uri,"?","&");

	ms_->SplitStringToMap(uri,'&','=');
	user_ = ms_->GetValueFromMapByKey("mobile");

	if (user_.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + user_ + "\r\n" +
		"state=上线\r\n";
	WriteIndexFile();

	return 1;
}

int chating::YiXin::CacheUploadFile(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = UrlDecode(uri);
	ms_->Replace(uri,"?","&");
	
	ms_->SplitStringToMap(uri,'&','=');
	string uid = ms_->GetValueFromMapByKey("uid");
	string cache_key = ms_->GetValueFromMapByKey("cacheKey");
	string md5 = ms_->GetValueFromMapByKey("md5");
	string key;

	string offset_s = ms_->GetValueFromMapByKey("offset");
	string complete = ms_->GetValueFromMapByKey("complete");

	string resourceType = ms_->GetValueFromMapByKey("resourceType");
	string buffType;
	string uploadType;
	if (resourceType == "1")
	{
		string cType = ms_->GetValueFromMapByKey("cType");
		string::size_type pos = cType.find("image/");
		if (pos != string::npos)
		{
			buffType = cType.substr(pos + strlen("image/"));
			uploadType = "上传图片";
		}
		else
		{
			uploadType = "上传";
		}
		key = md5;
	}
	else if (resourceType == "2")
	{
		buffType = "aac";
		uploadType = "上传语音";
		key = cache_key;
	}
	
	if (uid.empty() || key.empty() || offset_s.empty() || complete.empty())
	{
		return 0;
	}
	
	int offset = ms_->ToInt(offset_s);
	
	YiXinFileBuff *fb = new YiXinFileBuff();
	fb->offset = offset;
	fb->length = http_session_->m_datalen;
	fb->data = new char[fb->length];
	memset(fb->data,0,fb->length);
	memcpy(fb->data,http_session_->m_databuffer,fb->length);
	
	map<string, YiXinFileBuffList *>::iterator iter = file_buffer_.find(key);
	if (iter == file_buffer_.end())
	{
		YiXinFileBuffList *fbl = new YiXinFileBuffList();
		fbl->buffType = buffType;
		fbl->uploadType = uploadType;
		fbl->uid = uid;
		fbl->ct = time(0);
		fbl->buffs.push_back(fb);
		file_buffer_.insert(make_pair(key,fbl));
		if (complete == "true")
		{
			fbl->complete = true;
			
		}
		ReassembleFileBuff();
		
	}
	else
	{
		iter->second->ct = time(0);
		iter->second->buffs.push_back(fb);
		if (complete == "true")
		{
			iter->second->complete = true;
			
		}
		ReassembleFileBuff();
	}

	return 1;
}

int chating::YiXin::SendVoiceMessageM(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("?");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
	
	from_ = ms_->GetValueFromMapByKey("uid");

	string tmp_file_size = ms_->GetValueFromMapByKey("partsize");

	string resourceType = ms_->GetValueFromMapByKey("resourceType");
	string buffType;
	string uploadType;
	if (resourceType == "1")
	{
		string cType = ms_->GetValueFromMapByKey("cType");
		string::size_type pos = cType.find("image/");
		if (pos != string::npos)
		{
			buffType = cType.substr(pos + strlen("image/"));
			uploadType = "上传图片";
		}
		else
		{
			uploadType = "上传";
		}

	}
	else if (resourceType == "2")
	{
		buffType = "aac";
		uploadType = "上传语音";
	}
	
	if (from_.empty() || tmp_file_size.empty())
	{
		return 0;
	}
	int file_size = ms_->ToInt(tmp_file_size);
	
	if ( (0 < file_size) && (file_size < real_file_max_size_))
	{
		char file_name[128] = {0};
		sprintf(file_name,"yixin_%svoice.%s",from_.c_str(),buffType.c_str());
		string real_file_name = WriteTempRealFile(packet,file_name,file_size);
		output_text_ = 
			"from=" + from_ + "\r\n" +
			"state=" + uploadType + "\r\n";
		WriteIndexFile(real_file_name);
	}
	return 1;
}

int chating::YiXin::UploadVoiceMI(const char *packet)
{
	return 1;
}

int chating::YiXin::UploadPic(const char *packet)
{

	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = UrlDecode(uri);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	
	from_ = ms_->GetValueFromMapByKey("uid");
	user_ = from_;
	string fn = ms_->GetValueFromMapByKey("sid");

	if (user_.empty())
	{
		return 0;
	}


	int file_size = http_session_->m_datalen;
	if ( (0 < file_size) && (file_size < real_file_max_size_))
	{
		char file_name[128] = {0};
		sprintf(file_name,"yixin_%s.jpg",fn.c_str());
		string real_file_name = WriteTempRealFile(packet,file_name,file_size);
		output_text_ = 
			"from=" + from_ + "\r\n" +
			"state=上传图片\r\n";
		WriteIndexFile(real_file_name);
	}
	return 0;

}

int chating::YiXin::ReassembleFileBuff()
{
	time_t nt = time(0);
	
	for (map<string,YiXinFileBuffList *>::iterator itfbl = file_buffer_.begin(); itfbl != file_buffer_.end(); )
	{
		YiXinFileBuffList * fbl = itfbl->second;
		if ( ((nt - fbl->ct) > 60) || (fbl->complete == true))
		{
			int file_size = 0;

			for (list<YiXinFileBuff *>::iterator itvb1 = fbl->buffs.begin(); itvb1 != fbl->buffs.end(); ++itvb1)
			{
				file_size += (*itvb1)->length;
			}

			char *p = new char[file_size];
		
			for (list<YiXinFileBuff *>::iterator itvb2 = fbl->buffs.begin(); itvb2 != fbl->buffs.end(); ++itvb2)
			{
				YiXinFileBuff *vb = *itvb2;
				if ( vb->offset < file_size )
				{
					memcpy(p + vb->offset,vb->data,vb->length);
				}		
			}

			for (list<YiXinFileBuff *>::iterator itvb3 = fbl->buffs.begin(); itvb3 != fbl->buffs.end(); ++itvb3)
			{
				delete (*itvb3);
			}


			char file_name[128] = {0};
			sprintf(file_name,"yixin_%svoice.%s",itfbl->first.c_str(),itfbl->second->buffType.c_str());
			string real_file_name = WriteTempRealFile(p,file_name,file_size);
			delete p;
			output_text_ = 
				"from=" + itfbl->second->uid + "\r\n" +
				"state="+ itfbl->second->uploadType + "\r\n";
			WriteIndexFile(real_file_name);
		

			delete itfbl->second;
			file_buffer_.erase(itfbl++);
		}
		else
		{
			++itfbl;
		}
	}
	return 1;
}

