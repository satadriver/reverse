#ifndef HTTP_HTTP_PHONEDEVICE_PHONEDEVICETOUTIAO_H_H_H
#define HTTP_HTTP_PHONEDEVICE_PHONEDEVICETOUTIAO_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class PdTouTiao : public PhoneDevice
	{
	public:
		PdTouTiao(HttpSession *http_session, const string &pro_name);
		~PdTouTiao();
		
		static int IsPdTouTiao(HttpSession *http_session);
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