#ifndef HTTP_LOCATIONBLUED_H_H_H
#define HTTP_LOCATIONBLUED_H_H_H

#include "Location.h"

namespace location
{
	class Blued : public Location
	{
	public:
		Blued(HttpSession *http_session, const string &pro_name);
		~Blued();
		
		static int IsBlued(HttpSession *http_session);
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