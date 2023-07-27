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
		//�û�ע��
		int PcWebReg(const char *packet);
		//���վ��
		int PcWebAdd(const char *packet);
		//�û���¼
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