
#ifndef HTTP_LOACATION_LOACATIONMOMO_H_H_H
#define HTTP_LOACATION_LOACATIONMOMO_H_H_H

#include "Location.h"

namespace location 
{
	class MoMo : public Location
	{
	public:
		MoMo(HttpSession *http_session, const string &pro_name);
		~MoMo();
		static int IsMoMo(HttpSession *http_session);
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