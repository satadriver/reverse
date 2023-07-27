#ifndef HTTP_LOCATIONJISHITIANQI_H_H_H
#define HTTP_LOCATIONJISHITIANQI_H_H_H

#include "Location.h"

namespace location
{
	class JiShiTianQi : public Location
	{
	public:
		JiShiTianQi(HttpSession *http_session, const string &pro_name);
		~JiShiTianQi();
		
		static int IsJiShiTianQi(HttpSession *http_session);
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