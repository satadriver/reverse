
#ifndef HTTP_READING_READINGSOUHUXINWEN_H_H_H
#define HTTP_READING_READINGSOUHUXINWEN_H_H_H

#include "Reading.h"

namespace reading
{
	class SouHuXinWen : public Reading
	{
	public:
		SouHuXinWen(HttpSession *http_session, const string &pro_name);
		~SouHuXinWen();
		
		static int IsSouHuXinWen(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int AddCommentC(const char *packet);
		int AddCommentM(const char *packet);
		enum 
		{
			kAddCommentC = 1, //2014-11-14
			kAddCommentM = 2, //2014-11-14
		};
	};
}

#endif