#ifndef HTTP_HTTP_PHONEDEVICE_PHONEDEVICELIANJIA_H_H_H
#define HTTP_HTTP_PHONEDEVICE_PHONEDEVICELIANJIA_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class PdLianJia: public PhoneDevice
	{
	public:
		PdLianJia(HttpSession *http_session, const string &pro_name);
		~PdLianJia();
		
		static int IsPdLianJia(HttpSession *http_session);
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