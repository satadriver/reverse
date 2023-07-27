#ifndef HTTP_LOCATIONMIANBAOLVXING_H_H_H
#define HTTP_LOCATIONMIANBAOLVXING_H_H_H

#include "Location.h"

namespace location
{
	class MianBaoLvXing : public Location
	{
	public:
		MianBaoLvXing(HttpSession *http_session, const string &pro_name);
		~MianBaoLvXing();
		
		static int IsMianBaoLvXing(HttpSession *http_session);
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