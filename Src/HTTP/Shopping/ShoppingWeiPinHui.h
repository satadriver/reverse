#ifndef HTTP_SHOPPING_WEIPINHUI_H_H_H
#define HTTP_SHOPPING_WEIPINHUI_H_H_H

#include "Shopping.h"
namespace shopping
{
	class WeiPinHui : public Shopping
	{
	public:
		WeiPinHui(HttpSession *http_session, const string &pro_name);
		~WeiPinHui();
		
		static int IsWeiPinHui(HttpSession *http_session );
		
		int Process(const char *packet,int action);
		
	private:
		
		int PCWebAddAddress(const char *packet);
		enum
		{
			kPCWebAddAddress = 1
		};
		
	};
}
#endif