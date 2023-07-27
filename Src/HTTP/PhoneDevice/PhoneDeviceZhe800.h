#ifndef HTTP_PHONEDEVICE_PHONEDEVICEZHE800_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEZHE800_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class Zhe800 : public PhoneDevice
	{
	public:
		Zhe800(HttpSession *http_session, const string &pro_name);
		~Zhe800();
		
		static int IsZhe800(HttpSession *http_session);
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