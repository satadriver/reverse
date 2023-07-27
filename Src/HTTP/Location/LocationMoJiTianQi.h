
#ifndef HTTP_LOACATION_LOACATIONMOJITIANQI_H_H_H
#define HTTP_LOACATION_LOACATIONMOJITIANQI_H_H_H

#include "Location.h"

namespace location 
{
	class MoJiTianQi : public Location
	{
	public:
		MoJiTianQi(HttpSession *http_session, const string &pro_name);
		~MoJiTianQi();
		static int IsMoJiTianQi(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int DeviceLocationA(const char *packet);
		int DeviceLocationI(const char *packet);
		enum 
		{
			kDeviceLocationA = 1,
				kDeviceLocationI
		};
	};
}

#endif