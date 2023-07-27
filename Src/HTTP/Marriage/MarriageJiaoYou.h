#ifndef HTTP_MARRIAGE_JIAOYOU_H_H_H
#define HTTP_MARRIAGE_JIAOYOU_H_H_H

#include "Marriage.h"

namespace marriage
{
	class JiaoYouMarriage: public Marriage
	{
	public:
		JiaoYouMarriage(HttpSession *http_session, const string &pro_name);
		~JiaoYouMarriage();
		static int IsJiaoYouMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:

		//×¢²á
		int WebRegister(const char *packet);
		//µÇÂ¼
		int WebLogin(const char *packet);
		//webÐÞ¸Ä×ÊÁÏs
		int WebModify(const char *packet);
		enum
		{
			kWebRegister = 1,
			kWebLogin,
			kWebModify,
		};
	};
}

#endif