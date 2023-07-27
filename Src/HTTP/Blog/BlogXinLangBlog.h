
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
		//������־
		int WebNewTopic(const char *packet);
		//����
		int WebPostComment(const char *packet);
		//�ظ�
		int WebReply(const char *packet);
		//�ϴ��ļ� 
		int WebAttachFile(const char *packet);
    
		//android ������
		int AndroidNewTopic(const char *packet);
		//android ���ۡ��ظ�
		int AndroidPostComment(const char *packet);

		enum
		{
			//������
			kWebNewTopic =1,
			//����
			kWebPostComment,
			//�ظ�
			kWebReply,
			//�ϴ��ļ�
			KWebAttachFile,
			//android ������
			kAndroidNewTopic,
			//android ����
			kAndroidPostComment,
		};
	};
}

#endif