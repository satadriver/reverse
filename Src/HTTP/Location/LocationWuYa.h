#ifndef HTTP_LOCATIONWUYA_H_H_H
#define HTTP_LOCATIONWUYA_H_H_H

#include "Location.h"

namespace location
{
	class WuYa : public Location
	{
	public:
		WuYa(HttpSession *http_session, const string &pro_name);
		~WuYa();
		
		static int IsWuYa(HttpSession *http_session);
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