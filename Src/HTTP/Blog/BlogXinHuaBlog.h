
#ifndef HTTP_BLOG_BLOGXINHUABLOG_H_H_H
#define HTTP_BLOG_BLOGXINHUABLOG_H_H_H

#include "Blog.h"

namespace blog
{
	class XinHuaBlog: public Blog
	{
	public:
		XinHuaBlog(HttpSession *http_session, const string &pro_name);
		~XinHuaBlog();
		static int IsXinHuaBlog(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//登录 1
		int WebLogin(const char *packet);
		//登录 2
		int WebLoginSecond(const char *packet);
		//发表 博文
		int WebNewTopic(const char *packet);
		//评论
		int WebPostComment(const char *packet);
		//上传文件
		int WebAttachFile(const char *packet);
		//上传文件 图样
		int WebAttachFileTuyang(const char *packet);

		enum
		{
			//登录 1
			kWebLogin = 1,
			//登录 2
			kWebLoginSecond,
			//发表 博文
			kWebNewTopic,
			//评论
			kWebPostComment,
			//上传文件 博文
			KWebAttachFile,	
			//上传文件 图样
			KWebAttachFileTuyang
		};
	};
}
#endif