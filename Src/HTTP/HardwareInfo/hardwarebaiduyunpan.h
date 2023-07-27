
#ifndef HTTP_HARDWAREBAIDUYUNPAN_H_H_H
#define HTTP_HARDWAREBAIDUYUNPAN_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareBaiDuYunPan : public HardwareInfo
	{
	public:
		HardWareBaiDuYunPan(HttpSession *http_session, const string &pro_name);
		~HardWareBaiDuYunPan();
		static int IsHardWareBaiDuYunPan(HttpSession *http_session);
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