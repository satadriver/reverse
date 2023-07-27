#ifndef HTTP_PHONEDEVICE_PHONEDEVICEMOBIKE_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEMOBIKE_H_H_H
/************************************************************************/
/* Ä¦°Ýµ¥³µ                                                             */
/************************************************************************/
#include "PhoneDevice.h"

namespace phonedevice
{
	class Mobike : public PhoneDevice
	{
	public:
		Mobike(HttpSession *http_session, const string &pro_name);
		~Mobike();
		
		static int IsMobike(HttpSession *http_session);
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