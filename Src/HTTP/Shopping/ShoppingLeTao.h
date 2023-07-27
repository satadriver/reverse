
#ifndef HTTP_SHOPPING_LETAO_H_H_H
#define HTTP_SHOPPING_LETAO_H_H_H

#include "Shopping.h"

namespace shopping
{
	class LeTao : public Shopping
	{
	public:
		LeTao(HttpSession *http_session, const string &pro_name);
		~LeTao();		
		static int IsLeTao(HttpSession *http_session);		
		int Process(const char *packet,int action);		
	private:

		//pc登录
		int PCLogin(const char *packet);
		//pc新增地址 修改地址 
		int PCWebAddAddress(const char *packet);
		//pc提交订单
		int PCWebOrder(const char *packet);

		enum
		{
			kPCLogin = 1,
			kPCWebAddAddress,
			kPCWebOrder,
		};
	};
}


#endif