
#ifndef HTTP_LOCATION_LOCATIONZAKER_H_H_H
#define HTTP_LOCATION_LOCATIONZAKER_H_H_H

#include "Location.h"

namespace location
{
	class Zaker : public Location
	{
	public:
		Zaker(HttpSession *http_session, const string &pro_name);
		~Zaker();
		
		static int IsZaker(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceLocation = 1,
		};
		
		//<UID,IMEI_IMSI>
		static map<string,string>uid_deviceinfo_;
	};
}

#endif