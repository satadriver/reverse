#ifndef HTTP_SHOPPING_SHOPPINGKFC_H_H_H
#define HTTP_SHOPPING_SHOPPINGKFC_H_H_H

#include "Shopping.h"

namespace shopping
{
	class KFC : public Shopping
	{
	public:
		KFC(HttpSession *http_session, const string &pro_name);
		~KFC();
		
		static int IsKFC(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
		
	private:
		int AndroidNewOrder(const char *packet);
		int PCWebLogin(const char *packet);
		enum
		{
			kAndroidNewOrder = 1,
				kPCWebLogin
		};
		
	};
}

#endif 
