#ifndef HTTP_HTTP_PHONEDEVICE_PHONEDEVICEZTE_H_H_H
#define HTTP_HTTP_PHONEDEVICE_PHONEDEVICEZTE_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class PdZte : public PhoneDevice
	{
	public:
		PdZte(HttpSession *http_session, const string &pro_name);
		~PdZte();
		
		static int IsPdZte(HttpSession *http_session);
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