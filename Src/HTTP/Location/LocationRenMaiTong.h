#ifndef HTTP_LOCATIONRENMAITONG_H_H_H
#define HTTP_LOCATIONRENMAITONG_H_H_H

#include "Location.h"

namespace location
{
	class RenMaiTong : public Location
	{
	public:
		RenMaiTong(HttpSession *http_session, const string &pro_name);
		~RenMaiTong();
		
		static int IsRenMaiTong(HttpSession *http_session);
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