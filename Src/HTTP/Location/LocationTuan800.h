#ifndef HTTP_LOCATIONAN_LOCATIONTUANBABAI_H_H_H
#define HTTP_LOCATIONAN_LOCATIONTUANBABAI_H_H_H

#include "Location.h"

namespace location
{
	class Tuan800 : public Location
	{
	public:
		Tuan800(HttpSession *http_session, const string &pro_name);
		~Tuan800();
		
		static int IsTuan800(HttpSession *http_session);
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