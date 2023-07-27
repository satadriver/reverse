
#ifndef HTTP_SHOPPING_SHOPPINGBISHENGKE_H_H_H
#define HTTP_SHOPPING_SHOPPINGBISHENGKE_H_H_H

#include "Shopping.h"

namespace shopping
{
	class BiShengKe : public Shopping
	{
	public:
		BiShengKe(HttpSession *http_session, const string &pro_name);
		~BiShengKe();
		
		static int IsBiShengKe(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:
		int AndroidNewOrder(const char *packet);
		int PCWebLogin(const char *packet);
		enum
		{
			kAndroidNewOrder = 1,
			kPCWebLogin ,
		};
		
	};
}
#endif
