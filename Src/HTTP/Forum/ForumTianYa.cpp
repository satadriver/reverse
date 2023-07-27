#include "StdAfx.h"

#include "ForumTianYa.h"

forum::TianYa::TianYa(HttpSession *http_session, const string &pro_name) : Forum(http_session, pro_name)
{
	
}

forum::TianYa::~TianYa()
{
	
}

int forum::TianYa::IsTianYa(HttpSession *http_session)
{
	//发帖
	if((http_session->m_Requestline.m_Host.Find("bbs.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api?method=bbs.ice.compose")!=-1))
	{
	return kPCWebNewPost;
	}
	//评论
	if((http_session->m_Requestline.m_Host.Find("bbs.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api?method=bbs.ice.compose") == -1) &&
		(http_session->m_Requestline.m_URI.Find("/api?method=bbs.ice.reply") == -1))
	{
		return kPCWebNewComment;
	}
	//回复
	if((http_session->m_Requestline.m_Host.Find("bbs.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api?method=bbs.ice.reply")!=-1))
	{
		return kPCWebNewReply;
	}
	//android发帖
	if((http_session->m_Requestline.m_Host.Find("wireless.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v/forumStand/compose")!=-1))
	{
		return kAndroidNewPost;
	}
	//android评论
	if((http_session->m_Requestline.m_Host.Find("wireless.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v/forumStand/comment")!=-1))
	{
		return kAndroidNewComment;
	}
	//android回复
	if((http_session->m_Requestline.m_Host.Find("wireless.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v/forumStand/reply")!=-1))
	{
		return kAndroidNewReply;
	}
	//上传文件	
	if((http_session->m_Requestline.m_Host.Find("photo.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/photo?act=uploadphoto")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/photo?act=uploadphoto&watermark=0&serverapp=mobile") == -1))
	{
		return KPCWebUploadDocument;
	}
	//android上传文件 
	if((http_session->m_Requestline.m_Host.Find("photo.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/photo?act=uploadphoto&watermark=0&serverapp=mobile")!=-1))
	{
		return KAndroidUploadDocument;
	}
	return 0;
}

int forum::TianYa::Process(const char *packet,int action)
{
	//发帖
	if (action == kPCWebNewPost)
	{
		return PCWebNewPost(packet);
	}
	//评论
	if (action == kPCWebNewComment)
	{
		return PCWebNewComment(packet);
	}
	//回复
	if (action == kPCWebNewReply)
	{
		return PCWebNewReply(packet);
	}
	//上传文件
	if (action == KPCWebUploadDocument)
	{
		return PCWebUploadDocument(packet);	
	}
	//android发帖
	if (action == kAndroidNewPost)
	{
		return AndroidNewPost(packet);
	}
	//android评论
	if (action == kAndroidNewComment)
	{
		return AndroidNewComment(packet);
	}
	//android回复
	if (action == kAndroidNewReply)
	{
		return AndroidNewReply(packet);	
	}
	//android上传文件
	if (action == KAndroidUploadDocument)
	{
		return AndroidUploadDocument(packet);	
	}
	return 0;
}

//发帖
int forum::TianYa::PCWebNewPost(const char *packet)
{
	//从cookie中提取数据  user=w=xiaoxiaolong989&id=122840674&f=1
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);

	string userTemp = ms_->GetValueBetweenBeginEnd(cookie,"user=",";");
	//用户名
	string username = ms_->GetValueBetweenBeginEnd(userTemp,"w=","&");
	//用户ID
	string userid = ms_->GetValueBetweenBeginEnd(userTemp,"id=","&");

	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//模块名称
	string item = ms_->GetValueFromMapByKey("params.item",true);
	//标题
	string title = ms_->GetValueFromMapByKey("params.title",true);
	//信息内容
	string content = ms_->GetValueFromMapByKey("params.content",true);
	if ( title.empty() || content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +	//用户名	
		"userid=" + userid + "\r\n" + 	//用户ID	
		"oid=" + item + "\r\n" +		//模块名称	
		"msgtype=" + title + "\r\n" + 	//标题		
		"msg=" + content + "\r\n" + 	//信息内容
		"state=发帖\r\n";
	WriteIndexFile();
	return 1;
}

//评论
int forum::TianYa::PCWebNewComment(const char *packet)
{
	//从cookie中提取数据  user=w=xiaoxiaolong989&id=122840674&f=1
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);

	string userTemp = ms_->GetValueBetweenBeginEnd(cookie,"user=",";");
	//用户名
	string username = ms_->GetValueBetweenBeginEnd(userTemp,"w=","&");
	//用户ID
	string userid = ms_->GetValueBetweenBeginEnd(userTemp,"id=","&");

	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//模块名称
	string item = ms_->GetValueFromMapByKey("params.item",true);
	//信息内容
	string content = ms_->GetValueFromMapByKey("params.content",true);
	if (content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +	//用户名	
		"userid=" + userid + "\r\n" + 	//用户ID	
		"oid=" + item + "\r\n" +		//模块名称	
		"msg=" + content + "\r\n" + 	//信息内容
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

//回复
int forum::TianYa::PCWebNewReply(const char *packet)
{
	//从cookie中提取数据  user=w=xiaoxiaolong989&id=122840674&f=1
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);

	string userTemp = ms_->GetValueBetweenBeginEnd(cookie,"user=",";");
	//用户名
	string username = ms_->GetValueBetweenBeginEnd(userTemp,"w=","&");
	//用户ID
	string userid = ms_->GetValueBetweenBeginEnd(userTemp,"id=","&");

	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//模块名称
	string item = ms_->GetValueFromMapByKey("params.item",true);
	//文章地址
	string preUrl = ms_->GetValueFromMapByKey("params.preUrl",true);
	//博主名
	string preUserName = ms_->GetValueFromMapByKey("params.preUserName",true);
	//标题
	string title = ms_->GetValueFromMapByKey("params.title",true);
	//信息内容
	string content = ms_->GetValueFromMapByKey("params.content",true);
	if ( title.empty() || content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +		//用户名	
		"userid=" + userid + "\r\n" + 		//用户ID	
		"libname=" + preUserName + "\r\n" + //博主名
		"oid=" + item + "\r\n" +			//模块名称	
		"msgtype=" + title + "\r\n" + 		//标题		
		"msg=" + content + "\r\n" + 		//信息内容
		"nick=" + preUrl + "\r\n" +			//文章地址
		"state=回复\r\n";
	WriteIndexFile();
	return 1;
}


//上传文件
int forum::TianYa::PCWebUploadDocument(const char *packet)
{	
	//从cookie中提取数据  user=w=xiaoxiaolong989&id=122840674&f=1
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);

	string userTemp = ms_->GetValueBetweenBeginEnd(cookie,"user=",";");
	//用户名
	string username = ms_->GetValueBetweenBeginEnd(userTemp,"w=","&");
	//用户ID
	string userid = ms_->GetValueBetweenBeginEnd(userTemp,"id=","&");

	from_ = username;
	
	//文件部分
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
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	 
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}

//android发帖
int forum::TianYa::AndroidNewPost(const char *packet)
{
	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string username = ms_->GetValueFromMapByKey("userName",true);
	//用户ID
	string userid = ms_->GetValueFromMapByKey("userId",true);
	//模块名称
	string item = ms_->GetValueFromMapByKey("categoryId",true);
	//标题
	string title = ms_->GetValueFromMapByKey("title",true);
	//信息内容
	string content = ms_->GetValueFromMapByKey("content",true);
	if (content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +	//用户名	
		"userid=" + userid + "\r\n" + 	//用户ID	
		"oid=" + item + "\r\n" +		//模块名称	
		"msgtype=" + 	title + "\r\n" + //标题	
		"msg=" + content + "\r\n" + 	//信息内容
		"state=发帖\r\n";
	WriteIndexFile();
	return 1;
}

//android评论
int forum::TianYa::AndroidNewComment(const char *packet)
{	
	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	string username = ms_->GetValueFromMapByKey("userName",true);
	//用户ID
	string userid = ms_->GetValueFromMapByKey("userId",true);
	//模块名称
	string item = ms_->GetValueFromMapByKey("categoryId",true);
	//信息内容
	string content = ms_->GetValueFromMapByKey("content",true);
	if (content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +	//用户名	
		"userid=" + userid + "\r\n" + 	//用户ID	
		"oid=" + item + "\r\n" +		//模块名称	
		"msg=" + content + "\r\n" + 	//信息内容
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

//android回复
int forum::TianYa::AndroidNewReply(const char *packet)
{
	//数据部分
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');

	//用户名
	string username = ms_->GetValueFromMapByKey("userName",true);
	//用户ID
	string userid = ms_->GetValueFromMapByKey("userId",true);
	//模块名称
	string item = ms_->GetValueFromMapByKey("categoryId",true);
	//信息内容
	string content = ms_->GetValueFromMapByKey("content",true);
	if (content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +	//用户名	
		"userid=" + userid + "\r\n" + 	//用户ID	
		"oid=" + item + "\r\n" +		//模块名称	
		"msg=" + content + "\r\n" + 	//信息内容
		"state=回复\r\n";
	WriteIndexFile();
	return 1;
}

//android上传文件
int forum::TianYa::AndroidUploadDocument(const char *packet)
{	
	//从uri中提取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	from_ = ms_->GetValueFromMapByKey("idWriter", true);
	
	//文件部分
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"picture\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}
