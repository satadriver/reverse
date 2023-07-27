#ifndef HTTP_PHONEDEVICE_PHONEDEVICEBILIN_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEBILIN_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class BiLin : public PhoneDevice
	{
	public:
		BiLin(HttpSession *http_session, const string &pro_name);
		~BiLin();
		
		static int IsBiLin(HttpSession *http_session);
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