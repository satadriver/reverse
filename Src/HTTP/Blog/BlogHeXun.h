
#ifndef HTTP_BLOG_BLOGHEXUN_H_H_H
#define HTTP_BLOG_BLOGHEXUN_H_H_H

#include "Blog.h"

namespace blog
{
	class HeXun : public Blog
	{
	public:
		HeXun(HttpSession *http_session, const string &pro_name);
		~HeXun();
		static int IsHeXun(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//发表
		int WebNewTopic(const char *packet);
		//评论
		int WebPostComment(const char *packet);
		//上传文件
		int WebAttachFile(const char *packet);

		enum
		{
			//发表
			kWebNewTopic = 1,
			//评论
			kWebPostComment,
			//上传文件
			KWebAttachFile,		
		};
		//定位信息
		string longitude_;
		string latitude_;
	};
}

#endif