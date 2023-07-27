
#ifndef HTTP_TAXI_TAXIKUAIDI_H_H_H
#define HTTP_TAXI_TAXIKUAIDI_H_H_H

#include "taxi.h"

namespace taxi
{
	class KuaiDi : public Taxi
	{
	public:
		KuaiDi(HttpSession *http_session, const string &pro_name);
		~KuaiDi();
		
		static int IsKuaiDi(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	private:

		int PassengerLogin(const char *packet);
		int NewTextOrder(const char *packet);
		int NewVoiceOrder(const char *packet);
		int PreOrder(const char *packet);

		int DriverRegister(const char *packet);
		int DriverLogin(const char *packet);
		int DriverWorking(const char *packet);
		enum
		{
			kCMD_a = 1,
			kCMD_p,
			kCMD_d,
				kPassengerLogin,
				kNewTextOrder,
				kNewVoiceOrder,
				kPreOrder,
				kDriverRegister,
				kDriverLogin,
				kDriverWorking
		};


	
	};
}

#endif