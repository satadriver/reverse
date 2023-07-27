#ifndef HTTP_LOCATION_LOCATIONANBAUDUTUANGOU_H_H_H
#define HTTP_LOCATION_LOCATIONANBAUDUTUANGOU_H_H_H

#include "Location.h"

namespace location
{
	class BaiDuTuanGou : public Location
	{
	public:
		BaiDuTuanGou(HttpSession *http_session, const string &pro_name);
		~BaiDuTuanGou();
		
		static int IsBaiDuTuanGou(HttpSession *http_session);
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