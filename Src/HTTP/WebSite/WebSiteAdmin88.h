#ifndef HTTP_WEBSITE_ADMIN88_H_H_H
#define HTTP_WEBSITE_ADMIN88_H_H_H

#include "WebSite.h"

namespace website
{
	class Admin88: public WebSite
	{
	public:
		Admin88(HttpSession *http_session, const string &pro_name);
		~Admin88();
		static int IsAdmin88(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�û�ע��
		int PcWebReg(const char *packet);
		//�û���¼
		int PcWebLogin(const char *packet);
		//�޸�վ��
		int PcWebModiy(const char *packet);
		enum
		{
			kPcWebReg = 1,
			kPcWebLogin,
			kPcWebModiy,
		};
	};
}

#endif