#ifndef HTTP_LOCATIONAN_LOCATIONTUANGOU_H_H_H
#define HTTP_LOCATIONAN_LOCATIONTUANGOU_H_H_H

#include "Location.h"

namespace location
{
	class TuanGou : public Location
	{
	public:
		TuanGou(HttpSession *http_session, const string &pro_name);
		~TuanGou();
		
		static int IsTuanGou(HttpSession *http_session);
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