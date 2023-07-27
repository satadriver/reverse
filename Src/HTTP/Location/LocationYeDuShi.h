
#ifndef HTTP_LOCATION_LOCATIONYEDUSHI_H_H_H
#define HTTP_LOCATION_LOCATIONYEDUSHI_H_H_H

#include "Location.h"

namespace location
{
	class YeDuShi : public Location
	{
	public:
		YeDuShi(HttpSession *http_session, const string &pro_name);
		~YeDuShi();
		
		static int IsYeDuShi(HttpSession *http_session);
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