#ifndef HTTP_FORUM_FORUMXICIHUTONG_H_H_H
#define HTTP_FORUM_FORUMXICIHUTONG_H_H_H
/************************************************************************/
/*������ͬ                                                                     */
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
		//pc �˵�½
		int PCWebLogin(const char *packet);
		//pc �˷���
		int PCWebFaTie(const char *packet);
		//pc �� ���� ����
		int PCWebNewComment(const char *packet);

		enum
		{
			//pc �˵�½
			kPCWebLogin = 1,
			//pc �˷���
			kPCWebFaTie,
			//pc �˸��� ����
			kPCWebNewComment,
		};
	
		string password_;
	};
	
}
#endif