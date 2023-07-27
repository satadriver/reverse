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
			//发帖
			kPCWebNewPost = 1,
			//评论
			kPCWebNewComment,
			//回复
			kPCWebNewReply,
			//上传文件			
			KPCWebUploadDocument,
			//android发帖
			kAndroidNewPost,
			//android评论
			kAndroidNewComment,
			//android回复
			kAndroidNewReply,
			//android上传文件
			KAndroidUploadDocument
		};
	};
}
#endif