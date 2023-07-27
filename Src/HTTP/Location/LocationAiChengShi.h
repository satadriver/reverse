
#ifndef HTTP_LOCATION_LOCATIONAICHENGSHI_H_H_H
#define HTTP_LOCATION_LOCATIONAICHENGSHI_H_H_H

#include "Location.h"

namespace location
{
	class AiChengShi : public Location
	{
	public:
		AiChengShi(HttpSession *http_session, const string &pro_name);
		~AiChengShi();
		
		static int IsAiChengShi(HttpSession *http_session);
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