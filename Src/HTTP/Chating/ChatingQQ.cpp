
#include "StdAfx.h"

#include "ChatingQQ.h"

chating::QQ::QQ(HttpSession *http_session, const string &pro_name) : Chating(http_session,pro_name)
{
	
}

chating::QQ::~QQ()
{
	
}

int chating::QQ::IsQQ(HttpSession *http_session)
{

	/*
	if( (http_session->m_Requestline.m_Host.Find("ws.sj.qq.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("/webservices/getPCDownOnOff.do") != -1 ))	
	{
	//	return kLoginC;
	}
*/
	if( (http_session->m_Requestline.m_URI.Find("/cgi-bin/httpconn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("uin=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("groupcode=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("filesize=") != -1))	
	{
		return kSendGroupVoiceMessageM;
	}
	if( (http_session->m_Requestline.m_Host.Find("client.show.qq.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("/cgi-bin/qqshow_user_props_info") != -1 ))	
	{
		return kCreateChatingC;
	}
	if( (http_session->m_Requestline.m_Host.Find("d.web2.qq.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("/send_buddy_msg") != -1 ))	
	{
		return kSendMessageWC;
	}
	if( (http_session->m_Requestline.m_Host.Find("d.web2.qq.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("/send_qun_msg") != -1 ))	
	{
		return kSendGroupMessageWC;
	}
	if( (http_session->m_Requestline.m_Host.Find("pt.3g.qq.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("qzoneLogin") != -1 ))	
	{
		return kLoginQZoneWM;
	}
	if( (http_session->m_Requestline.m_Host.Find("pt.3g.qq.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("handleLogin") != -1 ))	
	{
		return kLoginPT3GWM;
	}
	if( (http_session->m_Requestline.m_Host.Find("qq.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("/analytics/upload") != -1 ))	
	{
		return kAnalyticsUpload;
	}
	//pc qq 查找人
	if (http_session->m_Requestline.m_Host.Find("cgi.find.qq.com") != -1 && 
		http_session->m_Requestline.m_URI.Find("/qqfind/buddy/search_v3") != -1)  
	{
		return kPcFindPeople;
	}
	//pc qq 查找群
	if (http_session->m_Requestline.m_Host.Find("qun.qq.com") != -1 && 
		http_session->m_Requestline.m_URI.Find("/cgi-bin/group_search/pc_group_search") != -1)
	{
		return kPcFindGroup;
	}
	return 0;
}

int chating::QQ::Process(const char *packet,int action)
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
	if (action == kSendMessageWC)
	{
		pro_name_ = "imsg_oicq";
		return SendMessageWC(packet);
	}
	if (action == kSendGroupMessageWC)
	{
		pro_name_ = "imsg_oicq";
		return SendGroupMessageWC(packet);
	}
	if (action == kLoginQZoneWM)
	{
		pro_name_ = "imsg_oicq";
		return LoginQZoneWM(packet);
	}
	if (action == kLoginPT3GWM)
	{
		pro_name_ = "imsg_oicq";
		return LoginPT3GWM(packet);
	}
	if (action == kAnalyticsUpload)
	{
		return AnalyticsUpload(packet);
	}
	//pc qq 查找人
	if (action == kPcFindPeople)
	{
		pro_name_ = "imsg_oicq";
		return PcFindPeople(packet);
	}
	//pc qq 查找群
	if (action == kPcFindGroup)
	{
		pro_name_ = "imsg_oicq";
		return PcFindGroup(packet);
	}
	return 0;
}

int chating::QQ::LoginC(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');

	user_ = ms_->GetValueFromMapByKey("qq");
	if (user_.empty())
	{
		return 0;
	}

	WriteVirtualIdentity(user_,"上线");
	WriteCaseInfo(user_);
	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"entity= 0\r\n" +
		"state= 上线\r\n";
	WriteIndexFile();
	return 1;
}

int chating::QQ::SendGroupVoiceMessageM(const char *packet)
{
	char *stream = strstr(packet,"#!AMR");
	if (!stream)
	{
		return 0;
	}

	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	
	ms_->SplitStringToMap(uri,'&','=');

	from_ = ms_->GetValueFromMapByKey("uin");
	string group = ms_->GetValueFromMapByKey("groupcode");
	string tmp_file_size = ms_->GetValueFromMapByKey("filesize");

	if (from_.empty() || tmp_file_size.empty())
	{
		return 0;
	}
	int file_size = ms_->ToInt(tmp_file_size);

	if ( (0 < file_size) && (file_size < real_file_max_size_))
	{
		char file_name[128] = {0};
		sprintf(file_name,"qq_%s_voice.amr",from_.c_str());
		string real_file_name = WriteTempRealFile(stream,file_name,file_size);
		output_text_ = 
			"from=" + from_ + "\r\n" +
			"mid=" + group + "\r\n" +
			"state=发送群语音消息(手机)\r\n";
		WriteIndexFile(real_file_name);
	}
	return 1;
}


int chating::QQ::CreateChatingC(const char *packet)
{
	return 1;
}

int chating::QQ::LoginPT3GWM(const char *packet)
{
	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	user_ = ms_->GetValueFromMapByKey("qq");
	string pass;
	string pass1 = ms_->GetValueFromMapByKey("pwd");
	string pass2 = ms_->GetValueFromMapByKey("hexpwd");
	pass2 = ms_->HexstringToAsciistring(pass2);
	if (!pass1.empty())
	{
		pass = pass1;
	}
	else
	{
		if (!pass2.empty())
		{
			pass = pass2;
		}
	}

	if (user_.empty() || pass.empty())
	{
		return 0;
	}
	
	if (user_.find("qq.com") != string::npos)
	{
		return 0;
	}
	
	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"entity= 0\r\n" +
		"stat= 账号信息\r\n";
	WriteIndexFile();
	
	return 1;
}

int chating::QQ::LoginQZoneWM(const char *packet)
{

	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	user_ = ms_->GetValueFromMapByKey("qq");
	string pass = ms_->GetValueFromMapByKey("pwd");
	if (user_.empty() || pass.empty())
	{
		return 0;
	}

	if (user_.find("qq.com") != string::npos)
	{
		return 0;
	}
	
	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"entity= 0\r\n" +
		"stat= 账号信息\r\n";
	WriteIndexFile();

	return 1;
}


int chating::QQ::SendMessageWC(const char *packet)
{
	user_ = GetQQ();
	from_ = user_;
	string src_data(packet);
	string b("\"content\":\"[\\\"");
	string e("\\\",");
	message_ = ms_->GetValueBetweenBeginEnd(src_data,b,e,true);
	if (message_.find("\\u") != string::npos)
	{
		ms_->Replace(message_,"\\\\u","\\u");
		message_ = ms_->UnicodeToGBKPrefix_u(message_);
	}

	if (from_.empty() || message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + user_ + "\r\n" +
		"sender=" + from_ + "\r\n" +
	//	"msg=" + message_ + "\r\n" +
		"entity= 1\r\n" +
		"state=发送消息(Web)\r\n";
	string tmp_file_name = WriteTempRealFile(message_.c_str(),"webqq_group_chat_content.txt",message_.size());
	WriteIndexFile(tmp_file_name);

	return 1;
}

int chating::QQ::SendGroupMessageWC(const char *packet)
{
	user_ = GetQQ();
	from_ = user_;
	string src_data(packet);
	string group = ms_->GetValueBetweenBeginEnd(src_data,"\"group_uin\":",",");

	string b("\"content\":\"[\\\"");
	string e("\\\",");
	message_ = ms_->GetValueBetweenBeginEnd(src_data,b,e,true);
	if (message_.find("\\u") != string::npos)
	{
		ms_->Replace(message_,"\\\\u","\\u");
		message_ = ms_->UnicodeToGBKPrefix_u(message_);
	}

	if (from_.empty() || message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + user_ + "\r\n" +
		"sender=" + from_ + "\r\n" +
		"qqgroupid=" + group + "\r\n" +
	//	"msg=" + message_ + "\r\n" +
		"entity= 1\r\n" +
		"state=发送群消息(Web)\r\n";
	string tmp_file_name = WriteTempRealFile(message_.c_str(),"webqq_group_chat_content.txt",message_.size());
	WriteIndexFile(tmp_file_name);
	return 1;
}

int chating::QQ::AnalyticsUpload(const char *packet)
{
	static list<string>contents;

	const char *zdata = NULL;
	if (http_session_->m_databuffer != NULL)
	{
		zdata = http_session_->m_databuffer;
	}

	if (zdata == NULL)
	{
		zdata = strstr(packet,"\r\n\r\n");
		if (zdata)
		{
			zdata = zdata + strlen("\r\n\r\n");
		}
	}

	if (zdata)
	{
		if (memcmp(zdata, "\x1f\x8b\x08\x00", 4) == 0)
		{
			
			const char *zdata_begin = zdata + 10; //gzip head = 10
			int zlength = 0;
			if (http_session_->m_databuffer != 0)
			{
				zlength = http_session_->m_databufsize - 10;
			}
			else
			{
				zlength = 1024 * 8;
			}
			 

			unsigned long length = zlength * 5;
			unsigned char *data = new unsigned char[length + 1];
			memset(data,0,length + 1);
			
			if (Compress::DecompressGZIP((unsigned char *)zdata_begin,zlength,(unsigned char *)data,&length) == -1)
			{
				delete [] data;
				return 0;
			}
			
			data = data + string((char *)data + 0x30).size() + 0x33;
			
			if ( (data[0] == 0xFF) && (data[1] == 0x80) && (data[2] == 0xFF) && (data[3] == 0x96))
			{
				data += 4;
				int ilen = data[0];
				char* sbegin = (char*)(data + 1);
				char* stail = sbegin + ilen;
				string os = string(sbegin, stail);
				data = (unsigned char*)stail;
				
				if (data[0] != 0xA6) return false;
				
				ilen = data[1];
				sbegin = (char*)(data + 2);
				stail = sbegin + ilen;
				os += string(" ");
				os += string(sbegin, stail);
				data = (unsigned char*)stail;
				
				if (data[0] != 0xB6) return -1;
				
				ilen = data[1];
				sbegin = (char*)(data + 2);
				stail = sbegin + ilen;
				string stmp = string(sbegin, stail);
				data = (unsigned char*)stail;
				
				StringManipulation sm;
				sm.SplitStringToMap(stmp,'&','=');
				
				string qq = sm.GetValueFromMapByKey("A1");
				if (qq == "10000")
				{
					return 0;
				}

				if (qq.size() > 11)
				{
					return 0;
				}
				string imei = sm.GetValueFromMapByKey("A2");
				string imsi = sm.GetValueFromMapByKey("A4");

				if (imsi == "" && imei == "")
				{
					return 0;
				}
				
				string content = imei + "\t" + imsi + "\t" + qq + "\r\n";
				contents.push_back(content);
				if (contents.size() > 99)
				{
					SYSTEMTIME now;
					GetLocalTime(&now);
					CString strtm;
					strtm.Format("%04d%02d%02d%02d%02d%02d", now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond);
					
					CString strfp;
					strfp.Format("v:\\netspy\\statdata\\esq\\tmp%s-%d.txt",strtm, rand());
					FILE *fp = fopen(strfp,"w+b");
					if (fp)
					{
						for (list<string>::iterator iter = contents.begin(); iter != contents.end(); ++iter)
						{
							fwrite((*iter).c_str(),1,(*iter).size(),fp);
						}
						
						fclose(fp);
						CString strfpo = strfp;
						strfp.Replace("tmp20","20");
						::rename(strfpo,strfp);
					}
					contents.clear();
					
				}
				return 1;
			}
		}
	}
	return 0;
}

//pc qq 查找人
int chating::QQ::PcFindPeople(const char *packet)
{
	//提取qq号
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,";","=");
	string qq = ms_->GetValueFromMapByKey("o_cookie",true);
    //要查找的 QQ号/手机号/邮箱/昵称/关键词
	string src_data(packet);
	if (src_data.rfind("num=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("num="));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	string key_word = ms_->GetValueFromMapByKey("keyword",true);
	if (qq.empty())
	{
		return 0;
	}
	//暂时这样写
	string context = "查找人(" + key_word + ")";
	output_text_ = 
		"user=" + qq + "\r\n" + 
		"state=" + context + "\r\n";
	WriteIndexFile();
	return 0;
}
//pc qq 查找群
int chating::QQ::PcFindGroup(const char *packet)
{
	//提取qq号
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,";","=");
	string qq = ms_->GetValueFromMapByKey("o_cookie",true);
    //要查找的 群名称
	string src_data(packet);
	int pos = src_data.rfind("k=");
	if (pos != -1)
	{
		src_data = src_data.substr(pos);
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	string key_word = ms_->GetValueFromMapByKey("k",true);
	if (qq.empty())
	{
		return 0;
	}
	//暂时这样写
	string context = "查找群(" + key_word + ")";
	output_text_ = 
		"user=" + qq + "\r\n" + 
		"state=" + context + "\r\n";
	WriteIndexFile();
	return 0;
}

string chating::QQ::GetQQ()
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