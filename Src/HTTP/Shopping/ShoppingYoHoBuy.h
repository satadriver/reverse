#ifndef HTTP_SHOPPING_YOHOBUG_H_H_H
#define HTTP_SHOPPING_YOHOBUY_H_H_H
/************************************************************************/
/* �л�                                                                 */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class YoHoBuy: public Shopping
	{
	public:
		YoHoBuy(HttpSession *http_session, const string &pro_name);
		~YoHoBuy();	
		static int IsYoHoBuy(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		//pc��¼��Ϣ
		int PCWebLogin(const char *packet);		
		//pc������ַ�޸ĵ�ַ
		int PCWebAddAddress(const char *packet);
		int PCWebModiy(const char *packet);

		enum
		{
			//pc��¼��Ϣ
			kPCWebLogin = 1,
            //pc������ַ
			kPCWebAddAddress,
			kPCWebModiy,
		};	
	};
}
#endif