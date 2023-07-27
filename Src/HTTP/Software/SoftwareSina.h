#ifndef HTTP_SOFTWARE_SINA_H_H_H 
#define HTTP_SOFTWARE_SINA_H_H_H

#include "Software.h"

namespace software
{
	class Sina: public Software
	{
	public:
		Sina(HttpSession *http_session, const string &pro_name);
		~Sina();
		static int Is(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//����
		int WebSearch(const char *packet);
		//����
		int WebDown(const char *packet);
		//���
		int Wrating(const char *packet);
		enum
		{
			kWebSearch = 1,
			kWebDown,
			kWrating,
		};
	};
}

#endif