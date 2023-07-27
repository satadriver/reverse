
#ifndef HTTP_LOCATION_LOCATIONYUENI_H_H_H
#define HTTP_LOCATION_LOCATIONYUENI_H_H_H

#include "Location.h"

namespace location
{
	class YueNi : public Location
	{
	public:
		YueNi(HttpSession *http_session, const string &pro_name);
		~YueNi();
		
		static int IsYueNi(HttpSession *http_session);
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