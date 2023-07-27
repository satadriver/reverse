#include "StdAfx.h"
#include "BlogWangYiBlog.h"

blog::WangYiBlog::WangYiBlog(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::WangYiBlog::~WangYiBlog()
{
	
}

int blog::WangYiBlog::IsWangYiBlog(HttpSession *http_session)
{
	//发表日志
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/editBlogNew.do?") != -1))	
	{
		return kWebNewTopic;
	}
	//发表晚安日志
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/LightPublishBean.publishGoodNightBlog.dwr") != -1))	
	{
		return kWebNewTopicNight;
	}
	//写心情
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/FeelingsBeanNew.addFeelingsCard.dwr") != -1))	
	{
		return KWebXinQing;
	}
	//评论
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/BlogBeanNew.addBlogComment.dwr") != -1))	
	{
		return kWebPostComment;
	}
	//转载
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/BlogBeanNew.addLightBlog.dwr") != -1))	
	{
		return KWebZhuanZai;
	}
	//回复
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/BlogBeanNew.addLightBlog.dwr") != -1))	
	{
		return kWebReply;
	}
	//1 发表文字时上传的图片 pc  
	if( (http_session->m_Requestline.m_Host.Find("upload.photo.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/anony/web/upload/userdefinesize?sitefrom=blogeditor") != -1))	
	{
		return KWebAttachFile;
	}
	//1 发表文字时上传的图片 android
	if( (http_session->m_Requestline.m_Host.Find("upload.photo.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/anony/web/upload/userdefinesize?sitefrom=blog_android") != -1))	
	{
		return KWebAttachFile;
	}
	//相册上传图片  
	if( (http_session->m_Requestline.m_Host.Find("upload.photo.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/web/upload?sitefrom=blogflash") != -1))	
	{
		return KWebAttachFile;
	}
	//android发表日志
	if( (http_session->m_Requestline.m_Host.Find("wap.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/w2/dwr/call/plaincall/MobileBlogBean.addBlog.dwr?") != -1))	
	{
		return kAndroidNewTopic;
	}
	//android写心情
	if( (http_session->m_Requestline.m_Host.Find("wap.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/w2/dwr/call/plaincall/MobileFeelingBean.addFeeling.dwr?") != -1))	
	{
		return KAndroidXinQing;
	}
	//android评论
	if( (http_session->m_Requestline.m_Host.Find("wap.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/w2/dwr/call/plaincall/MobileBlogBean.addBlogComment.dwr?") != -1))	
	{
		return kAndroidPostComment;
	}
	//android回复 lofter 中的内容
	if( (http_session->m_Requestline.m_Host.Find("wap.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/w2/dwr/call/plaincall/MobileBlogBean.replyBlogComment.dwr?") != -1))													
	{
		return kAndroidReply;
	}
	//android回复 博客小管
	if( (http_session->m_Requestline.m_Host.Find("wap.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/w2/dwr/call/plaincall/MobileMsgBean.replyMsg.dwr?") != -1))													
	{
		return kAndroidReply;
	}	
	return 0;
}

int blog::WangYiBlog::Process(const char *packet,int action)
{
	//发表日志
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//发表晚安日志
	if (action == kWebNewTopicNight)
	{
		return WebNewTopicNight(packet);
	}
	//写心情
	if (action == KWebXinQing)
	{
		return WebXinQing(packet);
	}
	//评论
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//转载
	if (action == KWebZhuanZai)
	{
		return WebZhuanZai(packet);
	}
	//回复
	if (action == kWebReply)
	{
		return WebReply(packet);
	}
	//上传文件 相册 日志
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	//android发表日志
	if (action == kAndroidNewTopic)
	{
		return AndroidNewTopic(packet);
	}
	//android写心情
	if (action == KAndroidXinQing)
	{
		return AndroidXinQing(packet);
	}
	//android评论
	if (action == kAndroidPostComment)
	{
		return AndroidPostComment(packet);
	}
	//android回复
	if (action == kAndroidReply)
	{
		return AndroidReply(packet);
	}
	return 0;
}

//发表日志
int blog::WangYiBlog::WebNewTopic(const char *packet)
{	
	//用户名，标题，内容，地址(省市区号)
	string address;
	//从cookie 中获得user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		//通行证号
		user_ = tmp.substr(0, pos);         
	}
	//地址(省市 区号0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");

    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//标题
	title_ = ms_->GetValueFromMapByKey("title",true);
	//信息内容
	content_ = ms_->GetValueFromMapByKey("HEContent",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" +    //通行证号
		"netmark=" + title_ + "\r\n" +  //标题
		"msg=" + 	content_ + "\r\n" + //信息内容 
		"fileurl=" + address + "\r\n" +  //地址
		"platform=电脑浏览器\r\n"
		"state=发表\r\n";
	WriteIndexFile();
	return 1;
}


//发表晚间日志
int blog::WangYiBlog::WebNewTopicNight(const char *packet)
{	
	//用户名，标题，内容，地址(省市区号)
	string address;
	//从cookie 中获得user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	//地址(省市 区号0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");	
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//标题
	 title_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e1=string:","\n",true);
	//开心的事
	string temp1 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e4=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e13=string:","\n",true);
	//为他人做的事
	string temp2 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e5=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e14=string:","\n",true);	
	//计划/工作/备忘
	string temp3 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e6=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e15=string:","\n",true);
	//比昨天的进步
	string temp4 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e7=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e16=string:","\n",true);
	//心灵/感悟/灵感
	string temp5 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e8=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e17=string:","\n",true);
	//关注/八卦/新闻
	string temp6 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e9=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e18=string:","\n",true);
	//健康/饮食/体重
	string temp7 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e10=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e19=string:","\n",true);
	//睡觉前的愿望
	string temp8 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e11=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e20=string:","\n",true);
	//还想说点什么
	string temp9 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e2=string:","\n",true);
	//内容
	content_ = temp1 + temp2 + temp3 + temp4 +temp5 + temp6 + temp7 + temp8 + temp9; 
	output_text_ =
		"mid=" + 	user_ + "\r\n" +    //通行证号
		"netmark=" + title_ + "\r\n" +  //标题
		"msg=" + 	content_ + "\r\n" + //信息内容 
		"fileurl=" + address + "\r\n" +  //地址
		"platform=电脑浏览器\r\n"
		"state=发表\r\n";
	WriteIndexFile();
	return 1;
}

//写心情
int blog::WangYiBlog::WebXinQing(const char *packet)
{
	//(用户名，博客名，内容，地址(省市区号))
	string blogName,blogNameGoal,blogIdGoal,address;
	//从cookie 中获得user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	//地址(省市 区号0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");	
	//数据部分
	string src_data(packet);	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//昵称 (备用)
	blogName = ms_->GetValueBetweenBeginEnd(src_data,"c0-e3=string:","\n",true);
	//内容
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e1=string:","\n",true);	
	output_text_ =
		"mid=" + 	user_ + "\r\n" +    //通行证号
		"msg=" + 	content_ + "\r\n" + //信息内容 
		"fileurl=" + address + "\r\n" +  //地址
		"platform=电脑浏览器\r\n"
		"state=发表\r\n";
	WriteIndexFile();
	return 1;
}

//评论
int blog::WangYiBlog::WebPostComment(const char *packet)
{
	string blogName,blogNameGoal,blogNickNameGoal,blogIdGoal,address;
	//从cookie 中获得user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	//地址(省市 区号0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");
	//数据部分
	string src_data(packet);	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//昵称
	blogName = ms_->GetValueBetweenBeginEnd(src_data,"c0-e5=string:","\n",true);

	//目标博客ID
	blogIdGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e9=number:","\n",true);
	//目标博客账号
	blogNameGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e10=string:","\n",true);
	//目标博客昵称
	blogNickNameGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e11=string:","\n",true);
	//内容
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e4=string:","\n",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" +				//通行证号
		"nick=" + 	blogName + "\r\n" +				//昵称
		"msg=" + 	content_ + "\r\n" +				//信息内容
		"oid=" + 	blogIdGoal + "\r\n" +			//目标博客ID
		"sex=" + 	blogNameGoal + "\r\n" +			//目标博客账号
		"clientmsg=" + 	blogNickNameGoal + "\r\n" + //目标博客昵称
		"fileurl=" + 	address + "\r\n" +			//地址
		"platform=电脑浏览器\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

//转载
int blog::WangYiBlog::WebZhuanZai(const char *packet)
{
	string address;
	//从cookie 中获得user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	//地址(省市 区号0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");
	
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//标题
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e4=string:","\n",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" +   //通行证号
		"netmark=" + title_ + "\r\n" + //标题
		"fileurl=" + address + "\r\n" + //地址
		"platform=电脑浏览器\r\n"
		"state=转载\r\n";
	WriteIndexFile();
	return 1;
}
//回复
int blog::WangYiBlog::WebReply(const char *packet)
{
	string blogName,blogNameGoal,blogNickNameGoal,blogIdGoal,address;
	//从cookie 中获得user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	//地址(省市 区号0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");
	//数据部分
	string src_data(packet);	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//标题
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e3=string:","\n",true);
	//内容
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e4=string:","\n",true);
	//昵称
	blogName = ms_->GetValueBetweenBeginEnd(src_data,"c0-e5=string:","\n",true);
	//目标博客ID
	blogIdGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e9=number:","\n",true);
	//目标博客账号
	blogNameGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e10=string:","\n",true);
	//目标博客昵称
	blogNickNameGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e11=string:","\n",true);

	output_text_ =
		"mid=" + 	user_ + "\r\n" +				//通行证号
		"nick=" + 	blogName + "\r\n" +				//昵称
		"netmark=" + title_ + "\r\n" +				//标题
		"msg=" + 	content_ + "\r\n" +				//信息内容
		"oid=" + 	blogIdGoal + "\r\n" +			//目标博客ID
		"sex=" + 	blogNameGoal + "\r\n" +			//目标博客账号
		"clientmsg=" + blogNickNameGoal + "\r\n" +  //目标博客昵称
		"fileurl=" + 	address + "\r\n" +			//地址
		"platform=电脑浏览器\r\n"
		"state=回复\r\n";
	WriteIndexFile();
	return 1;
}
//上传文件
int blog::WangYiBlog::WebAttachFile(const char *packet)
{
	//从cookie 中获得user_  http_session_->m_Requestline.m_URI.GetBuffer(0)
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	if (user_.empty())
	{
		user_ = "Null";
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

//android发表日志
int blog::WangYiBlog::AndroidNewTopic(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string addressIP;
	//昵称
	nick_ =  ms_->GetValueFromMapByKey("h");
	//IP 地址
	addressIP = ms_->GetValueFromMapByKey("ip");
	//数据部分
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//标题
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-param0=string:","\n",true);
	//内容
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-param1=string:","\n",true);
	output_text_ =
		"nick=" + 	nick_ + "\r\n" +	//昵称
		"netmark=" + title_ + "\r\n" + //标题
		"msg=" + 	content_ + "\r\n" + //信息内容
		"imsi=" + 	addressIP + "\r\n" + //IP地址
		"platform=手机app\r\n"
		"state=发表\r\n";
	WriteIndexFile();
	return 1;
}
//android写心情
int blog::WangYiBlog::AndroidXinQing(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string addressIP;
	//昵称
	nick_ =  ms_->GetValueFromMapByKey("h");
	//IP 地址
	addressIP = ms_->GetValueFromMapByKey("ip");
	//数据部分
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//内容
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-param0=string:","\n",true);
	output_text_ =
		"nick=" + 	nick_ + "\r\n" +	//昵称
		"msg=" + 	content_ + "\r\n" + //信息内容
		"imsi=" + 	addressIP + "\r\n" + //IP地址
		"platform=手机app\r\n"
		"state=发表\r\n";
	WriteIndexFile();
	return 1;
}
//android评论
int blog::WangYiBlog::AndroidPostComment(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string addressIP;
	//昵称
	nick_ =  ms_->GetValueFromMapByKey("h");
	//IP 地址
	addressIP = ms_->GetValueFromMapByKey("ip");

    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//内容
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-param0=string:","\n",true);
	output_text_ =
		"nick=" + 	nick_ + "\r\n" +	//昵称
		"msg=" + 	content_ + "\r\n" + //信息内容
		"imsi=" + 	addressIP + "\r\n" + //IP地址
		"platform=手机app\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}
//android回复
int blog::WangYiBlog::AndroidReply(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string addressIP;
	//昵称
	nick_ =  ms_->GetValueFromMapByKey("h");
	//IP 地址
	addressIP = ms_->GetValueFromMapByKey("ip");
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//内容
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-param1=string:","\n",true);
	output_text_ =
		"nick=" + 	nick_ + "\r\n" +	//昵称
		"msg=" + 	content_ + "\r\n" + //信息内容
		"imsi=" + 	addressIP + "\r\n" + //IP地址
		"platform=手机app\r\n"
		"state=回复\r\n";
	WriteIndexFile();
	return 1;
}
