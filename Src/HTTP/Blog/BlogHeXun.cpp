#include "StdAfx.h"
#include "BlogHeXun.h"

blog::HeXun::HeXun(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::HeXun::~HeXun()
{
	
}

int blog::HeXun::IsHeXun(HttpSession *http_session)
{
	//发表
	if( (http_session->m_Requestline.m_Host.Find("post.blog.hexun.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/PostArticleHandler.ashx?blogname") != -1))	
	{
		return kWebNewTopic;
	}
	//评论 1
	if( (http_session->m_Requestline.m_Host.Find("blog.hexun.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/CommentCode_type0.html") != -1))	
	{
		return kWebPostComment;
	}
	//上传文件 1 发表博客时上传的图片--目前取不到 2017-07-05
	if( (http_session->m_Requestline.m_Host.Find("postnew.photo.hexun.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/upload/FlashUpload") != -1))	
	{
		return KWebAttachFile;
	}
	//上传文件 2 相册中上传的图片    --目前取不到 2017-07-05
	if( (http_session->m_Requestline.m_Host.Find(".photo.hexun.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/PhotoAPI/Upload?userToken=") != -1))	
	{
		return KWebAttachFile;
	}
	return 0;
}

int blog::HeXun::Process(const char *packet,int action)
{
	//发表
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//评论
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//上传文件
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	return 0;
}


//发表
int blog::HeXun::WebNewTopic(const char *packet)
{	
	//从cookie 中获得所在城市
	string TOWN;
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
    if (cookie.find("TOWN=") != string::npos)
    {
		int pos = cookie.find("TOWN=");
		int tos = cookie.find(";",pos);
		//所在城市 (标准地址码)
		TOWN = cookie.substr(pos + strlen("TOWN="),tos - pos - strlen("TOWN="));
    }
	//数据部分
	string src_data(packet);
	//替换转义字符 < => &lt,> =>&gt,
	ms_->Replace(src_data,"&lt;","<");
	ms_->Replace(src_data,"&gt;",">");
	ms_->Replace(src_data,"&quot;","\"");
	ms_->Replace(src_data,"&amp;","&");
	ms_->Replace(src_data,"&nbsp;"," ");

	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	
	string blogname,TitleTextbox,ContentSpaw,SourceUrlTextbox,weibo,action;
	//博客ID
	blogname = ms_->GetValueFromMapByKey("blogname",true);
	//标题
	TitleTextbox = ms_->GetValueFromMapByKey("TitleTextbox",true);
	//内容
	ContentSpaw = ms_->GetValueFromMapByKey("ContentSpaw",true);
	//转载地址 (备用信息)
	SourceUrlTextbox = ms_->GetValueFromMapByKey("SourceUrlTextbox",true);

	//微博 同步 1 / 非同步 0 (备用信息)
	weibo = ms_->GetValueFromMapByKey("weibo",true);
	//草稿 1 / 正式 0        (备用信息)
	action = ms_->GetValueFromMapByKey("action",true);

	if (blogname.empty() || TitleTextbox.empty() || ContentSpaw.empty())
	{
		return 0;
	}
	
	output_text_ =
		"webname==" + 	blogname + "\r\n" +	  //博客ID
		"netmark=" + TitleTextbox + "\r\n" +  //标题
		"msg=" + 	ContentSpaw + "\r\n" +	  //信息内容
		"fileurl=" + TOWN + "\r\n" +		  //地址
		"platform=电脑浏览器\r\n"
		"state=发表\r\n";
	WriteIndexFile();
	return 1;
}

//评论
int blog::HeXun::WebPostComment(const char *packet)
{
	//从cookie 中获得所在城市
	string TOWN;
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
    if (cookie.find("TOWN=") != string::npos)
    {
		int pos = cookie.find("TOWN=");
		int tos = cookie.find(";",pos);
		//所在城市 (标准地址码)
		TOWN = cookie.substr(pos + strlen("TOWN"),tos - pos - strlen("TOWN"));
    }
	//数据部分
	string src_data(packet);	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	
	string UserNameTextbox,UserUrlTextbox,CommentTitle,NoHtmlCommentContent;
	//博客ID
	UserNameTextbox = ms_->GetValueFromMapByKey("UserID",true);
	//个人主页(备用)
	UserUrlTextbox = ms_->GetValueFromMapByKey("UserUrl",true);
	//标题
	CommentTitle = ms_->GetValueFromMapByKey("Title",true);
	//内容
	NoHtmlCommentContent = ms_->GetValueFromMapByKey("Content",true);
	
	if (CommentTitle.empty() || NoHtmlCommentContent.empty())
	{
		return 0;
	}
	output_text_ =
		"webname=" + 	UserNameTextbox + "\r\n" +	//博客ID
		"netmark=" + CommentTitle + "\r\n" +		//标题
		"msg=" + 	NoHtmlCommentContent + "\r\n" +	//信息内容	
		"fileurl=" + TOWN + "\r\n" +				//地址
		"platform=电脑浏览器\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

//上传文件
int blog::HeXun::WebAttachFile(const char *packet)
{
	//目前获取不到
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, "&", "=");
	user_ = ms_->GetValueFromMapByKey("email", true);
	if (user_.empty())
	{
		user_ = "null";
	}
	
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"Filedata\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	content_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,content_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}

