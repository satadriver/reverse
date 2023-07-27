#ifndef HTTP_SHOPPING_MIZHE_H_H_H
#define HTTP_SHOPPING_MIZHE_H_H_H
/************************************************************************/
/*米折                                                                  */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class MiZhe: public Shopping
	{
	public:
		MiZhe(HttpSession *http_session, const string &pro_name);
		~MiZhe();	
		static int IsMiZhe(HttpSession *http_session );	
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
		//pc位置信息 地区 86 中国
		int PCWebLocation(const char *packet);	

		//android登录信息
		int AndroidLogin(const char *packet);			
		//android新增地址 
		int AndroidAddAddress(const char *packet);  
		//android修改地址
		int AndroidModifyAddress(const char *packet); 
		//android提交订单
		int AndroidNewOrder(const char *packet);		
		//android位置信息
		int AndroidLocation(const char *packet);      
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
			//pc位置信息 地区 86 中国
			kPCWebLocation,
			//android登录信息
			kAndroidLogin,
			//android新增地址
			kAndroidAddAddress,
			//android修改地址
			kAndroidModifyAddress,
			//android提交订单
			kAndroidNewOrder,
			//android位置信息
			kAndroidLocation,
		};	
	};
}
#endif