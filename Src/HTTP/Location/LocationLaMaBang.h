#ifndef HTTP_LOCATIONLAMABANG_H_H_H
#define HTTP_LOCATIONLAMABANG_H_H_H

#include "Location.h"

namespace location
{
	class LaMaBang : public Location
	{
	public:
		LaMaBang(HttpSession *http_session, const string &pro_name);
		~LaMaBang();
		
		static int IsLaMaBang(HttpSession *http_session);
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