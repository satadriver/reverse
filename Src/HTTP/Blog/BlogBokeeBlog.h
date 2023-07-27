
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
		//��¼
		int WebLogin(const char *packet);
		//���� ����
		int WebNewTopic(const char *packet);
		//����
		int WebPostComment(const char *packet);
		//���� 
		int WebPostCommentTuyang(const char *packet);
		//�ϴ��ļ� ����
		int WebAttachFile(const char *packet);
		//������
		int WebNewPaper(const char *packet);
		//�ϴ��ļ� ���
		int WebAttachFileTuyang(const char *packet);

		enum
		{
			//��¼
			kWebLogin = 1,
			//���� ����
			kWebNewTopic,
			//���� ����
			kWebNewPaper,
			//����
			kWebPostComment,
			//���� 2
			kWebPostCommentTuyang,
			//�ϴ��ļ� ���������ͼƬ
			KWebAttachFile,	
			//�ϴ��ļ� ͼ��
			KWebAttachFileTuyang
		};
	};
}

#endif