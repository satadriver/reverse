
#ifndef HTTP_SHOPPING_ZHENPIN_H_H_H
#define HTTP_SHOPPING_ZHENPIN_H_H_H

#include "Shopping.h"


namespace shopping
{
	class ZhenPin : public Shopping
	{
	public:
		ZhenPin(HttpSession *http_session, const string &pro_name);
		~ZhenPin();
		
		static int IsZhenPin(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:
		
		int MobileAddAddress(const char *packet);
		int MoblieLogin(const char *packet);
		int MobileModiyAddress(const char *packet);
		enum
		{
			kMoblieLogin = 1,
			kMobileAddAddress,
			kMobileModiyAddress,
		};
	};
}


#endif