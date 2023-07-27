
#ifndef HTTP_LOCATION_LOCATIONLEODNGLI_H_H_H
#define HTTP_LOCATION_LOCATIONLEODNGLI_H_H_H

#include "Location.h"

namespace location
{
	class LeDongLi : public Location
	{
	public:
		LeDongLi(HttpSession *http_session, const string &pro_name);
		~LeDongLi();
		
		static int IsLeDongLi(HttpSession *http_session);
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