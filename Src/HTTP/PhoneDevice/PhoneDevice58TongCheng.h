#ifndef HTTP_HTTP_PHONEDEVICE_PHONEDEVICE58TONGCHENG_H_H_H
#define HTTP_HTTP_PHONEDEVICE_PHONEDEVICE58TONGCHENG_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class Pd58TongCheng : public PhoneDevice
	{
	public:
		Pd58TongCheng(HttpSession *http_session, const string &pro_name);
		~Pd58TongCheng();
		
		static int IsPd58TongCheng(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadPhoneDevice(const char *packet);
		int UploadPhoneDeviceSec(const char *packet);
		int UploadPhoneDeviceTre(const char *packet);
		enum 
		{
			kUploadPhoneDevice = 1,
            kUploadPhoneDeviceSec,
			kUploadPhoneDeviceTre,
		};
	};
}

#endif