
#ifndef HTTP_SHOPPING_JINGDONG_H_H_H
#define HTTP_SHOPPING_JINGDONG_H_H_H

#include "Shopping.h"


namespace shopping
{
	class JingDong : public Shopping
	{
	public:
		JingDong(HttpSession *http_session, const string &pro_name);
		~JingDong();
		
		static int IsJingDong(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:
		
		int PCWebNewOrder(const char *packet);

		// Android + iOS
		int PhoneNewOrder(const char *packet);

		int AndroidNewOrder(const char *packet); 
		int IOSNewOrder(const char *packet);
		enum
		{
			kPCWebNewOrder = 1,
			kPhoneNewOrder,
		};
	};
}


#endif