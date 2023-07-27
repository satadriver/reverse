#ifndef HTTP_FORUM_FORUMMAOPU_H_H_H
#define HTTP_FORUM_FORUMMAOPU_H_H_H
/************************************************************************/
/* Ã¨ÆË                                                                     */
/************************************************************************/

#include "Forum.h"

namespace forum
{
	class MaoPu : public Forum
	{
	public:
		MaoPu(HttpSession *http_session, const string &pro_name);
		~MaoPu();
		
		static int IsMaoPu(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int AndroidLogin(const char *packet);
		int PCWebLogin(const char *packet);
		int AndroidNewPost(const char *packet);
		int PCWebNewPost(const char *packet);
		int AndroidNewComment(const char *packet);
		int PCWebNewComment(const char *packet);
		enum
		{
			kAndroidLogin = 1,
			kPCWebLogin,
			kAndroidNewPost,
			kPCWebNewPost,
			kAndroidNewComment,
			kPCWebNewComment
		};
		
		string password_;
	};
	
}
#endif