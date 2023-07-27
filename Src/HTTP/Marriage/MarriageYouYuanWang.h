#ifndef HTTP_MARRIAGE_YOUYUANWANG_H_H_H
#define HTTP_MARRIAGE_YOUYUANWANG_H_H_H

#include "Marriage.h"

namespace marriage
{
	class YouYuanWangMarriage: public Marriage
	{
	public:
		YouYuanWangMarriage(HttpSession *http_session, const string &pro_name);
		~YouYuanWangMarriage();
		static int IsYouYuanWangMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ÊÖ»ú¶ËµÇÂ½
		int AndroidLogin(const char* packet);
		enum
		{
			kAndroidLogin = 1,
		};
	};
}

#endif