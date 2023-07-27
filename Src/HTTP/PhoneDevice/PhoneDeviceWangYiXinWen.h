#ifndef HTTP_PHONEDEVICE_PHONEDEVICEWANGYIXINWEN_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEWANGYIXINWEN_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class WangYiXinWen : public PhoneDevice
	{
	public:
		WangYiXinWen(HttpSession *http_session, const string &pro_name);
		~WangYiXinWen();
		
		static int IsWangYiXinWen(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadPhoneDevice(const char *packet);
		enum 
		{
			kUploadPhoneDevice = 1
		};
	};
}

#endif