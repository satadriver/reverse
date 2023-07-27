#ifndef HTTP_LOCATIONAN_LOCATIONWIFIBANLV_H_H_H
#define HTTP_LOCATIONAN_LOCATIONWIFIBANLV_H_H_H

#include "Location.h"

namespace location
{
	class WIFIBanLv : public Location
	{
	public:
		WIFIBanLv(HttpSession *http_session, const string &pro_name);
		~WIFIBanLv();
		
		static int IsWIFIBanLv(HttpSession *http_session);
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