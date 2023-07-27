
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
		//������־
		int WebNewTopic(const char *packet);
		//����
		int WebPostComment(const char *packet);
		//�ϴ��ļ� 
		int WebAttachFile(const char *packet);
		//������ϴ���Ƭ
        int WebAttachFileAlubm(const char *packet);
		enum
		{
			//������
			kWebNewTopic =1,
			//����
			kWebPostComment,
			//�ϴ��ļ�
			KWebAttachFile,
			//���
			KWebAttachFileAlubm
		};
	};
}

#endif