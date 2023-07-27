
#ifndef HTTP_LOCATION_LOCATIONKUAILEBEIYUN_H_H_H
#define HTTP_LOCATION_LOCATIONKUAILEBEIYUN_H_H_H

#include "Location.h"

namespace location
{
	class KuaiLeBeiYun : public Location
	{
	public:
		KuaiLeBeiYun(HttpSession *http_session, const string &pro_name);
		~KuaiLeBeiYun();
		
		static int IsKuaiLeBeiYun(HttpSession *http_session);
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