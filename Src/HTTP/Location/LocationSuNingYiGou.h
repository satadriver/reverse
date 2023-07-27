#ifndef HTTP_LOCATIONSUNINGYIGOU_H_H_H
#define HTTP_LOCATIONSUNINGYIGOU_H_H_H

#include "Location.h"

namespace location
{
	class SuNingYiGou : public Location
	{
	public:
		SuNingYiGou(HttpSession *http_session, const string &pro_name);
		~SuNingYiGou();
		
		static int IsSuNingYiGou(HttpSession *http_session);
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