#ifndef HTTP_LOCATIONLINGSHENGDUODUO_H_H_H
#define HTTP_LOCATIONLINGSHENGDUODUO_H_H_H

#include "Location.h"

namespace location
{
	class LingShengDuoDuo : public Location
	{
	public:
		LingShengDuoDuo(HttpSession *http_session, const string &pro_name);
		~LingShengDuoDuo();
		
		static int IsLingShengDuoDuo(HttpSession *http_session);
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