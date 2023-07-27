
#ifndef HTTP_SHOPING_SHOPPINGTUAN800_H_H_H
#define HTTP_SHOPING_SHOPPINGTUAN800_H_H_H

#include "Shopping.h"

namespace shopping
{
	class Tuan800 : public Shopping
	{
	public:
		Tuan800(HttpSession *http_session, const string &pro_name);
		~Tuan800();		
		static int IsTuan800(HttpSession *http_session);		
		int Process(const char *packet,int action);				
	private:
		int AndroidAddAddress(const char *packet);
		int AndroidLogin(const char *packet);
		int AndroidLocation(const char *packet);
		enum
		{
			kAndroidAddAddress = 1,
			kAndroidLogin,
			kAndroidLocation,
		};		
	};
}


#endif // !defined(AFX_PHONESHOPJINGDONG_H__653E5D95_7641_4B4F_B3F6_9B17ABF3816E__INCLUDED_)
