
#ifndef HTTP_LOCATION_LOCOTAINMAMAQUAN_H_H_H
#define HTTP_LOCATION_LOCOTAINMAMAQUAN_H_H_H

#include "Location.h"

namespace location
{
	class MaMaQuan : public Location
	{
	public:
		MaMaQuan(HttpSession *http_session, const string &pro_name);
		~MaMaQuan();
		
		static int IsMaMaQuan(HttpSession *http_session);
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
		static map<string,string>uid_postion_;
	};
}

#endif