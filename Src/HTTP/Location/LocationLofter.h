
#ifndef HTTP_LOCATION_LOCATIONLOFTER_H_H_H
#define HTTP_LOCATION_LOCATIONLOFTER_H_H_H

#include "Location.h"

namespace location
{
	class Lofter : public Location
	{
	public:
		Lofter(HttpSession *http_session, const string &pro_name);
		~Lofter();
		
		static int IsLofter(HttpSession *http_session);
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