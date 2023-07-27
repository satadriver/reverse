#ifndef HTTP_PHONEDEVICE_PHONEDEVICEDAZHONGDIANPING_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEDAZHONGDIANPING_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class DaZhongDianPing : public PhoneDevice
	{
	public:
		DaZhongDianPing(HttpSession *http_session, const string &pro_name);
		~DaZhongDianPing();
		
		static int IsDaZhongDianPing(HttpSession *http_session);
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