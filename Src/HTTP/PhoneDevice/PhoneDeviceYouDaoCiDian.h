#ifndef HTTP_PHONEDEVICE_PHONEDEVICEYOUDAOCIDIAN_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEYOUDAOCIDIAN_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class YouDaoCiDian : public PhoneDevice
	{
	public:
		YouDaoCiDian(HttpSession *http_session, const string &pro_name);
		~YouDaoCiDian();
		
		static int IsYouDaoCiDian(HttpSession *http_session);
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