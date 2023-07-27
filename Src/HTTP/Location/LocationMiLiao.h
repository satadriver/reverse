#ifndef HTTP_LOCATIONMILIAO_H_H_H
#define HTTP_LOCATIONMILIAO_H_H_H

#include "Location.h"

namespace location
{
	class MiLiao : public Location
	{
	public:
		MiLiao(HttpSession *http_session, const string &pro_name);
		~MiLiao();
		
		static int IsMiLiao(HttpSession *http_session);
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