#ifndef HTTP_LOCATIONHEMAP_H_H_H
#define HTTP_LOCATIONHEMAP_H_H_H

#include "Location.h"

namespace location
{
	class HeMap : public Location
	{
	public:
		HeMap(HttpSession *http_session, const string &pro_name);
		~HeMap();
		
		static int IsHeMap(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceLocation = 1
		};
	};
}

#endif