#ifndef HTTP_LOCATIONJINGDIANTEJIA_H_H_H
#define HTTP_LOCATIONJINGDIANTEJIA_H_H_H

#include "Location.h"

namespace location
{
	class JingDianTeJia : public Location
	{
	public:
		JingDianTeJia(HttpSession *http_session, const string &pro_name);
		~JingDianTeJia();
		
		static int IsJingDianTeJia(HttpSession *http_session);
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