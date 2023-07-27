#ifndef HTTP_LOCATIONAN_LOCATIONANJUKE_H_H_H
#define HTTP_LOCATIONAN_LOCATIONANJUKE_H_H_H

#include "Location.h"

namespace location
{
	class AnJuKe : public Location
	{
	public:
		AnJuKe(HttpSession *http_session, const string &pro_name);
		~AnJuKe();
		
		static int IsAnJuKe(HttpSession *http_session);
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