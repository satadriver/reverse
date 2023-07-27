
#ifndef HTTP_SHOPPING_TAOBAO_H_H_H
#define HTTP_SHOPPING_TAOBAO_H_H_H

#include "Shopping.h"

namespace shopping
{
	class TaoBao : public Shopping
	{
	public:
		TaoBao(HttpSession *http_session, const string &pro_name);
		~TaoBao();
		
		static int IsTaoBao(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:
		
		enum
		{
			kPCWebNewOrder = 1,
			kSellerDeliverGoods
		};
		
		int PCWebNewOrder(const char *packet);
		int SellerDeliverGoods(const char *packet);
		
		string GetLogsticsCompanyName(const string &company_code);
		
		string fee_;
	};
}


#endif