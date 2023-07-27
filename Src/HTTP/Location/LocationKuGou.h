#ifndef HTTP_LOCATIONKUGOU_H_H_H
#define HTTP_LOCATIONKUGOU_H_H_H

#include "Location.h"

namespace location
{
	class KuGou : public Location
	{
	public:
		KuGou(HttpSession *http_session, const string &pro_name);
		~KuGou();
		
		static int IsKuGou(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceLocation = 1,
		};
	};
}

#endif