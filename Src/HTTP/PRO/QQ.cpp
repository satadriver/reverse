
#include "StdAfx.h"

#include "QQ.h"

QQ::QQ()
{
	software_ = "QQ";
}

QQ::~QQ()
{
	
}

int QQ::IsQQ(HttpSession *http_session)
{

	if( (http_session->m_Requestline.m_URI.Find("/cgi-bin/httpconn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("uin=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("groupcode=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("filesize=") != -1))	
	{
		return kSendGroupVoiceMessageM;
	}

	if( (http_session->m_Requestline.m_Host.Find("d.web2.qq.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("/send_buddy_msg") != -1 ))	
	{
		return kSendMessageCW;
	}

	if( (http_session->m_Requestline.m_Host.Find("d.web2.qq.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("/send_qun_msg") != -1 ))	
	{
		return kSendGroupMessageCW;
	}

	if( (http_session->m_Requestline.m_Host.Find(".qq.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("/analytics/upload") != -1 ))	
	{
		return kUploadDeviceInfo;
	}



	return 0;
}

int QQ::Process(const char *packet,int action)
{
	if (action == kSendGroupVoiceMessageM)
	{
		return SendGroupVoiceMessageM(packet);
	}

	if (action == kLoginC)
	{
		pro_name_ = "imsg_oicq";
		return LoginC(packet);
	}

	if (action == kSendMessageCW)
	{
		pro_name_ = "imsg_oicq";
		return SendMessageCW(packet);
	}

	if (action == kSendGroupMessageCW)
	{
		pro_name_ = "imsg_oicq";
		return SendGroupMessageCW(packet);
	}

	if (action == kUploadDeviceInfo)
	{
		return UploadDeviceInfo(packet);
	}
	return 0;
}

int QQ::LoginC(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');

	string user = ms_->GetValueFromMapByKey("qq");
	if (user.empty())
	{
		return 0;
	}

	WriteVirtualIdentity(user,"上线");
	WriteCaseInfo(user);
	output_text_ = 
		"user=" + user + "\r\n" + 
		"entity= 0\r\n" +
		"stat= 上线\r\n";
	WriteIndexFile();
	return 1;
}

int QQ::SendGroupVoiceMessageM(const char *packet)
{
	char *stream = strstr(packet,"#!AMR");
	if (!stream)
	{
		return 0;
	}

	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	
	ms_->SplitStringToMap(uri,'&','=');

	string from = ms_->GetValueFromMapByKey("uin");
	string group = ms_->GetValueFromMapByKey("groupcode");
	string tmp_file_size = ms_->GetValueFromMapByKey("filesize");

	if (from.empty() || tmp_file_size.empty())
	{
		return 0;
	}
	int file_size = ms_->ToInt(tmp_file_size);

	if ( (0 < file_size) && (file_size < real_file_max_size_))
	{
		char file_name[128] = {0};
		sprintf(file_name,"qq_%s_voice.amr",from.c_str());
		string real_file_name = WriteTempRealFile(stream,file_name,file_size);
		output_text_ = 
			"from=" + from + "\r\n" +
			"mid=" + group + "\r\n" +
			"state=发送群语音消息(手机)\r\n";
		WriteIndexFile(real_file_name);
	}
	return 1;
}


int QQ::UploadDeviceInfo(const char *packet)
{
	return 1;
}

int QQ::SendMessageCW(const char *packet)
{
	string user = GetQQ();
	string from = user;
	string src_data(packet);
	string b("\"content\":\"[\\\"");
	string e("\\\",");
	string message = ms_->GetValueBetweenBeginEnd(src_data,b,e,true);
	if (message.find("\\u") != string::npos)
	{
		ms_->Replace(message,"\\\\u","\\u");
		message = ms_->UnicodeToGBKPrefix_u(message);
	}

	if (from.empty() || message.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + user + "\r\n" +
		"sender=" + from + "\r\n" +
	//	"msg=" + message_ + "\r\n" +
		"entity= 1\r\n" +
		"state=发送消息(Web)\r\n";
	string tmp_file_name = WriteTempRealFile(message.c_str(),"webqq_group_chat_content.txt",message.size());
	WriteIndexFile(tmp_file_name);

	return 1;
}

int QQ::SendGroupMessageCW(const char *packet)
{
	string user = GetQQ();
	string from = user;
	string src_data(packet);
	string group = ms_->GetValueBetweenBeginEnd(src_data,"\"group_uin\":",",");

	string b("\"content\":\"[\\\"");
	string e("\\\",");
	string message = ms_->GetValueBetweenBeginEnd(src_data,b,e,true);
	if (message.find("\\u") != string::npos)
	{
		ms_->Replace(message,"\\\\u","\\u");
		message = ms_->UnicodeToGBKPrefix_u(message);
	}

	if (from.empty() || message.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + user + "\r\n" +
		"sender=" + from + "\r\n" +
		"qqgroupid=" + group + "\r\n" +
	//	"msg=" + message_ + "\r\n" +
		"entity= 1\r\n" +
		"state=发送群消息(Web)\r\n";
	string tmp_file_name = WriteTempRealFile(message.c_str(),"webqq_group_chat_content.txt",message.size());
	WriteIndexFile(tmp_file_name);
	return 1;
}


string QQ::GetQQ()
{
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0);

	ms_->SplitStringToMap(cookie,';','=');
	string qq;
	qq = ms_->GetValueFromMapByKey("ptui_loginuin");
	if (!qq.empty())
	{
		return qq;
	}
	qq = ms_->GetValueFromMapByKey("pt2gguin");
	if (!qq.empty())
	{
		ms_->Replace(qq,"o","");
		return qq;
	}
	qq = ms_->GetValueFromMapByKey("uin");
	if (!qq.empty())
	{
		ms_->Replace(qq,"o","");
		return qq;
	}
	return qq;
}