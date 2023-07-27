#ifndef HTTP_SHOPPING_ZHE800_H_H_H
#define HTTP_SHOPPING_ZHE800_H_H_H
/************************************************************************/
/* 折800                                                                     */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class Zhe800: public Shopping
	{
	public:
		Zhe800(HttpSession *http_session, const string &pro_name);
		~Zhe800();	
		static int IsZhe800(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		//pc新增地址
		int PCWebAddAddress(const char *packet);
		//pc修改地址
		int PCWebModifyAddress(const char *packet);	

		//android新增地址 
		int AndroidAddAddress(const char *packet);  
		//android修改地址
		int AndroidModifyAddress(const char *packet); 
		//android提交订单
		int AndroidNewOrder(const char *packet);		
		enum
		{
            //pc新增地址
			kPCWebAddAddress = 1,
			//pc修改地址
			kPCWebModifyAddress,
			//android新增地址
			kAndroidAddAddress,
			//android修改地址
			kAndroidModifyAddress,
			//android提交订单
			kAndroidNewOrder,
		};	
	};
}
#endif