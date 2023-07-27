#ifndef HTTP_PHONEDEVICE_PHONEDEVICEMOJITIANQI_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEMOJITIANQI_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class MoJiTianQi : public PhoneDevice
	{
	public:
		MoJiTianQi(HttpSession *http_session, const string &pro_name);
		~MoJiTianQi();
		
		static int IsMoJiTianQi(HttpSession *http_session);
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