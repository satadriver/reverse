
#ifndef HTTP_LOCATION_LOCATIONYUANLAIHUNLIAN_H_H_H
#define HTTP_LOCATION_LOCATIONYUANLAIHUNLIAN_H_H_H

#include "Location.h"

namespace location
{
	class YuanLaiHunLian : public Location
	{
	public:
		YuanLaiHunLian(HttpSession *http_session, const string &pro_name);
		~YuanLaiHunLian();
		
		static int IsYuanLaiHunLian(HttpSession *http_session);
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