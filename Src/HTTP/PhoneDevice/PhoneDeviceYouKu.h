#ifndef HTTP_HTTP_PHONEDEVICE_PHONEDEVICEYOUKU_H_H_H
#define HTTP_HTTP_PHONEDEVICE_PHONEDEVICEYOUKU_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class YouKu : public PhoneDevice
	{
	public:
		YouKu(HttpSession *http_session, const string &pro_name);
		~YouKu();
		
		static int IsYouKu(HttpSession *http_session);
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