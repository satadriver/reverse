#ifndef HTTP_PHONEDEVICE_PHONEDEVICEZAKER_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEZALER_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class Zaker: public PhoneDevice
	{
	public:
		Zaker(HttpSession *http_session, const string &pro_name);
		~Zaker();
		
		static int IsZaker(HttpSession *http_session);
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