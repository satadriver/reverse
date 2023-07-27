#include "StdAfx.h"

#include "ForumBaiduTieBa.h"

map<string,string>forum::BaiDuTieBa::map_ = map<string,string>();

forum::BaiDuTieBa::BaiDuTieBa(HttpSession *http_session, const string &pro_name) : Forum(http_session, pro_name)
{
	
}

forum::BaiDuTieBa::~BaiDuTieBa()
{
	
}

int forum::BaiDuTieBa::IsBaiDuTieBa(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/tb/img/track.gif")!=-1))
	{
		return kLoginW;
	}
	
	if( (http_session->m_Requestline.m_Host.Find("tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/f/commit/thread/add")!=-1))
	{
		return kAddPostCW;
	}
	if((http_session->m_Requestline.m_Host.Find("tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/f/commit/post/add")!=-1))
	{
		return kAddCommentCW;
	}

	if((http_session->m_Requestline.m_Host.Find("c.tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/c/c/thread/add")!=-1))
	{
		return kAddPostM;
	}

	if((http_session->m_Requestline.m_Host.Find("c.tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/c/c/post/add")!=-1))
	{
		return kAddCommentM;
	}

	if((http_session->m_Requestline.m_Host.Find("tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mo/q/apubpost")!=-1))
	{
		return kAddMW;
	}

	return 0;
}

int forum::BaiDuTieBa::Process(const char *packet,int action)
{
	if (action == kLoginW)
	{
		return LoginW(packet);
	}

	if (action == kAddPostCW)
	{
		return AddPostCW(packet);
	}

	if (action == kAddCommentCW)
	{
		return AddCommentCW(packet);
	}

	if (action == kAddPostM)
	{
		return AddPostM(packet);
	}

	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}

	if (action == kAddMW)
	{
		return AddMW(packet);
	}
	return 0;
}
//µÇÂ¼
int forum::BaiDuTieBa::LoginW(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	
	string username = ms_->GetValueFromMapByKey("uname",true);
	
	string bduss = GetBDUSS();
	if (!bduss.empty() && !username.empty())
	{
		map<string,string>::iterator iter = map_.find(bduss);
		if (iter == map_.end())
		{
			map_.insert(std::make_pair(bduss,username));
			WriteBDUSS(bduss,username);
		}	
	}	
	WriteVirtualIdentity(username,"µÇÂ¼");
	return 0;
}

//µçÄÔä¯ÀÀÆ÷·¢Ìû
int forum::BaiDuTieBa::AddPostCW(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	bar_name_ = ms_->GetValueFromMapByKey("kw",true);
	title_ = ms_->GetValueFromMapByKey("title",true);
	content_ = ms_->GetValueFromMapByKey("content",true);

	if (bar_name_.empty() || title_.empty() || content_.empty())
	{
		return 0;
	}	
	string username = GetUserName(GetBDUSS());	
	WriteCaseInfo(content_);	
	output_text_ = 
		"user=" + username + "\r\n" + 
		"webname=" + bar_name_ + "\r\n" + 
		"msgtype=" + title_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=µçÄÔä¯ÀÀÆ÷\r\n" +
		"state=·¢Ìû\r\n";
	WriteIndexFile();
	return 1;
}

