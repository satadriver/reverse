
#ifndef HTTP_LOCATION_LOCATIONBAIHE_H_H_H
#define HTTP_LOCATION_LOCATIONBAIHE_H_H_H

#include "Location.h"

namespace location
{
	class BaiHe : public Location
	{
	public:
		BaiHe(HttpSession *http_session, const string &pro_name);
		~BaiHe();
		
		static int IsBaiHe(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceInfo = 1,
				kDeviceLocation,
		};
		

	};
}

#endif