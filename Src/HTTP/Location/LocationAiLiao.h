
#ifndef HTTP_LOCATION_LOCATIONAILIAO_H_H_H
#define HTTP_LOCATION_LOCATIONAILIAO_H_H_H

#include "Location.h"

namespace location
{
	class AiLiao : public Location
	{
	public:
		AiLiao(HttpSession *http_session, const string &pro_name);
		~AiLiao();
		
		static int IsAiLiao(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int Deviceinfo(const char *packet);
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceInfo = 1,
				kDeviceLocation,
		};
		
		//<UID,IMEI_IMSI>
		static map<string,string>uid_deviceinfo_;
	};
}

#endif