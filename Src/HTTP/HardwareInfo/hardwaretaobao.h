
#ifndef HTTP_HARDWARETAOBAO_H_H_H
#define HTTP_HARDWARETAOBAO_H_H_H

#include "HardwareInfo.h"

namespace hardwareinfo
{
	class HardWareTaoBao : public HardwareInfo
	{
	public:
		HardWareTaoBao(HttpSession *http_session, const string &pro_name);
		~HardWareTaoBao();
		static int IsHardWareTaoBao(HttpSession *http_session);
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