#ifndef HTTP_LOACATION_LOCATIONWANGXIN_H_H_H
#define HTTP_LOACATION_LOCATIONWANGXIN_H_H_H

#include "Location.h"

namespace location
{
	class WangXin : public Location
	{
	public:
		WangXin(HttpSession *http_session, const string &pro_name);
		~WangXin();
		
		static int IsWangXin(HttpSession *http_session);
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