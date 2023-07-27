#ifndef HTTP_LOCATIONNUOMI_H_H_H
#define HTTP_LOCATIONNUOMI_H_H_H

#include "Location.h"

namespace location
{
	class NuoMi : public Location
	{
	public:
		NuoMi(HttpSession *http_session, const string &pro_name);
		~NuoMi();
		
		static int IsNuoMi(HttpSession *http_session);
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