#ifndef HTTP_TAXI_TAXIYIDAOYONGCHE_H_H_H
#define HTTP_TAXI_TAXIYIDAOYONGCHE_H_H_H

#include "taxi.h"

namespace taxi
{
	class YiDaoYongChe : public Taxi
	{
	public:
		YiDaoYongChe(HttpSession *http_session, const string &pro_name);
		~YiDaoYongChe();
		
		static int IsYiDaoYongChe(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:
		
		int DriverLogin(const char *packet);
		
		enum
		{
			kDriverLogin = 1,
		};
		
	};
}

#endif