
#include "StdAfx.h"

#include "FeiXin.h"

FeiXin::FeiXin()
{
	
}

FeiXin::~FeiXin()
{
	
}

int FeiXin::Is(HttpSession *http_session)
{
	if( (http_session->m_Requestline.m_URI.Find("/ncfp/blockupload") != -1) &&
		(http_session->m_Requestline.m_URI.Find("tid=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("filesize=") != -1))	
	{
		return kSendVoiceMessageM;
	}

	if( ( http_session->m_Requestline.m_Host.Find("221.176.30.216")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/ciservice/services/mobileservice/syncaddressbook") != -1 ) )	
	{
		return kUploadContactsMA;
	}
	return 0;
}

int FeiXin::Process(const char *packet,int action)
{
	if (action == kSendVoiceMessageM)
	{
		return SendVoiceMessageM(packet);
	}

	if (action == kUploadContactsMA)
	{
		return UploadContactsMA(packet);
	}
	return 0;
}


int FeiXin::SendVoiceMessageM(const char *packet)
{
	char *hd = strstr(packet,"#!AMR");
	if (!hd)
	{
		return 0;
	}

	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	
	ms_->SplitStringToMap(uri,'&','=');

	string from = ms_->GetValueFromMapByKey("tid");

	string tmp_file_size = ms_->GetValueFromMapByKey("filesize");

	if (from.empty() || tmp_file_size.empty())
	{
		return 0;
	}
	int file_size = ms_->ToInt(tmp_file_size);

	if ( (0 < file_size) && (file_size < real_file_max_size_))
	{
		char file_name[128] = {0};
		sprintf(file_name,"FeiXin_%s_voice.amr",from.c_str());
		string real_file_name = WriteTempRealFile(hd,file_name,file_size);
		output_text_ = 
			"from=" + from + "\r\n" +
			"state=发送语音消息\r\n";
		WriteIndexFile(real_file_name);
	}

	return 1;

}

int FeiXin::UploadContactsMA(const char *packet)
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
		
		//将错就错
		string name = ms_->GetValueFromMapByKey("m",true);
		string phone = ms_->GetValueFromMapByKey("n");
		ms_->Replace(phone,"+86","");
		
		if (name.empty() || phone.empty())
		{
			continue;
		}
		
		output_text_ = 
			"nick=" + name+ "\r\n" + 
			"user=" + phone + "\r\n";
		WriteIndexFile();
		retval = 1;
	}
	return retval;
}