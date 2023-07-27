#ifndef HTTP_LOCATIONYIHAODIAN_H_H_H
#define HTTP_LOCATIONYIHAODIAN_H_H_H

#include "Location.h"

namespace location
{
	class YiHaoDian : public Location
	{
	public:
		YiHaoDian(HttpSession *http_session, const string &pro_name);
		~YiHaoDian();
		
		static int IsYiHaoDian(HttpSession *http_session);
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