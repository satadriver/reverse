#include "StdAfx.h"

#include "TianYa.h"

TianYa::TianYa()
{
	software_ = "ÌìÑÄ";
}

TianYa::~TianYa()
{
	
}

int TianYa::Is(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("bbs.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index_self_result.jsp")!=-1))
	{
		return kAddPostCW;
	}

	if((http_session->m_Requestline.m_Host.Find("wireless.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("v/proxy/forum/createItem")!=-1))
	{
		return kAddPostM;
	}

	if((http_session->m_Requestline.m_Host.Find("blog.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/blogger/post_read.asp")!=-1))
	{
		return kAddCommentCW;
	}

	if((http_session->m_Requestline.m_Host.Find("wireless.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v/forumStand/reply")!=-1))
	{
		return kAddCommentM;
	}

	return 0;
}

int TianYa::Process(const char *packet,int action)
{
	
	if (action == kAddPostCW)
	{
		return AddPostCW(packet);
	}
	if (action == kAddPostM)
	{
		return AddPostM(packet);
	}
	if (action == kAddCommentCW)
	{
		return AddCommentCW(packet);
	}
	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}

	return 0;
}

int TianYa::AddPostCW(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string title = ms_->GetValueFromMapByKey("block_name",true);
	string content = ms_->GetValueFromMapByKey("block_intro",true);
	if ( title.empty() || content.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content);
	output_text_ = 
		"msgtype=" + title + "\r\n" +
		"msg=" + content + "\r\n" +
		"state=·¢Ìû\r\n";
	
	WriteIndexFile();
	return 1;
}

int TianYa::AddPostM(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI);
	string::size_type pos = src_data.find("?");
	if (pos == string::npos)
	{
		return 0;
	}
	src_data = src_data.substr(pos + 1);

	ms_->SplitStringToMap(src_data,'&','=');

	string title = ms_->GetValueFromMapByKey("itemName",true);
	string content = ms_->GetValueFromMapByKey("desc",true);


	if (title.empty() || content.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content);

	output_text_ = 
		"msgtype=" + title + "\r\n" +
		"msg=" + content + "\r\n" +
		"state=·¢Ìû\r\n";
	
	WriteIndexFile();
	return 1;
}

int TianYa::AddCommentCW(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string content = ms_->GetValueFromMapByKey("comment",true);
	if ( content.empty())
	{
		return 0;
	}

	WriteCaseInfo(content);

	output_text_ = 
		"msg=" + content + "\r\n" +
		"state=¸úÌû\r\n";
	
	WriteIndexFile();
	return 1;
}

int TianYa::AddCommentM(const char *packet)
{
	string src_data(packet);

	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("content",true);

	if ( content.empty() )
	{
		return 0;
	}
	
	WriteCaseInfo(content);

	output_text_ =	
		"msg=" + content + "\r\n" +
		"state=¸úÌû\r\n";
	
	WriteIndexFile();
	return 1;
}
