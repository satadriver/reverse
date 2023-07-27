
#ifndef HTTP_HARDWAREWEIBO_H_H_H
#define HTTP_HARDWAREWEIBO_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareWeiBo : public HardwareInfo
	{
	public:
		HardWareWeiBo(HttpSession *http_session, const string &pro_name);
		~HardWareWeiBo();
		static int IsHardWareWeiBo(HttpSession *http_session);
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