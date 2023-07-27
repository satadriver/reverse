#ifndef HTTP_FORUM_FORUMTIANYA_H_H_H
#define HTTP_FORUM_FORUMTIANYA_H_H_H

#include "Forum.h"

namespace forum
{
	class TianYa : public Forum
	{
	public:
		TianYa(HttpSession *http_session, const string &pro_name);
		~TianYa();		
		static int IsTianYa(HttpSession *http_session);		
		int Process(const char *packet,int action);
		
	private:		
		int PCWebNewPost(const char *packet);
		int PCWebNewComment(const char *packet);
		int PCWebNewReply(const char *packet);
        int PCWebUploadDocument(const char *packet);

		int AndroidNewPost(const char *packet);
		int AndroidNewComment(const char *packet);
		int AndroidNewReply(const char *packet);
		int AndroidUploadDocument(const char *packet);

		enum
		{
			//����
			kPCWebNewPost = 1,
			//����
			kPCWebNewComment,
			//�ظ�
			kPCWebNewReply,
			//�ϴ��ļ�			
			KPCWebUploadDocument,
			//android����
			kAndroidNewPost,
			//android����
			kAndroidNewComment,
			//android�ظ�
			kAndroidNewReply,
			//android�ϴ��ļ�
			KAndroidUploadDocument
		};
	};
}
#endif