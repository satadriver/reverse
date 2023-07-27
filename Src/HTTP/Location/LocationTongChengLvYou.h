#ifndef HTTP_LOCATION_LOCATIONTONGCHENGLVYOU_H_H_H
#define HTTP_LOCATION_LOCATIONTONGCHENGLVYOU_H_H_H

#include "Location.h"

namespace location
{
	class TongChengLvYou : public Location
	{
	public:
		TongChengLvYou(HttpSession *http_session, const string &pro_name);
		~TongChengLvYou();
		
		static int IsTongChengLvYou(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int Deviceinfo(const char *packet);
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceLocation = 1
		};
		
		//<UID,IMEI_IMSI>
		static map<string,string>uid_deviceinfo_;
	};
}

#endif