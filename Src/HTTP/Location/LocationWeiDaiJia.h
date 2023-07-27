#ifndef HTTP_LOCATIONAN_LOCATIONWEIDAIJIA_H_H_H
#define HTTP_LOCATIONAN_LOCATIONWEIDAIJIA_H_H_H

#include "Location.h"

namespace location
{
	class WeiDaiJia : public Location
	{
	public:
		WeiDaiJia(HttpSession *http_session, const string &pro_name);
		~WeiDaiJia();
		
		static int IsWeiDaiJia(HttpSession *http_session);
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