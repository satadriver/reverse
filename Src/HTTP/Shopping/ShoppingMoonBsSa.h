#ifndef HTTP_SHOPPING_MOONBASA_H_H_H
#define HTTP_SHOPPING_MOONBASA_H_H_H
/************************************************************************/
/* 梦芭莎                                                               */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class MoonBaSa: public Shopping
	{
	public:
		MoonBaSa(HttpSession *http_session, const string &pro_name);
		~MoonBaSa();	
		static int IsMoonBaSa(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		//pc登录信息
		int PCWebLogin(const char *packet);		
		//pc新增地址
		int PCWebAddAddress(const char *packet);
		//pc修改地址
		int PCWebModifyAddress(const char *packet);	
		//pc提交订单
		int PCWebNewOrder(const char *packet);		
    
		//android登录
		int AndroidLogin(const char *packet);
		//android新增地址
		int AndroidAddAddress(const char *packet);
		//android修改地址
		int AndroidEditAddress(const char *packet);
		//android提交订单
		int AndroidOrder(const char *packet);
		enum
		{
			//pc登录信息
			kPCWebLogin = 1,
            //pc新增地址
			kPCWebAddAddress,
			//pc修改地址
			kPCWebModifyAddress,
			//pc提交订单
			kPCWebNewOrder,
			//android登录
			kAndroidLogin,
			//android新增地址
			kAndroidAddAddress,
			//android修改地址
			kAndroidEditAddress,
			//android提交订单
			kAndroidOrder,
		};	
	};
}
#endif