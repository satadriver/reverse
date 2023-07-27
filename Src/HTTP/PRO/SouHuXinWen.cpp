
#include "StdAfx.h"

#include "SouHuXinWen.h"

SouHuXinWen::SouHuXinWen()
{
	software_ = "搜狐新闻";
	pro_ = "phoneread_newssh";
}

SouHuXinWen::~SouHuXinWen()
{
	
}

int SouHuXinWen::Is(HttpSession *http_session)
{
	/*
	if( ( http_session->m_Requestline.m_Host.Find("changyan.sohu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/2/comment/submit") != -1 ) )	
	{
		return kAddCommentC;
	}

	if( ( http_session->m_Requestline.m_Host.Find("api.k.sohu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/share/v4/upload.go?") != -1 ) )	
	{
		return kAddCommentM;
	}
	*/
	return 0;
}

int SouHuXinWen::Process(const char *packet,int action)
{
	if (action == kAddCommentC)
	{
		return AddCommentC(packet);
	}

	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}
	return 0;
}


int SouHuXinWen::AddCommentC(const char *packet)
{		
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	
	string tmp = ms_->GetValueFromMapByKey("pp_login_time");

	vector<string>item_list;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = tmp.find("|",item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = tmp.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = tmp.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + strlen("|");
		}
		item_list.push_back(item);
	}

	string user;
	if (item_list.size() > 2)
	{
		user = item_list.at(1);
	}
	
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string content = ms_->GetValueFromMapByKey("content",true);
	string url = http_session_->m_Requestline.m_Referer.GetBuffer(0);

	if (content.empty() || url.empty())
	{
		return 0;
	}

	output_text_ = 
		"user=" + user + "\r\n" +
		"msg=" + content + "\r\n" +
		"oid=" + url + "\r\n" +
		"platform=电脑\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

int SouHuXinWen::AddCommentM(const char *packet)
{		
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;
	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	string user = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"mainPassport\"\r\nContent-Type: text/plain; charset=US-ASCII\r\nContent-Transfer-Encoding: 8bit\r\n\r\n",boundary);

	string url = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"shareUrl\"\r\nContent-Type: text/plain; charset=US-ASCII\r\nContent-Transfer-Encoding: 8bit\r\n\r\n",boundary);

	string content = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"content\"\r\nContent-Type: text/plain; charset=US-ASCII\r\nContent-Transfer-Encoding: 8bit\r\n\r\n",boundary);


	ms_->Replace(content,"{","");
	ms_->Replace(content,"}","");
	ms_->SplitStringToMap(content,',',':',"\"");

	content = ms_->GetValueFromMapByKey("ugc",true);


	if (user.empty() || content.empty())
	{
		return 0;
	}

	output_text_ = 
		"user=" + user + "\r\n" +
		"msg=" + content + "\r\n" +
		"oid=" + url + "\r\n" +
		"platform=手机\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

