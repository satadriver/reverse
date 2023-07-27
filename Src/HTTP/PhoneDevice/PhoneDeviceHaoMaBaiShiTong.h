#ifndef HTTP_PHONEDEVICE_PHONEDEVICEHAOMABAISHITONG_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEHAOMABAISHITONG_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class HaoMaBaiShiTong : public PhoneDevice
	{
	public:
		HaoMaBaiShiTong(HttpSession *http_session, const string &pro_name);
		~HaoMaBaiShiTong();
		
		static int IsHaoMaBaiShiTong(HttpSession *http_session);
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