#ifndef HTTP_WEBSITE_TENCENT_H_H_H
#define HTTP_WEBSITE_TENCENT_H_H_H

#include "WebSite.h"

namespace website
{
	class Tencent: public WebSite
	{
	public:
		Tencent(HttpSession *http_session, const string &pro_name);
		~Tencent();
		static int IsTencent(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ÃÌº”’æµ„
		int PcWebAdd(const char *packet);
		enum
		{
			kPcWebAdd = 1,
		};
	};
}

#endif