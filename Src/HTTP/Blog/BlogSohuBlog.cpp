#include "StdAfx.h"
#include "BlogSohuBlog.h"

blog::SohuBlog::SohuBlog(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::SohuBlog::~SohuBlog()
{
	
}

int blog::SohuBlog::IsSohuBlog(HttpSession *http_session)
{
	//发表博文
	if( (http_session->m_Requestline.m_Host.Find("blog.sohu.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/manage/entry.do") != -1))	
	{
		return kWebNewTopic;
	}
	//评论 回复
	if( (http_session->m_Requestline.m_Host.Find(".blog.sohu.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/a/app/discuss/save.htm?_input_encode=UTF-8") != -1))	
	{
		return kWebPostComment;	
	}
	//上传文件 博文
	if( (http_session->m_Requestline.m_Host.Find("upload.i.sohu.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/a/blog/upload") != -1))	
	{
		return KWebAttachFile;
	}
	return 0;
}

int blog::SohuBlog::Process(const char *packet,int action)
{
	//发表博文
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//评论 回复
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//上传文件 日志
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	return 0;
}

//发表日志
int blog::SohuBlog::WebNewTopic(const char *packet)
{	
	//从cookie 中得到 通行证号 
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//通行证号
	string pp_login_time = ms_->GetValueFromMapByKey("pp_login_time");     
	int pos = pp_login_time.find("|",0);
	if (pos != string::npos)
	{
		int tos = pp_login_time.find("|",pos + 1);
		user_ = pp_login_time.substr(pos + strlen("|"),tos - pos - strlen("|"));
	}
	//数据部分
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//标题
	title_ = ms_->GetValueFromMapByKey("entrytitle",true);
	//内容
	content_ = ms_->GetValueFromMapByKey("entrycontent",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" + //通行证号
		"netmark=" + title_ + "\r\n" + //标题
		"msg=" + 	content_ + "\r\n" + //信息内容
		"platform=电脑浏览器\r\n"
		"state=发表\r\n";
	WriteIndexFile();
	return 1;
}

//评论 / 回复
int blog::SohuBlog::WebPostComment(const char *packet)
{
	//从cookie 中得到 通行证号 
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//通行证号
	string pp_login_time = ms_->GetValueFromMapByKey("pp_login_time");     
	int pos = pp_login_time.find("|",0);
	if (pos != string::npos)
	{
		int tos = pp_login_time.find("|",pos + 1);
		user_ = pp_login_time.substr(pos + strlen("|"),tos - pos - strlen("|"));
	}
	//数据部分
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	content_ = ms_->GetValueFromMapByKey("content",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" +	//通行证号
		"msg=" + 	content_ + "\r\n" + //信息内容
		"platform=电脑浏览器\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

//上传文件
int blog::SohuBlog::WebAttachFile(const char *packet)
{
	//从cookie 中得到 通行证号 
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//通行证号
	string pp_login_time = ms_->GetValueFromMapByKey("pp_login_time");     
	int pos = pp_login_time.find("|",0);
	if (pos != string::npos)
	{
		int tos = pp_login_time.find("|",pos + 1);
		user_ = pp_login_time.substr(pos + strlen("|"),tos - pos - strlen("|"));
	}
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"file\"");          
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

 