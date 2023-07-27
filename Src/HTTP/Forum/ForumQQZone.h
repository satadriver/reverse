
#ifndef HTTP_FORUM_FORUMQQZONE_H_H_H
#define HTTP_FORUM_FORUMQQZONE_H_H_H
/************************************************************************/
/* QQ¿Õ¼ä													            */
/************************************************************************/
#include "Forum.h"

namespace forum
{
	class QQZone : public Forum
	{
	public:
		QQZone(HttpSession *http_session, const string &pro_name);
		~QQZone();
		
		static int IsQQZone(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:
		int AddPostCW(const char *packet);
		int AddCommentCW(const char *packet);

		int AddPostM(const char *packet);
		int AddCommentM(const char *packet);

		int AddMW(const char *packet);

		string GetBDUSS();
		string GetUserName(const string &bduss);
		enum
		{
			kAddPostCW=1,				//Computer Web
			kAddCommentCW,			//Computer Web
			kAddPostM,				//Mobile App; iOS+Android
			kAddCommentM,			//Mobile App; iOS+Android
			kAddMW,					//Mobile Web
		};
		string bar_name_;

		//<BAIDUID,USERNAME>
		static map<string,string>map_;
		static void ReadBDUSS();
		static void WriteBDUSS(const string &bduss, const string &username);
	};
}
#endif