#ifndef HTTP_SOFTWARE_ORSOON_H_H_H 
#define HTTP_SOFTWARE_ORSOON_H_H_H

#include "Software.h"

namespace software
{
	class Orsoon: public Software
	{
	public:
		Orsoon(HttpSession *http_session, const string &pro_name);
		~Orsoon();
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