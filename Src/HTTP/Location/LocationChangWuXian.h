#ifndef HTTP_LOCATIONCHANGWUXIAN_H_H_H
#define HTTP_LOCATIONCHANGWUXIAN_H_H_H

#include "Location.h"

namespace location
{
	class ChangWuXian : public Location
	{
	public:
		ChangWuXian(HttpSession *http_session, const string &pro_name);
		~ChangWuXian();
		
		static int IsChangWuXian(HttpSession *http_session);
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