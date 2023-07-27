#ifndef HTTP_FORUM_FORUMXICIHUTONG_H_H_H
#define HTTP_FORUM_FORUMXICIHUTONG_H_H_H
/************************************************************************/
/*Î÷ìôºúÍ¬                                                                     */
/************************************************************************/

#include "Forum.h"

namespace forum
{
	class XiCiHuTong: public Forum
	{
	public:
		XiCiHuTong(HttpSession *http_session, const string &pro_name);
		~XiCiHuTong();
		
		static int IsXiCiHuTong(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		//pc ¶ËµÇÂ½
		int PCWebLogin(const char *packet);
		//pc ¶Ë·¢Ìû
		int PCWebFaTie(const char *packet);
		//pc ¶Ë ¸úÌû ÆÀÂÛ
		int PCWebNewComment(const char *packet);

		enum
		{
			//pc ¶ËµÇÂ½
			kPCWebLogin = 1,
			//pc ¶Ë·¢Ìû
			kPCWebFaTie,
			//pc ¶Ë¸úÌû ÆÀÂÛ
			kPCWebNewComment,
		};
	
		string password_;
	};
	
}
#endif