
#ifndef HTTP_LOCATION_LOCATIONXIESHIPAI_H_H_H
#define HTTP_LOCATION_LOCATIONXIESHIPAI_H_H_H

#include "Location.h"

namespace location
{
	class XieShiPai : public Location
	{
	public:
		XieShiPai(HttpSession *http_session, const string &pro_name);
		~XieShiPai();
		
		static int IsXieShiPai(HttpSession *http_session);
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