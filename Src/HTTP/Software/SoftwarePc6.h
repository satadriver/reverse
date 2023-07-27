#ifndef HTTP_SOFTWARE_PC6_H_H_H 
#define HTTP_SOFTWARE_PC6_H_H_H

#include "Software.h"

namespace software
{
	class Pc6: public Software
	{
	public:
		Pc6(HttpSession *http_session, const string &pro_name);
		~Pc6();
		static int Is(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ËÑË÷
		int WebSearch(const char *packet);
		//ÏÂÔØ
		int WebDown(const char *packet);
		int WebDown1(const char *packet);
		enum
		{
			kWebSearch = 1,
			kWebDown,
			kWebDown1,
		};
	};
}

#endif