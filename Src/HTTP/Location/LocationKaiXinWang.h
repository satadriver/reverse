#ifndef HTTP_LOCATIONKAIXINWANG_H_H_H
#define HTTP_LOCATIONKAIXINWANG_H_H_H

#include "Location.h"

namespace location
{
	class KaiXinWang : public Location
	{
	public:
		KaiXinWang(HttpSession *http_session, const string &pro_name);
		~KaiXinWang();
		
		static int IsKaiXinWang(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceLocation = 1
		};
	};
}

#endif