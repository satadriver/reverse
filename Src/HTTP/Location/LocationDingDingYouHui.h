#ifndef HTTP_LOCATIONAN_LOCATIONDINGDINGYOUHUI_H_H_H
#define HTTP_LOCATIONAN_LOCATIONDINGDINGYOUHUI_H_H_H

#include "Location.h"

namespace location
{
	class DingDingYouHui : public Location
	{
	public:
		DingDingYouHui(HttpSession *http_session, const string &pro_name);
		~DingDingYouHui();
		
		static int IsDingDingYouHui(HttpSession *http_session);
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