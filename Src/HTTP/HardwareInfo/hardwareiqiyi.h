
#ifndef HTTP_HARDWAREIQIYI_H_H_H
#define HTTP_HARDWAREIQIYI_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareIqiYi : public HardwareInfo
	{
	public:
		HardWareIqiYi(HttpSession *http_session, const string &pro_name);
		~HardWareIqiYi();
		static int IsHardWareIqiYi(HttpSession *http_session);
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