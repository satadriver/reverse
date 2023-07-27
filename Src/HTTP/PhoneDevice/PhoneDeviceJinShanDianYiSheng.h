#ifndef HTTP_PHONEDEVICE_PHONEDEVICEJINSHANDIANCHIYISHENG_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEJINSHANDIANCHIYISHENG_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class JinShanDianChiYiSheng : public PhoneDevice
	{
	public:
		JinShanDianChiYiSheng(HttpSession *http_session, const string &pro_name);
		~JinShanDianChiYiSheng();
		
		static int IsJinShanDianChiYiSheng(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadPhoneDevice1(const char *packet);
		int UploadPhoneDevice2(const char *packet);
		enum 
		{
			kUploadPhoneDevice1 = 1, //2014-10-
			kUploadPhoneDevice2 = 2, //2014-11-13
		};
	};
}

#endif