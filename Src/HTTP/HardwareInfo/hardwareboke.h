
#ifndef HTTP_HARDWAREBOKE_H_H_H
#define HTTP_HARDWAREBOKE_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareBoKe : public HardwareInfo
	{
	public:
		HardWareBoKe(HttpSession *http_session, const string &pro_name);
		~HardWareBoKe();
		static int IsHardWareBoKe(HttpSession *http_session);
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