
#include "StdAfx.h"

#include "ChatingMiLiao.h"

chating::MiLiao::MiLiao(HttpSession *http_session, const string &pro_name) : Chating(http_session,pro_name)
{
	
}

chating::MiLiao::~MiLiao()
{
	
}

int chating::MiLiao::IsMiLiao(HttpSession *http_session)
{
	if( (http_session->m_Requestline.m_URI.Find("user") != -1) &&
		(http_session->m_Requestline.m_URI.Find("mms/upload") != -1) &&
		(http_session->m_Requestline.m_URI.Find("fn=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("fs=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("to=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("uuid=") != -1))	
	{
		return kSendVoiceMessageM;
	}
	return 0;
}

int chating::MiLiao::Process(const char *packet,int action)
{
	if (action == kSendVoiceMessageM)
	{
		return SendVoiceMessageM(packet);
	}
	return 0;
}


int chating::MiLiao::SendVoiceMessageM(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	
	ms_->SplitStringToMap(uri,'&','=');
	
	from_ = ms_->GetValueFromMapByKey("uuid");
	to_ = ms_->GetValueFromMapByKey("to");

	string tmp_file_size = ms_->GetValueFromMapByKey("fs");
	
	if (from_.empty() || to_.empty() || tmp_file_size.empty())
	{
		return 0;
	}

	int file_size = ms_->ToInt(tmp_file_size);
	
	char *stream = strstr(packet,"spx\"\r\n\r\n");
	if (stream)
	{
		if ( (0 < file_size) && (file_size < real_file_max_size_))
		{
			stream = stream + strlen("spx\"\r\n\r\n");
			char file_name[128] = {0};
			sprintf(file_name,"miliao_%s-%s_voice.spx",from_.c_str(),to_.c_str());
			string real_file_name = WriteTempRealFile(stream,file_name,file_size);
			output_text_ = 
				"from=" + from_ + "\r\n" +
				"to=" + to_ + "\r\n" +
				"state=·¢ËÍÓïÒôÏûÏ¢\r\n";
			WriteIndexFile(real_file_name);
			return 1;
		}
	}

	return 0;


}

