#ifndef HTTP_LOCATION_LOCATIONGANJJI_H_H_H
#define HTTP_LOCATION_LOCATIONGANJJI_H_H_H

#include "Location.h"

namespace location
{
	class GanJi : public Location
	{
	public:
		GanJi(HttpSession *http_session, const string &pro_name);
		~GanJi();
		
		static int IsGanJi(HttpSession *http_session);
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