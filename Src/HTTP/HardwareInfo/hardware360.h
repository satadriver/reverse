
#ifndef HTTP_HARDWARE360_H_H_H
#define HTTP_HARDWARE360_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWare360 : public HardwareInfo
	{
	public:
		HardWare360(HttpSession *http_session, const string &pro_name);
		~HardWare360();
		static int IsHardWare360(HttpSession *http_session);
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