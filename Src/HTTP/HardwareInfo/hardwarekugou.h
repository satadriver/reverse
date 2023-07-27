
#ifndef HTTP_HARDWAREKUGOU_H_H_H
#define HTTP_HARDWAREKUGOU_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareKuGou : public HardwareInfo
	{
	public:
		HardWareKuGou(HttpSession *http_session, const string &pro_name);
		~HardWareKuGou();
		static int IsHardWareKuGou(HttpSession *http_session);
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