
#ifndef HTTP_HARDWAREIMOOC_H_H_H
#define HTTP_HARDWAREIMOOC_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareImooc : public HardwareInfo
	{
	public:
		HardWareImooc(HttpSession *http_session, const string &pro_name);
		~HardWareImooc();
		static int IsHardWareImooc(HttpSession *http_session);
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