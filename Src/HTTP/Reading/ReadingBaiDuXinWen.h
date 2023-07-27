
#ifndef HTTP_READING_READINGBAIDUXINWEN_H_H_H
#define HTTP_READING_READINGBAIDUXINWEN_H_H_H

#include "Reading.h"

namespace reading
{
	class BaiDuXinWen: public Reading
	{
	public:
		BaiDuXinWen(HttpSession *http_session, const string &pro_name);
		~BaiDuXinWen();
		
		static int IsBaiDuXinWen(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:		
		int AddCommentM(const char *packet);
		enum 
		{
			kAddCommentM = 1, //2014-11-14
		};
	};
}

#endif