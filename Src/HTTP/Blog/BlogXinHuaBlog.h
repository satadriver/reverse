
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
		//��¼ 1
		int WebLogin(const char *packet);
		//��¼ 2
		int WebLoginSecond(const char *packet);
		//���� ����
		int WebNewTopic(const char *packet);
		//����
		int WebPostComment(const char *packet);
		//�ϴ��ļ�
		int WebAttachFile(const char *packet);
		//�ϴ��ļ� ͼ��
		int WebAttachFileTuyang(const char *packet);

		enum
		{
			//��¼ 1
			kWebLogin = 1,
			//��¼ 2
			kWebLoginSecond,
			//���� ����
			kWebNewTopic,
			//����
			kWebPostComment,
			//�ϴ��ļ� ����
			KWebAttachFile,	
			//�ϴ��ļ� ͼ��
			KWebAttachFileTuyang
		};
	};
}
#endif