//µçÄÔä¯ÀÀÆ÷¸úÌû
int forum::BaiDuTieBa::AddCommentCW(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	bar_name_ = ms_->GetValueFromMapByKey("kw",true);
	string post_id = ms_->GetValueFromMapByKey("tid",true);
	content_ = ms_->GetValueFromMapByKey("content",true);

	if (bar_name_.empty() || content_.empty() || post_id.empty())
	{
		return 0;
	}

	post_id = "http://tieba.baidu.com/p/" + post_id;

	string username = GetUserName(GetBDUSS());
	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" + 
		"webname=" + bar_name_ + "\r\n" + 
		"oid=" + post_id + "\r\n" + 
		"msg=" + content_ + "\r\n" +
		"platform=µçÄÔä¯ÀÀÆ÷\r\n" +
		"state=¸úÌû\r\n";
	
	WriteIndexFile();
	return 1;
}
//ÊÖ»úapp·¢Ìû
int forum::BaiDuTieBa::AddPostM(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string bduss = ms_->GetValueFromMapByKey("BDUSS");
	bar_name_ = ms_->GetValueFromMapByKey("kw",true);
	title_ = ms_->GetValueFromMapByKey("title",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	string imei = ms_->GetValueFromMapByKey("_phone_imei");
	
	if (bar_name_.empty() || title_.empty() || content_.empty() || imei.empty())
	{
		return 0;
	}	
	string username = GetUserName(bduss);
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" + 
		"imei=" + imei + "\r\n" + 
		"webname=" + bar_name_ + "\r\n" + 
		"msgtype=" + title_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=ÊÖ»úApp\r\n" +
		"state=·¢Ìû\r\n";	
	WriteIndexFile();
	return 1;
}
//ÊÖ»úapp¸úÌû
int forum::BaiDuTieBa::AddCommentM(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');	
	string bduss = ms_->GetValueFromMapByKey("BDUSS");
	bar_name_ = ms_->GetValueFromMapByKey("kw",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	string post_id = ms_->GetValueFromMapByKey("tid");
	string imei = ms_->GetValueFromMapByKey("_phone_imei");

	if (bar_name_.empty() || content_.empty() || imei.empty() || post_id.empty())
	{
		return 0;
	}	
	string username = GetUserName(bduss);
	post_id = "http://tieba.baidu.com/p/" + post_id;
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" + 
		"imei=" + imei + "\r\n" + 
		"webname=" + bar_name_ + "\r\n" + 
		"oid=" + post_id + "\r\n" + 
		"msg=" + content_ + "\r\n" +
		"platform=ÊÖ»úApp\r\n" +
		"state=¸úÌû\r\n";	
	WriteIndexFile();
	return 1;
}
//ÊÖ»úä¯ÀÀÆ÷·¢Ìû / ¸úÌù
int forum::BaiDuTieBa::AddMW(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	bar_name_ = ms_->GetValueFromMapByKey("word",true);
	title_ = ms_->GetValueFromMapByKey("ti",true);
	content_ = ms_->GetValueFromMapByKey("co",true);
	string post_id = ms_->GetValueFromMapByKey("z");
	string username = GetUserName(GetBDUSS());

	if (bar_name_.empty() || content_.empty())
	{
		return 0;
	}
	if (!title_.empty() && post_id.empty())
	{
		output_text_ = 
			"user=" + username + "\r\n" + 
			"webname=" + bar_name_ + "\r\n" + 
			"msgtype=" + title_ + "\r\n" +
			"msg=" + content_ + "\r\n" +
			"platform=ÊÖ»úä¯ÀÀÆ÷\r\n" +
			"state=·¢Ìû\r\n";
		WriteIndexFile();
		return 1;
	}
	if (title_.empty() && !post_id.empty())
	{
		post_id = "http://tieba.baidu.com/p/" + post_id;
		output_text_ = 
			"user=" + username + "\r\n" + 
			"webname=" + bar_name_ + "\r\n" + 
			"oid=" + post_id + "\r\n" + 
			"msg=" + content_ + "\r\n" +
			"platform=ÊÖ»úä¯ÀÀÆ÷\r\n" +
			"state=¸úÌû\r\n";
		WriteIndexFile();
		return 1;
	}
	return 0;	
}

//ÊÖ»úä¯ÀÀÆ÷
string forum::BaiDuTieBa::GetBDUSS()
{
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	return  ms_->GetValueFromMapByKey("BDUSS");	
}

string forum::BaiDuTieBa::GetUserName(const string &bduss)
{
	map<string,string>::iterator iter = map_.find(bduss);
	if (iter != map_.end())
	{
		return iter->second;
	}
	return "";
}

void forum::BaiDuTieBa::ReadBDUSS()
{
	const char *path = "e:\\netspy\\para\\bduss.txt";
	FILE *fp = fopen(path,"r");
	if (fp)
	{
		while ( !feof(fp))
		{
			char tmp[512] = {0};
			fgets(tmp,512,fp);
			string raw(tmp);
			string::size_type pos = raw.find(":");
			if (pos != string::npos)
			{
				string bduss = raw.substr(0,pos);
				string username = raw.substr(pos + 1);
				string::size_type posrn = username.find("\n");
				if (posrn != string::npos)
				{
					username = username.substr(0,posrn);
				}
				map_.insert(std::make_pair(bduss,username));
			}
		}		
		fclose(fp);
	}
}

void forum::BaiDuTieBa::WriteBDUSS(const string &bduss, const string &username)
{
	const char *path = "e:\\netspy\\para\\bduss.txt";
	FILE *fp = fopen(path,"a+");
	if (fp)
	{
		string tmp = bduss + ":" + username + "\n";
		fwrite(tmp.c_str(),1,tmp.size(),fp);
		fclose(fp);
	}
}