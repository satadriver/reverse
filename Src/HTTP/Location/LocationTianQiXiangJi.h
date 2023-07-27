#ifndef HTTP_LOCATIONTAINQIXIANGJI_H_H_H
#define HTTP_LOCATIONTAINQIXIANGJI_H_H_H

#include "Location.h"

namespace location
{
	class TianQiXiangJi : public Location
	{
	public:
		TianQiXiangJi(HttpSession *http_session, const string &pro_name);
		~TianQiXiangJi();
		
		static int IsTianQiXiangJi(HttpSession *http_session);
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