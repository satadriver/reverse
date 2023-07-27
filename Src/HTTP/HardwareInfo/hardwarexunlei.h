
#ifndef HTTP_HARDWAREXUNLEI_H_H_H
#define HTTP_HARDWAREXUNLEI_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareXunLei : public HardwareInfo
	{
	public:
		HardWareXunLei(HttpSession *http_session, const string &pro_name);
		~HardWareXunLei();
		static int IsHardWareXunLei(HttpSession *http_session);
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