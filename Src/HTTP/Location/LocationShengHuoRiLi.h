#ifndef HTTP_LOCATIONSHENGHUORILI_H_H_H
#define HTTP_LOCATIONSHENGHUORILI_H_H_H

#include "Location.h"

namespace location
{
	class ShengHuoRiLi : public Location
	{
	public:
		ShengHuoRiLi(HttpSession *http_session, const string &pro_name);
		~ShengHuoRiLi();
		
		static int IsShengHuoRiLi(HttpSession *http_session);
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