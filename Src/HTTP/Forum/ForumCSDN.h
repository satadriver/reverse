
#ifndef HTTP_FORUM_FORUMCSDN_H_H_H
#define HTTP_FORUM_FORUMCSDN_H_H_H

#include "Forum.h"

namespace forum
{
	class CSDN : public Forum
	{
	public:
		CSDN(HttpSession *http_session, const string &pro_name);
		~CSDN();
		
		static int IsCSDN(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:
		int LoginW(const char *packet);

		int AddPostCW(const char *packet);
		int AddPost(const char *packet);
		int AddCommentCW(const char *packet);
		int AddComment(const char *packet);

		int AddPostM(const char *packet);
		int AddCommentM(const char *packet);
		int addCommentM(const char *packet);

		int AddMW(const char *packet);

		string GetBDUSS();
		string GetUserName(const string &bduss);
		enum
		{
			MLoginW = 1,			//Web;Computer+Mobile
			kAddPostCW,				//Computer Web
			kAddCommentCW,			//Computer Web
			kAddPostM,				//Mobile App; iOS+Android
			kAddCommentM,			//Mobile App; iOS+Android
			kAddMW,					//Mobile Web
			kAddPost,
			kaddComment,
			kaddCommentM,
		};
		string bar_name_;

		//<BAIDUID,USERNAME>
		static map<string,string>map_;
		static void ReadBDUSS();
		static void WriteBDUSS(const string &bduss, const string &username);
	};
}
#endif