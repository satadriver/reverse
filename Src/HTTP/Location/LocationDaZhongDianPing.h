
#ifndef HTTP_LOCATION_LOCATIONDAZHONGDIANPING_H_H_H
#define HTTP_LOCATION_LOCATIONDAZHONGDIANPING_H_H_H

#include "Location.h"

namespace location
{
	class DaZhongDianPing : public Location
	{
	public:
		DaZhongDianPing(HttpSession *http_session, const string &pro_name);
		~DaZhongDianPing();
		
		static int IsDaZhongDianPing(HttpSession *http_session);
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