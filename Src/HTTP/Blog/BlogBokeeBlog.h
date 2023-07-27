
#ifndef HTTP_BLOG_BLOGBOKEEBLOG_H_H_H
#define HTTP_BLOG_BLOGBOKEEBLOG_H_H_H

#include "Blog.h"

namespace blog
{
	class BokeeBlog: public Blog
	{
	public:
		BokeeBlog(HttpSession *http_session, const string &pro_name);
		~BokeeBlog();
		static int IsBokeeBlog(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//登录
		int WebLogin(const char *packet);
		//发表 博文
		int WebNewTopic(const char *packet);
		//评论
		int WebPostComment(const char *packet);
		//评论 
		int WebPostCommentTuyang(const char *packet);
		//上传文件 博文
		int WebAttachFile(const char *packet);
		//发表画报
		int WebNewPaper(const char *packet);
		//上传文件 相册
		int WebAttachFileTuyang(const char *packet);

		enum
		{
			//登录
			kWebLogin = 1,
			//发表 博文
			kWebNewTopic,
			//发表 画报
			kWebNewPaper,
			//评论
			kWebPostComment,
			//评论 2
			kWebPostCommentTuyang,
			//上传文件 博文中添加图片
			KWebAttachFile,	
			//上传文件 图样
			KWebAttachFileTuyang
		};
	};
}

#endif