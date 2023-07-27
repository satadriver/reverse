
#ifndef HTTP_HARDWAREBAIDUDITU_H_H_H
#define HTTP_HARDWAREBAIDUDITU_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareBaiduDitu : public HardwareInfo
	{
	public:
		HardWareBaiduDitu(HttpSession *http_session, const string &pro_name);
		~HardWareBaiduDitu();
		static int IsHardWareBaiduDitu(HttpSession *http_session);
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