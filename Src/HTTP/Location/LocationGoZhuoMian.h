#ifndef HTTP_LOCATIONGOZHUOMIAN_H_H_H
#define HTTP_LOCATIONGOZHUOMIAN_H_H_H

#include "Location.h"

namespace location
{
	class GOZhuoMian : public Location
	{
	public:
		GOZhuoMian(HttpSession *http_session, const string &pro_name);
		~GOZhuoMian();
		
		static int IsGOZhuoMian(HttpSession *http_session);
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