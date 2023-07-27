
#include "StdAfx.h"

#include "ChatingAliIM.h"

chating::AliIM::AliIM(HttpSession *http_session, const string &pro_name) : Chating(http_session,pro_name)
{
	
}

chating::AliIM::~AliIM()
{
	
}

int chating::AliIM::IsAliIM(HttpSession *http_session)
{
	if( (http_session->m_Requestline.m_Host.Find("slice.wangxin.taobao.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/ul") != -1))	
	{
		return kSendVoiceMessageM;
	}
	return 0;
}

int chating::AliIM::Process(const char *packet,int action)
{
	if (action == kSendVoiceMessageM)
	{
		return SendVoiceMessageM(packet);
	}
	return 0;
}


int chating::AliIM::SendVoiceMessageM(const char *packet)
{
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;
	
	string src_data(packet);
	
	string tmp = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"args\"\r\n\r\n",boundary);
	
	ms_->Replace(tmp,"{","");
	ms_->Replace(tmp,"}","");
	ms_->Replace(tmp,"\"","");

	ms_->SplitStringToMap(tmp,',',':');

	string tmp_file_size = ms_->GetValueFromMapByKey("size");

	from_ = ms_->GetValueFromMapByKey("from_id",true);
	to_ = ms_->GetValueFromMapByKey("to_id",true);

	if (from_.empty() || to_.empty() || tmp_file_size.empty())
	{
		return 0;
	}

	ms_->Replace(from_,"cnhhupan","");
	ms_->Replace(to_,"cnhhupan","");

	int file_size = ms_->ToInt(tmp_file_size);
	char *stream = strstr(packet,"#!AMR");

	if (stream)
	{
		if ( (file_size > 0) && (file_size < real_file_max_size_) )
		{
			char file_name[128] = {0};
			sprintf(file_name,"ali_%s-%s_voice.amr",from_.c_str(),to_.c_str());
			string real_file_name = WriteTempRealFile(stream,file_name,file_size);
			output_text_ = 
				"from=" + from_ + "\r\n" +
				"to=" + to_ + "\r\n" +
				"state=·¢ËÍÓïÒôÏûÏ¢\r\n";
			WriteIndexFile(real_file_name);
		}
	}	
	return 1;

}

string chating::AliIM::GetAccount(const string &account)
{
	string::size_type tmp_pos = account.find("cnhhupan");
	if (tmp_pos != string::npos)
	{
		return account.substr(strlen("cnhhupan"));
	}
	else
	{
		return account;
	}
}