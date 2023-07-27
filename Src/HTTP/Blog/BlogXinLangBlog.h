
#ifndef HTTP_BLOG_BLOGSINABLOG_H_H_H
#define HTTP_BLOG_BLOGSINABLOG_H_H_H

#include "Blog.h"

namespace blog
{
	class SinaBlog: public Blog
	{
	public:
		SinaBlog(HttpSession *http_session, const string &pro_name);
		~SinaBlog();
		static int IsSinaBlog(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//发表日志
		int WebNewTopic(const char *packet);
		//评论
		int WebPostComment(const char *packet);
		//回复
		int WebReply(const char *packet);
		//上传文件 
		int WebAttachFile(const char *packet);
    
		//android 发博文
		int AndroidNewTopic(const char *packet);
		//android 评论、回复
		int AndroidPostComment(const char *packet);

		enum
		{
			//发表博文
			kWebNewTopic =1,
			//评论
			kWebPostComment,
			//回复
			kWebReply,
			//上传文件
			KWebAttachFile,
			//android 发博文
			kAndroidNewTopic,
			//android 评论
			kAndroidPostComment,
		};
	};
}

#endif