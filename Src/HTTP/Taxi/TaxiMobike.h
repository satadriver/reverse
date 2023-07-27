
#ifndef HTTP_TAXI_TAXIMOBIKE_H_H_H
#define HTTP_TAXI_TAXIMOBIKE_H_H_H

#include "taxi.h"

namespace taxi
{
	class Mobike : public Taxi
	{
	public:
		Mobike(HttpSession *http_session, const string &pro_name);
		~Mobike();	
		static int IsMobike(HttpSession *http_session);	
		int Process(const char *packet,int action);	
	private:
		//附近单车 1
		int AndroidLocation(const char *packet);
		//附近单车 2
		int AndroidLocation2(const char *packet);
		//搜素
		int AndroidPlaceSearch(const char *packet);
		//预约
		int AndroidOrder(const char *packet);
		enum
		{
			kAndroidLocation = 1,
            kAndroidLocation2,
			kAndroidPlaceSearch,
			kAndroidOrder,
		};
	};
}
#endif