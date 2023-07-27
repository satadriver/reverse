
#include "StdAfx.h"
#include "http.h"
#include "HttpSession.h"
#include "HttpPro.h"

HttpPro::HttpPro(HttpSession *http_session, const string &pro_name) : http_session_(http_session), pro_name_(pro_name)
{
	ms_ = new StringManipulation();
	pro_con_ = new ProtocolConfig();
    
	real_file_max_size_ = 5 * 1024 * 1024;
	unrepeated_time_ = 3 * 60;
}

HttpPro::HttpPro()
{
	ms_ = new StringManipulation();
	real_file_max_size_ = 5 * 1024 * 1024;
	unrepeated_time_ = 3 * 60;
}

HttpPro::~HttpPro()
{
	delete ms_;
}


int HttpPro::Is(HttpSession *http_session)
{
	bool result = 0;
	for (list<ProtocolCondition *>::iterator ipc = pro_con_->condition_.begin(); ipc != pro_con_->condition_.end(); ++ipc)
	{
		result = 1;
		if ( (*ipc)->position == "host")
		{
			if ( (*ipc)->compare_mode == ProtocolConfig::CompareMode::kEqual)
			{
				if (strcmp(http_session->m_Requestline.m_Host.GetBuffer(0),(*ipc)->value.c_str()) == 0)
				{
					;
				}
				else
				{
					return 0;
				}
			}
			else if ((*ipc)->compare_mode == ProtocolConfig::CompareMode::kFind)
			{
				if (http_session->m_Requestline.m_Host.Find((*ipc)->value.c_str()) != -1)
				{
					;
				}
				else
				{
					return 0;
				}
			}
		}

		if ( (*ipc)->position == "uri")
		{
			if ( (*ipc)->compare_mode == ProtocolConfig::CompareMode::kEqual)
			{
				if (strcmp(http_session->m_Requestline.m_URI.GetBuffer(0),(*ipc)->value.c_str()) == 0)
				{
					;
				}
				else
				{
					return 0;
				}
			}
			else if ((*ipc)->compare_mode == ProtocolConfig::CompareMode::kFind)
			{
				if (http_session->m_Requestline.m_URI.Find((*ipc)->value.c_str()) != -1)
				{
					;
				}
				else
				{
					return 0;
				}
			}
		}
	}
	return result;
}

int HttpPro::Process2()
{

	return 0;
}

void HttpPro::WriteIndexFile(const string &real_file_name)
{
	if (pro_name_ == "")
	{
		return ;
	}
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = http_session_->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	
	if (!real_file_name.empty())
	{
		http_session_->m_realfname = real_file_name.c_str();
	}
	http_session_->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.indexfile.txt",
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
		http_session_->m_sip, http_session_->m_dip, rand(),pro_name_.c_str());

	http_session_->WriteIndexFile(pro_name_.c_str(), pro_name_.c_str(),output_text_.c_str(),http_session_->m_dceprocotol->m_httpMissObj);
}

void HttpPro::WriteIndexFile1()
{
	if (pro_ == "" || http_session_ == NULL)
	{
		return ;
	}
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = http_session_->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	
	http_session_->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.indexfile.txt",
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
		http_session_->m_sip, http_session_->m_dip, rand(),pro_name_.c_str());
	http_session_->WriteIndexFile(pro_name_.c_str(), pro_name_.c_str(), 
		output_text_.c_str(),http_session_->m_dceprocotol->m_httpMissObj);

	http_session_ = NULL;
}

string HttpPro::WriteTempRealFile(const char *stream,const char *file_name,int file_size)
{
	char file_path[256] = {0};
	char *tmp_path = "v:\\netspy\\tmp\\dce2\\";
			
	SYSTEMTIME now;
	::GetLocalTime(&now);
	::sprintf(file_path,"%s%04d%02d%02d%02d%02d%02d_1_%u_%s",
		tmp_path,now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute,now.wSecond,rand(),file_name);
				
	FILE *f = fopen(file_path,"wb+");
	if (f)
	{
		::fwrite(stream,sizeof(char),file_size,f);
		::fclose(f);
		return string(file_path);
	}
	return "";

}

bool HttpPro::Unrepeated(map<string, time_t>&unreapted, const string &content)
{
	bool retval = false;
	time_t now_time = time(0);
	map<string ,time_t>::iterator iter = unreapted.find(content);

	if (iter == unreapted.end())
	{
		unreapted.insert(make_pair(content,now_time));
		retval = true;
	}
	else
	{
		time_t last_time = iter->second;
		if (now_time - last_time > unrepeated_time_ )
		{
			iter->second = now_time;
			retval = true;
		}
	}
	return retval;
}

void HttpPro::WriteLog(const char *module,const char *txt,...)
{
	::WriteLog(module,txt);
}

//ÐéÄâÉí·Ý
void HttpPro::WriteVirtualIdentity(const string &username,const string &state)
{
	HistoryRecord hisRec;
	hisRec.SetRecord(IDX_FORWTYPE,	"1");
	hisRec.SetRecord(IDX_ID,		username.c_str());
	hisRec.SetRecord(IDX_USER,		username.c_str());
	hisRec.SetRecord(IDX_STATE, state.c_str());
	http_session_->WriteHisData(pro_name_.c_str(),pro_name_.c_str(),&hisRec,http_session_->m_dceprocotol->m_httpMissObj,HTTP_PROTOCOL);
}

void HttpPro::WriteCaseInfo(const string &content)
{
	http_session_->m_OpenIndxFlg = TRUE;
	http_session_->m_OpenCaseFlg = TRUE;
	http_session_->m_OpenWarnFlg = TRUE;
	
	DWORD state = 0;
	SearchCaseOrWarn(http_session_, content.c_str(), content.size(), state,pro_name_.c_str());
}

string HttpPro::GetPostData_tmp(const char *packet)
{

	char *stream = strstr(packet,"\r\n\r\n");
	if (stream)
	{
		return string(stream + strlen("\r\n\r\n"));
	}
	else
	{
		return string (packet);
	}
}

int HttpPro::GetRealFileSize(const char *stream_begin,const char *stream_end)
{
	int len = 0;
	while(1)
	{
		if (memcmp(stream_begin,stream_end,strlen(stream_end)) == 0)
		{
			return len;
		}
		else
		{
			len++;
			stream_begin++;
			if (len > real_file_max_size_)
			{
				return 0;
			}
		}
	}
	return 0;
}