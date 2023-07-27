#ifndef HTTP_LOCATIONAN_LOCATIONMOXIUZHUOMIAN_H_H_H
#define HTTP_LOCATIONAN_LOCATIONMOXIUZHUOMIAN_H_H_H

#include "Location.h"

namespace location
{
	class MoXiuZhuoMian : public Location
	{
	public:
		MoXiuZhuoMian(HttpSession *http_session, const string &pro_name);
		~MoXiuZhuoMian();
		
		static int IsMoXiuZhuoMian(HttpSession *http_session);
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