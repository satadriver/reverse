
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
		//�������� 1
		int AndroidLocation(const char *packet);
		//�������� 2
		int AndroidLocation2(const char *packet);
		//����
		int AndroidPlaceSearch(const char *packet);
		//ԤԼ
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