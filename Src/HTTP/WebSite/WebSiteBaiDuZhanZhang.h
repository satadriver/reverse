#ifndef HTTP_WEBSITE_BAIDUZHANZHANG_H_H_H
#define HTTP_WEBSITE_BAIDUZHANZHANG_H_H_H

#include "WebSite.h"

namespace website
{
	class BaiDuZhanZhang: public WebSite
	{
	public:
		BaiDuZhanZhang(HttpSession *http_session, const string &pro_name);
		~BaiDuZhanZhang();
		static int IsBaiDuZhanZhang(HttpSession *http_session);
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