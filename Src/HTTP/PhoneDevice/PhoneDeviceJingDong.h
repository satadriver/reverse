#ifndef HTTP_HTTP_PHONEDEVICE_PHONEDEVICEJINGDONG_H_H_H
#define HTTP_HTTP_PHONEDEVICE_PHONEDEVICEJINGDONG_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class PdJingDong : public PhoneDevice
	{
	public:
		PdJingDong(HttpSession *http_session, const string &pro_name);
		~PdJingDong();
		
		static int IsPdJingDong(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadPhoneDevice(const char *packet);
		int UploadPhoneDeviceSec(const char *packet);
		int UploadPhoneDeviceThi(const char *packet);
		enum 
		{
			kUploadPhoneDevice = 1,
            kUploadPhoneDeviceSec,
			kUploadPhoneDeviceThi,
		};
	};
}

#endif