#ifndef HTTP_SHOPPING_KUAILEGOU_H_H_H
#define HTTP_SHOPPING_KUAILEGOU_H_H_H
/************************************************************************/
/* 快乐购                                                               */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class KuaiLeGou: public Shopping
	{
	public:
		KuaiLeGou(HttpSession *http_session, const string &pro_name);
		~KuaiLeGou();	
		static int IsKuaiLeGou(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		//pc登录信息
		int PCWebLogin(const char *packet);		
		//pc新增地址
		int PCWebAddAddress(const char *packet);
		//pc修改地址
		int PCWebModifyAddress(const char *packet);	

		enum
		{
			//pc登录信息
			kPCWebLogin = 1,
            //pc新增地址
			kPCWebAddAddress,
			//pc修改地址
			kPCWebModifyAddress,
		};	
	};
}
#endif