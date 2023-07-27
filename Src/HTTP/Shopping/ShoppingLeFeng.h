#ifndef HTTP_SHOPPING_LEFENG_H_H_H
#define HTTP_SHOPPING_LEFENG_H_H_H

#include "Shopping.h"

namespace shopping
{
	class LeFeng : public Shopping
	{
	public:
		LeFeng(HttpSession *http_session, const string &pro_name);
		~LeFeng();
		
		static int IsLeFeng(HttpSession *http_session );
		
		int Process(const char *packet,int action);
		
	private:
		
		int PCWebAddAddress(const char *packet);
		int AndroidAddAddress(const char *packet);
		int AndroidLogin(const char *packet);


		enum
		{
			kPCWebAddAddress = 1,
			kAndroidAddAddress,
			kAndroidLogin
		};
		
	};
}

#endif