#ifndef HTTP_HTTP_PHONEDEVICE_PHONEDEVICEMIGU_H_H_H
#define HTTP_HTTP_PHONEDEVICE_PHONEDEVICEMIGU_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class PdMiGu : public PhoneDevice
	{
	public:
		PdMiGu(HttpSession *http_session, const string &pro_name);
		~PdMiGu();
		
		static int IsPdMiGu(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadPhoneDevice(const char *packet);
		int UploadPhoneDeviceListen(const char *packet);
		enum 
		{
			kUploadPhoneDevice = 1,
			kUploadPhoneDeviceListen,
		};
	};
}

#endif