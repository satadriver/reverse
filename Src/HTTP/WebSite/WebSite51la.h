#ifndef HTTP_WEBSITE_51LA_H_H_H
#define HTTP_WEBSITE_51LA_H_H_H

#include "WebSite.h"

namespace website
{
	class LA51: public WebSite
	{
	public:
		LA51(HttpSession *http_session, const string &pro_name);
		~LA51();
		static int IsLA51(HttpSession *http_session);
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
			kPcWebModiy,
		};
	};
}

#endif