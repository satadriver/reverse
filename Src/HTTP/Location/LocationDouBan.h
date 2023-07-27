
#ifndef HTTP_LOCATION_LOCATIONDOUBAN_H_H_H
#define HTTP_LOCATION_LOCATIONDOUBAN_H_H_H

#include "Location.h"

namespace location
{
	class DouBan : public Location
	{
	public:
		DouBan(HttpSession *http_session, const string &pro_name);
		~DouBan();
		
		static int IsDouBan(HttpSession *http_session);
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