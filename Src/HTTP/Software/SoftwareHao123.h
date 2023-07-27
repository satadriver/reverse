#ifndef HTTP_SOFTWARE_HAO123_H_H_H 
#define HTTP_SOFTWARE_HAO123_H_H_H

#include "Software.h"

namespace software
{
	class Hao123: public Software
	{
	public:
		Hao123(HttpSession *http_session, const string &pro_name);
		~Hao123();
		static int Is(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//搜索
		int WebSearch(const char *packet);
		//下载
		int WebDown(const char *packet);
		//下载(包含地址)
		int WebDownAddress(const char *packet);
		enum
		{
			kWebSearch = 1,
			kWebDown,
			kWebDownAddress,
		};
	};
}

#endif