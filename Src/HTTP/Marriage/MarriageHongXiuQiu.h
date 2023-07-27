#ifndef HTTP_MARRIAGE_HONGXIUQIU_H_H_H
#define HTTP_MARRIAGE_HONGXIUQIU_H_H_H

#include "Marriage.h"

namespace marriage
{
	class HongXiuQiuMarriage: public Marriage
	{
	public:
		HongXiuQiuMarriage(HttpSession *http_session, const string &pro_name);
		~HongXiuQiuMarriage();
		static int IsHongXiuQiuMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ÍøÒ³×¢²á
		int PcWebRegister(const char *packet);
		//ÍøÒ³µÇÂ¼
		int PcWebLogin(const char *packet);
		//ÍøÒ³ÐÞ¸Ä×ÊÁÏ
		int PcWebModiy(const char *packet);

		enum
		{
			kPcWebRegister = 1,
			kPcWebLogin,
			kPcWebModiy,
		};
	};
}

#endif