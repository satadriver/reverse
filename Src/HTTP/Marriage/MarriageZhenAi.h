#ifndef HTTP_MARRIAGE_ZHENAI_H_H_H
#define HTTP_MARRIAGE_ZHENAI_H_H_H

#include "Marriage.h"

namespace marriage
{
	class ZhenAiMarriage: public Marriage
	{
	public:
		ZhenAiMarriage(HttpSession *http_session, const string &pro_name);
		~ZhenAiMarriage();
		static int IsZhenAiMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ÍøÒ³×¢²á
		int PcWebRegister(const char *packet);
		//ÍøÒ³µÇÂ¼
		int PcWebLogin(const char *packet);
		//ÍøÒ³ĞŞ¸Ä×ÊÁÏ
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