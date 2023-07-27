#ifndef HTTP_WEBSITE_51YES_H_H_H
#define HTTP_WEBSITE_51YES_H_H_H

#include "WebSite.h"

namespace website
{
	class YES51: public WebSite
	{
	public:
		YES51(HttpSession *http_session, const string &pro_name);
		~YES51();
		static int IsYES51(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�û�ע��
		int PcWebReg(const char *packet);
		//�û���¼
		int PcWebLogin(const char *packet);
		enum
		{
			kPcWebReg = 1,
			kPcWebLogin,
		};
	};
}

#endif