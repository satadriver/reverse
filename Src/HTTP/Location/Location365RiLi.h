#ifndef HTTP_LOCATIONSANLIUWURILI_H_H_H
#define HTTP_LOCATIONSANLIUWURILI_H_H_H

#include "Location.h"

namespace location
{
	class RiLi365 : public Location
	{
	public:
		RiLi365(HttpSession *http_session, const string &pro_name);
		~RiLi365();
		
		static int IsRiLi365(HttpSession *http_session);
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