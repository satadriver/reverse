
#ifndef HTTP_HARDWAREDUBA_H_H_H
#define HTTP_HARDWAREDUBA_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareDuBa : public HardwareInfo
	{
	public:
		HardWareDuBa(HttpSession *http_session, const string &pro_name);
		~HardWareDuBa();
		static int IsHardWareDuBa(HttpSession *http_session);
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