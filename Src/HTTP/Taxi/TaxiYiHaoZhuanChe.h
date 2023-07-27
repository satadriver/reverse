#ifndef HTTP_TAXI_TAXIYIHAOZHUANCHE_H_H_H
#define HTTP_TAXI_TAXIYIHAOZHUANCHE_H_H_H

#include "taxi.h"

namespace taxi
{
	class YiHaoZhuanChe : public Taxi
	{
	public:
		YiHaoZhuanChe(HttpSession *http_session, const string &pro_name);
		~YiHaoZhuanChe();
		
		static int IsYiHaoZhuanChe(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:
		
		int ZCDriverLogin(const char *packet);

		enum
		{
			kZCDriverLogin = 1,
		};
		
	};
}

#endif