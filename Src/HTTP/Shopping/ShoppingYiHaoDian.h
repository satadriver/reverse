#ifndef HTTP_SHOPPING_YIHAODIAN_H_H_H
#define HTTP_SHOPPING_YIHAODIAN_H_H_H


#include "Shopping.h"

namespace shopping
{
	class YiHaoDian : public Shopping
	{
	public:
		YiHaoDian(HttpSession *http_session, const string &pro_name);
		~YiHaoDian();	
		static int IsYiHaoDian(HttpSession *http_session);		
		int Process(const char *packet,int action);		
	private:		
		int PCWebAddAddress(const char *packet);
		enum
		{
			kPCWebAddress = 1
		};
	};
}

#endif