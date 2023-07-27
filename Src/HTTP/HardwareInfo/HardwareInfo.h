#ifndef HTTP_HARDWAREINFO_H_H_H
#define HTTP_HARDWAREINFO_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" //使用json 提取字符串

namespace hardwareinfo
{
	class HardwareInfo : public HttpPro
	{
	public:
		HardwareInfo(HttpSession *http_session, const string &pro_name);
		virtual ~HardwareInfo();
		
		static int ProcessHardwareInfo(HttpSession *http_session ,const char *packet);

	protected:
		string mid;          //硬件特征码
		string app_;         //软件
	};
}

#endif