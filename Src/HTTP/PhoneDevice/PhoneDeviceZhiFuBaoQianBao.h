#ifndef HTTP_PHONEDEVICE_PHONEDEVICEZHIFUBAOQIANBAO_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEZHIFUBAOQIANBAO_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class ZhiFuBaoQianBao : public PhoneDevice
	{
	public:
		ZhiFuBaoQianBao(HttpSession *http_session, const string &pro_name);
		~ZhiFuBaoQianBao();
		
		static int IsZhiFuBaoQianBao(HttpSession *http_session);
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