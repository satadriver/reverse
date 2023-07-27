#ifndef HTTP_SHOPPING_SHOPPINGYOUGOU_H_H_H
#define HTTP_SHOPPING_SHOPPINGYOUGOU_H_H_H

#include "Shopping.h"

namespace shopping
{
	class YouGou : public Shopping
	{
	public:
		YouGou(HttpSession *http_session, const string &pro_name);
		~YouGou();	
		static int IsYouGou(HttpSession *http_session);		
		int Process(const char *packet,int action);
		
	private:
		int PcAddress(const char *packet);
		int PCcreateOrder(const char *packet);
		int MobileLogin(const char *packet);
		int MobileEditAddress(const char *packet);
		int MobileAddAddress(const char *packet);
		enum
		{
			kPcAddress = 1,
			kPCcreateOrder,
			kMobileLogin,
			kMobileEditAddress,
			KMobileAddAddress
		};		
	};
}

#endif 
