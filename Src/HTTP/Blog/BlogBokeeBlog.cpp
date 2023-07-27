#include "StdAfx.h"
#include "BlogBokeeBlog.h"

blog::BokeeBlog::BokeeBlog(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::BokeeBlog::~BokeeBlog()
{
	
}

int blog::BokeeBlog::IsBokeeBlog(HttpSession *http_session)
{
	//登录
	if( (http_session->m_Requestline.m_Host.Find("sso.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/method/loginAjax?") != -1))	
	{
		return kWebLogin;
	}
	//发表 博文
	if( (http_session->m_Requestline.m_Host.Find("user.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/user.php/blog/add/") != -1))	
	{
		return kWebNewTopic;
	}
	//发表画报
	if( (http_session->m_Requestline.m_Host.Find("tuyang.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/method/admin/saveArticle") != -1))	
	{
		return kWebNewPaper;
	}
	//评论 
	if( (http_session->m_Requestline.m_Host.Find("home.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/home.php/comment/doadd") != -1))	
	{
		return kWebPostComment;
	}
	//评论 2
	if( (http_session->m_Requestline.m_Host.Find("tuyang.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/method/visitor/addArtComment/") != -1))	
	{
		return kWebPostCommentTuyang;
	}
	//上传文件 博客
	if( (http_session->m_Requestline.m_Host.Find("upload.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/newbokee_user/imageUp.php") != -1))	
	{
		return KWebAttachFile;
	}
	//上传文件 图样
	if( (http_session->m_Requestline.m_Host.Find("tuyang.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/method/upload") != -1))	
	{
		return KWebAttachFileTuyang;
	}
	return 0;
}

int blog::BokeeBlog::Process(const char *packet,int action)
{
	//登录
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//发表 博文
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//发表 画报
	if (action == kWebNewPaper)
	{
		return WebNewPaper(packet);
	}
	//评论
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//评论
	if (action == kWebPostCommentTuyang)
	{
		return WebPostCommentTuyang(packet);
	}
	//上传文件 博客
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	//上传文件 图样
	if (action == KWebAttachFileTuyang)
	{
		return WebAttachFileTuyang(packet);
	}
	return 0;
}

//登录
int blog::BokeeBlog::WebLogin(const char *packet)
{
	//从uri 中获取数据
	StringManipulation sm;	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	sm.Replace(uri, "?", "&");
	sm.SplitStringToMap(uri, '&', '=');
	//通行证号
	string username = sm.GetValueFromMapByKey("username");
	//密码
	string password = sm.GetValueFromMapByKey("password");
	//IP地址
	string ipAddress = sm.GetValueFromMapByKey("ip");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(username,"上线");
	output_text_ =
		"mid=" + 	username + "\r\n" +		//通行证号
		"pass=" + 	password + "\r\n" +		//密码
		"imsi=" + 	ipAddress + "\r\n" +    //IP地址
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}
 
//发表 博文
int blog::BokeeBlog::WebNewTopic(const char *packet)
{	
	//从cookie 中获取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//解码需要的变量
	int n = 0;
    //通行证号
	string username = ms_->GetValueFromMapByKey("sso_username",true);
	n = username.length();
	username = ms_->Base64Decode(username,n);

	//昵称
	string nickname = ms_->GetValueFromMapByKey("sso_nickname",true);
	nickname = ms_->Base64Decode(nickname,n);
	nickname= ms_->UTF8ToGBK(UrlDecode(nickname));
	//博客ID 
	string blogid = ms_->GetValueFromMapByKey("sso_uid",true);
	blogid = ms_->GetValueBetweenBeginEnd(blogid,"\"","\"",true);
	blogid = ms_->Base64Decode(blogid,n);
	//数据部分
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string src_dataEnd = src_data.substr(src_data.find("subject="));
	ms_->SplitStringToMap(src_dataEnd,'&','=');
	//标题
	nick_ = ms_->GetValueFromMapByKey("subject",true);
	//内容
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //通行证号
		"nick=" + 	nickname + "\r\n" + //昵称
		"webname=" + blogid + "\r\n" + //博客ID
		"netmark=" + nick_ + "\r\n" +  //标题
		"msg=" + 	content_ + "\r\n" + //信息内容
		"platform=电脑浏览器\r\n"
		"state=发表博文\r\n";
	WriteIndexFile();
	return 1;
}


//发表 画报
int blog::BokeeBlog::WebNewPaper(const char *packet)
{	
	//从cookie 中获取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//解码需要的变量
	int n = 0;
    //通行证号
	string username = ms_->GetValueFromMapByKey("sso_username",true);
	username = ms_->Base64Decode(username,n);
	//昵称
	string nickname = ms_->GetValueFromMapByKey("sso_nickname",true);
	nickname = ms_->Base64Decode(nickname,n);
	nickname= ms_->UTF8ToGBK(UrlDecode(nickname));
	//博客ID 
	string blogid = ms_->GetValueFromMapByKey("sso_uid",true);
	blogid = ms_->GetValueBetweenBeginEnd(blogid,"\"","\"",true);
	blogid = ms_->Base64Decode(blogid,n);
	//数据部分
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//标题
	nick_ = ms_->GetValueBetweenBeginEnd(src_data,"title=","&sr",true); 
	//内容
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"contentStr=","&tag",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //通行证号
		"nick=" + 	nickname + "\r\n" + //昵称
		"webname=" + blogid + "\r\n" + //博客ID
		"netmark=" + nick_ + "\r\n" +  //标题
		"msg=" + 	content_ + "\r\n" + //信息内容
		"platform=电脑浏览器\r\n"
		"state=发表画报\r\n";
	WriteIndexFile();
	return 1;
}

//评论
int blog::BokeeBlog::WebPostComment(const char *packet)
{
	//从cookie 中获取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//解码需要的变量
	int n = 0;
    //通行证号
	string username = ms_->GetValueFromMapByKey("sso_username",true);
	username = ms_->Base64Decode(username,n);
	//昵称
	string nickname = ms_->GetValueFromMapByKey("sso_nickname",true);
	nickname = ms_->Base64Decode(nickname,n);
	nickname= ms_->UTF8ToGBK(UrlDecode(nickname));
	//博客ID 
	string blogid = ms_->GetValueFromMapByKey("sso_uid",true);
	blogid = ms_->GetValueBetweenBeginEnd(blogid,"\"","\"",true);
	blogid = ms_->Base64Decode(blogid,n);
	//从uri 中获取数据
	StringManipulation sm;	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);	
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	//目标博客昵称
	nick_ = ms_->GetValueBetweenBeginEnd(uri,"uname=","&",true);
	//信息内容
	content_ = ms_->GetValueBetweenBeginEnd(uri,"content=","&",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //通行证号
		"nick=" + 	nickname + "\r\n" + //昵称
		"webname=" + blogid + "\r\n" + //博客ID
		"clientmsg=" + nick_ + "\r\n" + //目标博客昵称
		"msg=" + 	content_ + "\r\n" + //信息内容
		"platform=电脑浏览器\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}


//评论
int blog::BokeeBlog::WebPostCommentTuyang(const char *packet)
{
	//从uri 中获取数据
	StringManipulation sm;	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);	
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->SplitStringToMap(uri,'&','=');

	//通行账号
	string username = ms_->GetValueFromMapByKey("userName",true);
	//博客id
	string blogid = ms_->GetValueFromMapByKey("userId");
	//昵称
	string nickname = ms_->GetValueFromMapByKey("commentator",true);
	//信息内容
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //通行证号
		"nick=" + 	nickname + "\r\n" + //昵称
		"webname=" + blogid + "\r\n" + //博客ID
		"msg=" + 	content_ + "\r\n" + //信息内容
		"platform=电脑浏览器\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}
//上传文件 博客
int blog::BokeeBlog::WebAttachFile(const char *packet)
{
	//从cookie 中获取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//解码需要的变量
	int n = 0;
    //通行证号
	string username = ms_->GetValueFromMapByKey("sso_username",true);
	user_ = ms_->Base64Decode(username,n);
	
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"upfile\"");          
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


//上传文件 图样
int blog::BokeeBlog::WebAttachFileTuyang(const char *packet)
{
	//从cookie 中获取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//解码需要的变量
	int n = 0;
    //通行证号
	string username = ms_->GetValueFromMapByKey("sso_username",true);
	user_ = ms_->Base64Decode(username,n);
	
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