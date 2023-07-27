#ifndef HTTP_MARRIAGE_TAONAN_H_H_H
#define HTTP_MARRIAGE_TAONAN_H_H_H

#include "Marriage.h"

namespace marriage
{
	class TaoNanMarriage: public Marriage
	{
	public:
		TaoNanMarriage(HttpSession *http_session, const string &pro_name);
		~TaoNanMarriage();
		static int IsTaoNanMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//webÌÔÄĞ (ĞŞ¸Ä×ÊÁÏ µÇÂ¼ QQ×¢²á)
		int WebTaoNan(const char *packet);

		enum
		{
			kWebTaoNan = 1,
		};
	};
}

#endif