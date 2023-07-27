#ifndef HTTP_TAXI_TAXIDIDIZHUANCHE_H_H_H
#define HTTP_TAXI_TAXIDIDIZHUANCHE_H_H_H

#include "taxi.h"

namespace taxi
{
	class DiDiZhuanChe : public Taxi
	{
	public:
		DiDiZhuanChe(HttpSession *http_session, const string &pro_name);
		~DiDiZhuanChe();
		
		static int IsDiDiZhuanChe(HttpSession *http_session);
		
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