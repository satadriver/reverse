#ifndef HTTP_SHOPPING_KAOLAHAIGOU_H_H_H
#define HTTP_SHOPPING_KAOLAHAIGOU_H_H_H
/************************************************************************/
/* 网易考拉海购       
web:	 提交订单-	收货人账号(登录账号) 收货人地址(地址码) 添加地址-账户姓名，手机号，地址
android: 提交订单-	收货人姓名，电话(*)，收货人地址(地址码) 添加地址-账户姓名，手机号，地址(地址码)                                                */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class KaoLaHaiGou: public Shopping
	{
	public:
		KaoLaHaiGou(HttpSession *http_session, const string &pro_name);
		~KaoLaHaiGou();	
		static int IsKaoLaHaiGou(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		int PCWebAddAddress(const char *packet);	//pc添加地址
		int PCWebModifyAddress(const char *packet);	//pc修改地址
		int PCWebNewOrder(const char *packet);		//pc提交订单 

		int AndroidAddAddress(const char *packet);		//android添加地址
		int AndroidModifyAddress(const char *packet);	//android修改地址
		int AndroidNewOrder(const char *packet);		//android提交订单
		enum
		{
			kPCWebAddAddress = 1,
			kPCWebModifyAddress,
			kPCWebNewOrder,
			kAndroidAddAddress,
			kAndroidModifyAddress,
			kAndroidNewOrder,
		};	
	};
}
#endif