
#ifndef HTTP_LOCATION_LOCATIONDAMAI_H_H_H
#define HTTP_LOCATION_LOCATIONDAMAI_H_H_H

#include "Location.h"

namespace location
{
	class DaMai : public Location
	{
	public:
		DaMai(HttpSession *http_session, const string &pro_name);
		~DaMai();
		
		static int IsDaMai(HttpSession *http_session);
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