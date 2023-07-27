
#ifndef HTTP_TIANMAO_H_H_H
#define HTTP_TIANMAO_H_H_H

#include "../HttpPro.h"


class TianMao : public HttpPro
{
public:
	TianMao();
	~TianMao();
	
	int Is(HttpSession *http_session);
	
	int Process(const char *packet,int action);
	
private:
	
	int NewOrderCW(const char *packet);
	int SellerDeliverGoods(const char *packet);
	enum
	{
		kNewOrderCW = 1,
			kSellerDeliverGoods
	};

	
};


#endif