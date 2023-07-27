#ifndef HTTP_SOFTWARE_WMZHE_H_H_H 
#define HTTP_SOFTWARE_WMZHE_H_H_H

#include "Software.h"

namespace software
{
	class Wmzhe: public Software
	{
	public:
		Wmzhe(HttpSession *http_session, const string &pro_name);
		~Wmzhe();
		static int Is(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//����
		int WebSearch(const char *packet);
		//����
		int WebDown(const char *packet);
		enum
		{
			kWebSearch = 1,
			kWebDown,
		};
	};
}

#endif