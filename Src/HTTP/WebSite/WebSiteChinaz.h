#ifndef HTTP_WEBSITE_CHINAZ_H_H_H
#define HTTP_WEBSITE_CHINAZ_H_H_H

#include "WebSite.h"

namespace website
{
	class Chinaz: public WebSite
	{
	public:
		Chinaz(HttpSession *http_session, const string &pro_name);
		~Chinaz();
		static int IsChinaz(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//≤È—Ø’æµ„
		int PcWebSearch(const char *packet);
		enum
		{
			kPcWebSearch = 1,
		};
	};
}

#endif