#ifndef HTTP_LOCATION_LOCATIONANBAIXINGXING_H_H_H
#define HTTP_LOCATION_LOCATIONANBAIXINGXING_H_H_H

#include "Location.h"

namespace location
{
	class BaiXingWang : public Location
	{
	public:
		BaiXingWang(HttpSession *http_session, const string &pro_name);
		~BaiXingWang();
		
		static int IsBaiXingWang(HttpSession *http_session);
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