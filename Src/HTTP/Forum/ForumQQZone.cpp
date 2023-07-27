#include "StdAfx.h"

#include "ForumQQZone.h"
#include "../BoundaryParser.h"
map<string,string>forum::QQZone::map_ = map<string,string>();

forum::QQZone::QQZone(HttpSession *http_session, const string &pro_name) : Forum(http_session, pro_name)
{
	
}

forum::QQZone::~QQZone()
{
	
}

int forum::QQZone::IsQQZone(HttpSession *http_session)
{
	//发表说说/评论
	if( (http_session->m_Requestline.m_Host.Find("zone.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/proxy/domain/taotao.qzone.qq.com")!=-1))
	{
		return kAddPostCW;
	}
	//说说图片
	if((http_session->m_Requestline.m_Host.Find(".photo.qzone.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cgi-bin/upload/cgi_upload_image")!=-1))
	{
		return kAddCommentCW;
	}
	//日志
	if((http_session->m_Requestline.m_Host.Find("qzone.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("cgi-bin/blognew/")!=-1))
	{
		return kAddPostM;
	}
	//私密日志
	if((http_session->m_Requestline.m_Host.Find("qzone.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("qzone.qq.com/cgi-bin/privateblog/privateblog")!=-1))
	{
		return kAddCommentM;
	}
	//留言
	if((http_session->m_Requestline.m_Host.Find("qzone.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/proxy/domain/m.qzone.qq.com/cgi-bin/new")!=-1))
	{
		return kAddMW;
	}

	return 0;
}

int forum::QQZone::Process(const char *packet,int action)
{
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
//电脑浏览器发表说说、评论说说
int forum::QQZone::AddPostCW(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

    string id=ms_->GetValueFromMapByKey("topicId",true);

	if (id.empty())
	{
		string username = ms_->GetValueFromMapByKey("hostuin",true);
		content_ = ms_->GetValueFromMapByKey("con",true);
		
		if (username.empty() || content_.empty())
		{
			return 0;
		}
		WriteCaseInfo(content_);
		output_text_ = 
			"servername=" + username + "\r\n" + 
			"msg=" + content_ + "\r\n" +
			"platform=电脑浏览器\r\n" +
		"state=发说说\r\n";
	}
	else
	{
		string username= ms_->GetValueFromMapByKey("hostUin",true);
		content_ = ms_->GetValueFromMapByKey("content",true);
		user_ = ms_->GetValueFromMapByKey("uin",true);
		string commentid = ms_->GetValueFromMapByKey("commentUin",true);
		if (commentid.empty())
		{
			if (username.empty() || content_.empty() || user_.empty())
			{
				return 0;
			}
			WriteIndexFile(content_);
			output_text_ = 
				"servername=" + username + "\r\n" +
				"appname=" + user_ + "\r\n" +
				"msg=" + content_ + "\r\n" +
				"platform=电脑浏览器\r\n" +
			"state=评论说说\r\n";
		}
		else
		{
			WriteIndexFile(content_);
			output_text_ = 
				"servername=" + username + "\r\n" +
				"appname=" + user_ + "\r\n" +
				//"webname=" + commentid + "\r\n" + 
				"msg=" + content_ + "\r\n" +
				"platform=电脑浏览器\r\n" +
			"state=评论说说\r\n";
		}		
	}
	WriteIndexFile();
	return 1;
}

//发表说说图片
int forum::QQZone::AddCommentCW(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	boundary = "\r\n--" + boundary;	 
	user_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"uin\"\r\n\r\n",boundary,true);
	string photoname=ms_->GetValueBetweenBeginEnd(src_data,"name=\"Filename\"\r\n\r\n",boundary,true);
	if (photoname.empty() || user_.empty())
	{
		return 0;
	}

	output_text_ = 
		"servername=" + user_ + "\r\n" + 
		"msg=" + photoname + "\r\n" +
		"platform=说说图片\r\n" +
		"state=发帖\r\n";	
	WriteIndexFile();
	return 1;
}
//发表日志
int forum::QQZone::AddPostM(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->Replace(src_data,"&nbsp;","");
	ms_->SplitStringToMap(src_data,'&','=');
	string id=ms_->GetValueFromMapByKey("topicId",true);	
	if (id.empty())
	{
			string username = ms_->GetValueFromMapByKey("uin",true);
			title_ =ms_->GetValueFromMapByKey("title");
			content_ = ms_->GetValueFromMapByKey("html",true);
			
			if (username.empty() || content_.empty() || title_.empty())
			{
				return 0;
			}
			WriteCaseInfo(content_);
			
			output_text_ = 
				"servername=" + username + "\r\n" + 
				"msgtype=" + title_ + "\r\n" + 
				"msg=" + content_ + "\r\n" +
				"platform=电脑浏览器\r\n" +
			"state=发日志\r\n";
	}
	else
	{
		string username= ms_->GetValueFromMapByKey("hostUin",true);
		content_ = ms_->GetValueFromMapByKey("content",true);
		user_ = ms_->GetValueFromMapByKey("uin",true);
		string commentid = ms_->GetValueFromMapByKey("commentUin",true);
		if (commentid.empty())
		{
			if (username.empty() || content_.empty() || user_.empty())
			{
				return 0;
			}
			WriteIndexFile(content_);
			output_text_ = 
				"servername=" + username + "\r\n" +
				"appname=" + user_ + "\r\n" +
				"msg=" + content_ + "\r\n" +
				"platform=电脑浏览器\r\n" +
				"state=评论日志\r\n";
		}
		else
		{
			WriteIndexFile(content_);
			output_text_ = 
				"servername=" + username + "\r\n" +
				"appname=" + user_ + "\r\n" +
				//"webname=" + commentid + "\r\n" + 
				"msg=" + content_ + "\r\n" +
				"platform=电脑浏览器\r\n" +
				"state=评论日志\r\n";
		}	
	}
	
	WriteIndexFile();
	return 1;
}
//私密日志
int forum::QQZone::AddCommentM(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("uin",true);
	title_ = ms_->GetValueFromMapByKey("title",true);
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"&nbsp","</div>");

	if (user_.empty() || title_.empty() ||content_.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content_);
	output_text_ = 
		"servername=" + user_ + "\r\n" + 
		"msgtype=" + title_ + "\r\n" + 
		"msg=" + content_ + "\r\n" +
		"platform=电脑浏览器\r\n" +
		"state=私密日志\r\n";	
	WriteIndexFile();
	return 1;
}
//电脑浏览器留言
int forum::QQZone::AddMW(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("hostUin",true);
	string usernaem = ms_->GetValueFromMapByKey("uin",true);
	content_ =ms_->GetValueFromMapByKey("content",true);
	if (user_.empty() || usernaem.empty() || content_.empty())
	{
		return 0;
	}
	WriteCaseInfo(content_);
	output_text_ = 
		"netmark=" + user_ + "\r\n" + 
		"sex=" + usernaem + "\r\n" + 
		"msg=" + content_ + "\r\n" +
		"platform=电脑浏览器\r\n" +
		"state=空间留言\r\n";	
	WriteIndexFile();
	return 1;

}

