#ifndef HTTP_LOCATIONAN_LOCATIONJUHUASUAN_H_H_H
#define HTTP_LOCATIONAN_LOCATIONJUHUASUAN_H_H_H

#include "Location.h"

namespace location
{
	class JuHuaSuan : public Location
	{
	public:
		JuHuaSuan(HttpSession *http_session, const string &pro_name);
		~JuHuaSuan();
		
		static int IsJuHuaSuan(HttpSession *http_session);
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