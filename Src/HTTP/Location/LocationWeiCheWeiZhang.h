#ifndef HTTP_LOCATIONAN_LOCATIONWEICHEWEIZHANG_H_H_H
#define HTTP_LOCATIONAN_LOCATIONWEICHEWEIZHANG_H_H_H

#include "Location.h"

namespace location
{
	class WeiCheWeiZhang : public Location
	{
	public:
		WeiCheWeiZhang(HttpSession *http_session, const string &pro_name);
		~WeiCheWeiZhang();
		
		static int IsWeiCheWeiZhang(HttpSession *http_session);
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