
#ifndef HTTP_SHOPPING_TMALL_H_H_H
#define HTTP_SHOPPING_TMALL_H_H_H

#include "Shopping.h"

namespace shopping
{
	class TMall : public Shopping
	{
	public:
		TMall(HttpSession *http_session, const string &pro_name);
		~TMall();
		
		static int IsTMall(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:
		
		int PCWebNewOrder(const char *packet);
		int SellerDeliverGoods(const char *packet);
		enum
		{
			kPCWebNewOrder = 1,
			kSellerDeliverGoods
		};
		string fee_;
		
	};

}

#endif