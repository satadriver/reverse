
#include "StdAfx.h"

#include "TengXunWeiBo.h"

TengXunWeiBo::TengXunWeiBo()
{
	software_ = "腾讯微博";
	pro_ = "wb_qq";
}

TengXunWeiBo::~TengXunWeiBo()
{
	
}

int TengXunWeiBo::Is(HttpSession *http_session)
{
	if( (http_session->m_Requestline.m_Host.Find("api.t.qq.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/old/publish.php") != -1))	
	{
		return kAddWeiBo;
	}

	return 0;
}

int TengXunWeiBo::Process(const char *packet,int action)
{
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie,';','=');
	
	string user = ms_->GetValueFromMapByKey("ptui_loginuin",true);

	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	string content = ms_->GetValueFromMapByKey("content",true);

	if (user.empty() || content.empty())
	{
		return 0;
	}

	string weibo_id = ms_->GetValueFromMapByKey("pId");
	if (weibo_id == "")
	{
		output_text_ = 
			"user=" + user + "\r\n" +  
			"mid=" + user + "\r\n" + 
			"msg=" + content + "\r\n" +
			"msgtype=发表\r\n";
		WriteIndexFile();
		return 1;
		
	}
	else
	{
		string type = ms_->GetValueFromMapByKey("type");
		if (type == "1")
		{
			output_text_ = 
				"user=" + user + "\r\n" + 
				"oid=" + weibo_id + "\r\n"+
				"mid=" + user + "\r\n" + 
				"msg=" + content + "\r\n" +
				"msgtype=评论\r\n";
			WriteIndexFile();
			return 1;
		}
		if (type == "5")
		{
			output_text_ = 
				"user=" + user + "\r\n" + 
				"oid=" + weibo_id + "\r\n"+
				"mid=" + user + "\r\n" + 
				"msg=" + content + "\r\n" +
				"msgtype=转载\r\n";
			WriteIndexFile();
			return 1;
		}
	}
	return 0;
}


