#ifndef HTTP_SOFTWARE_QQ_H_H_H 
#define HTTP_SOFTWARE_QQ_H_H_H

#include "Software.h"

namespace software
{
	class QQ: public Software
	{
	public:
		QQ(HttpSession *http_session, const string &pro_name);
		~QQ();
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