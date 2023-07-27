#ifndef HTTP_SHOPPING_MIYA_H_H_H
#define HTTP_SHOPPING_MIYA_H_H_H
/************************************************************************/
/* 蜜芽                                                                    */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class MiYa: public Shopping
	{
	public:
		MiYa(HttpSession *http_session, const string &pro_name);
		~MiYa();	
		static int IsMiYa(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		//pc登录信息
		int PCWebLogin(const char *packet);		
		//pc新增地址
		int PCWebAddAddress(const char *packet);
		//android 登录\新增\修改地址
		int AndroidLogonAddress(const char *packet);	   
		enum
		{
			//pc登录信息
			kPCWebLogin = 1,
            //pc新增地址
			kPCWebAddAddress,
            //android 登录\新增\修改地址
			kAndroidLogonAddress,
		};	
	};
}
#endif