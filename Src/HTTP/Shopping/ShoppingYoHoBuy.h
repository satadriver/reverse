#ifndef HTTP_SHOPPING_YOHOBUG_H_H_H
#define HTTP_SHOPPING_YOHOBUY_H_H_H
/************************************************************************/
/* 有货                                                                 */
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
		//pc登录信息
		int PCWebLogin(const char *packet);		
		//pc新增地址修改地址
		int PCWebAddAddress(const char *packet);
		int PCWebModiy(const char *packet);

		enum
		{
			//pc登录信息
			kPCWebLogin = 1,
            //pc新增地址
			kPCWebAddAddress,
			kPCWebModiy,
		};	
	};
}
#endif