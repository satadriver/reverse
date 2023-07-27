#ifndef HTTP_HTTP_PHONEDEVICE_PHONEDEVICEZHUANZHUAN_H_H_H
#define HTTP_HTTP_PHONEDEVICE_PHONEDEVICEZHUANZHUAN_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class PdZhuanZhuan : public PhoneDevice
	{
	public:
		PdZhuanZhuan(HttpSession *http_session, const string &pro_name);
		~PdZhuanZhuan();
		
		static int IsPdZhuanZhuan(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadPhoneDevice(const char *packet);
		int UploadPhoneDeviceSec(const char *packet);
		int UploadPhoneLocation(const char *packet);
		enum 
		{
			kUploadPhoneDevice = 1,
            kUploadPhoneDeviceSec,
			kUploadPhoneLocation,
		};
	};
}

#endif