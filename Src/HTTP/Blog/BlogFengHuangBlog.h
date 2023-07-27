
#ifndef HTTP_BLOG_BLOGFENGHUANGBLOG_H_H_H
#define HTTP_BLOG_BLOGFENGHUANGBLOG_H_H_H

#include "Blog.h"

namespace blog
{
	class FengHuangBlog: public Blog
	{
	public:
		FengHuangBlog(HttpSession *http_session, const string &pro_name);
		~FengHuangBlog();
		static int IsFengHuangBlog(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//发表日志
		int WebNewTopic(const char *packet);
		//评论
		int WebPostComment(const char *packet);
		//上传文件 
		int WebAttachFile(const char *packet);
		//相册中上传照片
        int WebAttachFileAlubm(const char *packet);
		enum
		{
			//发表博文
			kWebNewTopic =1,
			//评论
			kWebPostComment,
			//上传文件
			KWebAttachFile,
			//相册
			KWebAttachFileAlubm
		};
	};
}

#endif