#ifndef HTTP_PHONEDEVICE_PHONEDEVICEWANDOUJIA_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEWANDOUJIA_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class WanDouJia : public PhoneDevice
	{
	public:
		WanDouJia(HttpSession *http_session, const string &pro_name);
		~WanDouJia();
		
		static int IsWanDouJia(HttpSession *http_session);
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