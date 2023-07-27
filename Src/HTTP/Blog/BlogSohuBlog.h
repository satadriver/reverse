
#ifndef HTTP_BLOG_BLOGSOHUBLOG_H_H_H
#define HTTP_BLOG_BLOGSOHUBLOG_H_H_H

#include "Blog.h"

namespace blog
{
	class SohuBlog: public Blog
	{
	public:
		SohuBlog(HttpSession *http_session, const string &pro_name);
		~SohuBlog();
		static int IsSohuBlog(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//发表日志
		int WebNewTopic(const char *packet);
		//评论 回复
		int WebPostComment(const char *packet);
		//上传文件 
		int WebAttachFile(const char *packet);
		enum
		{
			//发表博文
			kWebNewTopic =1,
			//评论 回复
			kWebPostComment,
			//上传文件
			KWebAttachFile,
		};
	};
}

#endif