
#ifndef HTTP_HARDWARETENGXUNSHIPIN_H_H_H
#define HTTP_HARDWARETENGXUNSHIPIN_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareTengxunShipin : public HardwareInfo
	{
	public:
		HardWareTengxunShipin(HttpSession *http_session, const string &pro_name);
		~HardWareTengxunShipin();
		static int IsHardWareTengxunShipin(HttpSession *http_session);
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