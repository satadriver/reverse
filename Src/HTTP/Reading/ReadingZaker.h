#ifndef HTTP_READING_READINZAKER_H_H_H
#define HTTP_READING_READINZAKER_H_H_H
/************************************************************************/
/* ZAKER                                                                */
/************************************************************************/
#include "Reading.h"

namespace reading
{
	class Zaker: public Reading
	{
	public:
		Zaker(HttpSession *http_session, const string &pro_name);
		~Zaker();	
		static int IsZaker(HttpSession *http_session);
		int Process(const char *packet,int action);	
	private:
		//android¶Ë µÇÂ¼
		int AndroidLogin(const char *packet);
		//android¶Ë ÆÀÂÛ
		int AddCommentM(const char *packet);
		enum 
		{
			kAndroidLogin = 1,
			kAddCommentM,		//2014-11-14
		};
	};
}
#endif