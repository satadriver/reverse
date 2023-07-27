#ifndef HTTP_SHOPPING_SHOPPINGMAILEGOU_H_H_H
#define HTTP_SHOPPING_SHOPPINGMAILEGOU_H_H_H
/************************************************************************/
/* 麦乐购                                                                     */
/************************************************************************/
#include "Shopping.h"

namespace shopping
{
	class MaiLeGOu : public Shopping
	{
	public:
		MaiLeGOu(HttpSession *http_session, const string &pro_name);
		~MaiLeGOu();		
		static int IsMaiLeGOu(HttpSession *http_session);	
		int Process(const char *packet,int action);	
	private:
		//pc登录信息
		int PCWebLogin(const char *packet);		
		//IE pc新增地址
		int PCWebAddAddress(const char *packet);
		//Google pc新增地址
		int PCWebGoogleAddAddress(const char *packet);
		//pc触屏版新增地址
		int PCWebChuPingAddAddress(const char *packet);
		//pc修改地址
		int PCWebModifyAddress(const char *packet);	
		//pc提交订单
		int PCWebNewOrder(const char *packet);		
		enum
		{
			//pc登录信息
			kPCWebLogin = 1,
            //IE pc新增地址
			kPCWebAddAddress,
			//Google pc 新增地址
			kPCWebGoogleAddAddress,
            //pc触屏版新增地址
			kPCWebChuPingAddAddress,	
			//pc修改地址
			kPCWebModifyAddress,
			//pc提交订单
			kPCWebNewOrder,
		};		
	};
}

#endif 
