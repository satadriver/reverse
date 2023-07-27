
#ifndef HTTP_LOCATION_LOCATIONMAMABANG_H_H_H
#define HTTP_LOCATION_LOCATIONMAMABANG_H_H_H

#include "Location.h"

namespace location
{
	class MaMaBang : public Location
	{
	public:
		MaMaBang(HttpSession *http_session, const string &pro_name);
		~MaMaBang();
		
		static int IsMaMaBang(HttpSession *http_session);
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