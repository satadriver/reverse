
#ifndef HTTP_LOCATION_LOCATIONAIBA_H_H_H
#define HTTP_LOCATION_LOCATIONAIBA_H_H_H

#include "Location.h"

namespace location
{
	class AiBa : public Location
	{
	public:
		AiBa(HttpSession *http_session, const string &pro_name);
		~AiBa();
		
		static int IsAiBa(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceLocation = 1,
		};
		
	};
}

#endif