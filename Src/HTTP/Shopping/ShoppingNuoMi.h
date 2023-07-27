
#ifndef HTTP_SHOPPING_SHOPPINGNUOMIWANG_H_H_H
#define HTTP_SHOPPING_SHOPPINGNUOMIWANG_H_H_H

#include "Shopping.h"

namespace shopping
{
	class NuoMi : public Shopping
	{
	public:
		NuoMi(HttpSession *http_session, const string &pro_name);
		~NuoMi();
		
		static int IsNuoMi(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
		
	private:
		int AndroidLocation(const char *packet);
		enum
		{
			kAndroidLocation = 1,
		};
		
	};
}


#endif // !defined(AFX_PHONESHOPJINGDONG_H__653E5D95_7641_4B4F_B3F6_9B17ABF3816E__INCLUDED_)
