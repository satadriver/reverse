#ifndef HTTP_FORUM_FORUMDISCUZ_H_H_H
#define HTTP_FORUM_FORUMDISCUZ_H_H_H


#include "Forum.h"

namespace forum
{
	class Discuz : public Forum
	{
	public:
		Discuz(HttpSession *http_session, const string &pro_name);
		~Discuz();
		
		static int IsDiscuz(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		int PCWebLogin(const char *packet);
		int PCWebNewPost(const char *packet);
		int PCWebNewComment(const char *packet);
		
		enum
		{
			kPCWebLogin = 1,
			kPCWebNewPost,
			kPCWebNewComment	
		};
		string host_;
	};
}
#endif