
#ifndef HTTP_BLOG_BLOGWANGYILOFTER_H_H_H
#define HTTP_BLOG_BLOGWANGYILOFTER_H_H_H

#include "Blog.h"
namespace blog
{
	class WangYiLofter: public Blog
	{
	public:
		WangYiLofter(HttpSession *http_session, const string &pro_name);
		~WangYiLofter();
		static int IsWangYiLofter(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//��¼
		int PCWebLogin(const char *packet);
		//����
		int WebNewTopic(const char *packet);
		//ͼƬ
		int WebNewPicture(const char *packet);
		//����
		int WebNewMusic(const char *packet);
		//��Ƶ
		int WebNewVideo(const char *packet);
		//���� �ظ�
		int WebPostComment(const char *packet);
		//������
		int WebNewLong(const char *packet);
		//�ϴ��ļ� GOOGLE
		int WebAttachFileGoogle(const char *packet);
		//android���� ͼƵ ��Ƶ
		int AndroidNewTopicAll(const char *packet);
		//android����
		int AndroidPostComment(const char *packet);

		enum
		{
			//��¼
			kPCWebLogin = 1,
			//����
			kWebNewTopic,
			//ͼƬ
			kWebNewPicture,
			//����
			kWebNewMusic,
			//��Ƶ
			kWebNewVideo,
			//���� �ظ�
			kWebPostComment,
			//������
			kWebNewLong,
			//�ϴ��ļ� Google
			KWebAttachFileGoogle,
			//android ���� ͼƬ ��Ƶ 
			kAndroidNewTopicAll,
			//android����
			kAndroidPostComment,
		};
	};
}

#endif