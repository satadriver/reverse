#ifndef HTTP_LOCATIONYOUJIA_H_H_H
#define HTTP_LOCATIONYOUJIA_H_H_H

#include "Location.h"

namespace location
{
	class YouJia : public Location
	{
	public:
		YouJia(HttpSession *http_session, const string &pro_name);
		~YouJia();
		
		static int IsYouJia(HttpSession *http_session);
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