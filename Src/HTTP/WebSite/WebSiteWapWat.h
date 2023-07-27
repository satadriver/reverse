#ifndef HTTP_WEBSITE_WAPWAT_H_H_H
#define HTTP_WEBSITE_WAPWAT_H_H_H

#include "WebSite.h"

namespace website
{
	class WapWat: public WebSite
	{
	public:
		WapWat(HttpSession *http_session, const string &pro_name);
		~WapWat();
		static int IsWapWat(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//用户注册
		int PcWebReg(const char *packet);
		//添加站点
		int PcWebAdd(const char *packet);
		//用户登录
		int PcWebLogin(const char *packet);
		enum
		{
			kPcWebReg = 1,
			kPcWebAdd,
			kPcWebLogin,
		};
	};
}

#endif