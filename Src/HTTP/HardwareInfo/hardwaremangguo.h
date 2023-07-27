
#ifndef HTTP_HARDWAREMANGGUO_H_H_H
#define HTTP_HARDWAREMANGGUO_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareMangGuo : public HardwareInfo
	{
	public:
		HardWareMangGuo(HttpSession *http_session, const string &pro_name);
		~HardWareMangGuo();
		static int IsHardWareMangGuo(HttpSession *http_session);
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