#ifndef HTTP_PHONEDEVICE_PHONEDEVICEFENGHUANGXINWEN_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEFENGHUANGXINWEN_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class FengHuangXinWen : public PhoneDevice
	{
	public:
		FengHuangXinWen(HttpSession *http_session, const string &pro_name);
		~FengHuangXinWen();
		
		static int IsFengHuangXinWen(HttpSession *http_session);
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