
#include "StdAfx.h"

#include "YiXin.h"

YiXin::YiXin()
{
	software_ = "易信";
}

YiXin::~YiXin()
{
	
}

int YiXin::Is(HttpSession *http_session)
{
	if( (http_session->m_Requestline.m_URI.Find("/nos/cacheUpload") != -1) &&
		(http_session->m_Requestline.m_URI.Find("uid=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("partsize=") != -1))	
	{
		return kSendVoiceMessageM;
	}
	return 0;
}

int YiXin::Process(const char *packet,int action)
{
	if (action == kSendVoiceMessageM)
	{
		return SendVoiceMessageM(packet);
	}
	return 0;
}


int YiXin::SendVoiceMessageM(const char *packet)
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
	
	string from = ms_->GetValueFromMapByKey("uid");

	string tmp_file_size = ms_->GetValueFromMapByKey("partsize");
	
	if (from.empty() || tmp_file_size.empty())
	{
		return 0;
	}
	int file_size = ms_->ToInt(tmp_file_size);
	
	if ( (0 < file_size) && (file_size < real_file_max_size_))
	{
		char file_name[128] = {0};
		sprintf(file_name,"yixin_%s_voice.aac",from.c_str());
		string real_file_name = WriteTempRealFile(packet,file_name,file_size);
		output_text_ = 
			"from=" + from + "\r\n" +
			"state=发送语音消息\r\n";
		WriteIndexFile(real_file_name);
	}
	return 1;
}

