#ifndef HTTP_PHONEDEVICE_PHONEDEVICEDOUGUOMEISHI_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEDOUGUOMEISHI_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class DouGuoMeiShi : public PhoneDevice
	{
	public:
		DouGuoMeiShi(HttpSession *http_session, const string &pro_name);
		~DouGuoMeiShi();
		
		static int IsDouGuoMeiShi(HttpSession *http_session);
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