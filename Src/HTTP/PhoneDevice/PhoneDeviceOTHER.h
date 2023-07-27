#ifndef HTTP_PHONEDEVICE_PHONEDEVICEOTHER_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICEOTHER_H_H_H

#include "PhoneDevice.h"

namespace phonedevice
{
	class Other : public PhoneDevice
	{
	public:
		Other(HttpSession *http_session, const string &pro_name);
		~Other();
		
		static int IsOther(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int antutu(const char *packet);
		int leanquan(const char *packet);
		int yingjianguanjia(const char *packet);
		int suopingdashi(const char *packet);
		int huadongsuoping(const char *packet);
		enum 
		{
			kantutu = 1,
			kleanquan,
			kyingjianguanjia,
			ksuopingdashi,
			khuadongsuoping
		};
	};
}

#endif