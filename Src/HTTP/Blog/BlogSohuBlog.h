
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
		//������־
		int WebNewTopic(const char *packet);
		//���� �ظ�
		int WebPostComment(const char *packet);
		//�ϴ��ļ� 
		int WebAttachFile(const char *packet);
		enum
		{
			//������
			kWebNewTopic =1,
			//���� �ظ�
			kWebPostComment,
			//�ϴ��ļ�
			KWebAttachFile,
		};
	};
}

#endif