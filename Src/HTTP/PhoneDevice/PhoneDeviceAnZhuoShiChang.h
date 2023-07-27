#ifndef HTTP_PHONEDEVICE_PHONEDEVICEANZHUOSHICHANG_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEANZHUOSHICHANG_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class AnZhuoShiChang : public PhoneDevice
	{
	public:
		AnZhuoShiChang(HttpSession *http_session, const string &pro_name);
		~AnZhuoShiChang();
		
		static int IsAnZhuoShiChang(HttpSession *http_session);
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