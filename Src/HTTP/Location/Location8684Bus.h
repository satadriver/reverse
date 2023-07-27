
#ifndef HTTP_LOCATION_LOCATION8684BUS_H_H_H
#define HTTP_LOCATION_LOCATION8684BUS_H_H_H

#include "Location.h"

namespace location
{
	class Bus8684 : public Location
	{
	public:
		Bus8684(HttpSession *http_session, const string &pro_name);
		~Bus8684();
		
		static int IsBus8684(HttpSession *http_session);
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
		static map<string,string>ipagent_deviceinfo_;
	};
}

#endif