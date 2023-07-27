
#ifndef HTTP_SHOPPING_HANDUYISHE_H_H_H
#define HTTP_SHOPPING_HANDUYISHE_H_H_H

#include "Shopping.h"


namespace shopping
{
	class HanDUYiShe : public Shopping
	{
	public:
		HanDUYiShe(HttpSession *http_session, const string &pro_name);
		~HanDUYiShe();
		
		static int IsHanDUYiShe(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:
		int PCWebEditAddress(const char *packet);
		int PCWebAddAddress(const char *packet);
		int PCLogin(const char *packet);
		enum
		{
			kPCLogin = 1,
			kPCWebAddAddress,	
			kPCWebEditAddress,
		};
	};
}


#endif