#ifndef HTTP_LOCATIONAN_LOCATIONMEITUAN_H_H_H
#define HTTP_LOCATIONAN_LOCATIONMEITUAN_H_H_H

#include "Location.h"

namespace location
{
	class MeiTuan : public Location
	{
	public:
		MeiTuan(HttpSession *http_session, const string &pro_name);
		~MeiTuan();
		
		static int IsMeiTuan(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int DeviceLocation(const char *packet);
		int DeviceLocation2(const char *packet);
		int DeviceLocationOrder(const char *packet);
		enum 
		{
			kDeviceLocation = 1,
			kDeviceLocation2,
			kDevideLocationOrder,
		};
	};
}

#endif