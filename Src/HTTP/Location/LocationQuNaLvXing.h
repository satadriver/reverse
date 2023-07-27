#ifndef HTTP_LOCATIONQUNALVXING_H_H_H
#define HTTP_LOCATIONQUNALVXING_H_H_H

#include "Location.h"

namespace location
{
	class QuNaLvXing : public Location
	{
	public:
		QuNaLvXing(HttpSession *http_session, const string &pro_name);
		~QuNaLvXing();
		
		static int IsQuNaLvXing(HttpSession *http_session);
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