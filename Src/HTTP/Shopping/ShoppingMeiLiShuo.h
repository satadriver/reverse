
#ifndef HTTP_SHOPPING_MEILISHUO_H_H_H
#define HTTP_SHOPPING_MEILISHUO_H_H_H

#include "Shopping.h"


namespace shopping
{
	class MeiLiShuo : public Shopping
	{
	public:
		MeiLiShuo(HttpSession *http_session, const string &pro_name);
		~MeiLiShuo();
		
		static int IsMeiLiShuo(HttpSession *http_session);	
		int Process(const char *packet,int action);
		
	private:
		int PcWebLogin(const char *packet);
		int PcWebAddAddress(const char *packet);
		int PcWebModifyAddress(const char *packet);
		int AndroidAddAddress(const char *packet);
		int AndroidModifyAddress(const char *packet);
		enum
		{
			kPcWebLogin = 1,
			kPcWebAddAddress,
			kPcWebModifyAddress,
			kAndroidAddAddress,
			kAndroidModifyAddress,
		};
	};
}

#endif