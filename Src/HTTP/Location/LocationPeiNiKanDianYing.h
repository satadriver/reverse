
#ifndef HTTP_LOCATION_LOCATIONPEINIKANDIANYING_H_H_H
#define HTTP_LOCATION_LOCATIONPEINIKANDIANYING_H_H_H

#include "Location.h"

namespace location
{
	class PeiNiKanDianYing : public Location
	{
	public:
		PeiNiKanDianYing(HttpSession *http_session, const string &pro_name);
		~PeiNiKanDianYing();
		
		static int IsPeiNiKanDianYing(HttpSession *http_session);
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