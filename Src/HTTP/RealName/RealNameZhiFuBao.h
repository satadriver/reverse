
#ifndef HTTP_REALNAME_REALNAMEZHIFUBAO_H_H_H
#define HTTP_REALNAME_REALNAMEZHIFUBAO_H_H_H

#include "RealName.h"

namespace realname
{
	class ZhiFuBao : public RealName
	{
	public:
		ZhiFuBao(HttpSession *http_session, const string &pro_name);
		~ZhiFuBao();
		
		static int IsZhiFuBao(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int BuyTrainTicket(const char *packet);
		int BuyFlightTicket(const char *packet);
		enum
		{
			kBuyTrainTicket = 1,
				kBuyFlightTicket = 2
		};
	};
}

#endif