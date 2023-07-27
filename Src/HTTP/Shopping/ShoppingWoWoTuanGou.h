#ifndef HTTP_SHOPPING_WOWOTUANGOU_H_H_H
#define HTTP_SHOPPING_WOWOTUANGOU_H_H_H


#include "Shopping.h"
namespace shopping
{
	class WoWoTuanGou : public Shopping
	{
	public:
		WoWoTuanGou(HttpSession *http_session, const string &pro_name);
		~WoWoTuanGou();
		
		static int IsWoWoTuanGou(HttpSession *http_session );
		
		int Process(const char *packet,int action);
		
	private:
		
		int PCWebAddAddress(const char *packet);
		int PCWebLogin(const char *packet);
		int PCWebShenBian(const char *packet);
		enum
		{
			kPCWebAddAddress = 1,
				kPCWebLogin = 2,
				kPCWebShenBian = 3
		};
	};
}
#endif