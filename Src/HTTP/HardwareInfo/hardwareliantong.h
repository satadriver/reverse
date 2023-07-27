
#ifndef HTTP_HARDWARELIANTONG_H_H_H
#define HTTP_HARDWARELIANTONG_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareLianTong : public HardwareInfo
	{
	public:
		HardWareLianTong(HttpSession *http_session, const string &pro_name);
		~HardWareLianTong();
		static int IsHardWareLianTong(HttpSession *http_session);
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