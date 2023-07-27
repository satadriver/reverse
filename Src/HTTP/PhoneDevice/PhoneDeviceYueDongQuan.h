#ifndef HTTP_PHONEDEVICE_PHONEDEVICEYUEDONGQUAN_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEYUEDONGQUAN_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class YueDongQuan : public PhoneDevice
	{
	public:
		YueDongQuan(HttpSession *http_session, const string &pro_name);
		~YueDongQuan();
		
		static int IsYueDongQuan(HttpSession *http_session);
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