
#ifndef HTTP_REALNAME_DONGHANG_H_H_H
#define HTTP_REALNAME_DONGHANG_H_H_H

#include "RealName.h"

namespace realname
{
	class DongHang : public RealName
	{
	public:
		DongHang(HttpSession *http_session, const string &pro_name);
		~DongHang();
		
		static int IsDongHang(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int BuyFlightTicket(const char *packet);
		enum
		{
			kBuyFlightTicket = 2
		};
	};
}

#endif