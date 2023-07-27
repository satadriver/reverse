#ifndef HTTP_PHONEDEVICE_PHONEDEVICEPPZhuShou_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEPPZhuShou_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class ZhuShouPP : public PhoneDevice
	{
	public:
		ZhuShouPP(HttpSession *http_session, const string &pro_name);
		~ZhuShouPP();
		
		static int IsZhuShouPP(HttpSession *http_session);
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