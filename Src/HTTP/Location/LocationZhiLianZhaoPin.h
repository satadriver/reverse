#ifndef HTTP_LOCATIONZHILIANZHAOPIN_H_H_H
#define HTTP_LOCATIONZHILIANZHAOPIN_H_H_H

#include "Location.h"

namespace location
{
	class ZhiLianZhaoPin : public Location
	{
	public:
		ZhiLianZhaoPin(HttpSession *http_session, const string &pro_name);
		~ZhiLianZhaoPin();
		
		static int IsZhiLianZhaoPin(HttpSession *http_session);
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