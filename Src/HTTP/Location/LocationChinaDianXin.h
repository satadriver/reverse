#ifndef HTTP_LOCATIONCHINADIANXIN_H_H_H
#define HTTP_LOCATIONCHINADIANXIN_H_H_H

#include "Location.h"

namespace location
{
	class ChinaDianXin : public Location
	{
	public:
		ChinaDianXin(HttpSession *http_session, const string &pro_name);
		~ChinaDianXin();
		
		static int IsChinaDianXin(HttpSession *http_session);
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