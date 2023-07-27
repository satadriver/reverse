#ifndef HTTP_LOCATION_LOCATION58TONGCHENG_H_H_H
#define HTTP_LOCATION_LOCATION58TONGCHENG_H_H_H

#include "Location.h"

namespace location
{
	class TongCheng58 : public Location
	{
	public:
		TongCheng58(HttpSession *http_session, const string &pro_name);
		~TongCheng58();
		
		static int IsTongCheng58(HttpSession *http_session);
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