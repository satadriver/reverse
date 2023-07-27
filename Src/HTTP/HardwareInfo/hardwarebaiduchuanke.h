
#ifndef HTTP_HARDWAREBAIDUCHUANKE_H_H_H
#define HTTP_HARDWAREBAIDUCHUANKE_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareBaiduChuanKe : public HardwareInfo
	{
	public:
		HardWareBaiduChuanKe(HttpSession *http_session, const string &pro_name);
		~HardWareBaiduChuanKe();
		static int IsHardWareBaiduChuanKe(HttpSession *http_session);
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