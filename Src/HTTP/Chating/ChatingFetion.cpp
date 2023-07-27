
#include "StdAfx.h"

#include "ChatingFetion.h"

chating::Fetion::Fetion(HttpSession *http_session, const string &pro_name) : Chating(http_session,pro_name)
{
	
}

chating::Fetion::~Fetion()
{
	
}

int chating::Fetion::IsFetion(HttpSession *http_session)
{
	if( (http_session->m_Requestline.m_URI.Find("/ncfp/blockupload") != -1) &&
		(http_session->m_Requestline.m_URI.Find("tid=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("filesize=") != -1))	
	{
		return kSendVoiceMessageM;
	}
	return 0;
}

int chating::Fetion::Process(const char *packet,int action)
{
	if (action == kSendVoiceMessageM)
	{
		return SendVoiceMessageM(packet);
	}
	return 0;
}


int chating::Fetion::SendVoiceMessageM(const char *packet)
{
	char *hd = strstr(packet,"#!AMR");
	if (!hd)
	{
		return 0;
	}

	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	
	ms_->SplitStringToMap(uri,'&','=');

	from_ = ms_->GetValueFromMapByKey("tid");

	string tmp_file_size = ms_->GetValueFromMapByKey("filesize");

	if (from_.empty() || tmp_file_size.empty())
	{
		return 0;
	}
	int file_size = ms_->ToInt(tmp_file_size);

	if ( (0 < file_size) && (file_size < real_file_max_size_))
	{
		char file_name[128] = {0};
		sprintf(file_name,"fetion_%s_voice.amr",from_.c_str());
		string real_file_name = WriteTempRealFile(hd,file_name,file_size);
		output_text_ = 
			"from=" + from_ + "\r\n" +
			"state=·¢ËÍÓïÒôÏûÏ¢\r\n";
		WriteIndexFile(real_file_name);
	}

	return 1;

}