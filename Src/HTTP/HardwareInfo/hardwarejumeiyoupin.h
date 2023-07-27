
#ifndef HTTP_HARDWAREjumeiyoupin_H_H_H
#define HTTP_HARDWAREjumeiyoupin_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareJumeiYoupin : public HardwareInfo
	{
	public:
		HardWareJumeiYoupin(HttpSession *http_session, const string &pro_name);
		~HardWareJumeiYoupin();
		static int IsHardWareJumeiYoupin(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int hardwareinfomation(const char *packet);
		enum
		{
			khardwareinfomation=1,
		};
	};
}

#endif