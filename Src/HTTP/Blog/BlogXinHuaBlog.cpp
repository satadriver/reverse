#include "StdAfx.h"
#include "BlogXinHuaBlog.h"

blog::XinHuaBlog::XinHuaBlog(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::XinHuaBlog::~XinHuaBlog()
{
	
}

int blog::XinHuaBlog::IsXinHuaBlog(HttpSession *http_session)
{
	//登录 1 标准登录
	if( (http_session->m_Requestline.m_Host.Find("login.home.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/ilogin.do?") != -1))	
	{
		return kWebLogin;
	}
	//登录 2 页面登录
	if( (http_session->m_Requestline.m_Host.Find("my.home.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/blog/ajaxLogin.do") != -1))	
	{
		return kWebLoginSecond;
	}
	//发表
	if( (http_session->m_Requestline.m_Host.Find("my.xuan.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/blogNew/articleAddSave.do") != -1))	
	{
		return kWebNewTopic;
	}
    //发表 2	
	if( (http_session->m_Requestline.m_Host.Find("my.home.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/blog/control/articlePost.do") != -1))	
	{
		return kWebNewTopic;
	}
	//评论 1
	if( (http_session->m_Requestline.m_Host.Find(".home.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/blog/commentAdd.do") != -1))	
	{
		return kWebPostComment;
	}
	//评论 2
	if( (http_session->m_Requestline.m_Host.Find(".home.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/portal/blog/addDiscuss/") != -1))	
	{
		return kWebPostComment;
	}
	//上传文件 博客
	if( (http_session->m_Requestline.m_Host.Find("my.xuan.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/blogNew/articleImgSave.do") != -1))	
	{
		return KWebAttachFile;
	}
	//上传文件 相册
	if( (http_session->m_Requestline.m_Host.Find("my.xuan.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/image/upload.do?") != -1))	
	{
		return KWebAttachFileTuyang;
	}
	return 0;
}

int blog::XinHuaBlog::Process(const char *packet,int action)
{
	//登录 1
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//登录 2
	if (action == kWebLoginSecond)
	{
		return WebLoginSecond(packet);
	}
	//发表 博文
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//评论
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//上传文件 博客
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	//上传文件 相册
	if (action == KWebAttachFileTuyang)
	{
		return WebAttachFileTuyang(packet);
	}
	return 0;
}

//登录 1
int blog::XinHuaBlog::WebLogin(const char *packet)
{
	//数据部分
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string src_dataEnd = src_data.substr(src_data.find("loginID="));
	ms_->SplitStringToMap(src_dataEnd,'&','=');
	//通行证号
	string username = ms_->GetValueFromMapByKey("loginID");
	//密码
	string password = ms_->GetValueFromMapByKey("password");
	//解码需要的变量
	int n = 0;
	//用户名 base64加密
	username = ms_->Base64Decode(username,n);
	password = ms_->Base64Decode(password,n);

	if (username.empty() || password.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(username,"上线");
	output_text_ =
		"mid=" + 	username + "\r\n" +		//通行证号
		"pass=" + 	password + "\r\n" +		//密码
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}
 

//登录 2
int blog::XinHuaBlog::WebLoginSecond(const char *packet)
{
	//数据部分
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string src_dataEnd = src_data.substr(src_data.find("senderName="));
	ms_->SplitStringToMap(src_dataEnd,'&','=');
	//通行证号
	string username = ms_->GetValueFromMapByKey("senderName");
	//密码
	string password = ms_->GetValueFromMapByKey("password");
	
	if (username.empty() || password.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(username,"上线");
	output_text_ =
		"mid=" + 	username + "\r\n" +		//通行证号
		"pass=" + 	password + "\r\n" +		//密码
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}

//发表 博文
int blog::XinHuaBlog::WebNewTopic(const char *packet)
{	
	//从cookie 中获取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//解码需要的变量
	int n = 0;
    //通行证号
	string username = ms_->GetValueFromMapByKey("userName",true);
	username = ms_->Base64Decode(username,n);
	//数据部分
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//标题
	nick_ = ms_->GetValueFromMapByKey("title",true);
	//内容
	content_ = ms_->GetValueFromMapByKey("contentText",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //通行证号
		"netmark=" + nick_ + "\r\n" +  //标题
		"msg=" + 	content_ + "\r\n" + //信息内容
		"platform=电脑浏览器\r\n"
		"state=发表\r\n";
	WriteIndexFile();
	return 1;
}


//评论
int blog::XinHuaBlog::WebPostComment(const char *packet)
{
	//从cookie 中获取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//解码需要的变量
	int n = 0;
    //通行证号
	string username = ms_->GetValueFromMapByKey("userName",true);
	username = ms_->Base64Decode(username,n);
	//数据部分
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//信息内容
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //通行证号
		"msg=" + 	content_ + "\r\n" + //信息内容
		"platform=电脑浏览器\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

//上传文件 博客
int blog::XinHuaBlog::WebAttachFile(const char *packet)
{
	//从cookie 中获取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//解码需要的变量
	int n = 0;
    //通行证号
	string username = ms_->GetValueFromMapByKey("userName",true);
	user_ = ms_->Base64Decode(username,n);
	//uc浏览器 没有
	if (user_.empty())
	{
		user_ = "null";
	}
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

//上传文件 相册
int blog::XinHuaBlog::WebAttachFileTuyang(const char *packet)
{
	//从cookie 中获取数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//解码需要的变量
	int n = 0;
    //通行证号
	string username = ms_->GetValueFromMapByKey("userName",true);
	user_ = ms_->Base64Decode(username,n);
	//uc浏览器 没有
	if (user_.empty())
	{
		user_ = "null";
	}	
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"imageFile\"");          
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