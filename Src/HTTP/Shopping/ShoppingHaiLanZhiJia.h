#ifndef HTTP_SHOPPING_HAILANZHIJIA_H_H_H
#define HTTP_SHOPPING_HAILANZHIJIA_H_H_H
/************************************************************************/
/* 海澜之家                                                             */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class HaiLanZhiJia: public Shopping
	{
	public:
		HaiLanZhiJia(HttpSession *http_session, const string &pro_name);
		~HaiLanZhiJia();	
		static int IsHaiLanZhiJia(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		//pc登录信息
		int PCWebLogin(const char *packet);		
		//pc新增地址修改地址
		int PCWebAddAddress(const char *packet);
		int PCWebModiyAdd(const char *packet);
		//手机端登录信息
		int MobileLogin(const char *packet);
		//手机端新增修改地址
		int MobileAddress(const char *packet);
		int MobileModiy(const char *packet);

		enum
		{
			//pc登录信息
			kPCWebLogin = 1,
            //pc新增地址
			kPCWebAddAddress,
			kPCWebModiyAdd,
			//手机端登录
			kMobileLogin,
			//手机端新增修改地址
			KMobileAddress,
			kMobileModiy,
		};	
	};
}
#endif