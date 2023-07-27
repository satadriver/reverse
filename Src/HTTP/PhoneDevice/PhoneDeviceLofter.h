#ifndef HTTP_PHONEDEVICE_PHONEDEVICELOFTER_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICELOFTER_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class Lofter: public PhoneDevice
	{
	public:
		Lofter(HttpSession *http_session, const string &pro_name);
		~Lofter();
		
		static int IsLofter(HttpSession *http_session);
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