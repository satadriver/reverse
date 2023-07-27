#ifndef HTTP_SOFTWARE_BAIDU_H_H_H 
#define HTTP_SOFTWARE_BAIDU_H_H_H

#include "Software.h"

namespace software
{
	class BaiDu: public Software
	{
	public:
		BaiDu(HttpSession *http_session, const string &pro_name);
		~BaiDu();
		static int Is(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ËÑË÷
		int WebSearch(const char *packet);
		//ÏÂÔØ
		int WebDown(const char *packet);
		enum
		{
			kWebSearch = 1,
			kWebDown,
		};
	};
}

#endif