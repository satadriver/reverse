#ifndef HTTP_LOCATIONZHOUMOQUNA_H_H_H
#define HTTP_LOCATIONZHOUMOQUNA_H_H_H

#include "Location.h"

namespace location
{
	class ZhouMoQuNa : public Location
	{
	public:
		ZhouMoQuNa(HttpSession *http_session, const string &pro_name);
		~ZhouMoQuNa();
		
		static int IsZhouMoQuNa(HttpSession *http_session);
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