
#ifndef HTTP_SHOPPING_SHOPPINGFANKE_H_H_H
#define HTTP_SHOPPING_SHOPPINGFANKE_H_H_H

#include "Shopping.h"

namespace shopping
{
	class FanKe : public Shopping
	{
	public:
		FanKe(HttpSession *http_session, const string &pro_name);
		~FanKe();
		static int IsFanKe(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		int PhoneAddAddress(const char *packet);
		enum
		{
			kPhoneAddAddress = 1,
		};
	};
}


#endif // !defined(AFX_PHONESHOPJINGDONG_H__653E5D95_7641_4B4F_B3F6_9B17ABF3816E__INCLUDED_)
