
#ifndef HTTP_TAXI_TAXIDIDI_H_H_H
#define HTTP_TAXI_TAXIDIDI_H_H_H

#include "taxi.h"

namespace taxi
{
	class DiDi : public Taxi
	{
	public:
		DiDi(HttpSession *http_session, const string &pro_name);
		~DiDi();
		
		static int IsDiDi(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:

		int PassengerLogin(const char *packet);
		int NewOrder(const char *packet);
		int TextNewOrder(const char *packet);
		int VoiceNewOrder(const char *packet);
		int PreOrder(const char *packet);

		int DriverRegister(const char *packet);
		int DriverLogin(const char *packet);
		
		enum
		{
			kPassengerLogin = 1,
				kNewOrder,
				kPreOrder,
				kDriverRegister,
				kDriverLogin,
				kZCDriverLogin,
		};
		//<IMEI,PHONE>
		static map<string,string>imei_phone_;
	};
}

#endif