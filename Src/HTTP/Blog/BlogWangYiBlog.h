
#ifndef HTTP_BLOG_BLOGWANGYIBLOG_H_H_H
#define HTTP_BLOG_BLOGWANGYIBLOG_H_H_H

#include "Blog.h"

namespace blog
{
	class WangYiBlog: public Blog
	{
	public:
		WangYiBlog(HttpSession *http_session, const string &pro_name);
		~WangYiBlog();
		static int IsWangYiBlog(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//������־
		int WebNewTopic(const char *packet);
		//��������־
		int WebNewTopicNight(const char *packet);
		//��������
		int WebXinQing(const char *packet);
		//����
		int WebPostComment(const char *packet);
		//ת��
		int WebZhuanZai(const char *packet);
		//�ظ�
		int WebReply(const char *packet);
		//�ϴ��ļ� 
		int WebAttachFile(const char *packet);

		//android������־
		int AndroidNewTopic(const char *packet);
		//androidд����
		int AndroidXinQing(const char *packet);
		//android����
		int AndroidPostComment(const char *packet);
		//android�ظ�
		int AndroidReply(const char *packet);
		enum
		{
			//������־
			kWebNewTopic =1,
			//��������־
			kWebNewTopicNight,
			//д����
			KWebXinQing,
			//����
			kWebPostComment,
			//ת��
			KWebZhuanZai,
			//�ظ�
			kWebReply,
			//�ϴ��ļ�
			KWebAttachFile,	
			//android������־
			kAndroidNewTopic,
			//androidд����
			KAndroidXinQing,
			//android����
			kAndroidPostComment,
			//android�ظ�
			kAndroidReply,
		};
	};
}

#endif