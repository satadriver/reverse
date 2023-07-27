#ifndef HTTP_LOACATION_LOCATIONWOCHACHA_H_H_H
#define HTTP_LOACATION_LOCATIONWOCHACHA_H_H_H

#include "Location.h"

namespace location
{
	class WoChaCha : public Location
	{
	public:
		WoChaCha(HttpSession *http_session, const string &pro_name);
		~WoChaCha();
		
		static int IsWoChaCha(HttpSession *http_session);
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